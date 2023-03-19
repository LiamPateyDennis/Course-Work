/*
 * testing.c
 *
 *  Created on: Mar 3, 2023
 *      Author: c3349900
 */


#include "Ass-1B.h"
#if DO_QUESTION == 5

//void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
//{
//  if (htim->Instance == htim6.Instance)
//  {
//    start_loop = 1;
//  }
//}


void Ass_1B_main (void)
{
// Variables
uint32_t sound[8] = {255,200,100,55,0,55,100,200};
uint32_t sound_off[8] = {0,0,0,0,0,0,0,0};
//uint32_t off = 0x0;
//uint32_t LD3 = 0x2000;
//uint32_t status;	//Orange
//uint32_t x = 4;
uint32_t duty_cycle[1] = {0};
//uint16_t duty_cycle[1] = {200};
//printf("%d\n",TIM4->CNT); // Duty Cycle = 50% at 1000
HAL_TIM_Base_Start(&htim2);

//HAL_TIM_Base_Start(&htim4);

uint16_t adc_val = 0;
// Initilise DAC+DMA
//HAL_DAC_Start_DMA(&hdac, DAC1_CHANNEL_1, sound, 8, DAC_ALIGN_12B_R);
//HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, &duty_cycle, 1);
HAL_TIM_Base_Start(&htim8);
HAL_ADC_Start_DMA(&(hadc1), &adc_val, 1);

//printf(HAL_ADC_GetState(&hadc1));
while (1) {
	// DAC Code
//	HAL_Delay(2000);
//	HAL_DAC_Stop_DMA(&hdac, DAC1_CHANNEL_1);
//	HAL_Delay(2000);
//	HAL_DAC_Start_DMA(&hdac, DAC1_CHANNEL_1, sound, 8, DAC_ALIGN_12B_R);

	// PWM Code

	HAL_Delay(500);
	printf("adc = %d\n",adc_val);

//	uint32_t duty_cycle[SAMPLES] = {0,0,0};
//	uint8_t i = 0;
//	uint8_t a = 0;
//	while (i<=150) {
//		i++;
//		a += 10;
//		duty_cycle[0] = a;
//		//HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_3);
//		//HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, duty_cycle, 1);
//		HAL_Delay(500);
//	}
//	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_1, duty_cycle, (uint16_t) 1);

	}
}
#endif
