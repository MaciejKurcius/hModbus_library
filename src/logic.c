/**
 *  \file 		logic.c
 *  \brief		File with all logic of program.
 *  \details	It contain all logic function.
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

/* INCLUDES */

#include <logic.h>


/* VARIABLES */

//System global variables
extern ParamU16TypeDef		ParamU16_1;
extern ParamU16TypeDef		ParamU16_2;
extern ParamU8TypeDef 		ParamU8_1;
extern ParamU8TypeDef 		ParamU8_2;
extern ParamFlagTypeDef	ParamFlags_1;
extern uint8_t BatteryState[];

//Local variables
BatteryTypeDef Battery;

//Extern variables
volatile StateTypeDef State;
extern const uint16_t NtcTab[];
extern volatile uint16_t BatteryThermistorVoltage; 

/* FUNCTIONS */

void HardFaultInfLoop(void){
	while(1){
		SetLowbatLed(Toggle);
		LL_mDelay(100);
	}
}

/**
 * \brief 	Function for prepare board logic to work
 * \param 	void
 * \see 	main()
 * \see 	BoardInit()
 * \note 	Function contain all action execute only one, after power is on.
 * \return 	void
 */

void MainLogicInit(void){
	BoardInit();
	LL_mDelay(250);
	if(BqCheckI2cAddr())	HardFaultInfLoop();
	BqInit();
	State = RobotOff;

	// WriteBoardVersion();
	InitBatteryStateBuffer();
	LL_mDelay(5);
	InitPbInfoBuffer();
	InitParamU16_1Buffer();
	InitParamU16_2Buffer();
	InitParamU8_1Buffer();
	InitParamU8_2Buffer();
	InitParamFlags_1Buffer();

	PowerOnInitProcedure();

	BqSetChargeCurrentByVal(0);
	
	Battery.LowVoltageFlag = 0;
}

void RobotOnActions(void){
	SwitchOnOffPowerLock(On);
	SetPowerPushButtonLed(On);
	State = RobotOn;
	SetParamFlag(&ParamFlags_1, 8, VS_SW_DEFAULT_STATE);		
	SetParamFlag(&ParamFlags_1, 24, ARM_BOARD_DEFAULT_STATE);
}

void RobotOffActions(void){
	SwitchOnOffPowerLock(Off);
	SetPowerPushButtonLed(Off);
	SwitchOnOffMCU(Off);
	LL_GPIO_ResetOutputPin(MCU_PWR_1_GPIO_Port, MCU_PWR_1_Pin);
	SetParamFlag(&ParamFlags_1, 24, 0);	//robotic ARM off
	SetParamFlag(&ParamFlags_1, 8, 0);	// VS_SW_CH1 off
	State = RobotOff;
}

void MainLogicLoop(void){
	UpdateParamFlags_1Buffer();
	UpdateParamU8_1Buffer();
	UpdateParamU8_2Buffer();
	UpdateParamU16_1Buffer();
	UpdateParamU16_2Buffer();
	
	BqLoopHandler();
	BatteryLoopHandler();
	SwitchOnOffVsSwCh1(GetParamFlag(ParamFlags_1, 8));
	DigitalBoardLoopHandler();
	UartReceivedFrameHandler();
	RoboticArmLoopHandler();

	switch(State){
		case RobotOn:
			break;
		case ServiceMode:
			;
			break;
		case RobotOff:
			break;
	}
}

