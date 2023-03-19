/*
 * Ass-3-Q4.c
 *
 *  Created on: 7 Apr. 2022
 *      Author: c3349900
 */

#include "Ass-5.h"
#if DO_QUESTION == 4


/*
 * Coding Understanding:
 * a = a + 2;
 * a += 2;
 *
 * b = b | 0x1000;
 * b |= 0x1000;
 * b
 * &(b)
 * &(GPIOD->ODR)
 *
 * LD5 = 0x1000
 * LD5 = 0x2000
 * LD6 = 0x4000
 */

/*
 * Stored Binary of Bits i.e.
 * LD4: 00000000000000000001000000000000
 */
uint32_t LD4 = 0x1000;
uint32_t LD5 = 0x4000;
uint32_t LD6 = 0x8000;
uint32_t off = 0x0;


/*
 * DMA w/ interrupt:
 * HAL_DMA_Start_IT (&hdma_memtomem_dma2_stream0, Input address(&), Output address(&), Number of words or bytes(int));
 *
 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)	{
// Check it is the correct pin



if (GPIO_Pin == JOY_C_Pin)	{
	//printf("C\n");
	//GPIOD->ODR = LD6;
	HAL_DMA_Start_IT (&hdma_memtomem_dma2_stream0, &(LD6), &(GPIOD->ODR), 1);
	}
if (GPIO_Pin == JOY_A_Pin)	{
	//printf("A\n");
	//GPIOD->ODR = LD4;
	HAL_DMA_Start_IT (&hdma_memtomem_dma2_stream0, &(LD4), &(GPIOD->ODR), 1);
	}
if (GPIO_Pin == JOY_D_Pin)	{
	//printf("D\n");
	//GPIOD->ODR = LD5;
	HAL_DMA_Start_IT (&hdma_memtomem_dma2_stream0, &(LD5), &(GPIOD->ODR), 1);
	}
else if (GPIO_Pin == JOY_CTR_Pin)	{
	//printf("CTR\n");
	//GPIOD->ODR = 0;
	HAL_DMA_Start_IT (&hdma_memtomem_dma2_stream0, &(off), &(GPIOD->ODR), 1);

	}
}


void Ass_3_main (void)
{

while (1)
{
	//10ms Delay
	HAL_Delay(10);
	}
}
#endif
