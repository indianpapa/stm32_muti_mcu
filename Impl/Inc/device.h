/*
 * device.h
 *
 *  Created on: 2018年2月24日
 *      Author: gene
 */

#ifndef INC_DEVICES_DEVICE_H_
#define INC_DEVICES_DEVICE_H_

#include "peripherals/oled_gpio.h"
#include "peripherals/input_gpio.h"
#include "peripherals/input_button.h"
#include "peripherals/output_gpio.h"

// Define OLED_0_95_INCH
extern oled_gpio_device_t OLED_0_95_INCH;

extern gpio_in_t GPIO_INPUT;

extern exti_button_device_t BUTTON_A;

extern exti_button_device_t BUTTON_B;

extern gpio_out_t GPIO_OUT;

#endif /* INC_DEVICES_DEVICE_H_ */
