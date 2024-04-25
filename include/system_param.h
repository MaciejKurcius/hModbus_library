/**
 *  \file 		system_param.h
 *  \brief		
 *  \details	
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

#ifndef SYSTEM_PARAM_H
#define SYSTEM_PARAM_H

#define READ         0
#define READ_WRITE   1

#include <logic.h>

typedef struct{
	uint8_t 	Value[32];
	uint8_t 	RWFlag[2];
}ParamU16TypeDef;

typedef struct{
	uint8_t 	Value[32];
	uint8_t 	RWFlag[4];
}ParamU8TypeDef;

typedef struct{
	uint8_t 	Value[32];
	uint8_t 	RWFlag[32];
}ParamFlagTypeDef;


void SystemParamLoopHandler(void);
void SystemParamInit(void);


void SetBitInU8Array(uint8_t* U8Array, uint8_t BytePointer, uint8_t BitPointer, uint8_t BitValue);
uint8_t GetBitInU8Array(uint8_t* U8Array, uint8_t BytePointer, uint8_t BitPointer);
uint16_t U8ToU16Conv(uint8_t* U8Value);
uint8_t* U16ToU8Conv(uint16_t U16Value, uint8_t* U8Value);

void SetU16Param(ParamU16TypeDef* ParamArray, uint8_t ParamPtr, uint16_t Value);
void SetU8Param(ParamU8TypeDef* ParamArray, uint8_t ParamPtr, uint8_t Value);
void SetParamFlag(ParamFlagTypeDef* ParamArray, uint8_t ParamPtr, uint8_t Value);

uint16_t GetU16Param(ParamU16TypeDef ParamArray, uint8_t ParamPtr);
uint8_t GetU8Param(ParamU8TypeDef ParamArray, uint8_t ParamPtr);
uint8_t GetParamFlag(ParamFlagTypeDef ParamArray, uint8_t ParamPtr);

void SetU16ParamMode(ParamU16TypeDef* ParamArray, uint8_t ParamPtr, uint8_t Mode);
void SetU8ParamMode(ParamU8TypeDef* ParamArray, uint8_t ParamPtr, uint8_t Mode);
void SetParamFlagBuffParamMode(ParamFlagTypeDef* ParamArray, uint8_t ParamPtr, uint8_t Mode);

uint8_t GetU16ParamMode(ParamU16TypeDef ParamArray, uint8_t ParamPtr);
uint8_t GetU8ParamMode(ParamU8TypeDef ParamArray, uint8_t ParamPtr);
uint8_t GetParamFlagBuffParamMode(ParamFlagTypeDef ParamArray, uint8_t ParamPtr);

void InitBatteryStateBuffer(void);
void InitPbInfoBuffer(void);
void InitParamU16_1Buffer(void);
void InitParamU16_2Buffer(void);
void InitParamU8_1Buffer(void);
void InitParamU8_2Buffer(void);
void InitParamFlags_1Buffer(void);

void UpdateBatteryStateBuffer(void);
void UpdatePbInfoBuffer(void);
void UpdateParamU16_1Buffer(void);
void UpdateParamU16_2Buffer(void);
void UpdateParamU8_1Buffer(void);
void UpdateParamU8_2Buffer(void);
void UpdateParamFlags_1Buffer(void);

void SendBatteryStateBuffer(void);
void SendPbInfoBuffer(void);
void SendParamU16_1Buffer(void);
void SendParamU16_2Buffer(void);
void SendParamU8_1Buffer(void);
void SendParamU8_2Buffer(void);
void SendParamFlags_1Buffer(void);

#endif /* SYSTEM_PARAM_H */