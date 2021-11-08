/*
 * sysclk_impl.c
 *
 *  Created on: 2018年2月18日
 *      Author: gene
 */

#include "common/sysclk.h"
#include "hal.h"

bool_t sysclk_config() {
	
}

#if defined(STM32F103xB)
bool_t sysclk_config() {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

#if defined(STM32F407xx)
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
#elif defined(STM32F411xE)
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
#endif

	/**Initializes the CPU, AHB and APB busses clocks */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; // 外部晶振，8MHz
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		return false;
	}

	 /**Initializes the CPU, AHB and APB busses clocks */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
		|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHB时钟 ：48MHz
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;               // APB2时钟：48MHz

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		return false;
	}

	return true;
}
#endif

#ifdef STM32F407xx

#endif
