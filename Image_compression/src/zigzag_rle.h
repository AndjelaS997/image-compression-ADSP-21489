/*
 * zigzag_rle.h
 *
 */

#ifndef ZIGZAG_RLE_H_
#define ZIGZAG_RLE_H_

#include "types_sharc.h"
#include <stdio.h>

void write_block_zigzag_rle(FILE* f, const s16 q[64], s16* prev_dc);

#endif /* ZIGZAG_RLE_H_ */
