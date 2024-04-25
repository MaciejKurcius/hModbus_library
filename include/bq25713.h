/**
 *  \file 		bq25713.h
 *  \brief		Library for BQ25713 handling.
 *  \details	It contain all functions for handling BQ25713. Library using STM32 LL library
 *  			for handling I2C communication. To use them, you should previous configure I2C
 *  			transmission and write suitable parameters to 'bq25713_config.h' file.
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 *  \note
 */

#ifndef INC_BQ25713_H_
#define INC_BQ25713_H_

/* INCLUDES */

#include <bq25713_config.h>
#include <main.h>


/* DEFINES */

#define BQ_RX_ERR 0xFFFF

//define BQ read/write register addresses
#define CHARGE_OPTION0_0		0x00
#define CHARGE_OPTION0_1 		0x01
#define CHARGE_CURRENT_0		0x02
#define CHARGE_CURRENT_1		0x03
#define MAX_CHARGE_VOLTAGE_0	0X04
#define MAX_CHARGE_VOLTAGE_1	0x05
#define CHARGE_OPTION1_0		0x30
#define CHARGE_OPTION1_1		0x31
#define CHARGE_OPTION2_0		0x32
#define CHARGE_OPTION2_1		0x33
#define	CHARGE_OPTION3_0		0x34
#define CHARGE_OPTION3_1		0x35
#define PROCHOT_OPTION0_0		0x36
#define PROCHOT_OPTION0_1		0x37
#define PROCHOT_OPTION1_0		0x38
#define PROCHOT_OPTION1_1		0x39
#define ADC_OPTION_0			0x3A
#define ADC_OPTION_1			0x3B
#define INPUT_VOLTAGE_0			0x0A
#define INPUT_VOLTAGE_1			0x0B
#define MIN_SYSTEM_VOLTAGE_0	0x0C
#define MIN_SYSTEM_VOLTAGE_1	0x0D
#define IIN_HOST_0				0x0E
#define IIN_HOST_1				0x0F

//define BQ read only registers addresses
#define CHARGER_STATUS_0 		0X20	//handled
#define CHARGER_STATUS_1		0x21	//handled
#define PROCHOT_STATUS_0		0x22	//to check
#define PROCHOT_STATUS_1		0x23	//to check
#define IIN_DPM_0				0x24	//handled
#define IIN_DPM_1				0x25	//handled
#define ADCVBUS_PSYS_0			0x26	//handled
#define ADCVBUS_PSYS_1 			0x27	//handled
#define ADCIBAT_0 				0x28	//handled
#define ADCIBAT_1 				0x29	//handled
#define ADCIINCMPIN_0 			0x2A	//not used
#define ADCIINCMPIN_1 			0x2B	//not used
#define ADCVSYSVBAT_0 			0x2C	//handled
#define ADCVSYSVBAT_1 			0x2D	//handled
#define	OTG_VOLTAGE_0 			0x06	//not used
#define OTG_VOLTAGE_1			0x07	//not used
#define OTG_CURRENT_0			0x08	//not used
#define OTG_CURRENT_1			0x09	//not used
#define MANUFACTURER_ID			0x2E
#define DEVICE_ID				0x2F

//another defines
#define AVG_BATTERY_VOLTAGE_BUFFER_SIZE				100
#define AVG_BATTERY_CHARGE_CURRENT_BUFFER_SIZE		10

/* TYPE DEF ENUM */

typedef enum{
	ChargeCurrentMode 		= 0x01,
	DischargeCurrentMode 	= 0x00
}BqIbatModeTypeDef;

typedef enum{
	LowPowerModeEnable		= 0x01,
	LowPowerModeDisable		= 0x00
}BqPowerModeTypeDef;

typedef enum{
	Enable				= 0x01,
	Disable				= 0x00
}BqOptionStateTypeDef;

typedef enum{
	ContinousMode		= 0x80,
	OneShootMode		= 0x00,
	TransmissionErr		= 0x01
}BqAdcModeTypeDef;

/* STRUCTURES */

