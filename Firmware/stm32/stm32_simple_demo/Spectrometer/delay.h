/*
 * delay.h
 *
 *  Created on: Mar 2, 2022
 *      Author: user
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>
#include "stm32l4xx.h"

#define DEALAY_USEC_TIMER htim2
extern TIM_HandleTypeDef DEALAY_USEC_TIMER;

void delayMicroseconds(uint16_t us);

#endif /* DELAY_H_ */
