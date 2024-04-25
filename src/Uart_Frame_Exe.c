/**
 *  \file 		Uart_Frame_exe.c
 *  \brief		UART protocol frame define file
 *  \details	It contain all UART protocol frame definitions and proper action for them.
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

#include <Uart_Lib.h>

extern ParamU16TypeDef		ParamU16_1;
extern ParamU16TypeDef		ParamU16_2;
extern ParamU8TypeDef 		ParamU8_1;
extern ParamU8TypeDef 		ParamU8_2;
extern ParamFlagTypeDef	ParamFlags_1;
extern uint8_t BatteryState[];

extern void UartFrameExecute(UartFrameTypeDef *frame){
	switch(frame->command){
		case 1:	//PB info - read only
			if(GetParamBuffOperationMode(frame) == Read){
				SendPbInfoBuffer();
			}
			break;
		case 2:	//Battery state - read only
			if(GetParamBuffOperationMode(frame) == Read){	
				BatteryState[0] = 1;	//set battery state buff read flag to 1
			}
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
		case 10: //ParamU16_1
			if(GetParamBuffOperationMode(frame) == Read){
				SendParamU16_1Buffer();
			}
			if(GetParamBuffOperationMode(frame) == Write){
				for(uint8_t i = 0; i < frame->size; i++){
					if(GetU16ParamMode(ParamU16_1, i/2)){
						ParamU16_1.Value[i] = frame->data[i];
					}
				}
			}
			break;
		case 11: //ParamU16_2
			if(GetParamBuffOperationMode(frame) == Read){
				SendParamU16_2Buffer();
			}
			if(GetParamBuffOperationMode(frame) == Write){
				for(uint8_t i = 0; i < frame->size; i++){
					if(GetU16ParamMode(ParamU16_2, i/2)){
						ParamU16_2.Value[i] = frame->data[i];
					}
				}
			}
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			break;
		case 15: //ParamU8_1
			if(GetParamBuffOperationMode(frame) == Read){
				SendParamU8_1Buffer();
			}
			if(GetParamBuffOperationMode(frame) == Write){
				for(uint8_t i = 0; i < frame->size; i++){
					if(GetU8ParamMode(ParamU8_1, i)){
						ParamU8_1.Value[i] = frame->data[i];
					}
				}
			}
			break;
		case 16: //ParamU8_2
			if(GetParamBuffOperationMode(frame) == Read){
				SendParamU8_2Buffer();
			}
			if(GetParamBuffOperationMode(frame) == Write){
				for(uint8_t i = 0; i < frame->size; i++){
					if(GetU8ParamMode(ParamU8_2, i)){
						ParamU8_2.Value[i] = frame->data[i];
					}
				}
			}
			break;
		case 17:
			break;
		case 18:
			break;
		case 19:
			break;
		case 20: //Param_Flags
			if(GetParamBuffOperationMode(frame) == Read){
				SendParamFlags_1Buffer();
			}
			if(GetParamBuffOperationMode(frame) == Write){
				for(uint8_t i = 0; i < frame->size * 8; i++){
					if(GetParamFlagBuffParamMode(ParamFlags_1, i)){
						SetParamFlagBuffParamMode(&ParamFlags_1, i, (frame->data[i / 8] >> (i % 8) & 0x01));	
					}
				}
			}
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
		{
			uint8_t ParamSize = frame->data[0];
			uint8_t ParamArrayNum = frame->data[1];
			uint8_t ParamNum = frame->data[2];
			uint16_t ParamValue = ( frame->data[10] << 8 ) | ( frame->data[11] );
			if(ParamSize == 1){
				if(ParamArrayNum == 1)	SetParamFlag(&ParamFlags_1, 0, 0x01);
				if(ParamArrayNum == 1)	SetParamFlag(&ParamFlags_1, ParamNum, (uint8_t)ParamValue & 0x01);
			}
			if(ParamSize == 8){
				if(ParamArrayNum == 1)	SetU8Param(&ParamU8_1, ParamNum, (uint8_t)ParamValue);
				if(ParamArrayNum == 2)	SetU8Param(&ParamU8_2, ParamNum, (uint8_t)ParamValue);
			}
			if(ParamSize == 16){
				if(ParamArrayNum == 1)	SetU16Param(&ParamU16_1, ParamNum, ParamValue);
				if(ParamArrayNum == 2)	SetU16Param(&ParamU16_2, ParamNum, ParamValue);
			}
			break;
		}
	}
}
/****END OF FILE****/