/* VARIABLES */

volatile uint16_t 	BqBatteryDischargeCurrent;
volatile uint16_t	BqBatteryChargeCurrent;
volatile uint16_t	BqAvgBatteryChargeCurrent;
volatile uint16_t 	BqBatteryVoltage;
volatile uint16_t 	BqAvgBatteryVoltage;
volatile uint16_t 	BqSystemVoltage;
volatile uint16_t 	BqInputVoltage;				//VBUS in datasheet
volatile uint16_t 	BqSystemPower;  			//PSYS in datasheet
volatile uint16_t 	BqInputCurrentInDpm;
volatile uint16_t 	BqInputCurrent;
volatile uint8_t 	BqI2cAddr;
volatile uint8_t 	BqChargeCurrentMsbSetpoint;
volatile uint8_t 	BqChargeCurrentLsbSetpoint;
volatile uint8_t	BqFlagExtAdapterOv;
volatile uint8_t	BqFlagExtAdapterOc;
volatile uint8_t	BqFlagBatteryOc;
volatile uint8_t	BqFlagSystemOv;

/* FUNCTIONS PROTOTYPES */

/* Functions for I2C communication with BQ module */
uint8_t BqCheckI2cAddr(void);
uint8_t BqWriteRegister(uint8_t RegAddr, uint8_t* Value, uint8_t Size);
uint8_t BqReadRegister(uint8_t RegAddr, uint8_t* Buff, uint8_t Size);
uint8_t BqWriteByte(uint8_t RegAddr, uint8_t Value);
uint16_t BqReadByte(uint8_t RegAddr);
// uint16_t BqI2cTimeoutExceedAction(void);

/* Functions for setting BQ parameters */
uint8_t BqInit(void);
uint8_t BqSetMaxChargeVoltage(uint8_t MSByte, uint8_t LSByte);
uint8_t BqSetChargeCurrentByReg(uint8_t MSByte, uint8_t LSByte);
uint8_t BqSetChargeCurrentByVal(uint16_t ChargeCurrent);
uint8_t BqSetAcAdptMaxCurrent(uint16_t AcAdptMaxCurrent);
uint8_t BqSetMinAdapterInputVoltage(uint8_t MSByte, uint8_t LSByte);
uint8_t BqResetAllReg(void);
uint8_t BqSetPowerMode(BqPowerModeTypeDef PwrMode);
uint8_t BqSetIbatMode(BqIbatModeTypeDef IbatMode);
uint8_t BqSetIbat(BqOptionStateTypeDef State);
void 	BqClearSystemOverVoltageFlag(void);
/* Functions for reading BQ parameters */
uint16_t BqCheckAdapterPresent(void);
uint16_t BqCheckBatteryPresent(void);
uint8_t BqInputVoltageUpdate(void);
uint8_t BqSystemPowerUpdate(void);
uint8_t BqBatteryChargeCurrentUpdate(void);
uint8_t BqBatteryDischargeCurrentUpdate(void);
uint8_t BqBatteryVoltageUpdate(void);
uint8_t BqSystemVoltageUpdate(void);
uint8_t BqInputCurrentInDpmUpdate(void);
uint8_t BqInputCurrentUpdate(void);
uint8_t BqAdcRegistersUpdate(void);
/* Auxiliary functons */
uint8_t BqAdcConversionStart(void);
BqAdcModeTypeDef BqCheckAdcConversionMode(void);
uint8_t BqIsNthBitSet(uint16_t Value, uint8_t BitNumber);
/* External function */
uint8_t BqLoopHandler(void);	// Use this function in main loop in program
uint8_t BqErrorHandler(void);

#if BQ_DEBUG_MODE == 1

/* DEFINES */

/* TYPE DEF ENUM */

typedef enum{
	False	= 0,
	True 	= 1
}BoolTypeDef;

/* STRUCTURES */

