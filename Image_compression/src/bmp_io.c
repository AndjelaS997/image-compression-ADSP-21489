/*
 * bmp_io.c
 * - ucitavanje 24/32-bit BMP (bez kompresije)
 */
#include "types_sharc.h"
#include "bmp_io.h"
#include <stdio.h>

static u16 read_u16_le(FILE* in)
{
    int b0 = fgetc(in);
    int b1 = fgetc(in);
    if(b0 == EOF || b1 == EOF) return 0;
    return (u16)((b1 << 8) | b0);
}

static u32 read_u32_le(FILE* in)
{
    int b0 = fgetc(in);
    int b1 = fgetc(in);
    int b2 = fgetc(in);
    int b3 = fgetc(in);
    if(b0 == EOF || b1 == EOF || b2 == EOF || b3 == EOF) return 0;
    return (u32)((b3 << 24) | (b2 << 16) | (b1 << 8) | b0);
}

static void write_u16_le(FILE* out, u16 v)
{
    fputc(v & 0xFF, out);
    fputc((v >> 8) & 0xFF, out);
}

static void write_u32_le(FILE* out, u32 v)
{
    fputc(v & 0xFF, out);
    fputc((v >> 8) & 0xFF, out);
    fputc((v >> 16) & 0xFF, out);
    fputc((v >> 24) & 0xFF, out);
}

int bmp_read(FILE* in, bmp_header_t* h, u32* pixels, int max_pixels, int* out_h)
{
    if(!in || !h || !pixels || !out_h) return 0;

    fseek(in, 0, SEEK_SET);

    h->type = read_u16_le(in);
    if(h->type != 0x4D42) { printf("FAIL: type\n"); return 0; };

    h->size = read_u32_le(in);
    h->reserved1 = read_u16_le(in);
    h->reserved2 = read_u16_le(in);
    h->offset = read_u32_le(in);

    h->header_size = read_u32_le(in);
    h->width = (s32)read_u32_le(in);
    h->height = (s32)read_u32_le(in);
    h->planes = read_u16_le(in);
    h->bits_per_pixel = read_u16_le(in);
    h->compression = read_u32_le(in);
    h->image_size = read_u32_le(in);
    h->x_pixel_per_meter = (s32)read_u32_le(in);
    h->y_pixel_per_meter = (s32)read_u32_le(in);
    h->colors_used = read_u32_le(in);
    h->colors_important = read_u32_le(in);

    //TEST
    /*printf("BMP hdr: type=0x%04X size=%lu off=%lu hdrsz=%lu w=%ld h=%ld planes=%u bpp=%u comp=%lu imgSz=%lu\n",
           (unsigned)h->type,
           (unsigned long)h->size,
           (unsigned long)h->offset,
           (unsigned long)h->header_size,
           (long)h->width,
           (long)h->height,
           (unsigned)h->planes,
           (unsigned)h->bits_per_pixel,
           (unsigned long)h->compression,
           (unsigned long)h->image_size);
     */

    if(h->planes != 1)
    {
    	//printf("FAIL: planes=%u\n", (unsigned)h->planes);
    	return 0;
    }
    if(h->compression != 0) {
    	//printf("FAIL: compression=%lu\n", (unsigned long)h->compression);
    	return 0;
    }
    if(h->bits_per_pixel != 24 && h->bits_per_pixel != 32){
        //printf("FAIL: bpp=%u\n", (unsigned)h->bits_per_pixel);
        return 0;
    }

    int bottom_up = (h->height > 0);
    int height_abs = bottom_up ? h->height : -h->height;
    *out_h = height_abs;

    long count = (long)h->width * height_abs;
    if(count <= 0 || count > max_pixels) return 0;

    int bpp = (h->bits_per_pixel == 32) ? 4 : 3;
    int row_stride = (h->width * bpp + 3) & ~3;
    int padding = row_stride - (h->width * bpp);

    fseek(in, h->offset, SEEK_SET);
    //printf("Zapocinjemo citanje.\n");

    for(int i = 0; i < height_abs; i++)
    {
        int row = bottom_up ? (height_abs - 1 - i) : i;
        u32* dst = &pixels[row * h->width];

        for(int j = 0; j < h->width; j++)
        {
            int B = fgetc(in);
            int G = fgetc(in);
            int R = fgetc(in);
            if(B == EOF || G == EOF || R == EOF) return 0;

            if(bpp == 4 && fgetc(in) == EOF) return 0;

            dst[j] = ((u32)R << 16) | ((u32)G << 8) | (u32)B;
        }

        for(int k = 0; k < padding; k++)
            if(fgetc(in) == EOF) return 0;
    }

    //printf("Zavrsavamo citanje.\n");
    return 1;
}

int bmp_write(FILE* out, const bmp_header_t* h,
              const u32* pixels, int height_abs)
{
    if(!out || !h || !pixels) return 0;

    //printf("Zapocinjemo pisanje.\n");

    int row_stride = (h->width * 3 + 3) & ~3;
    int padding = row_stride - h->width * 3;
    u32 img_size = row_stride * height_abs;

    write_u16_le(out, 0x4D42);
    write_u32_le(out, 54 + img_size);
    write_u16_le(out, 0);
    write_u16_le(out, 0);
    write_u32_le(out, 54);

    write_u32_le(out, 40);
    write_u32_le(out, h->width);
    write_u32_le(out, height_abs);
    write_u16_le(out, 1);
    write_u16_le(out, 24);
    write_u32_le(out, 0);
    write_u32_le(out, img_size);
    write_u32_le(out, h->x_pixel_per_meter);
    write_u32_le(out, h->y_pixel_per_meter);
    write_u32_le(out, 0);
    write_u32_le(out, 0);

    for(int i = 0; i < height_abs; i++)
    {
        const u32* src = &pixels[(height_abs - 1 - i) * h->width];
        for(int j = 0; j < h->width; j++)
        {
            u32 p = src[j];
            fputc(p & 0xFF, out);
            fputc((p >> 8) & 0xFF, out);
            fputc((p >> 16) & 0xFF, out);
        }
        for(int k = 0; k < padding; k++) fputc(0, out);
    }
    //printf("Zavrsavamo pisanje.\n");
    return 1;
}