void BatteryChargingLoopHandler(void){

	// Charge LED Handling
	if(Battery_IsActiveFlag_Attach() && (UsbcAdpt_IsActiveFlag_Attach() || IndAdpt_IsActiveFlag_Attach())){
		if((BqAvgBatteryChargeCurrent > BATTERY_FULL_CURRENT_TRESHOLD) || (BqAvgBatteryVoltage < BATTERY_FULL_VOLTAGE_TRESHOLD)){
			SetChargeLed(Toggle);
		}
		else if((BqAvgBatteryChargeCurrent <= BATTERY_FULL_CURRENT_TRESHOLD) && (BqAvgBatteryVoltage > BATTERY_FULL_VOLTAGE_TRESHOLD)){
			SetChargeLed(On);
			Battery.Status = full;
		}
	}
	else{
		SetChargeLed(Off);
		Battery.Status = not_charging;
	}

	// Low Bat LED Handling
	if(Battery_IsActiveFlag_Attach()){
		if(BqAvgBatteryVoltage < BATTERY_DISCHARGED_VOLTAGE_TRESHOLD){
			Battery.LowVoltageFlag = 1;
		}
		else if((BqAvgBatteryVoltage > BATTERY_DISCHARGED_VOLTAGE_TRESHOLD) && (UsbcAdpt_IsActiveFlag_Attach() || IndAdpt_IsActiveFlag_Attach())){
			Battery.LowVoltageFlag = 0;
		}
	}
	else{
		Battery.LowVoltageFlag = 1;
	}

	if(Battery.LowVoltageFlag == 1)		SetLowbatLed(On);
	if(Battery.LowVoltageFlag == 0)		SetLowbatLed(Off);

	// Charge current handling
	if(Battery_IsActiveFlag_Attach() && (Battery.Temperature < BATTERY_MAX_TEMP)){
		// BqSetAcAdptMaxCurrent(AC_ADPT_MAX_CURRENT);
		if(UsbcAdpt_IsActiveFlag_Attach()){
			BqSetAcAdptMaxCurrent(USBC_ADPT_MAX_CURRENT);
			BqSetChargeCurrentByVal(USBC_CHARGE_CURRENT);
			Battery.Status = charging;
		}
		else if(IndAdpt_IsActiveFlag_Attach()){
			BqSetAcAdptMaxCurrent(IND_ADPT_MAX_CURRENT);
			LL_mDelay(500);
			BqSetChargeCurrentByVal(IND_ADPT_CHARGE_CURRENT);
			Battery.Status = charging;
		}
		else
			Battery.Status = discharging;
	}
	else{
		BqSetChargeCurrentByVal(0);
		Battery.Status = not_charging;
	}
}

void PowerOnInitProcedure(void){
	uint16_t PowerOnDelay = POWER_ON_DELAY;
	while(LL_GPIO_IsInputPinSet(PWR_SW_INT_GPIO_Port, PWR_SW_INT_Pin)){		//check if pwr button is pressed
		if(LL_SYSTICK_IsActiveCounterFlag())
			if(PowerOnDelay != 0)
				PowerOnDelay--;
		if(PowerOnDelay == 0){
			RobotOnActions();
			break;
		}
	}
	NVIC_EnableIRQ(EXTI4_15_IRQn);	//enable interrupts from PWR_SW_INT
}

void PowerOnPushButtonItProcedure(void){
	uint16_t PowerOffDelay = POWER_OFF_DELAY;
	uint16_t PowerOnDelay = POWER_ON_DELAY;
	uint32_t SbcPowerOffDelay = SBC_POWER_OFF_DELAY + SBC_POWER_OFF_PBLED_BLINK_DELAY;
	uint8_t LedFlag = 0;
	if(LL_GPIO_IsOutputPinSet(POWER_LOCK_GPIO_Port, POWER_LOCK_Pin)){
		while(LL_GPIO_IsInputPinSet(PWR_SW_INT_GPIO_Port, PWR_SW_INT_Pin)){
			if(LL_SYSTICK_IsActiveCounterFlag())
				if(PowerOnDelay != 0)
					PowerOnDelay--;
			if(PowerOnDelay == 0){
				RobotOnActions();
				return;
			}
		}
	}
	else{
		while(LL_GPIO_IsInputPinSet(PWR_SW_INT_GPIO_Port, PWR_SW_INT_Pin)){
			if(LL_SYSTICK_IsActiveCounterFlag())
				if(PowerOffDelay != 0)
					PowerOffDelay--;
			if(PowerOffDelay == 0){
				while(1){
					LL_GPIO_SetOutputPin(MCU_PWR_1_GPIO_Port, MCU_PWR_1_Pin);
					if(LL_SYSTICK_IsActiveCounterFlag()){
						if(SbcPowerOffDelay != 0){
							SbcPowerOffDelay--;
						}
					}
					if(!(SbcPowerOffDelay % SBC_POWER_OFF_PBLED_BLINK_DELAY)){
						if(LedFlag == 0)
							SetPowerPushButtonLed(Toggle);
						LedFlag = 1;
					}
					else
						LedFlag = 0;

					if(	!LL_GPIO_IsInputPinSet(SBC_PWR_ON_SNS_GPIO_Port, SBC_PWR_ON_SNS_Pin) 
						|| (SbcPowerOffDelay <= SBC_POWER_OFF_PBLED_BLINK_DELAY + 10) 
						|| LL_GPIO_IsInputPinSet(MCU_PWR_2_GPIO_Port, MCU_PWR_2_Pin)){
						RobotOffActions();
						return;
					}
					}
				return;
			}
		}
	}
}

