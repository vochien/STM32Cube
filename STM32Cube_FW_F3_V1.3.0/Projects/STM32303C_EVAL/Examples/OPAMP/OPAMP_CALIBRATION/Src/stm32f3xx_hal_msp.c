/**
  ******************************************************************************
  * @file    OPAMP/OPAMP_CALIBRATION/Src/stm32f3xx_hal_msp.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F3xx_HAL_Examples
  * @{
  */

/** @defgroup OPAMP_CALIBRATION
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static DMA_HandleTypeDef  hdma_dac;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief DAC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hdac: DAC handle pointer
  * @retval None
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
  GPIO_InitTypeDef          GPIO_InitStruct;

  /*##-1- Enable peripherals and  Clocks ####################################*/
  /* DAC Periph clock enable */
  __DAC1_CLK_ENABLE();
  /* Enable GPIO clock ****************************************/
  __GPIOA_CLK_ENABLE();
 /* DMA clock enable */
  __DMA2_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* DAC Channelx GPIO pin configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*##-3- Configure the DMA streams ##########################################*/
  /* Set the parameters to be configured for Channelx*/
  hdma_dac.Instance = DMA2_Channel4;
  
  hdma_dac.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_dac.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_dac.Init.MemInc = DMA_MINC_ENABLE;
  hdma_dac.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_dac.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_dac.Init.Mode = DMA_CIRCULAR;
  hdma_dac.Init.Priority = DMA_PRIORITY_HIGH;

  HAL_DMA_Init(&hdma_dac);

  /* Associate the initialized DMA handle to the the DAC handle */
  __HAL_LINKDMA(hdac, DMA_Handle2, hdma_dac);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* Enable the DMA Stream IRQ Channel */
  HAL_NVIC_SetPriority(DMA2_Channel4_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel4_IRQn);
}

/**
  * @brief  DeInitializes the DAC MSP.
  * @param  hdac: pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
{
  /*##-1- Reset peripherals ##################################################*/
  __DAC1_FORCE_RESET();
  __DAC1_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the DAC Channel GPIO pin */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);

  /*##-3- Disable the DMA Streams ############################################*/
  /* De-Initialize the DMA Stream associate to Channel */
  if(hdma_dac.Instance != NULL)
    HAL_DMA_DeInit(&hdma_dac);

  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(DMA2_Channel4_IRQn);
}

/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /* TIM2 Periph clock enable */
  __TIM2_CLK_ENABLE();
}

/**
  * @brief TIM MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Reset peripherals ##################################################*/
  __TIM2_FORCE_RESET();
  __TIM2_RELEASE_RESET();
}

/**
  * @brief OPAMP MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hdac: DAC handle pointer
  * @retval None
  */
void HAL_OPAMP_MspInit(OPAMP_HandleTypeDef *hopamp)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Configure peripheral GPIO ##########################################*/  
  /* Input GPIO used as OPAMP1 non inverting input is already configured in
     analog mode in HAL_DAC_MspDeInit() */

  /* Enable GPIO clock  */
  __GPIOA_CLK_ENABLE();

  /* Configure the GPIO used as OPAMP1 output */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief  DeInitializes the OPAMP MSP.
  * @param  hdac: pointer to a OPAMP_HandleTypeDef structure that contains
  *         the configuration information for the specified OPAMP.  
  * @retval None
  */
void HAL_OPAMP_MspDeInit(OPAMP_HandleTypeDef *hopamp)
{
  /* De-initialize the OPAMP1 output GPIO pin */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
