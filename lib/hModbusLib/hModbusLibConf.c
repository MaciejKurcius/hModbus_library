#include "hModbusLib_conf.h"
#include <stm32l0xx_ll_utils.h>
#include <stm32l0xx_ll_usart.h>
#include <hModbusLib.h>

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
void SetGpioPin(hModbusCtrlOutTypeDef Output){
    LL_GPIO_SetOutputPin(Output.Port, Output.Pin);
}

// Write function to reset GPIO pin
void ResetGpioPin(hModbusCtrlOutTypeDef Output){
    LL_GPIO_ResetOutputPin(Output.Port, Output.Pin);
}

/* TX DATA funcions */

// Write function to send one byte data via UART
void hModbusUsartTx8(hModbusTypeDef Handle, uint8_t TxData){
    LL_USART_TransmitData8(Handle.UartHandle, TxData);
}

/* RX DATA funcions */

// Write function to receive one byte data via UART
uint8_t hModbusUsartRx8(hModbusTypeDef Handle){
    return LL_USART_ReceiveData8(Handle.UartHandle);
    return 0;
}

/* UART Flags */

// Write function from your framework which return UART Iddle flag
uint32_t GetUartIdleFlag(hModbusTypeDef Handle){
    return LL_USART_IsActiveFlag_IDLE(Handle.UartHandle);
}

// Write function from your framework to clear UART Iddle flag
void ClearUartIdleFlag(hModbusTypeDef Handle){
    LL_USART_ClearFlag_IDLE(Handle.UartHandle);
}

// Write function from your framework which return UART TXE flag
uint32_t hModbusGetUartTxeFlag(hModbusTypeDef Handle){
    return LL_USART_IsActiveFlag_TXE(Handle.UartHandle);
}

// Write function from your framework which return UART TC flag
uint32_t hModbusGetUartTcFlag(hModbusTypeDef Handle){
    return LL_USART_IsActiveFlag_TC(Handle.UartHandle);
}

// Write function from your framework to clear UART TC flag
void hModbusClearUartTcFlag(hModbusTypeDef Handle){
    LL_USART_ClearFlag_TC(Handle.UartHandle);
}

// Write all actions to execute when hModbus struct is init
void hModbusUsartInit(hModbusTypeDef Handle){
    ; 
}

// Write function from your framework which return UART RXNE flag
uint32_t hModbusGetUartRxneFlag(hModbusTypeDef Handle){
    return LL_USART_IsActiveFlag_RXNE(Handle.UartHandle);
}

// Write function from your framework to enable UART RXNE interrupt
void hModbusEnableRxneIt(hModbusTypeDef Handle){
    LL_USART_EnableIT_RXNE(Handle.UartHandle);
}
