/*
 * delay.c
 *
 *  Created on: Mar 2, 2022
 *      Author: user
 */

#include <delay.h>


void delayMicroseconds(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&DEALAY_USEC_TIMER, 0);  // set the counter value a 0
	while ((uint16_t)__HAL_TIM_GET_COUNTER(&DEALAY_USEC_TIMER) < us)
		;  // wait for the counter to reach the us input in the parameter
}
