/*
 * systick_impl.c
 *
 *  Created on: 2017年9月4日
 *      Author: gene
 */

#include "hal.h"
#include "common/systick.h"

static callback_void_void S_USER_CALLBACK = null;

static uint_32 N_CYCLES_US, MAX_DELAY_US, S_DELAY_OVERFLOW_US;

/** Initialize systick, return max delay in us.*/
void systick_initialize(callback_void_void callback) {
	S_USER_CALLBACK = callback;

	N_CYCLES_US = HAL_RCC_GetHCLKFreq() / 1000000;

    /**Configure the Systick*/
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    MAX_DELAY_US = SysTick_LOAD_RELOAD_Msk / N_CYCLES_US;

    S_DELAY_OVERFLOW_US = 0UL;

    SysTick->VAL = 0UL;
    SysTick->LOAD = 0UL;
}

void systick_postdelay(uint_32 us) {
	// Reset counter
	SysTick->VAL = 0UL;

	if (us > MAX_DELAY_US) {
		S_DELAY_OVERFLOW_US = us - MAX_DELAY_US;
		SysTick->LOAD = (uint32_t)(SysTick_LOAD_RELOAD_Msk - 1UL);
	} else {
		S_DELAY_OVERFLOW_US = 0UL;
	    SysTick->LOAD = (uint32_t)(N_CYCLES_US * us - 1UL);
	}

	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void systick_clear() {
	S_DELAY_OVERFLOW_US = 0UL;
	SysTick->VAL = 0UL;
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

uint_32 systick_get_timeleft() {
	return S_DELAY_OVERFLOW_US + SysTick->VAL / N_CYCLES_US;
}

void HAL_SYSTICK_Callback(void) {
	if (S_DELAY_OVERFLOW_US > MAX_DELAY_US) {
		// Systick continue.
		S_DELAY_OVERFLOW_US -= MAX_DELAY_US;
	} else if (S_DELAY_OVERFLOW_US > 0UL) {
		// Count down overflow time
		S_DELAY_OVERFLOW_US = 0UL;
		SysTick->VAL = 0UL;
		SysTick->LOAD = (uint32_t)(N_CYCLES_US * S_DELAY_OVERFLOW_US - 1UL);
	} else {
	    SysTick->VAL = 0UL;
	    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
	    S_USER_CALLBACK();
	}
}