typedef struct{
	//register address 0x01
	BoolTypeDef EnLwpwr				:1;
	BoolTypeDef WdtmrAdj_1			:1;
	BoolTypeDef WdtmrAdj_0			:1;
	BoolTypeDef IdpmAutoDisable		:1;
	BoolTypeDef OtgOnChargOk		:1;
	BoolTypeDef EnOoa				:1;
	BoolTypeDef	PwmFreq				:1;
	BoolTypeDef LowPtmRipple		:1;
	//register address 0x00
	BoolTypeDef SysShortDisable		:1;
	BoolTypeDef EnLearn				:1;
	BoolTypeDef IadptGain			:1;
	BoolTypeDef	IbatGain			:1;
	BoolTypeDef EnLdo				:1;
	BoolTypeDef EnIpdm				:1;
	BoolTypeDef ChrgInhibit			:1;
	//RxErr flag
	BoolTypeDef RxErr				:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}ChargeOption0BqRegTypeDef;

typedef struct{
	//register address 0x31
	BoolTypeDef EnIbat				:1;
	BoolTypeDef	EnProchotLpwr_1		:1;
	BoolTypeDef	EnProchotLpwr_0		:1;
	BoolTypeDef EnPsys				:1;
	BoolTypeDef	RsnsRac				:1;
	BoolTypeDef RsnsRsr				:1;
	BoolTypeDef PsysRatio			:1;
	BoolTypeDef PtmPinsel			:1;
	//register address 0x30
	BoolTypeDef CmpRef				:1;
	BoolTypeDef CmpPol				:1;
	BoolTypeDef CmpDeg_1			:1;
	BoolTypeDef CmpDeg_0			:1;
	BoolTypeDef ForceLatchoff		:1;
	BoolTypeDef	EnPtm				:1;
	BoolTypeDef	EnShipDchg			:1;
	BoolTypeDef AutoWakeUpEn		:1;
	//RxErr flag
	BoolTypeDef RxErr				:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}ChargeOption1BqRegTypeDef;

typedef struct{
	//register address 0x33
	BoolTypeDef PkpwrTovldDeg_1		:1;
	BoolTypeDef PkpwrTovldDeg_0		:1;
	BoolTypeDef	EnPkpwrIdpm			:1;
	BoolTypeDef EnPkpwrVsys			:1;
	BoolTypeDef PkpwrOvldStat		:1;
	BoolTypeDef	PkpwrRelaxStat		:1;
	BoolTypeDef PkpwrTmax_1			:1;
	BoolTypeDef PkpwrTmax_0			:1;
	//register address 0x32
	BoolTypeDef EnExtilim			:1;
	BoolTypeDef EnIchgIdchg			:1;
	BoolTypeDef	Q2Ocp				:1;
	BoolTypeDef	AcxOcp				:1;
	BoolTypeDef	EnAcoc				:1;
	BoolTypeDef AcocVth				:1;
	BoolTypeDef EnBatoc				:1;
	BoolTypeDef BatocVth			:1;
	//RxErr flag
	BoolTypeDef RxErr				:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}ChargeOption2BqRegTypeDef;

typedef struct{
	//register address 0x35
	BoolTypeDef EnHiz				:1;
	BoolTypeDef ResetReg			:1;
	BoolTypeDef ResetVindpm			:1;
	BoolTypeDef EnOtg				:1;
	BoolTypeDef EnIcoMode			:1;
	//register address 0x34
	BoolTypeDef EnConVap			:1;
	BoolTypeDef OtgVapMode			:1;
	BoolTypeDef IlAvg_1				:1;
	BoolTypeDef IlAvg_0				:1;
	BoolTypeDef OtgRangeLow			:1;
	BoolTypeDef BatfetoffHiz		:1;
	BoolTypeDef PsysOthIdchg		:1;
	//RxErr flag
	BoolTypeDef RxErr				:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}ChargeOption3BqRegTypeDef;

