/*
 * output_gpio.h
 *
 *  Created on: 2018年2月27日
 *      Author: gene
 */

#ifndef CORE_PERIPHERALS_OUTPUT_GPIO_H_
#define CORE_PERIPHERALS_OUTPUT_GPIO_H_

#include "base/types.h"
#include "hal.h"

typedef struct gpio_out {
	DEF_FUNC_POINTER(init_device, bool_t, void);
    DEF_FUNC_POINTER(set_state, void, GPIO_PinState);
    GPIO_PinState state;
} gpio_out_t;

#define DECLARE_GPIO_OUT_FUNCS(name, gpio, pin)					\
static void _gpio_out_set_state_##name(GPIO_PinState state) {	\
	name.state = state;											\
	HAL_GPIO_WritePin(gpio, pin, state);						\
}

#define DECLARE_GPIO_OUT(name, init_gpio, gpio, pin)		\
DECLARE_GPIO_OUT_FUNCS(name, gpio, pin)						\
gpio_out_t name = {											\
	init_gpio,												\
	_gpio_out_set_state_##name,								\
	GPIO_PIN_RESET											\
};

#endif /* CORE_PERIPHERALS_OUTPUT_GPIO_H_ */
