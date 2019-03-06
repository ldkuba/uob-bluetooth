/*
 * fuzzer.c
 *
 *  Created on: Feb 21, 2019
 *      Author: nawro
 */

#include "fuzzer.h"
#include "console/console.h"

void initFuzzer(struct log *logger)
{
	fuzzer_log = logger;
}

void modifyName(uint8_t *txdata, uint8_t *payload_len)
{
	//Start of actual string not the segment
	uint8_t nameStart = 18;
	uint8_t originalNameLength = 14;
	// Name string and type
	uint8_t nameLength = 7;
	char name[] = {'A', 'L', 'I', ' ', 'Z', 'A', 'B'};

	uint8_t bufferOffset = originalNameLength - nameLength;

	// Set name segment length
	txdata[nameStart-2] = nameLength + 1;

	// Make space in the buffer if the name is longest than original
	if(bufferOffset > 0)
	{
		for(int i = *payload_len + 2 + (-bufferOffset); i >= nameStart + nameLength; i--)
		{
			txdata[i] = txdata[i - (-bufferOffset)];
		}
	}

	// Override with new name
	for(int i = 0; i < nameLength; i++)
	{
		txdata[nameStart + i] = name[i];
	}

	// If name was shorter than original pull up buffer
	for(int i = nameStart + nameLength; i < *payload_len + 2 - bufferOffset; i++)
	{
		txdata[i] = txdata[i + bufferOffset];
	}

	//set new payload length
	*payload_len -= bufferOffset;
	txdata[1] = *payload_len;
}

void modifyTxBuffer(uint8_t *txdata, uint8_t *payload_len)
{
	//uint8_t currentChannel;

	//ble_phy_getchan(&currentChannel);

	uint8_t firstByte = txdata[0];
	uint8_t packetType = firstByte & 15;

	switch(packetType) {
		case ADV_IND:
			//Modify name to test
			modifyName(txdata, payload_len);
			break;

		default:
			break;
	}

}

void notifyFuzzerRx(uint8_t *rxbuf, struct ble_mbuf_hdr *rxhdr)
{

}
