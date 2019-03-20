/*
 * bitmask.c
 *
 *  Created on: Mar 5, 2019
 *      Author: nawro
 */

#include "bitmask.h"

//Operations
void setMaskBit(bitmask_t *mask, int n)
{
	mask[WORD_OFFSET(n)] |= (1 << BIT_OFFSET(n));
}

void clearMaskBit(bitmask_t *mask, int n)
{
	mask[WORD_OFFSET(n)] &= ~(1 << BIT_OFFSET(n));
}

int getMaskBit(bitmask_t *mask, int n)
{
	return (mask[WORD_OFFSET(n)] & (1 << BIT_OFFSET(n))) != 0;
}

void clearAllMaskBits(bitmask_t *mask)
{
	for(int i = 0; i < BITS_PER_WORD; i++)
	{
		clearMaskBit(mask, i);
	}
}

void setAllMaskBits(bitmask_t *mask)
{
	for(int i = 0; i < BITS_PER_WORD; i++)
	{
		setMaskBit(mask, i);
	}
}
