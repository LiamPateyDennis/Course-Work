/*
 * Ass-1A.h
 *
 *  Created on: Feb 28, 2023
 *      Author: c3349900
 */

#ifndef INC_ASS_1A_H_
#define INC_ASS_1A_H_

#include "main.h"
// for printf
#include <stdio.h>
// Question to be completed
#define DO_QUESTION 1
// Define a state machine for the LEDs
enum LED_States {Orange, Red, Blue, Green};
enum CLOCK_STATES {CW, ACW, Off};
// Intuitive names for LEDs
#define LED_ORG LD3_Pin
#define LED_RED LD5_Pin
#define LED_BLU LD6_Pin
#define LED_GRN LD4_Pin
#define SAMPLES 3
// To use variables declared in main.c
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim8;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
extern DAC_HandleTypeDef hdac;
// Assignment functions
void Ass_1A_main (void);


#endif /* INC_ASS_1A_H_ */
