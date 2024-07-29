/**
 *  \file 		logic.h
 *  \brief		File with all logic of program.
 *  \details	It contain all logic function.
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

#ifndef __LOGIC_H
#define __LOGIC_H

/* INCLUDES */

#include <main.h>
#include <string.h>

/* DEFINES */

#define ARRAY_LENGTH(arg)	(sizeof(arg) / sizeof(arg[0]))

/* TYPE DEF ENUM */

typedef enum{
	Off		= 0,
	On		= 1,
	Toggle	= 2
}OutputModeTypeDef;

/* STRUCTURES */

/* FUNCTION PROTOTYPES */

void HardFaultInfLoop(void);
void MainLogicInit(void);
void MainLogicLoop(void);
void PowerOnInitProcedure(void);
void PowerOnPushButtonItProcedure(void);
void SetPowerPushButtonLed(OutputModeTypeDef Mode);

#endif
/****END OF FILE****/
