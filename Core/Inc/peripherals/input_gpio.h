/*
 * input_gpio.h
 *
 *  Created on: 2018年7月9日
 *      Author: gene
 */

#ifndef DAZE_INC_PERIPHERAL_SUPPORT_INPUT_GPIO_H_
#define DAZE_INC_PERIPHERAL_SUPPORT_INPUT_GPIO_H_

#include "base/types.h"
#include "hal.h"

typedef struct gpio_in {
	DEF_FUNC_POINTER(init_device, bool_t, void);
    DEF_FUNC_POINTER(get_state, GPIO_PinState, void);
} gpio_in_t;

#define DECLARE_GPIO_IN_FUNCS(name, gpio, pin)				\
static GPIO_PinState _gpio_in_get_state_##name() {			\
	return HAL_GPIO_ReadPin(gpio, pin);						\
}

#define DECLARE_GPIO_IN(name, init_gpio, gpio, pin)			\
DECLARE_GPIO_IN_FUNCS(name, gpio, pin)						\
gpio_in_t name = {											\
	init_gpio,												\
	_gpio_in_get_state_##name								\
};

#endif /* DAZE_INC_PERIPHERAL_SUPPORT_INPUT_GPIO_H_ */
