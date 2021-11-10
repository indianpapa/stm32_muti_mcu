#ifndef CORE_PERIPHERALS_OUTPUT_PWM_H_
#define CORE_PERIPHERALS_OUTPUT_PWM_H_

#include "base/types.h"
#include "hal.h"

typedef struct pwm_out {
	DEF_FUNC_POINTER(init_device, bool_t, void);
    DEF_FUNC_POINTER(start, void, void);
	DEF_FUNC_POINTER(stop, void, void);
	DEF_FUNC_POINTER(setCompare, void, uint_16);
} pwm_out_t;

#define DECLARE_PWM_OUT_FUNCS(name, htimx, CHAL)				\
static void _gpio_out_start_##name() {							\
	HAL_TIM_PWM_Start(&htimx, CHAL);							\
}																\
static void _pwm_out_stop_##name() {							\
	HAL_TIM_PWM_Stop(&htimx, CHAL);								\
}																\
static void _pwm_out_setCompare_##name(uint_16 val) {			\
	__HAL_TIM_SetCompare(&htimx, CHAL, val);					\
}

#define DECLARE_PWM_OUT(name, init_pwm, htimx, CHAL)		\
DECLARE_PWM_OUT_FUNCS(name, htimx, CHAL)					\
pwm_out_t name = {											\
	init_pwm,												\
	_gpio_out_start_##name,									\
	_pwm_out_stop_##name,									\
	_pwm_out_setCompare_##name								\
};

#endif //CORE_PERIPHERALS_OUTPUT_PWM_H_