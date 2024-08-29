#include "hModbusLibConf.h"
#include <stm32l0xx_ll_utils.h>
#include <stm32l0xx_ll_usart.h>
#include <hModbusLib.h>
#include <usart.h>

extern volatile uint32_t SysTick_counter;

// Write delay miliseconds function from your framework
void hModbusDelay(uint32_t DelayTime){
    LL_mDelay(DelayTime);
}

// Write function which return system clock tick in miliseconds resolution
uint32_t hModbusGetSystemClock(void){
    return SysTick_counter;
}

// Write function to set GPIO pin
void hModbusSetGpioPin(hModbusCtrlOutTypeDef Output){
    LL_GPIO_SetOutputPin(Output.Port, Output.Pin);
}

// Write function to reset GPIO pin
void hModbusResetGpioPin(hModbusCtrlOutTypeDef Output){
    LL_GPIO_ResetOutputPin(Output.Port, Output.Pin);
}

/* TX DATA funcions */

// Write function to send one byte data via UART
void hModbusUsartTx8(hModbusTypeDef* Handle, uint8_t TxData){
    LL_USART_TransmitData8(Handle->UartHandle, TxData);
}

/* RX DATA funcions */

// Write function to receive one byte data via UART
uint8_t hModbusUsartRx8(hModbusTypeDef* Handle){
    return LL_USART_ReceiveData8(Handle->UartHandle);
}

/* UART Flags */

// Write function from your framework which return UART Iddle flag
uint32_t hModbusGetUartIdleFlag(hModbusTypeDef* Handle){
    return LL_USART_IsActiveFlag_IDLE(Handle->UartHandle);
}

// Write function from your framework to clear UART Iddle flag
void hModbusClearUartIdleFlag(hModbusTypeDef* Handle){
    LL_USART_ClearFlag_IDLE(Handle->UartHandle);
}

// Write function from your framework to enable UART IDLE interrupt
void hModbusEnableIdleIt(hModbusTypeDef* Handle){
    LL_USART_EnableIT_IDLE(Handle->UartHandle);
}

// Write function from your framework to disable UART IDLE interrupt
void hModbusDisableIdleIt(hModbusTypeDef* Handle){
    LL_USART_DisableIT_IDLE(Handle->UartHandle);
}

// Write function from your framework which return UART TXE flag
uint32_t hModbusGetUartTxeFlag(hModbusTypeDef* Handle){
    return LL_USART_IsActiveFlag_TXE(Handle->UartHandle);
}

// Write function from your framework which return UART TC flag
uint32_t hModbusGetUartTcFlag(hModbusTypeDef* Handle){
    return LL_USART_IsActiveFlag_TC(Handle->UartHandle);
}

// Write function from your framework to clear UART TC flag
void hModbusClearUartTcFlag(hModbusTypeDef* Handle){
    LL_USART_ClearFlag_TC(Handle->UartHandle);
}

// Write all common for all instances actions to execute when hModbus struct is init
void hModbusUsartInit(hModbusTypeDef* Handle){
	;
}

// Write function from your framework which return UART RXNE flag
uint32_t hModbusGetUartRxneFlag(hModbusTypeDef* Handle){
    return LL_USART_IsActiveFlag_RXNE(Handle->UartHandle);
}

// Write function from your framework to enable UART RXNE interrupt
void hModbusEnableRxneIt(hModbusTypeDef* Handle){
    LL_USART_EnableIT_RXNE(Handle->UartHandle);
}

void hModbusMaster1UartInit(void){
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

	NVIC_SetPriority(USART1_IRQn, 5);
	NVIC_EnableIRQ(USART1_IRQn);

	USART_InitStruct.BaudRate = 9600;
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

	// LL_USART_EnableIT_ERROR(USART1);

	LL_USART_DisableIT_TXE(USART1);
	LL_USART_DisableIT_TC(USART1);

	LL_USART_ClearFlag_IDLE(USART1);
	LL_USART_DisableIT_IDLE(USART1);
}