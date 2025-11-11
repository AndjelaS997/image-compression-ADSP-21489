/*
 * quant.h
 *
 */

#ifndef QUANT_H_
#define QUANT_H_

#pragma once
#include "types_sharc.h"

void quantize8x8(const float F[64], const u8 Q[64], s16 Fq[64]);

#endif /* QUANT_H_ */
