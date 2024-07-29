/**
 * \file		bsp.h
 * \brief		Boar Support Package file
 * \details		It contain all peripherals init function (if they don't have another self file)
 * \author		Maciej Kurcius
 * \version		1.0
 * \date		24.11.2021
 */

#ifndef __BSP_H
#define __BSP_H

/* INCLUDES */
#include <main.h>
#include <gpio.h>
#include <usart.h>

/* DEFINES */

/* TYPE DEF ENUM */

/* STRUCTURES */

/* VARIABLES */

/* FUNCTION PROTOTYPES */

void BoardInit(void);
void SysTickInit(void);
void SystemClockConfig(void);

#endif

/****END OF FILE****/
