/**
 *  \file 		config.h
 *  \brief		Firmware config file
 *  \details	Contains all config defines and flag for firmware that can be changed by user.
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <main.h>
/* ============================================= */

#define SBC_POWER_OFF_PBLED_BLINK_DELAY 500
#define SBC_POWER_OFF_DELAY 60000
#define POWER_ON_DELAY 	100
#define POWER_OFF_DELAY 500
#define MAIN_LOOP_DELAY	100

/* UART */
// #define UART_BAUD_RATE 38400
#define UART_BAUD_RATE 9600

#endif
/****END OF FILE****/