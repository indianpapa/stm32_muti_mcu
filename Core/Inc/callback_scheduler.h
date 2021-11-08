/*
 * callback_scheduler.h
 *
 *  Created on: 2018年1月26日
 *      Author: gene
 */

#ifndef DAZE_INC_CALLBACK_SCHEDULER_H_
#define DAZE_INC_CALLBACK_SCHEDULER_H_

#include "base/types.h"

// Define MAX_SCHEDULER_CALLBACK_COUNT in makefine

bool_t initialze_scheduler();

bool_t callback_scheduler_postdely_s(callback_void_void /*callback*/, uint_32 /*delay time in s*/);

bool_t callback_scheduler_postdely_ms(callback_void_void /*callback*/, uint_32 /*delay time in ms*/);

bool_t callback_scheduler_postdely_us(callback_void_void /*callback*/, uint_32 /*delay time in us*/);

void delay_s(uint_32);

void delay_ms(uint_32);

#endif /* DAZE_INC_CALLBACK_SCHEDULER_H_ */
