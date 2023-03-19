/*
 * Ass-3-Q1.c
 *
 *  Created on: Apr 6, 2022
 *      Author: c3349900
 */

#include "Ass-5.h"

#if DO_QUESTION == 1
// Variable to count interrupts
volatile static uint16_t sum = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
if (htim->Instance == TIM3) {
HAL_GPIO_TogglePin (LD6_GPIO_Port, LD6_Pin);
sum += 1;
}
}
void Ass_5_main (void)
{
sum = 0;
HAL_TIM_Base_Start_IT(&htim3);
//HAL_Delay (5000);
while (sum >= 10) {
	TIM3->ARR = 499;
}

while (1) {
}
}
#endif
