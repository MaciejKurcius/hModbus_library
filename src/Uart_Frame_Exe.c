/**
 *  \file 		Uart_Frame_exe.c
 *  \brief		UART protocol frame define file
 *  \details	It contain all UART protocol frame definitions and proper action for them.
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

// #include <Uart_Lib.h>
#include <uart_lib.h>

extern void UartFrameExecute(UartFrameTypeDef *frame){
	switch(frame->command){
		case 1:	//PB info - read only
			// if(GetParamBuffOperationMode(frame) == Read){
			// 	SendPbInfoBuffer();
			// }
			break;
		case 2:	
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10: 
			break;
		case 11: 
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			break;
		case 15: 
			break;
		case 16: 
			break;
		case 17:
			break;
		case 18:
			break;
		case 19:
			break;
		case 20: 
			break;
		case 21:
			break;
		case 22:
			break;
		case 23:
			break;
		case 24:
			break;
		case 25:
			break;
		}
	}

/****END OF FILE****/