void DigitalBoardLoopHandler(void){
	//Turn On main Digital Board
	if(LL_GPIO_IsOutputPinSet(LED_PWR_PB_GPIO_Port, LED_PWR_PB_Pin)){	//check if robot is power on
		if(!SwitchOnOffDCInverters(On)){
			LL_GPIO_SetOutputPin(MCU_OUT_PWR_ENABLE_GPIO_Port, MCU_OUT_PWR_ENABLE_Pin);  //turn on main Digital Board
		}
	}
	else{
		SwitchOnOffDCInverters(Off);
		LL_GPIO_ResetOutputPin(MCU_OUT_PWR_ENABLE_GPIO_Port, MCU_OUT_PWR_ENABLE_Pin);		//turn off main Digital Board
	}
	if(LL_GPIO_IsOutputPinSet(DIGITAL_BOARD_RST_GPIO_Port, DIGITAL_BOARD_RST_Pin)) 			//check if digital board Digital Board is reseting
		if(LL_GPIO_IsOutputPinSet(MCU_OUT_PWR_ENABLE_GPIO_Port, MCU_OUT_PWR_ENABLE_Pin)){	//check if digital board power is enable
			LL_mDelay(100);																	//delay
			LL_GPIO_ResetOutputPin(DIGITAL_BOARD_RST_GPIO_Port, DIGITAL_BOARD_RST_Pin);		//disable digital board Digital Board reseting
		}
}

//Red Power Board LED (on the Digital Board) handler
void SetLowbatLed(OutputModeTypeDef Mode){
	switch(Mode){
	case Off:		LL_GPIO_ResetOutputPin(LED_LOWBAT_GPIO_Port, LED_LOWBAT_Pin); 	break;
	case On:		LL_GPIO_SetOutputPin(LED_LOWBAT_GPIO_Port, LED_LOWBAT_Pin);		break;
	case Toggle:	LL_GPIO_TogglePin(LED_LOWBAT_GPIO_Port, LED_LOWBAT_Pin);		break;
	default: break;
	}
}

//Green Power Board LED (on the Digital Board) handler
void SetChargeLed(OutputModeTypeDef Mode){
	switch(Mode){
	case Off:		LL_GPIO_ResetOutputPin(LED_CHARGE_GPIO_Port, LED_CHARGE_Pin); 	break;
	case On:		LL_GPIO_SetOutputPin(LED_CHARGE_GPIO_Port, LED_CHARGE_Pin);		break;
	case Toggle:	LL_GPIO_TogglePin(LED_CHARGE_GPIO_Port, LED_CHARGE_Pin);		break;
	default: break;
	}
}

//Power push button LED handler
void SetPowerPushButtonLed(OutputModeTypeDef Mode){
	switch(Mode){
	case Off:		LL_GPIO_ResetOutputPin(LED_PWR_PB_GPIO_Port, LED_PWR_PB_Pin); 	break;
	case On:		LL_GPIO_SetOutputPin(LED_PWR_PB_GPIO_Port, LED_PWR_PB_Pin);		break;
	case Toggle:	LL_GPIO_TogglePin(LED_PWR_PB_GPIO_Port, LED_PWR_PB_Pin);		break;
	default: break;
	}
}

