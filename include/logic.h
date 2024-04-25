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
#include <system_param.h>
#include <string.h>

/* DEFINES */

#define BATTERY_TYPE 		LION
#define BATTERY_CAPACITY	7800
#define ARRAY_LENGTH(arg)	(sizeof(arg) / sizeof(arg[0]))

/* TYPE DEF ENUM */

typedef enum{
	Off		= 0,
	On		= 1,
	Toggle	= 2
}OutputModeTypeDef;

typedef enum{
	ServiceMode	= 0,
	RobotOn		= 1,
	RobotOff	= 2
}StateTypeDef;

typedef enum{
	unknown_status = 0,
	charging = 1,
	discharging = 2,
	not_charging = 3,
	full = 4
}BatteryStatusTypeDef;

typedef enum{
	unknown_health = 0,
	good = 1,
	overhaet = 2,
	dead = 3,
	overvoltage = 4,
	unspec_failure = 5,
	cold = 6,
	watchdog_timer_expire = 7,
	safety_timer_expire = 8
}BatteryHealthTypeDef;

typedef enum{
	unknown_type = 0,
	NIMH = 1,
	LION = 2,
	LIPO = 3,
	LIFE = 4,
	NICD = 5,
	LIMN = 6
}BatteryTechnologyTypeDef;



/* STRUCTURES */

typedef struct{
	//ROS battery msgs variables
	uint16_t					Voltage;
	int16_t						Temperature;
	uint16_t					Current;
	uint16_t					ChargeCurrent;
	uint16_t					Capacity;
	uint16_t					DesignCapacity;
	uint8_t						Percentage;
	BatteryStatusTypeDef 		Status;
	BatteryHealthTypeDef		Health;
	BatteryTechnologyTypeDef 	Technology;
	uint8_t						Present;
	uint16_t 					Cell_voltage;
	int16_t						Cell_temperature;
	//rest battery variables
	uint8_t						FlagOc;
	uint8_t 					FlagCharging;
	uint8_t						LowVoltageFlag;
}BatteryTypeDef;

typedef struct{
	uint8_t		ExtAdapterFlagPresent;
	uint8_t		ExtAdapterFlagOv;
	uint8_t		ExtAdapterFlagOc;
	uint16_t	ExtAdapterCurrent;
	uint16_t	ExtAdapterVoltage;
}ExtAdapterTypeDef;



/* FUNCTION PROTOTYPES */

void HardFaultInfLoop(void);
void RobotOffActions(void);
void RobotOnActions(void);
void MainLogicInit(void);
void MainLogicLoop(void);
void BatteryChargingLoopHandler(void);
void PowerOnInitProcedure(void);
void PowerOnPushButtonItProcedure(void);
void DigitalBoardLoopHandler(void);
void SetLowbatLed(OutputModeTypeDef Mode);
void SetChargeLed(OutputModeTypeDef Mode);
void SetPowerPushButtonLed(OutputModeTypeDef Mode);
void SwitchOnOffVsSwCh1(OutputModeTypeDef Mode);
void SwitchOnOffVsSwCh2(OutputModeTypeDef Mode);
void SwitchOnOffPowerLock(OutputModeTypeDef Mode);
void SwitchOnOffPowerLock(OutputModeTypeDef Mode);
void SwitchOnOffRoboticArmInverter(OutputModeTypeDef Mode);
void SwitchOnOffMCU(OutputModeTypeDef Mode);
void BatteryLoopHandler(void);
uint16_t GetBatteryTemperature(void);
void RoboticArmLoopHandler(void);
void RoboticArmInverterReset(void);
uint8_t DC12V_IsActiveFlag_Pg(void);
uint8_t DC5V_IsActiveFlag_Pg(void);
uint8_t VsSwCh1_IsActiveFlag_Pg(void);
uint8_t VsSwCh2_IsActiveFlag_Pg(void);
uint8_t IndAdpt_IsActiveFlag_Attach(void);
uint8_t UsbcAdpt_IsActiveFlag_Attach(void);
uint8_t Battery_IsActiveFlag_Attach(void);
uint8_t SwitchOnOffDCInverters(OutputModeTypeDef Mode);


#endif
/****END OF FILE****/
