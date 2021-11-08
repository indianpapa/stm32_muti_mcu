/*
 * oled_gpio.h
 *
 *  Created on: 2018年2月22日
 *      Author: gene
 */

#ifndef DAZE_INC_PERIPHERALS_OLED_GPIO_H_
#define DAZE_INC_PERIPHERALS_OLED_GPIO_H_

#include "base/types.h"
#include "base/core_utils.h"
#include "hal.h"

#define TYPE8X16   1
#define TYPE16X16  2
#define TYPE6X8    3

typedef struct oled_gpio_device {
	DEF_FUNC_POINTER(init_device, bool_t, void);
	DEF_FUNC_POINTER(print, void, uint_8, uint_8, char*, uint_8, uint_8);
	DEF_FUNC_POINTER(setpixel, void, uint_8, uint_8);
	DEF_FUNC_POINTER(appendLine, void, char*);
	char buffer[4][64];
} oled_gpio_device_t;

typedef struct _oled_gpio_device_internal {
	DEF_FUNC_POINTER(init_gpio, bool_t, void);
	DEF_FUNC_POINTER(clk_control, void, GPIO_PinState);
	DEF_FUNC_POINTER(mosi_control, void, GPIO_PinState);
	DEF_FUNC_POINTER(rst_control, void, GPIO_PinState);
	DEF_FUNC_POINTER(dc_control, void, GPIO_PinState);
} _oled_gpio_device_internal_t;

bool_t oled_gpio_device_init(_oled_gpio_device_internal_t*);
void oled_print6x8(_oled_gpio_device_internal_t*, uint_8, uint_8, char*, const char*);
void oled_print8x16(_oled_gpio_device_internal_t*, uint_8, uint_8, char*, const char*);
void oled_print16x16(_oled_gpio_device_internal_t*, uint_8, uint_8, char*, const char*, const char*);
void oled_print(_oled_gpio_device_internal_t*, uint_8, uint_8, char*, uint_8, uint_8);
void oled_put_pixel(_oled_gpio_device_internal_t*, uint_8, uint_8);
void oled_clear(_oled_gpio_device_internal_t*);
void oled_clear_y(_oled_gpio_device_internal_t*, uint_8);
void oled_clear_line_area(_oled_gpio_device_internal_t*, uint_8, uint_8, uint_8);

#define DECLARE_OLED_GPIO_PIN_SETRESET_FUNCS(name, gpiox, scl_pinx, sda_pinx, rst_pinx, dc_pinx)			\
static bool_t _oled_gpio_init_device##name() {																\
	return oled_gpio_device_init(&_inter_##name);                    										\
}																											\
static void _oled_gpio_print##name(uint_8 x, uint_8 y, char* ch, uint_8 char_size, uint_8 ascii_size) {		\
	oled_print(&_inter_##name, x, y, ch, char_size, ascii_size);											\
}																											\
static void _oled_gpio_setpix##name(uint_8 x, uint_8 y) {													\
	oled_put_pixel(&_inter_##name, x, y);																	\
}																											\
static void _oled_gpio_append_line##name(char* line) {														\
	if (core_strcpy(name.buffer[0], name.buffer[1])) {                                                      \
	    oled_print(&_inter_##name, 0, 0, name.buffer[0], TYPE6X8, TYPE8X16);		                        \
    }                                                                                                       \
    if (core_strcpy(name.buffer[1], name.buffer[2])) {                                                      \
	    oled_print(&_inter_##name, 0, 18, name.buffer[1], TYPE6X8, TYPE8X16);		                        \
    }                                                                                                       \
    if (core_strcpy(name.buffer[2], name.buffer[3])) {                                                      \
	    oled_print(&_inter_##name, 0, 35, name.buffer[2], TYPE6X8, TYPE8X16);		                        \
    }                                                                                                       \
    if (core_strcpy(name.buffer[3], line)) {                                                                \
        oled_print(&_inter_##name, 0, 50, name.buffer[3], TYPE6X8, TYPE8X16);                               \
    }                                                                                                       \
}																											\
static void _oled_gpio_clk_control##name(GPIO_PinState state) {												\
	HAL_GPIO_WritePin(gpiox, scl_pinx, state);																\
}																											\
static void _oled_gpio_mosi_control##name(GPIO_PinState state) {											\
	HAL_GPIO_WritePin(gpiox, sda_pinx, state);																\
}																											\
static void _oled_gpio_rst_control##name(GPIO_PinState state) {												\
	HAL_GPIO_WritePin(gpiox, rst_pinx, state);																\
}																											\
static void _oled_gpio_dc_control##name(GPIO_PinState state) {												\
	HAL_GPIO_WritePin(gpiox, dc_pinx, state);																\
}

#define DECLARE_OLED_GPIO_DISPLAYER(name, init_pin, gpiox, scl_pinx, sda_pinx, rst_pinx, dc_pinx)			\
extern _oled_gpio_device_internal_t _inter_##name;															\
DECLARE_OLED_GPIO_PIN_SETRESET_FUNCS(name, gpiox, scl_pinx, sda_pinx, rst_pinx, dc_pinx)					\
_oled_gpio_device_internal_t _inter_##name = {																\
	init_pin,																								\
	_oled_gpio_clk_control##name,																			\
	_oled_gpio_mosi_control##name,																			\
	_oled_gpio_rst_control##name,							\
	_oled_gpio_dc_control##name								\
};															\
oled_gpio_device_t name = {									\
	_oled_gpio_init_device##name,							\
	_oled_gpio_print##name,									\
	_oled_gpio_setpix##name,								\
	_oled_gpio_append_line##name,							\
	{"", "", "", ""}							            \
};

#endif /* DAZE_INC_PERIPHERALS_OLED_GPIO_H_ */
