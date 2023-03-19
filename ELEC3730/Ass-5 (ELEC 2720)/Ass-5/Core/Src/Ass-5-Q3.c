/*
 * Ass-3-Q3.c
 *
 *  Created on: Apr 6, 2022
 *      Author: c3349900
 */

#include "Ass-5.h"
#if DO_QUESTION == 3

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

#define DEBOUNCE_INTERVAL 100
/*
 * Stored Binary of Bits i.e.
 * LD4: 00000000000000000001000000000000
 */
uint32_t LD4 = 0x1000;
uint32_t LD3 = 0x2000;
uint32_t LD5 = 0x4000;
uint32_t LD6 = 0x8000;
uint32_t off = 0x0;
uint32_t br = 0xC000;
uint32_t brt = 0xE000;
uint32_t brtl = 0xF000;
uint32_t tlb = 0xB000;
uint32_t lb = 0x9000;
uint32_t rtl = 0x7000;
uint32_t tl = 0x3000;
uint32_t debounce_counter;
uint32_t adc;

GPIO_PinState JoyState;

enum CDirection {CW, ACW};
enum CDirection cdirection = CW;
enum MODE {X, Y};
enum MODE mode = X;
enum LED {L3, L4, L5, L6};
enum LED led = L3;
enum COOL {a,b,c,d,e,f,g,h,i};
enum COOL cool = a;
enum debounceState {LOCKED, RELEASED, OFF};
enum debounceState debounce_state = OFF;

/*
 * DMA w/ interrupt:
 * HAL_DMA_Start_IT (&hdma_memtomem_dma2_stream0, Input address(&), Output address(&), Number of words or bytes(int));
 *
 *LD3:
 *HAL_DMA_Start_IT (&hdma_memtomem_dma2_stream0, &(LD3), &(GPIOD->ODR), 1);
 */

//--CALLBACK FUNCTION FOR CHANGE SWITCH BUT FOR BLUE BUTTON
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)	{
//// Check it is the correct pin
//
//
//
//if (GPIO_Pin == JOY_C_Pin)	{
//	//printf("C\n");
//	//GPIOD->ODR = LD6;
//
//	}
//if (GPIO_Pin == JOY_A_Pin)	{
//	//printf("A\n");
//	//GPIOD->ODR = LD4;
//
//	}
//if (GPIO_Pin == JOY_D_Pin)	{
//	//printf("D\n");
//	//GPIOD->ODR = LD5;
//
//	}
//else if (GPIO_Pin == JOY_CTR_Pin)	{
//	//printf("CTR\n");
//	//GPIOD->ODR = 0;
//	HAL_DMA_Start_IT (&hdma_memtomem_dma2_stream0, &(off), &(GPIOD->ODR), 1);
//
//	}
//}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)	{


	if (GPIO_Pin == B1_Pin)	{

		switch(debounce_state) {
		case OFF:
			debounce_state = RELEASED;
			break;
		break;
		}
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	if (hadc == &hadc1) {
		//printf("%4d\n", adc);
		TIM2->PSC = adc * 15 + 1;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//	if (htim->Instance == TIM3) {
//		HAL_ADC_Start_DMA (&(hadc1), (uint32_t*) &(adc), 1);
//	}

	if (htim->Instance == TIM2) {
		switch(mode){
			case X:

		switch (cdirection) {

		case CW:
			switch (led) {

			case L3:
				HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD4),
						&(GPIOD->ODR), 1);
				led = L4;
				break;
			case L4:
				HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD6),
						&(GPIOD->ODR), 1);
				led = L6;
				break;
			case L6:
				HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD5),
						&(GPIOD->ODR), 1);
				led = L5;
				break;
			case L5:
				HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD3),
						&(GPIOD->ODR), 1);
				led = L3;
				break;
			}
			break;

		case ACW:
			switch (led) {
			case L3:
				HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD5),
						&(GPIOD->ODR), 1);
				led = L5;
				break;
			case L5:
				HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD6),
						&(GPIOD->ODR), 1);
				led = L6;
				break;
			case L6:
				HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD4),
						&(GPIOD->ODR), 1);
				led = L4;
				break;
			case L4:
				HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD3),
						&(GPIOD->ODR), 1);
				led = L3;
				break;
			}
			break;
		}

		case Y:
