/*
 * zigzag_rle.c
 *
 */

#include "zigzag_rle.h"

/* JPEG zigzag mapiranje */
static const u8 ZZ[64] = {
	     0,  1,  8, 16,  9,  2,  3, 10,
	    17, 24, 32, 25, 18, 11,  4,  5,
	    12, 19, 26, 33, 40, 48, 41, 34,
	    27, 20, 13,  6,  7, 14, 21, 28,
	    35, 42, 49, 56, 57, 50, 43, 36,
	    29, 22, 15, 23, 30, 37, 44, 51,
	    58, 59, 52, 45, 38, 31, 39, 46,
	    53, 60, 61, 54, 47, 55, 62, 63
};

/*Pomoæne funkcije za upis u little-endian
static void write_u8(FILE* f, u8 v)
{
    fputc((int)v, f);
}

static void write_s16_le(FILE* f, s16 v)
{
    u16 uv = (u16)v;
    fputc((int)(uv & 0xFF), f);
    fputc((int)((uv >> 8) & 0xFF), f);
}
*/

//Ova funkcija radi: zigzag -> DC diff -> AC RLE -> EOB
void write_block_zigzag_rle(FILE* f, const s16 q[64], s16* prev_dc)
{
	s16 zz[64];

	// zigzag
    for(int i=0; i<64; i++)
    {
       zz[i] = q[ZZ[i]];
    }

    // DC diff
    s16 dc = zz[0];
    s16 dc_diff = (s16)(dc - *prev_dc);
    *prev_dc = dc;

    fprintf(f, "%d ", (int)dc_diff);
    //write_s16_le(f, dc_diff);

    /* AC rle */
    int run=0;
    for(int i=1; i<64; i++)
    {
    	s16 val=zz[i];

    	if(val==0)
    	{
    		run++;
    		continue;
    	}

        //write_u8(f, (u8)run);
        //write_s16_le(f, val);
        fprintf(f, "%u %d ", (unsigned)run, (int)val);
    	run = 0;
    }

    /* EOB-end of block marker*/
     //write_u8(f, 0);
     //write_s16_le(f, 0);
    fprintf(f, "0 0\n");
}
