/*
 * Ass-3-Q2.c
 *
 *  Created on: Apr 6, 2022
 *      Author: c3349900
 */


#include "Ass-5.h"

#if DO_QUESTION == 2

#define SPI_RD 0x80 // The bit to set SPI to read.
uint8_t auchSpiTxBuf[2], auchSpiRxBuf[2];
void Ass_5_main (void)
{

__HAL_SPI_ENABLE(&hspi1);
// Now set up the MEMS chip for 3.125 Hz sampling rate.
// Initial setup of the MEMS sample rate
auchSpiTxBuf[0] = 0x20; // Control Reg 0x20
//auchSpiTxBuf[1] = 0x12; // Sampling at 3.125 Hz, Y accelerometer
auchSpiTxBuf[1] = 0x11; // Sampling at 3.125 Hz, Y accelerometer
// Now set the chip select low
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
printf("Control value read = 0x%x", auchSpiRxBuf[0]);
while(1) {
	HAL_GPIO_TogglePin (LD6_GPIO_Port, LD6_Pin); //LD6 has been labelled BLUE
	// Set up the auchSpiTxBuf for a read.
//	auchSpiTxBuf[0] = 0x2b | SPI_RD; // Control Reg 0x2B Y value to be read
	auchSpiTxBuf[0] = 0x29 | SPI_RD; // Control Reg 0x2B Y value to be read
	// Now set the chip select low
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
	// Now write the address.
	HAL_SPI_Transmit(&hspi1, auchSpiTxBuf, 1, 50);
	// Now Read the data
	HAL_SPI_Receive(&hspi1, auchSpiRxBuf, 1, 50);
	//Now set the CS pin high again
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
	// Now print out the value to see if it is correct
	printf("Value read = %d\n", (int8_t)auchSpiRxBuf[0]);
}
}
#endif
