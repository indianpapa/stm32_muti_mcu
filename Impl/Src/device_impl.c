/*
 * device.c
 *
 *  Created on: 2018年2月24日
 *      Author: gene
 */

#include "device.h"
#include "hal.h"

// GPIO CLK
static bool_t s_GpioA_Clk_Enabled = false;
static bool_t s_GpioB_Clk_Enabled = false;
static bool_t s_GpioC_Clk_Enabled = false;

// TIM
static bool_t s_Pwm_Tim2_Initialized = false;

//GPIOB Pin6,7,8,9 作为OLED 输出
static bool_t OLED_0_95_INCH_gpio_init_func() {
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	if (!s_GpioB_Clk_Enabled) {
		s_GpioB_Clk_Enabled = true;
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

// GPIOA pin 0 作为下拉中断输入
static bool_t EXIT_BUTTON_A_gpio_init_func() {
    GPIO_InitTypeDef GPIO_InitStruct;

    if (!s_GpioA_Clk_Enabled) {
        s_GpioA_Clk_Enabled = true;
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

    if (!s_GpioA_Clk_Enabled) {
        s_GpioA_Clk_Enabled = true;
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

    if (!s_GpioC_Clk_Enabled) {
        s_GpioC_Clk_Enabled = true;
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

TIM_HandleTypeDef htim2;

static bool_t PWM_TIM2_Init() {
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = G_Rcc_Frequency - 1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	// PWM freq = 1000k / Period
    htim2.Init.Period = 20000;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		return false;
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		return false;
    }

    if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
		return false;
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
		return false;
    }

	__HAL_RCC_TIM2_CLK_ENABLE();

	return true;
}

static bool_t PWM_OUT_A_init_func() {
	TIM_OC_InitTypeDef sConfigOC = {0};
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (!s_Pwm_Tim2_Initialized) {
		s_Pwm_Tim2_Initialized = true;
		PWM_TIM2_Init();
	}

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
		return false;
    }

	// TIM2_CHANNEL_2 -> GPIOA 1
	GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	return true;
}

DECLARE_PWM_OUT(PWM_OUT_A, PWM_OUT_A_init_func, htim2, TIM_CHANNEL_2);