typedef struct{
	//register addres 0x37
	BoolTypeDef Ilim2Vth_4			:1;
	BoolTypeDef Ilim2Vth_3			:1;
	BoolTypeDef Ilim2Vth_2			:1;
	BoolTypeDef Ilim2Vth_1			:1;
	BoolTypeDef Ilim2Vth_0			:1;
	BoolTypeDef IcritDeg_1			:1;
	BoolTypeDef IcritDeg_0			:1;
	BoolTypeDef ProchotVdpm_80_90	:1;
	//register address 0x36
	BoolTypeDef VsysTh1_3			:1;
	BoolTypeDef VsysTh1_2			:1;
	BoolTypeDef VsysTh1_1			:1;
	BoolTypeDef VsysTh1_0			:1;
	BoolTypeDef VsysTh2_1			:1;
	BoolTypeDef VsysTh2_0			:1;
	BoolTypeDef InomDeg				:1;
	BoolTypeDef LowerProchotVdpm	:1;
	//RxErr flag
	BoolTypeDef RxErr				:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}ProchotOption0BqRegTypeDef;

typedef struct{
	//register address 0x39
	BoolTypeDef IdchgVth_5				:1;
	BoolTypeDef IdchgVth_4				:1;
	BoolTypeDef IdchgVth_3				:1;
	BoolTypeDef IdchgVth_2				:1;
	BoolTypeDef	IdchgVth_1				:1;
	BoolTypeDef	IdchgVth_0				:1;
	BoolTypeDef IdchgDeg_1				:1;
	BoolTypeDef IdchgDeg_0				:1;
	//register address 0x38
	BoolTypeDef ProchotProfileVdpm		:1;
	BoolTypeDef ProchotProfileComp		:1;
	BoolTypeDef ProchotProfileIcrit		:1;
	BoolTypeDef ProchotProfileInom		:1;
	BoolTypeDef ProchotProfileIdchg 	:1;
	BoolTypeDef ProchotProfileVsys		:1;
	BoolTypeDef ProchotProfileBatpres	:1;
	BoolTypeDef ProchotProfileAcok		:1;
	//RxErr flag
	BoolTypeDef RxErr				:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}ProchotOption1BqRegTypeDef;

typedef struct{
	//register address 0x3B
	BoolTypeDef AdcConv				:1;
	BoolTypeDef AdcStart			:1;
	BoolTypeDef AdcFullscale		:1;
	//register address 0x3A
	BoolTypeDef EnAdcCmpin			:1;
	BoolTypeDef EnAdcVbus			:1;
	BoolTypeDef EnAdcPsys			:1;
	BoolTypeDef EnAdcIin			:1;
	BoolTypeDef EnAdcIdchg			:1;
	BoolTypeDef EnAdcIchg			:1;
	BoolTypeDef EnAdcVsys			:1;
	BoolTypeDef EnAdcVbat			:1;
	//RxErr flag
	BoolTypeDef RxErr				:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}AdcOptionBqRegTypeDef;

typedef struct{
	//register address 0x21
	BoolTypeDef AcStat 			:1;
	BoolTypeDef IcoDone 		:1;
	BoolTypeDef InVap 			:1;
	BoolTypeDef InVindpm 		:1;
	BoolTypeDef InIindpm 		:1;
	BoolTypeDef InFchrg			:1;
	BoolTypeDef InPchrg 		:1;
	BoolTypeDef InOtg 			:1;
	//register address 0x20
	BoolTypeDef FaultAcov 		:1;
	BoolTypeDef FaultBatoc 		:1;
	BoolTypeDef FaultAcoc 		:1;
	BoolTypeDef SysOvpStat	 	:1;
	BoolTypeDef FaultSysShort 	:1;
	BoolTypeDef FaultLatchoff 	:1;
	BoolTypeDef FaultOtgOvp 	:1;
	BoolTypeDef FaultOtgUvp 	:1;
	//RxErr flag
	BoolTypeDef RxErr			:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}ChargerStatusBqRegTypeDef;

