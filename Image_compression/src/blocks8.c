/*
 * blocks8.c
 *  - segmentisanje slike na blokove 8x8
 *  - centriranje vrijednosti piksela oko nule
 */


#include "types_sharc.h"

#include "blocks8.h"

/* ogranici v na [lo, hi] */
static int restrict_(int v, int lo, int hi)
{
	if(v < lo) return lo;
	if(v > hi) return hi;
	return v;
}

int blocks_x(int width)
{
	return (width + 7) / 8;
}

int blocks_y(int height)
{
	return (height + 7) / 8;
}

void extract_block_centered(const u32* pixels, int width, int height, int bx, int by, s16 out[64])
{
	int x0 = bx * 8;
	int y0 = by * 8;

	for(int y=0; y < 8; y++)
	{
		int sy= restrict_(y0 + y, 0, height - 1);

		for(int x=0; x <8; x++)
		{
			int sx= restrict_(x0 + x, 0, width-1);

			/*uzmi Y iz grayscale*/
			u8 Y = (u8)(pixels[sy * width + sx] & 0xFF);

			/*centriraj oko 0*/
			out[y * 8 + x] = (s16)Y - 128;
		}
	}
}

