#ifndef hMODBUS_LIB_CONF_H
#define hMODBUS_LIB_CONF_H

#include <stdio.h>
#include <stm32l051xx.h>
#include <stm32l0xx_ll_gpio.h>


// x86_64-linux-gnu/diagslave -m rtu -o 1 -a 1 -b 9600 -d 8 -p none  /dev/ttyUSB0 <- command for digislave


#define HMODBUS_RXTX_SIZE 64  // Write RX and TX buffer size - size is common for each one instance. 254 bytes is max for modbus RTU standard

#define UART_HANDLE_TYPE USART_TypeDef*     // Write your UART type def struct ptr from your framework
#define GPIO_TYPE_DEF GPIO_TypeDef*         // Write your GPIO port type def struct ptr from your framework

void hModbusMaster1UartInit(void);

#endif