#include "spi_batch_func.h"
#include "stm32f4xx_hal_spi.h"


/////////////taken from stm32f4xx_hal_spi.c



/**
  * @brief  Handle SPI Communication Timeout.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *              the configuration information for SPI module.
  * @param  Flag SPI flag to check
  * @param  State flag state to check
  * @param  Timeout Timeout duration
  * @param  Tickstart tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef SPI_WaitFlagStateUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, FlagStatus State,
                                                       uint32_t Timeout, uint32_t Tickstart)
{
  while ((__HAL_SPI_GET_FLAG(hspi, Flag) ? SET : RESET) != State)
  {
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - Tickstart) >= Timeout) || (Timeout == 0U))
      {
        /* Disable the SPI and reset the CRC: the CRC value should be cleared
        on both master and slave sides in order to resynchronize the master
        and slave for their respective CRC calculation */

        /* Disable TXE, RXNE and ERR interrupts for the interrupt process */
        __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));

        if ((hspi->Init.Mode == SPI_MODE_MASTER) && ((hspi->Init.Direction == SPI_DIRECTION_1LINE)
                                                     || (hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY)))
        {
          /* Disable SPI peripheral */
          __HAL_SPI_DISABLE(hspi);
        }

        /* Reset CRC Calculation */
        if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
        {
          SPI_RESET_CRC(hspi);
        }

        hspi->State = HAL_SPI_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hspi);

        return HAL_TIMEOUT;
      }
    }
  }

  return HAL_OK;
}


#define SPI_DEFAULT_TIMEOUT 100U
#define SPI_BSY_FLAG_WORKAROUND_TIMEOUT 1000U /*!< Timeout 1000 µs             */

/**
  * @brief  Handle the check of the RXTX or TX transaction complete.
  * @param  hspi SPI handle
  * @param  Timeout Timeout duration
  * @param  Tickstart tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef SPI_EndRxTxTransaction(SPI_HandleTypeDef *hspi, uint32_t Timeout, uint32_t Tickstart)
{
  /* Timeout in µs */
  __IO uint32_t count = SPI_BSY_FLAG_WORKAROUND_TIMEOUT * (SystemCoreClock / 24U / 1000000U);
  /* Erratasheet: BSY bit may stay high at the end of a data transfer in Slave mode */
  if (hspi->Init.Mode == SPI_MODE_MASTER)
  {
    /* Control the BSY flag */
    if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_BSY, RESET, Timeout, Tickstart) != HAL_OK)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
      return HAL_TIMEOUT;
    }
  }
  else
  {
    /* Wait BSY flag during 1 Byte time transfer in case of Full-Duplex and Tx transfer
    * If Timeout is reached, the transfer is considered as finish.
    * User have to calculate the timeout value to fit with the time of 1 byte transfer.
    * This time is directly link with the SPI clock from Master device.
    */
    do
    {
      if (count == 0U)
      {
        break;
      }
      count--;
    } while (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_BSY) != RESET);
  }

  return HAL_OK;
}


/**
  * @brief  Transmit uint16_t value <count> times in blocking mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  data uint16_t value
  * @param  count number of copies of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */

HAL_StatusTypeDef Test_HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint16_t data, uint32_t count, uint32_t Timeout)
{
  HAL_StatusTypeDef errorcode = HAL_OK;

  /* Check Direction parameter */
  assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE(hspi->Init.Direction));

  __HAL_LOCK(hspi); /* Process Locked */

  /* Init tickstart for timeout management*/
  uint32_t tickstart = HAL_GetTick();

  if (hspi->State != HAL_SPI_STATE_READY)
  {
    errorcode = HAL_BUSY;
    goto error;
  }

  if (count == 0U)
  {
    errorcode = HAL_ERROR;
    goto error;
  }

  /* Set the transaction information */
  hspi->State       = HAL_SPI_STATE_BUSY_TX;
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;

  /* Configure communication direction : 1Line */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
  {
    SPI_1LINE_TX(hspi);
  }

#if (USE_SPI_CRC != 0U)
  /* Reset CRC Calculation */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
  }
#endif /* USE_SPI_CRC */

  /* Check if the SPI is already enabled */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    __HAL_SPI_ENABLE(hspi); /* Enable SPI peripheral */
  }

  uint32_t txCount;
  txCount = 0; // g++ throw an error about goto if it is written like uint16_t txCount = 0;

  /* Transmit data in 16 Bit mode */
  if (hspi->Init.DataSize == SPI_DATASIZE_16BIT)
  {
//	  //todo: do we need this?
//    if ((hspi->Init.Mode == SPI_MODE_SLAVE))
//    {
//      hspi->Instance->DR = data;
//      return errorcode;
//    }
    /* Transmit data in 16 Bit mode */
    while (txCount < count)
    {
      /* Wait until TXE flag is set to send data */
      if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE))
      {
        hspi->Instance->DR = data;
        ++txCount;
      }
      else
      {
        /* Timeout management */
        if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
        {
          errorcode = HAL_TIMEOUT;
          goto error;
        }
      }
    }//while
  }
  /* Transmit data in 8 Bit mode */
  else
  {
//	  //todo: do we need this?
//    if ((hspi->Init.Mode == SPI_MODE_SLAVE))
//    {
//      *((__IO uint8_t *)&hspi->Instance->DR) = ;
//      hspi->pTxBuffPtr += sizeof(uint8_t);
//      return errorcode;
//    }
  	bool firstByte;
  	firstByte = true;
    while (txCount < count*2) // bcs we send just one byte instead of two
    {
      /* Wait until TXE flag is set to send data */
      if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE))
      {
    	if(firstByte)
    	{
    		*((__IO uint8_t *)&hspi->Instance->DR) = (data>>8); // hi part
    	}
    	else
    	{
    		*((__IO uint8_t *)&hspi->Instance->DR) = (data & 0x00FF); // lo part
    	}

    	firstByte = !firstByte;

        ++txCount;
      }
      else
      {
        /* Timeout management */
        if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
        {
          errorcode = HAL_TIMEOUT;
          goto error;
        }
      }
    }//while
  }//else
#if (USE_SPI_CRC != 0U)
  /* Enable CRC Transmission */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
  }
#endif /* USE_SPI_CRC */

  /* Check the end of the transaction */
  if (SPI_EndRxTxTransaction(hspi, Timeout, tickstart) != HAL_OK)
  {
    hspi->ErrorCode = HAL_SPI_ERROR_FLAG;
  }

  /* Clear overrun flag in 2 Lines communication mode because received is not read */
  if (hspi->Init.Direction == SPI_DIRECTION_2LINES)
  {
    __HAL_SPI_CLEAR_OVRFLAG(hspi);
  }

  if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
  {
    errorcode = HAL_ERROR;
  }

error:
  hspi->State = HAL_SPI_STATE_READY;
  /* Process Unlocked */
  __HAL_UNLOCK(hspi);
  return errorcode;
}


