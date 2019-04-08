/*
 * fuzzer.c
 *
 *  Created on: Feb 21, 2019
 *      Author: nawro
 */

#include "fuzzer.h"
#include "console/console.h"

void initFuzzer(struct log *logger, fuzzer_task_list_t *taskList)
{
	fuzzer_log = logger;

	if(taskList == NULL)
	{
		fuzzerStatus = FUZZER_EXIT_ABORT;
		return;
	}

	// Initialise the first task
	tasks = taskList;
	currentTask = taskList;
	settings = *(currentTask->task.state_settings);

	if(currentTask->task.start_cb != NULL)
	{
		currentTask->task.start_cb();
	}

	fuzzerStatus = FUZZER_RUNNING;
}


void cleanupFuzzer()
{
	currentTask = tasks;

	while(currentTask != NULL)
	{
		free(currentTask->task.state_settings);

		fuzzer_task_list_t *tmp = currentTask->next;
		free(currentTask);
		currentTask = tmp;
	}
}

void modifyName(uint8_t *txdata, uint8_t *payload_len, const char* param_name, uint8_t length)
{
	//Start of actual string not the segment
	int nameStart = 18;
	int originalNameLength = 14;
	// Name string and type
	int nameLength = length;
	char name[nameLength];

	for(int i = 0; i < nameLength; i++)
	{
		name[i] = *(param_name + i);
	}

	int bufferOffset = originalNameLength - nameLength;

	// Set name segment length
	txdata[nameStart-2] = nameLength + 1;

	// Make space in the buffer if the name is longer than original
	if(bufferOffset < 0)
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

	for(int i = nameStart + nameLength; i < *payload_len + 2 - (-bufferOffset); i++)
	{
		txdata[i] = txdata[i + bufferOffset];
	}

	//set new payload length
	*payload_len -= bufferOffset;
	txdata[1] = *payload_len;
}

// Clears a buffer for filtering
void clearBuffer(uint8_t *txdata, uint8_t *payload_len)
{
	for(int i = 0; i < *payload_len; i++)
	{
		txdata[i + 2] = 0;
	}

	txdata[0] = 0;
	txdata[1] = 0;
	*payload_len = 0;
}

int isInConnState(void)
{
	int rc;
	uint8_t txChannel = -1;

	rc = ble_phy_getchan(&txChannel);
	if(rc != 0 || txChannel < 0)
	{
		return -1;
	}

	if(txChannel == 37 || txChannel == 38 || txChannel == 39)
	{
		return 0;
	}else
	{
		return 1;
	}
}

void filterMessagesTx(uint8_t *txdata, uint8_t *payload_len)
{
	int rc;
	uint8_t txChannel = -1;
	int enabled;

	rc = ble_phy_getchan(&txChannel);
	if(rc != 0 || txChannel < 0)
	{
		return;
	}

	// Determine what type of message is going to be sent
	if(txChannel == 37 || txChannel == 38 || txChannel == 39)
	{
		// Advertising message
		uint8_t firstByte = txdata[0];
		uint8_t packetType = firstByte & 15;

		enabled = getMaskBit(&settings.filterAdvMessagesTX, packetType);
		if(enabled)
		{
			clearBuffer(txdata, payload_len);
		}

	}else
	{
		// Data message
		uint8_t hdr_byte = txdata[0];
		uint8_t dataPacketType = (hdr_byte & BLE_LL_DATA_HDR_LLID_MASK);

		//LL_CTRL message
		if(dataPacketType == LLCTRL)
		{
			// Check operation codes if is LLCTRL packet
			uint8_t opcode = txdata[2];
			enabled = getMaskBit(&settings.filterLLCtrlMessagesTX, opcode);
			if(enabled)
			{
				clearBuffer(txdata, payload_len);
			}
		}

		enabled = getMaskBit(&settings.filterDataMessagesTX, dataPacketType);
		if(enabled)
		{
			clearBuffer(txdata, payload_len);
		}
	}
}

void modifyTxBuffer(uint8_t *txdata, uint8_t *payload_len)
{
	int result = FUZZER_TASK_INACTIVE;

	// Run the current task
	if(currentTask->task.tx_task_descriptor != NULL)
	{
		result = currentTask->task.tx_task_descriptor(txdata, payload_len);
	}

	switch(result)
	{
	case FUZZER_TASK_COMPLETE:

		if(currentTask->task.end_cb != NULL)
		{
			currentTask->task.end_cb();
		}

		// Switch to next task or end if it was the last
		if(currentTask->next != NULL)
		{
			// Switch to next task
			currentTask = currentTask->next;

			// update global settings
			settings = *(currentTask->task.state_settings);

			// Run the starting callback
			if(currentTask->task.start_cb)
			{
				currentTask->task.start_cb();
			}
		}else
		{
			// Exit the fuzzer with a success code, all tasks completed
			cleanupFuzzer();
			fuzzerStatus = FUZZER_EXIT_SUCCESS;
		}

		break;

	case FUZZER_TASK_BUSY:
		filterMessagesTx(txdata, payload_len);
		break;
	case FUZZER_TASK_ABORT:
		cleanupFuzzer();
		fuzzerStatus = FUZZER_EXIT_ABORT;
		break;
	case FUZZER_TASK_INACTIVE:
		// Filter messages according to settings
		filterMessagesTx(txdata, payload_len);
		break;

	default:
		break;
	}
}

void notifyFuzzerRx(uint8_t *rxbuf, struct ble_mbuf_hdr *rxhdr)
{
	int result = FUZZER_TASK_INACTIVE;

	if(currentTask->task.rx_task_descriptor != NULL)
	{
		result = currentTask->task.rx_task_descriptor(rxbuf, rxhdr);
	}

	switch(result)
	{
	case FUZZER_TASK_COMPLETE:

		if(currentTask->task.end_cb != NULL)
		{
			currentTask->task.end_cb();
		}

		if(currentTask->next != NULL)
		{
			currentTask = currentTask->next;

			settings = *(currentTask->task.state_settings);

			if(currentTask->task.start_cb)
			{
				currentTask->task.start_cb();
			}
		}else
		{
			cleanupFuzzer();
			fuzzerStatus = FUZZER_EXIT_SUCCESS;
		}

		break;

	case FUZZER_TASK_BUSY:
		break;
	case FUZZER_TASK_ABORT:
		cleanupFuzzer();
		fuzzerStatus = FUZZER_EXIT_ABORT;
		break;
	case FUZZER_TASK_INACTIVE:
		//Possibly filter incoming messages in the future.
		//This will require some more digging
		break;

	}
}
