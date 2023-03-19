/*
 * Ass-1A-Q1.c
 *
 *  Created on: Feb 28, 2023
 *      Author: c3349900
 */

#include "Ass-1B.h"
// Replace X with a question number
#if DO_QUESTION == 1
// Flag to indicate start of loop
volatile uint8_t start_loop = 0;
// Performance information
uint32_t loop_count = 0;
uint32_t wait_count;
uint32_t wait_count_min = 1 << 31;
uint32_t wait_count_max = 0;
uint32_t wait_count_avg = 0;
uint32_t debounce_counter = 0;

uint8_t button = 0; //If button was pressed
uint16_t timer_20 = 0; //Pause Timer
uint16_t grind_duration = 500; // 5 - 15 s => (500 - 1500) * 10 ms (Determined by the adc1)
uint16_t timer_grind = 0; // Start / Grind Timer
uint32_t sound[8] = {255,200,100,55,0,55,100,200}; // Stored sound array for speaker
uint32_t duty_cycle[1] = {0}; // bright
uint16_t adc_val = 0;


// Initialise button press and debouncing
#define DEBOUNCE_INTERVAL 100
GPIO_PinState JoyState;
enum debounceState {LOCKED, RELEASED, OFF};
enum debounceState debounce_state = OFF;
enum MODE {X,Y};
enum MODE mode = X;

enum LED {Up, Down};
enum LED led = Down;

// Initialise State Machine
enum Grinder {Stop, Start, Pause};
enum Grinder grinder = Stop;
enum Grinder grinder_next = Stop;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)  {
      if (GPIO_Pin == B1_Pin) {

    	    //Need to include with debounce, not sure, testing required
            switch(debounce_state) {
            case OFF:
                  debounce_state = RELEASED;
                  break;
            case LOCKED:
            	break;
            case RELEASED:
            	break;
            }
      }

}
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
  if (htim->Instance == htim3.Instance)
  {
    start_loop = 1;
  }
}

void Ass_1B_main ()
{
  // Initialise the timer and start running
  HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, &duty_cycle, 1);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start(&htim2);

  HAL_ADC_Start_DMA(&(hadc1), &adc_val, 1);
  HAL_TIM_Base_Start(&htim8);
  // Ignore first interrupt
  while (start_loop == 0);
  start_loop = 0;
  // Grinder application
  printf("Start while loop\n");
  while (loop_count < 6000) // Run for 1min assuming 10ms loop duration
  {
    // Wait for next loop
    if (start_loop == 1)
    {
//      printf ("WARNING: Loop time exceeded\n");
    }
    wait_count = 0;
    while (start_loop == 0)
    {
      wait_count++;
    }
    start_loop = 0;
    // Performance monitoring
    loop_count++;
    wait_count_avg = (wait_count_avg+wait_count*(loop_count-1))/loop_count;
    if (wait_count > wait_count_max)
      wait_count_max = wait_count;
    if (wait_count < wait_count_min)
      wait_count_min = wait_count;
    // **** START COFFEE GRINDER MAIN LOOP CODE ****
    // Button Debouncing
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

  // Button Position (1 or 0)
  button = JoyState;
//  printf("button = %d\n",button); working
  grind_duration = 500 + (adc_val+1)*1000/256;
  // Next State
  switch(grinder) {
  case Stop:
	  if (button == 1) {
		  grinder_next = Start;
		  button = 0;
		  HAL_DAC_Start_DMA(&hdac, DAC1_CHANNEL_1, sound, 8, DAC_ALIGN_12B_R);
		  duty_cycle[0] = 1999;
	  }
	  timer_20 = 0;
	  timer_grind = 0;
	  break;
  case Start:
	  if (button == 1) {
		  grinder_next = Pause;
		  button = 0;
		  HAL_DAC_Stop_DMA(&hdac, DAC1_CHANNEL_1);
		  duty_cycle[0] = 50;
		  led = Down;
	  }
	  timer_grind++;
	  if (timer_grind >= grind_duration) {
		  timer_grind = 0;
		  grinder_next = Stop; // Finished grinding
		  HAL_DAC_Stop_DMA(&hdac, DAC1_CHANNEL_1);
		  duty_cycle[0] = 0;
	  }
	  else {

	  }
	  break;
  case Pause:
	  if (button == 1) {
		  grinder_next = Start;
		  timer_20 = 0;
		  HAL_DAC_Start_DMA(&hdac, DAC1_CHANNEL_1, sound, 8, DAC_ALIGN_12B_R);
		  duty_cycle[0] = 1999;
	  }
	  timer_20++;
	  // Check if 20s has passed
	  if (timer_20 == 2000-1) {
		  timer_20 = 0;
		  grinder_next = Stop;
		  HAL_DAC_Stop_DMA(&hdac, DAC1_CHANNEL_1);
		  duty_cycle[0] = 0;
	  }
	  if (timer_20 % 200 == 0) {
		  switch (led) {
		  case Up:
			  led = Down;
			  break;
		  case Down:
			  led = Up;
			  break;

		  }
	  }
	  switch(led) {
	  case Up:
		  duty_cycle[0] = (50 + (timer_20 % 200)*1949/200);
		  break;
	  case Down:
		  duty_cycle[0] = (1999 - (timer_20 % 200)*1949/200);
		  break;
	  }
//		  printf("%d\n",duty_cycle[0]);
//		  printf("%d\n",timer_20);
//		  printf("%d\n",(200+(timer_20 % 400)*1799/400));

	  break;
  	  }

  // Update State
  if (grinder_next != grinder) {
	  switch (grinder_next) {
	  case Stop:
		  grinder = Stop;
		  printf("Stop\n");
		  break;
	  case Start:
		  grinder = Start;
		  printf("Start\n");
		  break;
	  case Pause:
		  grinder = Pause;
		  printf("Pause\n");
		  break;
	  }
  }
    // ****  END COFFEE GRINDER MAIN LOOP CODE ****

  } // end of while
  // Print performance information
  printf("INFO: loop_count     = %ld (%fs)\n",loop_count,(float)loop_count*(float)0.01);
  printf("INFO: wait_count_max = %ld\n", wait_count_max);
  printf("INFO: wait_count_avg = %ld\n", wait_count_avg);
  printf("INFO: wait_count_min = %ld\n", wait_count_min);

} // end of Ass_1B_main ()
#endif
