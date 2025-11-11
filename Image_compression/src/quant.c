/*
 *  quant.c
 *
 */

#include "quant.h"

// ispravno zaokruživanje i za + i za - brojeve
static s16 round_to_s16(float x)
{
    if (x >= 0.0)
    	return (s16)(x + 0.5);
    else
    	return (s16)(x - 0.5);
}

/* Kvantizacija: q[i] = round(F[i] / Q[i]) */
void quantize8x8(const float F[64], const u8 Q[64], s16 Fq[64])
{
	for(int i = 0; i < 64; i++)
	{
		Fq[i] = round_to_s16(F[i] / (float)Q[i]);
	}
}
