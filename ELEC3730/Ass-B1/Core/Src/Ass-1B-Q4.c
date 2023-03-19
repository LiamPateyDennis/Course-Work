/*
 * Ass-1A-Q4.c
 *
 *  Created on: Mar 2, 2023
 *      Author: c3349900
 */


#include "Ass-1B.h"
#if DO_QUESTION == 4
volatile uint16_t speed = 0;
volatile uint8_t start_loop = 0;
volatile uint16_t variable = 0;

//uint32_t debounce_counter;
//uint32_t adc;

//case CW:
//
//                  switch (led) {
//
//
//
//                  case L3:
//
//                        HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD4),
//
//                                    &(GPIOD->ODR), 1);
//
//                        led = L4;
//
//                        break;
//
//                  case L4:
//
//                        HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD6),
//
//                                    &(GPIOD->ODR), 1);
//
//                        led = L6;
//
//                        break;
//
//                  case L6:
//
//                        HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD5),
//
//                                    &(GPIOD->ODR), 1);
//
//                        led = L5;
//
//                        break;
//
//                  case L5:
//
//                        HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD3),
//
//                                    &(GPIOD->ODR), 1);
//
//                        led = L3;
//
//                        break;
//
//                  }
//
//                  break;
//
//
//
//            case ACW:
//
//                  switch (led) {
//
//                  case L3:
//
//                        HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD5),
//
//                                    &(GPIOD->ODR), 1);
//
//                        led = L5;
//
//                        break;
//
//                  case L5:
//
//                        HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD6),
//
//                                    &(GPIOD->ODR), 1);
//
//                        led = L6;
//
//                        break;
//
//                  case L6:
//
//                        HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD4),
//
//                                    &(GPIOD->ODR), 1);
//
//                        led = L4;
//
//                        break;
//
//                  case L4:
//
//                        HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD3),
//
//                                    &(GPIOD->ODR), 1);
//
//                        led = L3;
//
//                        break;

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
if (htim->Instance == htim3.Instance) {
// this flag indicates that the timer is up
start_loop = 1;
speed += 1;
}

}
void Ass_1B_main(void)
{
volatile uint32_t value = 0;
//uint16_t loop = 0;
// Initial ADC conversion
HAL_ADC_Start(&hadc1);
enum LED_States State = Green;
enum LED_States Next_State = Green;
enum CLOCK_STATES Clock_States = CW;

uint32_t LD4 = 0x1000; //Green
uint32_t LD3 = 0x2000;	//Orange
uint32_t LD5 = 0x4000;	//Red
uint32_t LD6 = 0x8000;	//Blue
uint32_t off = 0x0;
//uint32_t br = 0xC000;
//uint32_t brt = 0xE000;
//uint32_t brtl = 0xF000;
//uint32_t tlb = 0xB000;
//uint32_t lb = 0x9000;
//uint32_t rtl = 0x7000;
//uint32_t tl = 0x3000;


//uint16_t loop = 0;
// Initialise the timer and start running
HAL_TIM_Base_Start_IT(&htim3);
// Ignore first interrupt but won't happen
while (start_loop == 0) {
HAL_Delay (1);
}
start_loop = 0;
// State machine
// LEDs moving in a circle clockwise
while (1) {

while (start_loop == 0) {
HAL_Delay (1);
}

if (HAL_ADC_PollForConversion (&hadc1, 1000) == HAL_OK) {
value = HAL_ADC_GetValue (&hadc1);
//printf ("-> Got %4ld at Q3-Loop = %d\n", value, ++loop);
}

if (value <= 2000) {
	Clock_States = ACW;
	variable = (int)(100 - (((1999 - value)/19.99) + 1));
//	printf("ACW");
}
else if (value >= 2096) {
	Clock_States = CW;
	variable = (int)(100 - (((value - 2096)/19.99) + 1));
//	printf("CW");
}
else {
	Clock_States = Off;
	variable = (int)30;
//	printf("off");
}
// Make sure that the timer is not already up

//printf ("%d\n",speed);
//printf ("%d\n",variable);
//printf("%d\n",value);
if (speed >= variable)
{
speed = 0;

// Update state
switch (Clock_States) {
case CW:
	switch (State) {
			case Orange:
			Next_State = Green; //Green
			break;
			case Red:
			Next_State = Orange; // Orange
			break;
			case Blue:
			Next_State = Red; //Red
			break;
			case Green:
			Next_State = Blue; //Blue
			break;
		}
break;
case ACW:
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
break;
case Off:
	break;
break;
}
// Update state
if (Next_State != State) {
switch (Next_State) {
case Orange:
	GPIOD->ODR = LD3;
break;
case Red:
	GPIOD->ODR = LD5;
break;
case Blue:
	GPIOD->ODR = LD6;
break;
case Green:
	GPIOD->ODR = LD4;
break;
}
State = Next_State;
}
}
//printf ("-> Q2-Loop = %d\n", ++loop);
HAL_ADC_Start (&hadc1);


}

}
#endif
