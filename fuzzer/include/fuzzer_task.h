/*
 * fuzzer_task.h
 *
 *  Created on: Feb 26, 2019
 *      Author: nawro
 */

#ifndef REPOS_UOB_BLUETOOTH_INCLUDE_FUZZER_TASK_H_
#define REPOS_UOB_BLUETOOTH_INCLUDE_FUZZER_TASK_H_

#include "fuzzer_settings.h"
#include "nimble/ble.h"

// Types of fuzzer tasks
#define FUZZER_TASK_TYPE_TX 	0
#define FUZZER_TASK_TYPE_RX		1

// Fuzzer task descriptor return values
/*
 * Complete is a task which just finished
 *
 * Busy is a task which is active but not yet finished
 *
 * Abort is a task which due to unexpected circumstances has to be dropped
 *
 * Inactive is used for when the current task type does not match the
 * function it was called in (TX and expected RX or opposite)
 */
#define FUZZER_TASK_COMPLETE 	0
#define FUZZER_TASK_BUSY 		1
#define FUZZER_TASK_ABORT		2
#define FUZZER_TASK_INACTIVE	3

// Task callback typedefs
typedef int (*fuzzer_task_tx_cb_t)(uint8_t *txdata, uint8_t *payload_len);
typedef int (*fuzzer_task_rx_cb_t)(uint8_t *rxbuf, struct ble_mbuf_hdr *rxhdr);

struct fuzzer_task_t;
struct fuzzer_task_list_t;

// Struct to hold info about a fuzzer task
typedef struct fuzzer_task_t {

	//Pointer to linked list
	struct fuzzer_task_list_t *task_list;

	// Start and end callbacks
	void (*start_cb)(void);
	void (*end_cb)(void);

	// Fuzzer global settings
	fuzzer_settings_t *state_settings;

	/* Descriptor of task. Tasks are described by a function pointer
	 * which defines their behaviour.
	 *
	 * If the task is a tx task the function will be called by the "modifyTxBuffer()"
	 * function in order to modify the outgoing message.
	 *
	 * If the task is a rx task it represents which and or how many received packets
	 * the task is waiting for.
	 */

	fuzzer_task_tx_cb_t tx_task_descriptor;
	fuzzer_task_rx_cb_t rx_task_descriptor;

} fuzzer_task_t;

// Linked list of fuzzer_tasks
typedef struct fuzzer_task_list_t {
	fuzzer_task_t task;
	struct fuzzer_task_list_t *next;
} fuzzer_task_list_t;

#endif /* REPOS_UOB_BLUETOOTH_INCLUDE_FUZZER_TASK_H_ */
