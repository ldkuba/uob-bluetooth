/*
 * bitmask.h
 *
 * Simple bit mask implementation which is used to implement filters.
 *
 *  Created on: Mar 4, 2019
 *      Author: nawro
 */
#pragma once

#ifndef REPOS_UOB_BLUETOOTH_INCLUDE_BITMASK_H_
#define REPOS_UOB_BLUETOOTH_INCLUDE_BITMASK_H_

#include <limits.h>
#include <stdint.h>


typedef uint32_t bitmask_t;
#define BITS_PER_WORD (sizeof(bitmask_t) * CHAR_BIT)
#define WORD_OFFSET(b) ((b) / BITS_PER_WORD)
#define BIT_OFFSET(b)  ((b) % BITS_PER_WORD)

//Operations
// Sets a mask bit at a specific index
void setMaskBit(bitmask_t *mask, int n);

// Clears a mask bit at a specific index
void clearMaskBit(bitmask_t *mask, int n);

// Gets the value of the bit at a specific index
int getMaskBit(bitmask_t *mask, int n);

// Clears all mask bits
void clearAllMaskBits(bitmask_t *mask);

// Sets all mask bits
void setAllMaskBits(bitmask_t *mask);

#endif /* REPOS_APACHE_MYNEWT_NIMBLE_UOB_BLUETOOTH_INCLUDE_BITMASK_H_ */