// Top			HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD3), &(GPIOD->ODR), 1);
//	Left		HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD4), &(GPIOD->ODR), 1);
//	Bottom		HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD6), &(GPIOD->ODR), 1);
//	Right		HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD5), &(GPIOD->ODR), 1);
// Top			HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD3), &(GPIOD->ODR), 1);
//	Left		HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD4), &(GPIOD->ODR), 1);
//	Bottom		HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD6), &(GPIOD->ODR), 1);
//	Right		HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD5), &(GPIOD->ODR), 1);
			switch (cdirection) {

			case CW:
				switch (cool) {

					case a:
						HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD6), &(GPIOD->ODR), 1);
						cool = b;
						break;
					case b:
						HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(br), &(GPIOD->ODR), 1);
						cool = c;
						break;
					case c:
						HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(brt), &(GPIOD->ODR), 1);
						cool = d;
						break;
					case d:
						HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(brtl), &(GPIOD->ODR), 1);
						cool = e;
						break;
					case e:
						HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(rtl), &(GPIOD->ODR), 1);
						cool = f;
						break;
					case f:
						HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(tl), &(GPIOD->ODR), 1);
						cool = g;
						break;
					case g:
						HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD4), &(GPIOD->ODR), 1);
						cool = h;
						break;
					case h:
						HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(off), &(GPIOD->ODR), 1);
						cool = a;
						break;

					}
					break;

				case ACW:
					switch (cool) {
									case a:
										HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD6), &(GPIOD->ODR), 1);
										cool = h;
										break;
									case b:
										HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(br), &(GPIOD->ODR), 1);
										cool = a;
										break;
									case c:
										HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(brt), &(GPIOD->ODR), 1);
										cool = b;
										break;
									case d:
										HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(brtl), &(GPIOD->ODR), 1);
										cool = c;
										break;
									case e:
										HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(rtl), &(GPIOD->ODR), 1);
										cool = d;
										break;
									case f:
										HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(tl), &(GPIOD->ODR), 1);
										cool = e;
										break;
									case g:
										HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(LD4), &(GPIOD->ODR), 1);
										cool = f;
										break;
									case h:
										HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, &(off), &(GPIOD->ODR), 1);
										cool = g;
										break;
									}
									break;

			}
		}
	}
}

void Ass_5_main(void) {
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) &adc, (uint32_t) 1);

	__HAL_SPI_ENABLE(&hspi1);

	#define SPI_RD 0x80 // The bit to set SPI to read.
	uint8_t auchSpiTxBuf[2], auchSpiRxBuf[2];

	auchSpiTxBuf[0] = 0x20; // Control Reg 0x20
	auchSpiTxBuf[1] = 0x11; // Sampling at 3.125 Hz, Y accelerometer, 0x11 for X


	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
	// Now write the set up data.
	HAL_SPI_Transmit(&hspi1, auchSpiTxBuf, 2, 50); // Send the set up command.
	//Now set the CS pin high again
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
	// We can now read the date back to check that the write was successful
	// Set up the auchSpiTxBuf for a read.
	auchSpiTxBuf[0] = 0x20 | SPI_RD; // Control Reg 0x20 to be read
	// Now set the chip select low
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
	// Now write the address.
	HAL_SPI_Transmit(&hspi1, auchSpiTxBuf, 1, 50);
	// Now Read the data
	HAL_SPI_Receive(&hspi1, auchSpiRxBuf, 1, 50);
	// Now set the CS pin high again
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
	// Now print out the value to see if it is correct
	while (1) {
		//pin_state = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
		//printf("%d\n", pin_state);

		//HAL_Delay(5);
		//printf("Got %4d: |\n", TIM3->PSC);
		switch(debounce_state) {
							  	  case RELEASED:
							  		  // Read JOY_C only when RELEASED
							  		  JoyState = HAL_GPIO_ReadPin (B1_GPIO_Port, B1_Pin);
							  		  if (JoyState == 1) {
							  			  debounce_state = LOCKED;
							  			switch(mode) {
							  			case X:
							  				//printf("in x\n");
							  				mode = Y;
							  				//HAL_Delay(10);
							  				break;
							  			case Y:
							  				//printf("in y\n");
							  				mode = X;
							  				//HAL_Delay(10);
							  				break;
							  				}

							  		  }
							  		  break;
							  	  case LOCKED:
							  		  JoyState = 0; // Manually reset JoyState when LOCKED
							  		  if ( debounce_counter > DEBOUNCE_INTERVAL) {
							  			  debounce_state = OFF; // RELEASE after time-out
							  			  debounce_counter = 0;
							  		  }
							  		  else {
							  			  ++debounce_counter; // still LOCKED, increase counter
							  		  }
							  		  break;
							  	  case OFF:
							  		  break;
							  	  }

		auchSpiTxBuf[0] = 0x29 | SPI_RD; // Control Reg 0x2B Y value to be read or X = 0x29
// Now set the chip select low
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
// Now write the address.
		HAL_SPI_Transmit(&hspi1, auchSpiTxBuf, 1, 50);
// Now Read the data
		HAL_SPI_Receive(&hspi1, auchSpiRxBuf, 1, 50);
//Now set the CS pin high again
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
// Now print out the value to see if it is correct
		//printf("%4d\n", (int8_t) auchSpiRxBuf[0]);
		if ((int8_t) auchSpiRxBuf[0] >= 0) {
			cdirection = ACW;
		}
		else if ((int8_t) auchSpiRxBuf[0] < 0) {
			cdirection = CW;
		}
		HAL_Delay(10);

	}
}
#endif
