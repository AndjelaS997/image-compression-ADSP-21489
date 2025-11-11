/*
 * rgb_gray.c
 *
 */

#include "rgb_gray.h"
#include "types_sharc.h"


void rgb_to_gray(u32* pixels, int width, int height)
{
	//printf("Pocinje konvertovanje iz rgb u gray.\n");

	int n = width * height;
	for(int i=0; i < n; i++)
	{
		int R = (pixels[i] >> 16)& 0xFF;
		int G = (pixels[i] >> 8)& 0xFF;
		int B = (pixels[i])& 0xFF;

        int Y = (77*R + 150*G + 29*B + 128) >> 8;  // 0..255
        pixels[i] = ((u32)Y << 16) | ((u32)Y << 8) | (u32)Y;
	}
	//printf("Zavrsena konvertovanje iz rgb u gray.\n");
}
