/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "main.h"
#include "device.h"
#include "core.h"
#include "callback_scheduler.h"

#define DELAY_TIME_S 1

int counter = 0;
char message[12];

int t_sec = 0, t_min = 0;

void loop(uint_16 type, void* pv) {
    GPIO_OUT.set_state(GPIO_OUT.state == GPIO_PIN_SET ? GPIO_PIN_RESET : GPIO_PIN_SET);

	t_sec++;
	if (t_sec == 60) {
		t_min++;
		t_sec = 0;
	}

	sprintf(message, "%d:%2d     ", t_min, t_sec);

	OLED_0_95_INCH.appendLine(message);

	callback_scheduler_postdely_s(loop, null, 0, DELAY_TIME_S);
}

void BTN_A_DOWN_CALLBACK(uint_8 id) {
    sprintf(message, "%d", counter++);

    OLED_0_95_INCH.appendLine(message);
}

void BTN_B_DOWN_CALLBACK(uint_8 id) {
    sprintf(message, "%d", counter++);

    OLED_0_95_INCH.appendLine(message);
}

int main(void) {
    core_init();

	PWM_OUT_A.init_device();

    OLED_0_95_INCH.init_device();


    GPIO_OUT.init_device();
    GPIO_OUT.set_state(GPIO_PIN_SET);

    BUTTON_A.init_device();

    BUTTON_B.init_device();

    BUTTON_A.set_action_callback(BUTTON_CALLBACK_DOWN, BTN_A_DOWN_CALLBACK);
    BUTTON_A.set_action_callback(BUTTON_CALLBACK_UP, BTN_A_DOWN_CALLBACK);

    BUTTON_B.set_action_callback(BUTTON_CALLBACK_DOWN, BTN_B_DOWN_CALLBACK);
    BUTTON_B.set_action_callback(BUTTON_CALLBACK_UP, BTN_B_DOWN_CALLBACK);

    callback_scheduler_postdely_s(loop, null, 0, 1);

    OLED_0_95_INCH.appendLine("stared");

    while (1) {}
}
