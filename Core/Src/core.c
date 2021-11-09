/*
 * daze.c
 *
 *  Created on: 2018年2月15日
 *      Author: gene
 */

#include "core.h"
#include "hal.h"
#include "common/sysclk.h"
#include "callback_scheduler.h"

void core_init() {
	HAL_Init();
	
	// Config the system clock
	sysclk_config();

    // configure outer RCC on gpio
	GPIO_RCC_config();

	// Init the scheduler
	initialze_scheduler();
}
