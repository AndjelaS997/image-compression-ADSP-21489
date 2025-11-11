/*****************************************************************************
 * Image_compression.c
 *****************************************************************************/

#include <sys/platform.h>
#include "adi_initialize.h"
#include <def21489.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cycle_count.h>
#include <sru21489.h>
#include <SYSREG.h>

#include "types_sharc.h"
#include "Image_compression.h"
#include "bmp_io.h"
#include "rgb_gray.h"
#include "dct8.h"
#include "blocks8.h"
#include "quant.h"
#include "zigzag_rle.h"

#define MAX_PIXELS 250000

#pragma section("seg_sram1")
u32 pixels[MAX_PIXELS];

//static void probe(const char* mode);

/* JPEG standard luminance quant table */
static const u8 Q[64] = {
    16,11,10,16,24,40,51,61,
    12,12,14,19,26,58,60,55,
    14,13,16,24,40,57,69,56,
    14,17,22,29,51,87,80,62,
    18,22,37,56,68,109,103,77,
    24,35,55,64,81,104,113,92,
    49,64,78,87,103,121,120,101,
    72,92,95,98,112,100,103,99
};



static void write_u16_le(FILE* f, u16 v)
{
    fputc((int)(v & 0xFF), f);
    fputc((int)((v >> 8) & 0xFF), f);
}

cycle_t start_count;
cycle_t final_count;

void init(void);


int main(void)
{
	init();
    adi_initComponents();

    // sve LED ugasi na pocetku
    sysreg_bit_clr(sysreg_FLAGS, FLG4);
    sysreg_bit_clr(sysreg_FLAGS, FLG5);
    sysreg_bit_clr(sysreg_FLAGS, FLG6);
    SRU(LOW, DAI_PB03_I);
    SRU(LOW, DAI_PB04_I);
    SRU(LOW, DAI_PB15_I);
    SRU(LOW, DAI_PB16_I);
    SRU(LOW, DAI_PB17_I);

    //probe("r");
    //probe("rb");

    int height_abs = 0;
    bmp_header_t bmp_h;

    // LED1: pocinje ucitavanje
    sysreg_bit_set(sysreg_FLAGS, FLG4);

    START_CYCLE_COUNT(start_count);
    FILE* in = fopen("img2.bmp", "r");
    if(!in)return 1;
    if(!bmp_read(in, &bmp_h, pixels, MAX_PIXELS, &height_abs))
    {
    	SRU(HIGH, DAI_PB17_I); // LED8 = error
        fclose(in);
        return 1;
    }
    STOP_CYCLE_COUNT(final_count, start_count);
    PRINT_CYCLES("Ucitavanje slike: ", final_count);
    fclose(in);

    //TEST
    //printf("Slika ucitana: w=%d h=%d abs=%d\n", (int)bmp_h.width, (int)bmp_h.height, height_abs);

    // LED2: grayscale gotov
    START_CYCLE_COUNT(start_count);
    rgb_to_gray(pixels, (int)bmp_h.width, height_abs);
    STOP_CYCLE_COUNT(final_count, start_count);
    PRINT_CYCLES("rgb_to_gray: ", final_count);
    sysreg_bit_set(sysreg_FLAGS, FLG5);

    // LED3: upis sive gotov
    START_CYCLE_COUNT(start_count);
    FILE* dbg = fopen("img1_gray.bmp", "w");
    if(dbg)
    {
        bmp_write(dbg, &bmp_h, pixels, height_abs);
        fclose(dbg);
    }
	STOP_CYCLE_COUNT(final_count, start_count);
	PRINT_CYCLES("Ispis sive: ", final_count);
	sysreg_bit_set(sysreg_FLAGS, FLG6);


    int bxN = blocks_x((int)bmp_h.width);
    int byN = blocks_y(height_abs);

    FILE* cf = fopen("img1.dct", "w");
    if(!cf)
    {
    	SRU(HIGH, DAI_PB17_I);//LED8 = error
        return 1;
    }

    // LED4: header/Q upisan - krece kodovanje
    fprintf(cf, "DCT8_TXT %d %d %d %d\n", (int)bmp_h.width, (int)height_abs, (int)bxN, (int)byN);

    fprintf(cf, "Q ");
    for(int i = 0; i < 64; i++)
        fprintf(cf, "%u ", (unsigned)Q[i]);
    fprintf(cf, "\n");
    SRU(HIGH, DAI_PB03_I);

    dct8x8_init();

    s16 block[64];
    float F[64];
    s16 q[64];
    s16 prev_dc = 0;

    uint64_t sum_ext=0, sum_dct=0, sum_q=0, sum_rle=0;
    u32 blocks = 0;
    for(int by = 0; by < byN; by++)
    {
        for(int bx = 0; bx < bxN; bx++)
        {
        	blocks++;
        	START_CYCLE_COUNT(start_count);
            extract_block_centered(pixels, (int)bmp_h.width, height_abs, bx, by, block);
            STOP_CYCLE_COUNT(final_count, start_count);
            sum_ext += (uint64_t)final_count;

            START_CYCLE_COUNT(start_count);
            dct8x8(block, F);
            STOP_CYCLE_COUNT(final_count, start_count);
            sum_dct += (uint64_t)final_count;

            START_CYCLE_COUNT(start_count);
            quantize8x8(F, Q, q);
            STOP_CYCLE_COUNT(final_count, start_count);
            sum_q += (uint64_t)final_count;

            START_CYCLE_COUNT(start_count);
            write_block_zigzag_rle(cf, q, &prev_dc);
            STOP_CYCLE_COUNT(final_count, start_count);
            sum_rle += (uint64_t)final_count;

            //“živ”-blocks(blink LED5 svakih ~64 blokova)
            if((blocks & 63u) == 0u)
            	SRU(HIGH, DAI_PB04_I);
            else
            	SRU(LOW,  DAI_PB04_I);
        }
    }
    fclose(cf);

    // LED5: završena obrada svih blokova (upali stalno)
    SRU(HIGH, DAI_PB04_I);

    printf("Blocks=%lu\n", (unsigned long)blocks);
    printf("extract total=%llu avg=%llu\n", (unsigned long long)sum_ext, (unsigned long long)(sum_ext/blocks));
    printf("dct total=%llu avg=%llu\n", (unsigned long long)sum_dct, (unsigned long long)(sum_dct/blocks));
    printf("quant total=%llu avg=%llu\n", (unsigned long long)sum_q,   (unsigned long long)(sum_q/blocks));
    printf("rle total=%llu avg=%llu\n", (unsigned long long)sum_rle, (unsigned long long)(sum_rle/blocks));

    // LED6: ispis statistike gotov
    SRU(HIGH, DAI_PB15_I);

    printf("ZAVRSENO SVE!\n");

    // LED7: kraj programa
    SRU(HIGH, DAI_PB16_I);

    return 0;
}

