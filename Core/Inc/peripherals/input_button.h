/*
 * input_button.h
 *
 *  Created on: 2018年2月25日
 *      Author: gene
 */

#ifndef CORE_INC_PERIPHERALS_INPUT_BUTTON_H_
#define CORE_INC_PERIPHERALS_INPUT_BUTTON_H_

#include "base/types.h"
#include "hal.h"

#define BUTTON_CALLBACK_DOWN 0
#define BUTTON_CALLBACK_UP 1

TYPEDEF_FUNC_POINTER(button_callback, void, uint_8);

typedef struct exti_button_device {
	DEF_FUNC_POINTER(init_device, bool_t, void);
	/*action: one of BUTTON_CALLBACK_DOWN, BUTTON_CALLBACK_UP.*/
	DEF_FUNC_POINTER(set_action_callback, void, uint_8/*action*/, button_callback/*cb*/);
	DEF_FUNC_POINTER(is_pressed, bool_t, void);
} exti_button_device_t;

typedef struct _exti_button_device_internal {
	uint_8 id;
	DEF_FUNC_POINTER(init_gpio, bool_t, void);
	button_callback action_callbacks[2];
} _exti_button_device_internal_t;

bool_t _exti_button_init(_exti_button_device_internal_t*);

void _exti_button_pressed(_exti_button_device_internal_t*, uint_16);

#define DECLARE_EXTI_BTN_FUNCS(name, EXTIn, pin)									\
void EXTIn##_IRQHandler(void) {														\
	_exti_button_pressed(&_inter_##name, pin);										\
}																					\
static bool_t _exti_btn_init_device##name(void)	{									\
	return _exti_button_init(&_inter_##name);										\
}																					\
static void _exti_btn_set_action_cb_##name(uint_8 action, button_callback cb) {		\
	_inter_##name.action_callbacks[action] = cb;									\
}																					\
static bool_t _exti_btn_is_pressed_##name() {										\
	return false;  /*TODO: implement*/												\
}

#define DECLARE_EXTI_BUTTON(name, ID, init_gpio, EXTIn, pin)	\
extern _exti_button_device_internal_t _inter_##name;			\
DECLARE_EXTI_BTN_FUNCS(name, EXTIn, pin)						\
_exti_button_device_internal_t _inter_##name = {				\
	ID,															\
	init_gpio,													\
	{null, null}												\
};																\
exti_button_device_t name = {									\
    _exti_btn_init_device##name,								\
	_exti_btn_set_action_cb_##name,								\
	_exti_btn_is_pressed_##name									\
};

#endif /* CORE_INC_PERIPHERALS_INPUT_BUTTON_H_ */
