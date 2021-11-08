/*
 * systick.h
 *
 *  Created on: 2017年9月4日
 *      Author: gene
 */

#ifndef CORE_INC_COMMON_SYSTICK_H_
#define CORE_INC_COMMON_SYSTICK_H_

#include "base/types.h"

/** Initialize systick with a callback.*/
void systick_initialize(callback_void_void);

/** Post delay in us.*/
void systick_postdelay(uint_32 us);

/** Get left time to trigger callback in us.*/
uint_32 systick_get_timeleft();

/** To clear timer.*/
void systick_clear();

#endif /* CORE_INC_COMMON_SYSTICK_H_ */
