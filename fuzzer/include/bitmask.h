/*
 * bitmask.h
 *
 *  Created on: Mar 4, 2019
 *      Author: nawro
 */
#pragma once

#ifndef REPOS_APACHE_MYNEWT_NIMBLE_UOB_BLUETOOTH_INCLUDE_BITMASK_H_
#define REPOS_APACHE_MYNEWT_NIMBLE_UOB_BLUETOOTH_INCLUDE_BITMASK_H_

#include <limits.h>
#include <stdint.h>


typedef uint32_t bitmask_t;
#define BITS_PER_WORD (sizeof(bitmask_t) * CHAR_BIT)
#define WORD_OFFSET(b) ((b) / BITS_PER_WORD)
#define BIT_OFFSET(b)  ((b) % BITS_PER_WORD)

//Operations
void setMaskBit(bitmask_t *mask, int n);

void clearMaskBit(bitmask_t *mask, int n);

int getMaskBit(bitmask_t *mask, int n);

void clearAllMaskBits(bitmask_t *mask);

void setAllMaskBits(bitmask_t *mask);

#endif /* REPOS_APACHE_MYNEWT_NIMBLE_UOB_BLUETOOTH_INCLUDE_BITMASK_H_ */
