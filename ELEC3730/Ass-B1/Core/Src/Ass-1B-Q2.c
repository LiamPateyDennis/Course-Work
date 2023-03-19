/*
 * Ass-1A-Q2.c
 *
 *  Created on: 2 Mar. 2023
 *      Author: c3349900
 */

#include "Ass-1B.h"
#if DO_QUESTION == 2
volatile uint8_t start_loop = 0;
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
if (htim->Instance == htim3.Instance) {
// this flag indicates that the timer is up
start_loop = 1;
}
}
void Ass_1B_main(void)
{
enum LED_States State = Green;
enum LED_States Next_State = Green;
uint16_t loop = 0;
// Initialise the timer and start running
HAL_TIM_Base_Start_IT(&htim3);
// Ignore first interrupt
while (start_loop == 0) {
HAL_Delay (1);
}
start_loop = 0;
// State machine
// LEDs moving in a circle clockwise
while (1) {
// Make sure that the timer is not already up
if (start_loop == 1) {
printf("WARNING: Loop time exceeded\n");
}
while (start_loop == 0) {
HAL_Delay (1);
}
start_loop = 0;
// Update state
switch (State) {
case Orange:
Next_State = Red;
break;
case Red:
Next_State = Blue;
break;
case Blue:
Next_State = Green;
break;
case Green:
Next_State = Orange;
break;
}
// Update state
if (Next_State != State) {
switch (Next_State) {
case Orange:
HAL_GPIO_TogglePin (GPIOD, LED_ORG);
break;
case Red:
HAL_GPIO_TogglePin (GPIOD, LED_RED);
break;
case Blue:
HAL_GPIO_TogglePin (GPIOD, LED_BLU);
break;
case Green:
HAL_GPIO_TogglePin (GPIOD, LED_GRN);
break;
}
State = Next_State;
}
printf ("-> Q2-Loop = %d\n", ++loop);
}
}
#endif
