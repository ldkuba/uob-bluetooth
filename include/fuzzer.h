/*
 * fuzzer.h
 *
 *  Created on: Feb 21, 2019
 *      Author: nawro
 */

#ifndef REPOS_APACHE_MYNEWT_NIMBLE_UOB_BLUETOOTH_INCLUDE_FUZZER_H_
#define REPOS_APACHE_MYNEWT_NIMBLE_UOB_BLUETOOTH_INCLUDE_FUZZER_H_

#include <stdint.h>
#include "log/log.h"

// Logging
struct log *fuzzer_log;
#define FUZZER_LOG(lvl, ...) \
    LOG_ ## lvl(fuzzer_log, LOG_MODULE_PERUSER + 0, __VA_ARGS__)

enum BlePacketType {
	ADV_IND = 0x0,
};

void initFuzzer();
void modifyName(uint8_t *txdata, uint8_t *payload_len);
void modifyTxBuffer(uint8_t *txdata, uint8_t *payload_len);


#endif /* REPOS_APACHE_MYNEWT_NIMBLE_UOB_BLUETOOTH_INCLUDE_FUZZER_H_ */