void init(void)
{
	//** LED01**//
	SRU(HIGH,DPI_PBEN06_I);
	SRU(FLAG4_O,DPI_PB06_I);
	//** LED02**//
	SRU(HIGH,DPI_PBEN13_I);
	SRU(FLAG5_O,DPI_PB13_I);
	//** LED03**//
	SRU(HIGH,DPI_PBEN14_I);
	SRU(FLAG6_O,DPI_PB14_I);
	//** LED04**//
	SRU(HIGH,DAI_PBEN03_I);
	SRU(HIGH,DAI_PB03_I);
	//** LED05**//
	SRU(HIGH,DAI_PBEN04_I);
	SRU(HIGH,DAI_PB04_I);
	//** LED06**//
	SRU(HIGH,DAI_PBEN15_I);
	SRU(HIGH,DAI_PB15_I);
	//** LED07**//
	SRU(HIGH,DAI_PBEN16_I);
	SRU(HIGH,DAI_PB16_I);
	//** LED08**//
	SRU(HIGH,DAI_PBEN17_I);
	SRU(HIGH,DAI_PB17_I);
	//Setting flag pins as outputs
	sysreg_bit_set(sysreg_FLAGS, (FLG4O|FLG5O|FLG6O) );
	//Setting HIGH to flag pins
	sysreg_bit_set(sysreg_FLAGS, (FLG4|FLG5|FLG6) );
}

/*TEST
 * static void probe(const char* mode)
{
    FILE* f = fopen("C:\\Users\\Sasa\\cces\\2.12.1\\Image_compression\\Debug\\img1.bmp", mode);
    if(!f) { printf("probe %s: fopen FAIL\n", mode); return; }

    int a = fgetc(f);
    int b = fgetc(f);
    printf("probe %s: first bytes = %02X %02X\n", mode, (unsigned)a & 0xFF, (unsigned)b & 0xFF);
    fclose(f);
}
*/

