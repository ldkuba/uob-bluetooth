/*
 * fuzzer.h
 *
 *  Created on: Feb 21, 2019
 *      Author: nawro
 */

#ifndef REPOS_UOB_BLUETOOTH_INCLUDE_FUZZER_H_
#define REPOS_UOB_BLUETOOTH_INCLUDE_FUZZER_H_

#include <stdint.h>
#include "log/log.h"

#include "fuzzer_task.h"

#include "bitmask.h"
#include "controller/ble_phy.h"
#include "controller/ble_ll.h"

// Logging - currently not supported. Is known to break the fuzzer
struct log *fuzzer_log;
#define FUZZER_LOG(lvl, ...) \
    LOG_ ## lvl(fuzzer_log, LOG_MODULE_PERUSER + 0, __VA_ARGS__)

// Fuzzer status codes
#define FUZZER_RUNNING 			0
#define FUZZER_EXIT_SUCCESS		1
#define FUZZER_EXIT_ABORT 		2

//PDU Type - 4 bits
enum BleAdvPacketType {
	ADV_IND = 0x0,
	ADV_DIRECT_IND = 0x1,
	ADV_NONCONN_IND = 0x2,
	SCAN_REQ = 0x3,
	SCAN_RSP = 0x4,
	CONNECT_REQ = 0x5,
	ADV_SCAN_IND = 0x6,
	ADV_EXT_IND = 0x7,
	AUX_CONNECT_RSP = 0x8
};

//LLID - 2 bits
enum BleDataPacketType {
	RESERVED = 0,
	L2CAP_CONT = 1,
	L2CAP_START = 2,
	// LLCTRL is expanded by the opcode in the BleLLCtrlPacketType
	LLCTRL = 3
};

// LL Control Operation code - 8 bits
enum BleLLCtrlPacketType {
	LL_CONNECTION_UPDATE_IND = 0x00,
	LL_CHANNEL_MAP_IND = 0x01,
	LL_TERMINATE_IND = 0x02,
	LL_ENC_REQ = 0x03,
	LL_ENC_RSP = 0x04,
	LL_START_ENC_REQ = 0x05,
	LL_START_ENC_RSP = 0x06,
	LL_UNKNOWN_RSP = 0x07,
	LL_FEATURE_REQ = 0x08,
	LL_FEATURE_RSP = 0x09,
	LL_PAUSE_ENC_REQ = 0x0A,
	LL_PAUSE_ENC_RSP = 0x0B,
	LL_VERSION_IND = 0x0C,
	LL_REJECT_IND = 0x0D,
	LL_SLAVE_FEATURE_REQ = 0x0E,
	LL_CONNECTION_PARAM_REQ = 0x0F,
	LL_CONNECTION_PARAM_RSP = 0x10,
	LL_REJECT_EXT_IND = 0x11,
	LL_PING_REQ = 0x12,
	LL_PING_RSP = 0x13,
	LL_LENGTH_REQ = 0x14,
	LL_LENGTH_RSP = 0x15,
	LL_PHY_REQ = 0x16,
	LL_PHY_RSP = 0x17,
	LL_PHY_UPDATE_IND = 0x18,
	LL_MIN_USED_CHANNELS_IND = 0x19,
	LL_CTE_REQ = 0x1A,
	LL_CTE_RSP = 0x1B,
	LL_PERIODIC_SYNC_IND = 0x1C,
	LL_CLOCK_ACCURACY_REQ = 0x1D,
	LL_CLOCK_ACCURACY_RSP = 0x1E,
	// All values from 0x1F - 0xFF re reserved
	LL_RESERVED = 0x1F
};

// Fuzzer data
// Holds the information about the status of the fuzzer
int fuzzerStatus;

// Holds global setting of the fuzzer currently in use
fuzzer_settings_t settings;

// Pointer to the list of tasks being executed by the fuzzer
fuzzer_task_list_t *tasks;

// Pointer to the current task being executed
fuzzer_task_list_t *currentTask;

// Initializes the fuzzer and sets up the first task
void initFuzzer(struct log *logger, fuzzer_task_list_t *taskList);

// Performs clean up on all fuzzer related code. Frees all components of the fuzzer tasks created by the user
void cleanupFuzzer();

// Auxiliary function modifying the name of a device in a ADV_IND packet
void modifyName(uint8_t *txdata, uint8_t *payload_len, const char* param_name, uint8_t length);

// Checks if the packet which is being sent originates from a connection context
int isInConnState(void);

// Sets message length to 0 and everything else to 0 if the message
// type should be filtered, otherwise does nothing
void filterMessagesTx(uint8_t *txdata, uint8_t *payload_len);

// Tx intercept
void modifyTxBuffer(uint8_t *txdata, uint8_t *payload_len);

// Rx intercept
void notifyFuzzerRx(uint8_t *rxbuf, struct ble_mbuf_hdr *rxhdr);

#endif /* REPOS_UOB_BLUETOOTH_INCLUDE_FUZZER_H_ */
