#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>
#include "stm32f4xx.h"

extern TIM_HandleTypeDef htim2;
void delayMicroseconds(uint32_t us);

#endif /* DELAY_H_ */
