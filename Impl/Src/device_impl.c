/*
 * device.c
 *
 *  Created on: 2018年2月24日
 *      Author: gene
 */

#include "device.h"
#include "hal.h"

static bool_t GPIOA_CLK_ENABLED = false;
static bool_t GPIOB_CLK_ENABLED = false;
static bool_t GPIOC_CLK_ENABLED = false;

//GPIOB Pin6,7,8,9 作为OLED 输出
static bool_t OLED_0_95_INCH_gpio_init_func() {
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	if (!GPIOB_CLK_ENABLED) {
		GPIOB_CLK_ENABLED = true;
	    __HAL_RCC_GPIOB_CLK_ENABLE();
	}

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);

	/*Configure GPIO pins : PB6 PB7 PB8 PB9 */
	GPIO_InitStruct.Pin =  GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	return true;
}

//需要四个GPIO引脚，分别对应SPI的SCL,SDA,RST,DC控制线
DECLARE_OLED_GPIO_DISPLAYER(OLED_0_95_INCH, OLED_0_95_INCH_gpio_init_func, GPIOB, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9)

// GPIOB Pin5 作为GPIO下拉输入
static bool_t GPIO_INPUT_init_func() {
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	if (!GPIOB_CLK_ENABLED) {
		GPIOB_CLK_ENABLED = true;
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}

	GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    return true;
}

DECLARE_GPIO_IN(GPIO_INPUT, GPIO_INPUT_init_func, GPIOB, GPIO_PIN_5);

// GPIOA pin 0 作为下拉中断输入
static bool_t EXIT_BUTTON_A_gpio_init_func() {
    GPIO_InitTypeDef GPIO_InitStruct;

    if (!GPIOA_CLK_ENABLED) {
        GPIOA_CLK_ENABLED = true;
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    return true;
}

// GPIOA pin 1 作为下拉中断输入
static bool_t EXIT_BUTTON_B_gpio_init_func() {
    GPIO_InitTypeDef GPIO_InitStruct;

    if (!GPIOA_CLK_ENABLED) {
        GPIOA_CLK_ENABLED = true;
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }

    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    return true;
}

DECLARE_EXTI_BUTTON(BUTTON_A, 0, EXIT_BUTTON_A_gpio_init_func, EXTI0, GPIO_PIN_0)

DECLARE_EXTI_BUTTON(BUTTON_B, 1, EXIT_BUTTON_B_gpio_init_func, EXTI1, GPIO_PIN_1)

// GPIOB pin 9 作为浮空输出
static bool_t GPIO_OUT_A_gpio_init_func() {
    GPIO_InitTypeDef GPIO_InitStruct;

    if (!GPIOC_CLK_ENABLED) {
        GPIOC_CLK_ENABLED = true;
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    /* 初始化LED1对应引脚IO */
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    return true;
}

DECLARE_GPIO_OUT(GPIO_OUT, GPIO_OUT_A_gpio_init_func, GPIOC, GPIO_PIN_13)