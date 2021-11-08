/*
 * input_button.c
 *
 *  Created on: 2018年2月26日
 *      Author: gene
 */

#include "peripherals/input_button.h"

bool_t _exti_button_init(_exti_button_device_internal_t* device) {
	return device->init_gpio();
}

void _exti_button_pressed(_exti_button_device_internal_t* device, uint_16 pin) {
	if (__HAL_GPIO_EXTI_GET_IT(pin) != RESET) {
	    __HAL_GPIO_EXTI_CLEAR_IT(pin);
	    if (device->action_callbacks[BUTTON_CALLBACK_DOWN] != null) {
	    	device->action_callbacks[BUTTON_CALLBACK_DOWN](device->id);
	    }
	}
}
