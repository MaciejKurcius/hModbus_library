/**
 *  \file 		usart.c
 *  \brief		GPIO configuration file
 *  \details	It contain all GPIO init and handling function
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

/* INCLUDES */

#include <usart.h>

/* VARIABLES */

/* FUNCTION */

/**
 * \brief 	UART peripherals config and init
 * \param 	void
 * \see 	BoardInit()
 * \note 	Function to configure all UART peripherals and init them. This function
 *			should contain all action, which must be execute before using UART transmission.
 * \return 	void
 */
void UartInit(void){
	  LL_USART_InitTypeDef USART_InitStruct = {0};
	  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	  // Peripheral clock enable
	  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
	  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	  /*
	  USART1 GPIO Configuration
	  PB6   ------> USART1_TX
	  PB7   ------> USART1_RX
	  */
	  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  NVIC_SetPriority(USART1_IRQn, 0);
	  NVIC_EnableIRQ(USART1_IRQn);

	  USART_InitStruct.BaudRate = UART_BAUD_RATE;
	  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	  LL_USART_Init(USART1, &USART_InitStruct);
	  LL_USART_DisableIT_CTS(USART1);
	  LL_USART_ConfigAsyncMode(USART1);
	  LL_USART_Enable(USART1);
	  LL_USART_EnableIT_RXNE(USART1);
	  LL_USART_EnableIT_ERROR(USART1);
	  LL_USART_DisableIT_TXE(USART1);
	  LL_USART_DisableIT_TC(USART1);
	  //
//	  LL_USART_DisableIT_RXNE(USART1);
	  LL_USART_DisableIT_IDLE(USART1);

	  UartLibInit();
}

/****END OF FILE****/
