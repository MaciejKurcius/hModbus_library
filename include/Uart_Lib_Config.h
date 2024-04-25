/**
 *  \file 		Uart_Lib_Config.h
 *  \brief		UART protocol file
 *  \details	It contain all UART protocol config parameters. See also Uart_Frame_Exe.c
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

#ifndef UART_LIB_CONFIG_H
#define UART_LIB_CONFIG_H

/* DEFINES */


/* Before using this library, add them to your project.
 * This lib use STM_32_LL Lib for UART. Fill all defines below to use lib with your code.
 * You also must change the Usart IT function in your code. The template is below (write in the 'x'
 * USART number which you use in your code).
 *
	void USARTx_IRQHandler(void){
		if(LL_USART_IsActiveFlag_RXNE(USARTx)){	//check if receive data register is not empty
			UsartRxIT();
		}
		if(LL_USART_IsActiveFlag_TXE(USARTx)){	//check if transmit data register is not empty
			UsartTxIT();
		}
	}

 */

#define UART						USART1	//write UART* which is configured in usart.c file
#define RX_TIMEOUT					200
#define UART_TXBUF_SIZE				100
#define UART_RXBUF_SIZE				100
#define UART_MAX_FRAME_ARGS_SIZE	50		//max number of arguments in frames
#define UART_MODE					frame 	//write 'frame' for framing protocol or write 'standard' to send data without framing protocol


#endif
/****END OF FILE****/
