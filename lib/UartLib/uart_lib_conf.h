/**
 *  \file 		uart_lib_conf.h
 *  \brief		UART protocol file
 *  \details	It contain all UART protocol config parameters.
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		23.07.2024
 */

#ifndef UART_LIB_CONF_H
#define UART_LIB_CONF_H

#include "uart_lib.h"
#include "stm32l0xx_ll_usart.h"

#define HMODBUS_RTU

// Usart instance declaration
#define UART_ID_1                       1
#define UART_ID_1_MAX_TXBUF_SIZE        100   
#define UART_ID_1_MAX_RXBUF_SIZE        100   
#define UART_ID_1_MODE                  frame   

// Copy this code and change ID number for each used instances. See also uart_lib_conf.c file.

/* Usart instance declaration
#define UART_ID_2                       2
#define UART_ID_2_MAX_TXBUF_SIZE        100   
#define UART_ID_2_MAX_RXBUF_SIZE        100   
#define UART_ID_2_MODE                  frame   
*/




#endif
/****END OF FILE****/
