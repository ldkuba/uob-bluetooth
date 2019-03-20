/*
 * fuzzer_settings.h
 *
 *  Created on: Mar 11, 2019
 *      Author: nawro
 */

#ifndef REPOS_APACHE_MYNEWT_NIMBLE_UOB_BLUETOOTH_INCLUDE_FUZZER_SETTINGS_H_
#define REPOS_APACHE_MYNEWT_NIMBLE_UOB_BLUETOOTH_INCLUDE_FUZZER_SETTINGS_H_

#include "bitmask.h"

typedef struct fuzzer_settings_t {

	uint8_t channel;

	// RX message filter - currently not used but
	// possibly implemented in the future
	/*
	bitmask_t filterAdvMessagesRX;
	bitmask_t filterDataMessagesRX;
	bitmask_t filterLLCtrlMessagesRX;
	*/

	// TX message filter
	bitmask_t filterAdvMessagesTX;
	bitmask_t filterDataMessagesTX;
	bitmask_t filterLLCtrlMessagesTX;

} fuzzer_settings_t;


#endif /* REPOS_APACHE_MYNEWT_NIMBLE_UOB_BLUETOOTH_INCLUDE_FUZZER_SETTINGS_H_ */
