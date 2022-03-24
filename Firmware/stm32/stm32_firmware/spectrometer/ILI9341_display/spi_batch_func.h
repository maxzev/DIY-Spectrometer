#ifndef SPI_TEST_H_
#define SPI_TEST_H_

#include "stm32f4xx_hal.h"

HAL_StatusTypeDef Test_HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint16_t data, uint32_t count, uint32_t Timeout);

#endif /* SPI_TEST_H_ */
