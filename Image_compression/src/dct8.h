/*
 * dct8.h
 *
 */

#ifndef DCT8_H_
#define DCT8_H_

#include "types_sharc.h"

void dct8x8_init(void);
void dct8x8(const s16 in[64], float out[64]);

#endif /* DCT8_H_ */