typedef struct{
	//register address 0x23
	BoolTypeDef EnProchotExit		:1;
	BoolTypeDef ProchotWidth_1		:1;
	BoolTypeDef ProchotWidth_0		:1;
	BoolTypeDef ProchotClear		:1;
	BoolTypeDef StatVapFail			:1;
	BoolTypeDef StatExitVap			:	1;
	//register address 0x22
	BoolTypeDef StatVpdm 			:1;
	BoolTypeDef StatComp 			:1;
	BoolTypeDef StatIcrit 			:1;
	BoolTypeDef StatInom 			:1;
	BoolTypeDef StatIdchg 			:1;
	BoolTypeDef StatVsys 			:1;
	BoolTypeDef StatBatteryRemoval 	:1;
	BoolTypeDef StatAdapterRemoval 	:1;
	//RxErr flag
	BoolTypeDef RxErr				:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}ProchotStatusBqRegTypeDef;

typedef struct{
	//register address 0x03
	BoolTypeDef InvalidWrite_2	:1;
	BoolTypeDef InvalidWrite_1	:1;
	BoolTypeDef InvalidWrite_0	:1;
	BoolTypeDef Bit6			:1;
	BoolTypeDef Bit5			:1;
	BoolTypeDef Bit4			:1;
	BoolTypeDef Bit3			:1;
	BoolTypeDef Bit2			:1;
	//register address 0x02
	BoolTypeDef	Bit1			:1;
	BoolTypeDef Bit0			:1;
	//RxErr flag
	BoolTypeDef RxErr			:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}ChargeCurrentBqRegTypeDef;

typedef struct{
	//register address 0x05
	BoolTypeDef InvalidWrite	:1;
	BoolTypeDef Bit11			:1;
	BoolTypeDef Bit10			:1;
	BoolTypeDef Bit9			:1;
	BoolTypeDef	Bit8			:1;
	BoolTypeDef Bit7			:1;
	BoolTypeDef Bit6			:1;
	BoolTypeDef Bit5			:1;
	//register address 0x04
	BoolTypeDef Bit4			:1;
	BoolTypeDef Bit3			:1;
	BoolTypeDef Bit2			:1;
	BoolTypeDef Bit1			:1;
	BoolTypeDef Bit0			:1;
	//RxErr flag
	BoolTypeDef RxErr			:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}MaxChargeVoltageBqRegTypeDef;

typedef struct{
	//register address 0x0D
	BoolTypeDef InvalidWrite_1	:1;
	BoolTypeDef InvalidWrite_0	:1;
	BoolTypeDef Bit5			:1;
	BoolTypeDef Bit4			:1;
	BoolTypeDef Bit3			:1;
	BoolTypeDef Bit2			:1;
	BoolTypeDef Bit1			:1;
	BoolTypeDef Bit0			:1;
	//register address 0x0C
	//not used
	//RxErr flag
	BoolTypeDef RxErr			:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}MinSystemVoltageBqRegTypeDef;

typedef struct{
	//register address 0x0F
	BoolTypeDef InvalidWrite	:1;
	BoolTypeDef	Bit6			:1;
	BoolTypeDef Bit5			:1;
	BoolTypeDef Bit4			:1;
	BoolTypeDef	Bit3			:1;
	BoolTypeDef Bit2			:1;
	BoolTypeDef Bit1 			:1;
	BoolTypeDef Bit0			:1;
	//register address 0x0E
	//not used
	//RxErr flag
	BoolTypeDef RxErr			:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}IinHostBqRegTypeDef;

typedef struct{
	//register address 0x25
	BoolTypeDef InvalidWrite	:1;
	BoolTypeDef Bit6			:1;
	BoolTypeDef Bit5			:1;
	BoolTypeDef Bit4			:1;
	BoolTypeDef Bit3			:1;
	BoolTypeDef Bit2			:1;
	BoolTypeDef Bit1			:1;
	BoolTypeDef Bit0			:1;
	//register address 0x24
	//not used
	//RxErr flag
	BoolTypeDef RxErr			:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}IinDpmBqRegTypeDef;