void SwitchOnOffVsSwCh1(OutputModeTypeDef Mode){
	switch(Mode){
	case Off:		LL_GPIO_ResetOutputPin(VS_SW_DRV_CH1_GPIO_Port, VS_SW_DRV_CH1_Pin);	break;
	case On:		LL_GPIO_SetOutputPin(VS_SW_DRV_CH1_GPIO_Port, VS_SW_DRV_CH1_Pin);	break;
	case Toggle:	LL_GPIO_TogglePin(VS_SW_DRV_CH1_GPIO_Port, VS_SW_DRV_CH1_Pin);		break;
	default: break;
	}
}

void SwitchOnOffVsSwCh2(OutputModeTypeDef Mode){
	switch(Mode){
	case Off:		LL_GPIO_ResetOutputPin(VS_SW_DRV_CH2_GPIO_Port, VS_SW_DRV_CH2_Pin);	break;
	case On:		LL_GPIO_SetOutputPin(VS_SW_DRV_CH2_GPIO_Port, VS_SW_DRV_CH2_Pin);	break;
	case Toggle:	LL_GPIO_TogglePin(VS_SW_DRV_CH2_GPIO_Port, VS_SW_DRV_CH2_Pin);	break;
	default: break;
	}
}

void SwitchOnOffPowerLock(OutputModeTypeDef Mode){
	switch(Mode){
	case Off:		LL_GPIO_SetOutputPin(POWER_LOCK_GPIO_Port, POWER_LOCK_Pin);	break;
	case On:		LL_GPIO_ResetOutputPin(POWER_LOCK_GPIO_Port, POWER_LOCK_Pin);	break;
	case Toggle:	LL_GPIO_TogglePin(POWER_LOCK_GPIO_Port, POWER_LOCK_Pin);	break;
	default: break;
	}
}

void SwitchOnOffRoboticArmInverter(OutputModeTypeDef Mode){
	switch(Mode){
		case Off:		
			LL_GPIO_ResetOutputPin(RA_ENABLE_GPIO_Port, RA_ENABLE_Pin);	break;
		case On:		
			LL_GPIO_SetOutputPin(RA_ENABLE_GPIO_Port, RA_ENABLE_Pin);	break;
		case Toggle:	
			LL_GPIO_TogglePin(RA_ENABLE_GPIO_Port, RA_ENABLE_Pin);	break;
		default: 
			break;
	}
}

void SwitchOnOffMCU(OutputModeTypeDef Mode){
	switch(Mode){
	case On:		LL_GPIO_SetOutputPin(MCU_OUT_PWR_ENABLE_GPIO_Port, MCU_OUT_PWR_ENABLE_Pin);	break;
	case Off:		LL_GPIO_ResetOutputPin(MCU_OUT_PWR_ENABLE_GPIO_Port, MCU_OUT_PWR_ENABLE_Pin);	break;
	case Toggle:	LL_GPIO_TogglePin(MCU_OUT_PWR_ENABLE_GPIO_Port, MCU_OUT_PWR_ENABLE_Pin);	break;
	default: break;
	}
}

uint8_t SwitchOnOffDCInverters(OutputModeTypeDef Mode){
	uint8_t counter = 0;
	switch (Mode){
	case On:
		SwitchOnOffVsSwCh2(On);
		while(!VsSwCh2_IsActiveFlag_Pg()){
			if(LL_SYSTICK_IsActiveCounterFlag())
				counter ++;
			if(counter >= VS_SW_CH2_SLEW_RATE+INV_TIMEOUT_OFFSET)
				return 1;
		}
		counter = 0;
		while(!DC12V_IsActiveFlag_Pg()){
			if(LL_SYSTICK_IsActiveCounterFlag())
				counter ++;
			if(counter >= DC12V_INV_SLEW_RATE+INV_TIMEOUT_OFFSET)
				return 1;
		}
		counter = 0;
		while(!DC5V_IsActiveFlag_Pg()){
			if(LL_SYSTICK_IsActiveCounterFlag())
				counter ++;
			if(counter >= DC5V_INV_SLEW_RATE+INV_TIMEOUT_OFFSET)
				return 1;
		}
		break;
	case Off:
		SwitchOnOffVsSwCh2(Off);
		break;
	default:
		break;
	}
	return 0;
}

