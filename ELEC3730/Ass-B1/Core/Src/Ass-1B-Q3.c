/*
 * Ass-1A-Q3.c
 *
 *  Created on: Mar 2, 2023
 *      Author: c3349900
 */

#include "Ass-1B.h"
#if DO_QUESTION == 3
void Ass_1B_main ()
{
uint32_t value;
uint16_t loop = 0;
// Initial ADC conversion
HAL_ADC_Start (&hadc1);
while (1) {
HAL_Delay (250);
if (HAL_ADC_PollForConversion (&hadc1, 1000) == HAL_OK) {
value = HAL_ADC_GetValue (&hadc1);
printf ("-> Got %4ld at Q3-Loop = %d\n", value, ++loop);
}
else {
printf ("-> Error reading ADC input at Loop = %d\n", ++loop);
}
HAL_ADC_Start (&hadc1);
}
}
#endif
