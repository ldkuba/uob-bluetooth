/*
 * fuzzer_task.h
 *
 *  Created on: Feb 26, 2019
 *      Author: nawro
 */

#ifndef REPOS_APACHE_MYNEWT_NIMBLE_UOB_BLUETOOTH_INCLUDE_FUZZER_TASK_H_
#define REPOS_APACHE_MYNEWT_NIMBLE_UOB_BLUETOOTH_INCLUDE_FUZZER_TASK_H_

// Types of fuzzer tasks
#define FUZZER_TASK_TYPE_TX 	0
#define FUZZER_TASK_TYPE_RX		1

// Task callback typedefs
typedef uint8_t (*fuzzer_task_tx_cb_t)(uint8_t *txdata, uint8_t payload_len);
typedef uint8_t (*fuzzer_task_rx_cb_t)(void);

// Struct to hold info about a fuzzer "state" or "task"
typedef struct fuzzer_task {

	// Start and end callbacks
	void* start_cb;
	void* end_cb;

	// Fuzzer global settings
	fuzzer_settings* state_settings;

	/* TODO: Descriptor of task. Tasks are described by a function pointer
	 * which defines its behaviour.
	 *
	 * If the task is a tx task the function will be called by the "modifyTxBuffer()"
	 * function in order to modify the outgoing message.
	 *
	 * If the task is a rx task it represents which and or how many received packets
	 * the task is waiting for.
	 */
	uint8_t task_type;
	fuzzer_task_tx_cb_t tx_task_descriptor;
	fuzzer_task_tx_cb_t rx_task_descriptor;

} fuzzer_task;

#endif /* REPOS_APACHE_MYNEWT_NIMBLE_UOB_BLUETOOTH_INCLUDE_FUZZER_TASK_H_ */
