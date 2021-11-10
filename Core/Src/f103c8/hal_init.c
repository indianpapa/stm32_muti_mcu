/*
 * hal_init.c
 *
 *  Created on: 2018年7月10日
 *      Author: gene
 */

#include "hal.h"

void HAL_MspInit(void) {
	/* USER CODE BEGIN MspInit 0 */

	/* USER CODE END MspInit 0 */

	__HAL_RCC_AFIO_CLK_ENABLE();

    __HAL_RCC_PWR_CLK_ENABLE();

	/**DISABLE: JTAG-DP Disabled and SW-DP Disabled*/
	__HAL_AFIO_REMAP_SWJ_DISABLE();	
}

void GPIO_RCC_config() {
	__HAL_RCC_GPIOD_CLK_ENABLE();
}