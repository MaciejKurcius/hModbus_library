/**
  * File Name          : uart_lib_conf.h
  * Description        : Husarion uart lib config file
  * 
**/

#include "uart_lib_conf.h"

// uint8_t ParserRawDataBuf[UART_MAX_DATA_LENGTH];

#ifdef UART_ID_1
    uint8_t UartId_1_RxBuf[UART_ID_1_MAX_RXBUF_SIZE];
    uint8_t UartId_1_TxBuf[UART_ID_1_MAX_TXBUF_SIZE];
    UartTypeDef UartId_1 = {UART_ID_1, {UART_ID_1_MAX_RXBUF_SIZE, UartId_1_RxBuf}, {UART_ID_1_MAX_TXBUF_SIZE, UartId_1_TxBuf}, UART_ID_1_MODE};
#endif


// Copy this code and change ID number for each used instances. See also uart_lib_conf.h file.
#ifdef UART_ID_2
    UartTypeDef UartId_2 = {UART_ID_2};
#endif

void UartInit(UartTypeDef UartHandle, UartModeTypeDef Mode){
    UartHandle.RxFifo.length = 0;
    UartHandle.RxFifo.counter = 0;
    UartHandle.TxFifo.length	= 0;
    UartHandle.TxFifo.counter = 0;
    // UartHandle.Parser.AccDataSize = 0;
}

extern void UartLibDelay(uint32_t DelayTime){
    LL_mDelay(DelayTime);
}


extern uint8_t UartReceiveData8(UartTypeDef UartHandle){
    switch(UartHandle.UartId){
        case UART_ID_1:
            return LL_USART_ReceiveData8(USART1);
        default:
            return 0;
    }
}

extern void UartTransmitData8(UartTypeDef UartHandle, uint8_t Data2Send){
    switch (UartHandle.UartId)
    {
    case UART_ID_1:
        LL_USART_TransmitData8(USART1, Data2Send);
    default:
        break;
    }
}

extern void UartDisableItTxe(UartTypeDef UartHandle){
    switch (UartHandle.UartId)
    {
    case UART_ID_1:
        LL_USART_DisableIT_TXE(USART1);
        LL_USART_DisableIT_TC(USART1);
    default:
        break;
    }   
}

extern void UartEnableItTxe(UartTypeDef UartHandle){
    switch (UartHandle.UartId)
    {
    case UART_ID_1:
        LL_USART_EnableIT_TXE(USART1);
    default:
        break;
    }   
}

extern void UartDisableItRxne(UartTypeDef UartHandle){
    switch (UartHandle.UartId)
    {
    case UART_ID_1:
        LL_USART_DisableIT_RXNE(USART1);
    default:
        break;
    }   
}

extern void UartEnableItRxne(UartTypeDef UartHandle){
    switch (UartHandle.UartId)
    {
    case UART_ID_1:
        LL_USART_EnableIT_RXNE(USART1);
    default:
        break;
    }   
}

extern void UartClearFlags(UartTypeDef UartHandle){
    switch (UartHandle.UartId)
    {
    case UART_ID_1:
		LL_USART_ClearFlag_PE(USART1);
		LL_USART_ClearFlag_FE(USART1);
		LL_USART_ClearFlag_IDLE(USART1);
		LL_USART_ClearFlag_TC(USART1);
		LL_USART_ClearFlag_EOB(USART1);
		LL_USART_ClearFlag_CM(USART1);
		LL_USART_ClearFlag_ORE(USART1);
		LL_USART_ClearFlag_NE(USART1);
    default:
        break;
    }   
}

extern void UartLibInit(UartTypeDef UartHandle){
    switch (UartHandle.UartId)
    {
    case UART_ID_1:
        ;
    default:
        break;
    }   
}