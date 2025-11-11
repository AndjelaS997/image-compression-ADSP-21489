/*
 * blocks8.h
 *
 */

#ifndef BLOCKS8_H_
#define BLOCKS8_H_

/*pomocne - da ynam koliko blokova imam*/
int blocks_x(int width);
int blocks_y(int height);

void extract_block_centered(const u32* pixels, int width, int height, int bx, int by, s16 out[64]);

#endif /* BLOCKS8_H_ */