typedef struct{
	//register address 0x0B
	BoolTypeDef InvalidWrite_1	:1;
	BoolTypeDef InvalidWrite_0	:1;
	BoolTypeDef Bit7			:1;
	BoolTypeDef Bit6			:1;
	BoolTypeDef Bit5			:1;
	BoolTypeDef Bit4			:1;
	BoolTypeDef Bit3			:1;
	BoolTypeDef Bit2			:1;
	//register address 0x0A
	BoolTypeDef Bit1			:1;
	BoolTypeDef Bit0			:1;
	//RxErr flag
	BoolTypeDef RxErr			:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}InputVoltageBqRegTypeDef;


typedef struct{
	//register address 0x07
	BoolTypeDef InvalidWrite_1	:1;
	BoolTypeDef InvalidWrite_0	:1;
	//register address 0x06
	;
	//RxErr flag
	BoolTypeDef RxErr			:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}OtgVoltageBqRegTypeDef;

typedef struct{
	//register address 0x09
	BoolTypeDef InvalidWrite	:1;
	//register address 0x08
	;
	//RxErr flag
	BoolTypeDef RxErr			:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}OtgCurrentBqRegTypeDef;

typedef struct{
	//register address 0x27
	BoolTypeDef VbusBit7	:1;
	BoolTypeDef VbusBit6	:1;
	BoolTypeDef VbusBit5	:1;
	BoolTypeDef VbusBit4	:1;
	BoolTypeDef VbusBit3	:1;
	BoolTypeDef VbusBit2	:1;
	BoolTypeDef VbusBit1	:1;
	BoolTypeDef VbusBit0	:1;
	//register address 0x26
	BoolTypeDef PsysBit7	:1;
	BoolTypeDef PsysBit6	:1;
	BoolTypeDef PsysBit5	:1;
	BoolTypeDef PsysBit4	:1;
	BoolTypeDef PsysBit3	:1;
	BoolTypeDef PsysBit2	:1;
	BoolTypeDef PsysBit1	:1;
	BoolTypeDef PsysBit0	:1;
	//RxErr flag
	BoolTypeDef RxErr		:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}AdcVbusPsysBqRegTypeDef;

typedef struct{
	//register address 0x29
	BoolTypeDef IchgBit6	:1;
	BoolTypeDef IchgBit5	:1;
	BoolTypeDef IchgBit4	:1;
	BoolTypeDef IchgBit3	:1;
	BoolTypeDef IchgBit2	:1;
	BoolTypeDef IchgBit1	:1;
	BoolTypeDef IchgBit0	:1;
	//register address 0x28
	BoolTypeDef IdchgBit6	:1;
	BoolTypeDef IdchgBit5	:1;
	BoolTypeDef IdchgBit4	:1;
	BoolTypeDef IdchgBit3	:1;
	BoolTypeDef IdchgBit2	:1;
	BoolTypeDef IdchgBit1	:1;
	BoolTypeDef IdchgBit0	:1;
	//RxErr flag
	BoolTypeDef RxErr		:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}AdcIbatBqRegTypeDef;

typedef struct {
	//register address 0x2B
	BoolTypeDef IinBit7		:1;
	BoolTypeDef IinBit6		:1;
	BoolTypeDef IinBit5		:1;
	BoolTypeDef IinBit4		:1;
	BoolTypeDef IinBit3		:1;
	BoolTypeDef IinBit2		:1;
	BoolTypeDef IinBit1		:1;
	BoolTypeDef IinBit0		:1;
	//register address 0x2A
	BoolTypeDef CmpinBit7	:1;
	BoolTypeDef CmpinBit6	:1;
	BoolTypeDef CmpinBit5	:1;
	BoolTypeDef CmpinBit4	:1;
	BoolTypeDef CmpinBit3	:1;
	BoolTypeDef CmpinBit2	:1;
	BoolTypeDef CmpinBit1	:1;
	BoolTypeDef CmpinBit0	:1;
	//RxErr flag
	BoolTypeDef RxErr		:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}AdcIinCmpinBqRegTypeDef;