uint8_t DC12V_IsActiveFlag_Pg(void){
	if(LL_GPIO_IsInputPinSet(PG_12VDC_GPIO_Port, PG_12VDC_Pin))
		return 1;
	else
		return 0;
}

uint8_t DC5V_IsActiveFlag_Pg(void){
	if(LL_GPIO_IsInputPinSet(PG_5VDC_GPIO_Port, PG_5VDC_Pin))
		return 1;
	else
		return 0;
}

uint8_t VsSwCh1_IsActiveFlag_Pg(void){
	if(LL_GPIO_IsInputPinSet(VS_SW_CH1_PG_GPIO_Port, VS_SW_CH1_PG_Pin))
		return 1;
	else
		return 0;
}

uint8_t VsSwCh2_IsActiveFlag_Pg(void){
	if(LL_GPIO_IsInputPinSet(VS_SW_CH2_PG_GPIO_Port, VS_SW_CH2_PG_Pin))
		return 1;
	else
		return 0;
}

uint8_t IndAdpt_IsActiveFlag_Attach(void){
	if(LL_GPIO_IsInputPinSet(IND_ADPT_SNS_GPIO_Port, IND_ADPT_SNS_Pin))
		return 1;
	else
		return 0;
}

uint8_t UsbcAdpt_IsActiveFlag_Attach(void){
	if(LL_GPIO_IsInputPinSet(USBC_ADPT_SNS_GPIO_Port, USBC_ADPT_SNS_Pin))
		return 1;
	else
		return 0;
}

uint8_t Battery_IsActiveFlag_Attach(void){
	// For v1.3 board version
	if(strcmp(GetBoardVersion(), "v1.3\n") == 0){
		if(BatteryThermistorVoltage > BATTERY_DISCONNECTED_ADC_TRESHOLD)
			return 0;
		else
			return 1;
	}

	// For older board versions
	if(LL_GPIO_IsInputPinSet(BAT_REMOVAL_GPIO_Port, BAT_REMOVAL_Pin))
		return 0;
	else
		return 1;
}

void BatteryLoopHandler(void){
	BeginAdc();	//start battery temperatur measuring
	BatteryChargingLoopHandler();

	if(Battery_IsActiveFlag_Attach())
		Battery.Present = 1;
	else
		Battery.Present = 0;

	Battery.Temperature = GetBatteryTemperature();
	Battery.Current = BqBatteryDischargeCurrent;
	Battery.Voltage = BqBatteryVoltage;
	Battery.ChargeCurrent = BqBatteryChargeCurrent;
	Battery.Technology = BATTERY_TYPE;
	Battery.Health = unknown_health;
	Battery.DesignCapacity = BATTERY_CAPACITY;
	//
	UpdateBatteryStateBuffer();
	if(BatteryState[0] == 1){
		SendBatteryStateBuffer();
	}
}

uint16_t GetBatteryTemperature(void){
	if(BatteryThermistorVoltage / 10 > 330) return NtcTab[330];
	else if (BatteryThermistorVoltage / 10 < 0)	return NtcTab[0];
	return (NtcTab[BatteryThermistorVoltage / 10] + BATTERY_TEMP_OFFSET);
}

void RoboticArmLoopHandler(void){
	if(GetParamFlag(ParamFlags_1, 25)){ //checking if RA inverter reset flag is set
		RoboticArmInverterReset();
	}
	SwitchOnOffRoboticArmInverter(GetParamFlag(ParamFlags_1, 24));
}

void RoboticArmInverterReset(void){
	SwitchOnOffRoboticArmInverter(Off);
	LL_mDelay(200);
	SwitchOnOffRoboticArmInverter(On);
	//clear RA inverter reset flag
	SetBitInU8Array(ParamFlags_1.Value, 3, 1, 0);
}

void SendFirmwareVersion(void){
	char TxBuff[12] = FIRMWARE_VERSION;
	UartSendFrameD(2, 12, (uint8_t*)TxBuff);
}


/****END OF FILE****/
