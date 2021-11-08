//
// Created by Gene Fang on 2021/11/5.
//

#ifndef CMAKE_STM32_HAL_H
#define CMAKE_STM32_HAL_H

#if defined(STM32F103xB)
#include "stm32f1xx_hal.h"
#elif defined(STM32F407xx)
#include "stm32f4xx_hal.h"
#elif defined(STM32F411xE)
#include "stm32f4xx_hal.h"
#else
#error "MCU class not supported"
#endif

#endif //CMAKE_STM32_HAL_H
