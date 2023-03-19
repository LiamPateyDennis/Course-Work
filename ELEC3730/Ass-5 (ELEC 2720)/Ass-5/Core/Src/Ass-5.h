/*
 * Ass-3.h
 *
 *  Created on: Apr 6, 2022
 *      Author: c3349900
 */

#ifndef SRC_ASS_5_H_
#define SRC_ASS_5_H_

#include "main.h"
// for printf
#include <stdio.h>
// Question to be completed
#define DO_QUESTION 3
// Parameters

void Ass_5_main (void);

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim2;
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream1;
extern DMA_HandleTypeDef hdma_adc1;
//extern I2C_HandleTypeDef hi2c1;

#endif /* SRC_ASS_5_H_ */
