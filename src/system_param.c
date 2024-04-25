/**
 *  \file 		system_param.c
 *  \brief		
 *  \details	
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

#include <system_param.h>

//Extern variables
extern BatteryTypeDef Battery;


//Variables
ParamU16TypeDef		ParamU16_1 		= {.Value = {0}, .RWFlag = {0}};
ParamU16TypeDef		ParamU16_2 		= {.Value = {0}, .RWFlag = {0}};
ParamU8TypeDef 		ParamU8_1 		= {.Value = {0}, .RWFlag = {0}};
ParamU8TypeDef 		ParamU8_2 		= {.Value = {0}, .RWFlag = {0}};
ParamFlagTypeDef	ParamFlags_1 	= {.Value = {0}, .RWFlag = {0}};
uint8_t BatteryState[18];
char PbInfo[25];

// start of bit functions
void WriteParamToUint16Buff(uint8_t* U16Array, uint8_t MsbPtr, uint16_t Value){
	*(U16Array + MsbPtr) = (Value >> 8) & 0xFF;
	*(U16Array + MsbPtr + 1) = Value & 0xFF;
}

void SetBitInU8Array(uint8_t* U8Array, uint8_t BytePointer, uint8_t BitPointer, uint8_t BitValue){
	if(BitValue == 0) *(U8Array + BytePointer) &= ~(1 << BitPointer);
	if(BitValue == 1) *(U8Array + BytePointer) |= (1 << BitPointer);
}

uint8_t GetBitInU8Array(uint8_t* U8Array, uint8_t BytePointer, uint8_t BitPointer){
    return (*(U8Array + BytePointer) >> BitPointer) & 0x01;
}

uint16_t U8ToU16Conv(uint8_t* U8Value){
    return (uint16_t)(*(U8Value) + (*(U8Value + 1) << 8));
}

uint8_t* U16ToU8Conv(uint16_t U16Value, uint8_t* U8Value){
    *(U8Value) = (U16Value & 0x00FF);
    *(U8Value + 1) = (U16Value & 0xFF00) >> 8;
    return U8Value;
}

// end of bit funtions

void SetU16Param(ParamU16TypeDef* ParamArray, uint8_t ParamPtr, uint16_t Value){
    if(GetU16ParamMode(*ParamArray, ParamPtr)){
        ParamArray->Value[ParamPtr * 2] = Value & 0x00FF;
        ParamArray->Value[(ParamPtr * 2) + 1] = (Value & 0xFF00) >> 8;
    }
}

void SetU8Param(ParamU8TypeDef* ParamArray, uint8_t ParamPtr, uint8_t Value){
    if(GetU8ParamMode(*ParamArray, ParamPtr)){
        ParamArray->Value[ParamPtr] = Value;
    }
}

void SetParamFlag(ParamFlagTypeDef* ParamArray, uint8_t ParamPtr, uint8_t Value){
    if(GetParamFlagBuffParamMode(*ParamArray, ParamPtr)){
        SetBitInU8Array(ParamArray->Value, ParamPtr / 8, ParamPtr % 8, Value);
    }
}

uint16_t GetU16Param(ParamU16TypeDef ParamArray, uint8_t ParamPtr){
    return ParamArray.Value[ParamPtr * 2] << 8 | ParamArray.Value[ParamPtr * 2 + 1];
}

uint8_t GetU8Param(ParamU8TypeDef ParamArray, uint8_t ParamPtr){
    return ParamArray.Value[ParamPtr];
}

uint8_t GetParamFlag(ParamFlagTypeDef ParamArray, uint8_t ParamPtr){
    return ( ParamArray.Value[ParamPtr / 8] >> (ParamPtr % 8) ) & 0x01;
}

// set param mode
void SetU16ParamMode(ParamU16TypeDef* ParamArray, uint8_t ParamPtr, uint8_t Mode){
    SetBitInU8Array(ParamArray->RWFlag, ParamPtr / 8, ParamPtr % 8, Mode);
}

void SetU8ParamMode(ParamU8TypeDef* ParamArray, uint8_t ParamPtr, uint8_t Mode){
    SetBitInU8Array(ParamArray->RWFlag, ParamPtr / 8, ParamPtr % 8, Mode);
}

void SetParamFlagBuffParamMode(ParamFlagTypeDef* ParamArray, uint8_t ParamPtr, uint8_t Mode){
    SetBitInU8Array(ParamArray->RWFlag, ParamPtr / 8, ParamPtr % 8, Mode);
}

// get param mode
uint8_t GetU16ParamMode(ParamU16TypeDef ParamArray, uint8_t ParamPtr){
    return GetBitInU8Array(ParamArray.RWFlag, ParamPtr / 8, ParamPtr % 8);
}

uint8_t GetU8ParamMode(ParamU8TypeDef ParamArray, uint8_t ParamPtr){
    return GetBitInU8Array(ParamArray.RWFlag, ParamPtr / 8, ParamPtr % 8);
}

uint8_t GetParamFlagBuffParamMode(ParamFlagTypeDef ParamArray, uint8_t ParamPtr){
    return GetBitInU8Array(ParamArray.RWFlag, ParamPtr / 8, ParamPtr % 8);
}



// init param buffers
void InitBatteryStateBuffer(void){
    ;//No actions. All parameters are read only.
}

void InitPbInfoBuffer(void){
	memcpy(PbInfo+1, FIRMWARE_VERSION, 13);
	memcpy(PbInfo+13, GetBoardVersion(), BOARD_VER_MEM_SIZE);
}

void InitParamU16_1Buffer(void){
    ;
}

void InitParamU16_2Buffer(void){
    ;
}

void InitParamU8_1Buffer(void){
    ;
}

void InitParamU8_2Buffer(void){
    ;
}

void InitParamFlags_1Buffer(void){
	ParamFlags_1.RWFlag[0] = 1;
    SetParamFlagBuffParamMode(&ParamFlags_1, 8, READ_WRITE);
    SetParamFlagBuffParamMode(&ParamFlags_1, 9, READ);
    SetParamFlagBuffParamMode(&ParamFlags_1, 10, READ_WRITE);
    SetParamFlagBuffParamMode(&ParamFlags_1, 11, READ);
    SetParamFlagBuffParamMode(&ParamFlags_1, 12, READ);
    SetParamFlagBuffParamMode(&ParamFlags_1, 13, READ);
    SetParamFlagBuffParamMode(&ParamFlags_1, 14, READ_WRITE);
    SetParamFlagBuffParamMode(&ParamFlags_1, 15, READ_WRITE);
    SetParamFlagBuffParamMode(&ParamFlags_1, 16, READ_WRITE);
    SetParamFlagBuffParamMode(&ParamFlags_1, 17, READ_WRITE);
    SetParamFlagBuffParamMode(&ParamFlags_1, 18, READ_WRITE);
    SetParamFlagBuffParamMode(&ParamFlags_1, 19, READ_WRITE);

    SetParamFlagBuffParamMode(&ParamFlags_1, 24, READ_WRITE);
    SetParamFlagBuffParamMode(&ParamFlags_1, 25, READ_WRITE);

    SetParamFlagBuffParamMode(&ParamFlags_1, 32, READ);
    SetParamFlagBuffParamMode(&ParamFlags_1, 33, READ);
    SetParamFlagBuffParamMode(&ParamFlags_1, 34, READ);
    SetParamFlagBuffParamMode(&ParamFlags_1, 35, READ_WRITE);
    SetParamFlagBuffParamMode(&ParamFlags_1, 36, READ_WRITE);   
    SetParamFlagBuffParamMode(&ParamFlags_1, 37, READ);

    SetParamFlagBuffParamMode(&ParamFlags_1, 40, READ);
    SetParamFlagBuffParamMode(&ParamFlags_1, 41, READ);
    SetParamFlagBuffParamMode(&ParamFlags_1, 42, READ_WRITE);
}


//update param buffers

void UpdateBatteryStateBuffer(void){
	BatteryState[1] = (Battery.Voltage >> 8) & 0xFF;
	BatteryState[2] = Battery.Voltage & 0xFF;
	BatteryState[3] = (Battery.Temperature >> 8) & 0xFF;
	BatteryState[4] = Battery.Temperature & 0xFF;
	BatteryState[5] = (Battery.Current >> 8) & 0xFF;
	BatteryState[6] = Battery.Current & 0xFF;
	BatteryState[7] = (Battery.ChargeCurrent >> 8) & 0xFF;
	BatteryState[8] = Battery.ChargeCurrent & 0xFF;
	BatteryState[9] = (Battery.Capacity >> 8) & 0xFF;
	BatteryState[10] = Battery.Capacity & 0xFF;
	BatteryState[11] = (Battery.DesignCapacity >> 8) & 0xFF;
	BatteryState[12] = Battery.DesignCapacity & 0xFF;
	BatteryState[13] = Battery.Percentage;
	BatteryState[14] = Battery.Status;
	BatteryState[15] = Battery.Health;
	BatteryState[16] = Battery.Technology;
	BatteryState[17] = Battery.Present;
}

void UpdatePbInfoBuffer(void){
    ;
}

void UpdateParamU16_1Buffer(void){
	;
}

void UpdateParamU16_2Buffer(void){
	;
}

void UpdateParamU8_1Buffer(void){
	;
}

void UpdateParamU8_2Buffer(void){
	if(ParamU8_1.Value[0] == 0); //Update
	if(ParamU8_1.Value[0] == 1){ //Write
		ParamU8_1.Value[0] = 0;
	}; 
}

void UpdateParamFlags_1Buffer(void){
	if(ParamFlags_1.Value[0] == 0){
		//byte 1
		// SetBitInU8Array(ParamFlags_1.Value, 1, 0, LL_GPIO_IsOutputPinSet(VS_SW_DRV_CH1_GPIO_Port,	VS_SW_DRV_CH1_Pin));
		// bit 1 - usied to controll VS SW CH1 state. No need to update.
		SetBitInU8Array(ParamFlags_1.Value, 1, 2, LL_GPIO_IsOutputPinSet(VS_SW_DRV_CH2_GPIO_Port,	VS_SW_DRV_CH2_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 1, 3, LL_GPIO_IsInputPinSet(VS_SW_CH2_PG_GPIO_Port,	VS_SW_CH2_PG_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 1, 4, LL_GPIO_IsInputPinSet(PG_5VDC_GPIO_Port,			PG_5VDC_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 1, 5, LL_GPIO_IsInputPinSet(PG_12VDC_GPIO_Port,		PG_12VDC_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 1, 6, LL_GPIO_IsOutputPinSet(POWER_LOCK_GPIO_Port,		POWER_LOCK_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 1, 7, LL_GPIO_IsOutputPinSet(VBUS_DISCH_GPIO_Port,		VBUS_DISCH_Pin));
		//byte 2
		SetBitInU8Array(ParamFlags_1.Value, 2, 0, LL_GPIO_IsOutputPinSet(MCU_PWR_1_GPIO_Port,			MCU_PWR_1_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 2, 1, LL_GPIO_IsInputPinSet(MCU_PWR_2_GPIO_Port,			MCU_PWR_2_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 2, 2, LL_GPIO_IsOutputPinSet(MCU_OUT_PWR_ENABLE_GPIO_Port,	MCU_OUT_PWR_ENABLE_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 2, 3, LL_GPIO_IsOutputPinSet(DIGITAL_BOARD_RST_GPIO_Port,	DIGITAL_BOARD_RST_Pin));
		// SetBitInU8Array(ParamFlags_1.Value, 2, 4, LL_GPIO_IsInputPinSet());
		// SetBitInU8Array(ParamFlags_1.Value, 2, 5, LL_GPIO_IsInputPinSet());
		// SetBitInU8Array(ParamFlags_1.Value, 2, 6, LL_GPIO_IsInputPinSet());
		// SetBitInU8Array(ParamFlags_1.Value, 2, 7, LL_GPIO_IsInputPinSet());
		//byte 3
		// bit 0 - using to controll RA enable state. No need to update
		// bit 1 - Robotic Arm inverter reset. Proceeding in RA inverter lopp handler 
		// SetBitInU8Array(ParamFlags_1.Value, 3, 2, LL_GPIO_IsInputPinSet());
		// SetBitInU8Array(ParamFlags_1.Value, 3, 3, LL_GPIO_IsInputPinSet());
		// SetBitInU8Array(ParamFlags_1.Value, 3, 4, LL_GPIO_IsInputPinSet());
		// SetBitInU8Array(ParamFlags_1.Value, 3, 5, LL_GPIO_IsInputPinSet());
		// SetBitInU8Array(ParamFlags_1.Value, 3, 6, LL_GPIO_IsInputPinSet());
		// SetBitInU8Array(ParamFlags_1.Value, 3, 7, LL_GPIO_IsInputPinSet());
		//byte 4
		SetBitInU8Array(ParamFlags_1.Value, 4, 0, LL_GPIO_IsInputPinSet(IND_ADPT_SNS_GPIO_Port,	IND_ADPT_SNS_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 4, 1, LL_GPIO_IsInputPinSet(USBC_ADPT_SNS_GPIO_Port,	USBC_ADPT_SNS_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 4, 2, LL_GPIO_IsInputPinSet(BAT_REMOVAL_GPIO_Port,		BAT_REMOVAL_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 4, 3, LL_GPIO_IsOutputPinSet(LED_CHARGE_GPIO_Port,		LED_CHARGE_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 4, 4, LL_GPIO_IsOutputPinSet(LED_LOWBAT_GPIO_Port,		LED_LOWBAT_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 4, 5, LL_GPIO_IsInputPinSet(CHRG_OK_GPIO_Port,			CHRG_OK_Pin));
		// SetBitInU8Array(ParamFlags_1.Value, 4, 6, LL_GPIO_IsInputPinSet());
		// SetBitInU8Array(ParamFlags_1.Value, 4, 7, LL_GPIO_IsInputPinSet());
		//byte 5
		SetBitInU8Array(ParamFlags_1.Value, 5, 0, LL_GPIO_IsInputPinSet(SBC_PWR_ON_SNS_GPIO_Port,	SBC_PWR_ON_SNS_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 5, 1, LL_GPIO_IsInputPinSet(PWR_SW_INT_GPIO_Port,		PWR_SW_INT_Pin));
		SetBitInU8Array(ParamFlags_1.Value, 5, 2, LL_GPIO_IsOutputPinSet(LED_PWR_PB_GPIO_Port,		LED_PWR_PB_Pin));
		// SetBitInU8Array(ParamFlags_1.Value, 5, 3, LL_GPIO_IsInputPinSet());
		// SetBitInU8Array(ParamFlags_1.Value, 5, 4, LL_GPIO_IsInputPinSet());
		// SetBitInU8Array(ParamFlags_1.Value, 5, 5, LL_GPIO_IsInputPinSet());
		// SetBitInU8Array(ParamFlags_1.Value, 5, 6, LL_GPIO_IsInputPinSet());
		// SetBitInU8Array(ParamFlags_1.Value, 5, 7, LL_GPIO_IsInputPinSet());
	}
	if(ParamFlags_1.Value[0] == 1){
		ParamFlags_1.Value[0] = 0;
	}
}


// send param buffers

void SendBatteryStateBuffer(void){
	BatteryState[0] = 0;
	UartSendFrameD(2, 18, BatteryState);
}

void SendPbInfoBuffer(void){
	PbInfo[0] = 0;
	UartSendFrameD(1, 25, (uint8_t*)PbInfo);
}

void SendParamU16_1Buffer(void){
	UpdateParamU16_1Buffer();
	ParamU16_1.Value[0] = 0;
	UartSendFrameD(10, 32, ParamU16_1.Value);
}

void SendParamU16_2Buffer(void){
	UpdateParamU16_2Buffer();
	ParamU16_2.Value[0] = 0;
	UartSendFrameD(11, 32, ParamU16_2.Value);
}

void SendParamU8_1Buffer(void){
	UpdateParamU8_1Buffer();
	ParamU8_1.Value[0] = 0;
	UartSendFrameD(15, 32, ParamU8_1.Value);
}

void SendParamU8_2Buffer(void){
	UpdateParamU8_2Buffer();
	ParamU8_2.Value[0] = 0;
	UartSendFrameD(16, 32, ParamU8_2.Value);
}

void SendParamFlags_1Buffer(void){
	UpdateParamFlags_1Buffer();
	ParamFlags_1.Value[0] = 0;
	UartSendFrameD(20, 32, ParamFlags_1.Value);
}