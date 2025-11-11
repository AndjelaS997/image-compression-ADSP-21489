/*
 * bmp_io.h
 *
 */

#ifndef BMP_IO_H_
#define BMP_IO_H_

#include <stdio.h>
#include "types_sharc.h"

#pragma pack(push, 1)
typedef struct{
    u16 type;
    u32 size;
    u16 reserved1;
    u16 reserved2;
    u32 offset;

    u32 header_size;
    s32 width;
    s32 height;          // može biti negativna (top-down BMP)
    u16 planes;
    u16 bits_per_pixel;
    u32 compression;
    u32 image_size;
    s32 x_pixel_per_meter;
    s32 y_pixel_per_meter;
    u32 colors_used;
    u32 colors_important;
} bmp_header_t;
#pragma pack(pop)

int bmp_read(FILE* in, bmp_header_t* h, u32* pixels, int max_pixels, int* out_height_abs);
int bmp_write(FILE* out, const bmp_header_t* h, const u32* pixels, int height_abs);

int bmp_read_mem(const u8* data, int data_len, bmp_header_t* h,
                 u32* pixels, int max_pixels, int* out_h);
#endif /* BMP_IO_H_ */
