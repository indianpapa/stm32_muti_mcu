/*
 * daze.c
 *
 *  Created on: 2018年2月15日
 *      Author: gene
 */

#include "core.h"
#include "common/sysclk.h"
#include "callback_scheduler.h"

void core_init() {
	// Config the system clock
	sysclk_config();

	// Init the scheduler
	initialze_scheduler();
}
