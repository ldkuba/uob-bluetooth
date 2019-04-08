/*
 * fuzzer_settings.h
 *
 *  Created on: Mar 11, 2019
 *      Author: nawro
 */

#ifndef REPOS_UOB_BLUETOOTH_INCLUDE_FUZZER_SETTINGS_H_
#define REPOS_UOB_BLUETOOTH_INCLUDE_FUZZER_SETTINGS_H_

#include "bitmask.h"

// Defines the global settings of the fuzzer, active during a running task
typedef struct fuzzer_settings_t {

	// RX message filters - currently not used but
	// possibly implemented in the future
	/*
	bitmask_t filterAdvMessagesRX;
	bitmask_t filterDataMessagesRX;
	bitmask_t filterLLCtrlMessagesRX;
	*/

	// TX message filters
	bitmask_t filterAdvMessagesTX;
	bitmask_t filterDataMessagesTX;
	bitmask_t filterLLCtrlMessagesTX;

} fuzzer_settings_t;


#endif /* REPOS_UOB_BLUETOOTH_INCLUDE_FUZZER_SETTINGS_H_ */