typedef struct{
	//register address 0x2D
	BoolTypeDef VsysBit7	:1;
	BoolTypeDef VsysBit6	:1;
	BoolTypeDef VsysBit5	:1;
	BoolTypeDef VsysBit4	:1;
	BoolTypeDef VsysBit3	:1;
	BoolTypeDef VsysBit2	:1;
	BoolTypeDef VsysBit1	:1;
	BoolTypeDef VsysBit0	:1;
	//register address 0x2C
	BoolTypeDef VbatBit7	:1;
	BoolTypeDef VbatBit6	:1;
	BoolTypeDef VbatBit5	:1;
	BoolTypeDef VbatBit4	:1;
	BoolTypeDef VbatBit3	:1;
	BoolTypeDef VbatBit2	:1;
	BoolTypeDef VbatBit1	:1;
	BoolTypeDef VbatBit0	:1;
	//RxErr flag
	BoolTypeDef RxErr		:1;
	uint8_t 	RegMSByte;
	uint8_t 	RegLSByte;
}AdcVsysVbatBqRegTypeDef;

/* VARIABLES */

volatile ChargeOption0BqRegTypeDef		ChargeOption0;
volatile ChargeOption1BqRegTypeDef 		ChargeOption1;
volatile ChargeOption2BqRegTypeDef 		ChargeOption2;
volatile ChargeOption3BqRegTypeDef 		ChargeOption3;
volatile ProchotOption0BqRegTypeDef		ProchotOption0;
volatile ProchotOption1BqRegTypeDef		ProchotOption1;
volatile AdcOptionBqRegTypeDef 			AdcOption;
volatile ChargerStatusBqRegTypeDef 		ChargerStatus;
volatile ProchotStatusBqRegTypeDef 		ProchotStatus;
volatile ChargeCurrentBqRegTypeDef 		ChargeCurrent;
volatile MaxChargeVoltageBqRegTypeDef	MaxChargeVoltage;
volatile MinSystemVoltageBqRegTypeDef	MinSystemVoltage;
volatile IinHostBqRegTypeDef			IinHost;
volatile IinDpmBqRegTypeDef				IinDpm;
volatile InputVoltageBqRegTypeDef		InputVoltage;
volatile OtgVoltageBqRegTypeDef			OtgVoltage;
volatile OtgCurrentBqRegTypeDef			OtgCurrent;
volatile AdcVbusPsysBqRegTypeDef		AdcVbusPsys;
volatile AdcIbatBqRegTypeDef			AdcIbat;
volatile AdcIinCmpinBqRegTypeDef		AdcIinCmpin;
volatile AdcVsysVbatBqRegTypeDef		AdcVsysVbat;

/* FUNCTIONS PROTOTYPES */

uint8_t BqAllRegUpdate(void);
uint8_t BqChargeOption0RegUpdate(void);
uint8_t BqChargeOption1RegUpdate(void);
uint8_t BqChargeOption2RegUpdate(void);
uint8_t BqChargeOption3RegUpdate(void);
uint8_t BqProchotOption0RegUpdate(void);
uint8_t BqProchotOption1RegUpdate(void);
uint8_t BqAdcOptionRegUpdate(void);
uint8_t BqChargerStatusRegUpdate(void);
uint8_t BqProchotStatusRegUpdate(void);
uint8_t BqChargeCurrentRegUpdate(void);
uint8_t BqMaxChargeVoltageRegUpdate(void);
uint8_t BqMinSystemVoltageRegUpdate(void);
uint8_t BqIinHostRegUpdate(void);
uint8_t	BqIinDpmRegUpdate(void);
uint8_t	BqInputVoltageRegUpdate(void);
uint8_t	BqOtgVoltageRegUpdate(void);
uint8_t BqOtgCurrentRegUpdate(void);
uint8_t BqAdcVbusPsysRegUpdate(void);
uint8_t	BqAdcIbatRegUpdate(void);
uint8_t	BqAdcIinCmpinRegUpdate(void);
uint8_t AdcVsysVbatRegUpdate(void);

#endif //BQ_DEBUG_MODE

#endif
/****END OF FILE****/
