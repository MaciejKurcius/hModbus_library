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
#include "mmodbus.h"

/* VARIABLES */

/* FUNCTIONS */

void HardFaultInfLoop(void){
	while(1);
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
	LL_mDelay(50);
	PowerOnInitProcedure();
	// mmodbus_init(2500);
  
}

void MainLogicLoop(void){
	uint16_t delay = 100;
	uint16_t TxData[10] = {0,1,1,0,0,1,1,1,0,0};
	uint16_t RetData;
	uint16_t RetDataTab[10];
	mmodbus_readHoldingRegister16i(10, 0, &RetData);
	LL_mDelay(delay);
	mmodbus_readHoldingRegisters16i(10, 0, 10, RetDataTab);
	LL_mDelay(delay);
	mmodbus_writeCoil(10, 7, 1);// working ok
	LL_mDelay(delay);
	mmodbus_writeHoldingRegisters16i(10,0, 10, TxData);
	LL_mDelay(delay);
	mmodbus_writeHoldingRegister16i(10, 0, 7); // working ok
	LL_mDelay(delay);

}


void PowerOnInitProcedure(void){
	uint16_t PowerOnDelay = POWER_ON_DELAY;
	while(LL_GPIO_IsInputPinSet(PWR_SW_INT_GPIO_Port, PWR_SW_INT_Pin)){		//check if pwr button is pressed
		if(LL_SYSTICK_IsActiveCounterFlag())
			if(PowerOnDelay != 0)
				PowerOnDelay--;
		if(PowerOnDelay == 0){
			SwitchOnOffPowerLock(On);
			SetPowerPushButtonLed(On);
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
					}
				return;
			}
		}
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

void SwitchOnOffPowerLock(OutputModeTypeDef Mode){
	switch(Mode){
	case Off:		LL_GPIO_SetOutputPin(POWER_LOCK_GPIO_Port, POWER_LOCK_Pin);	break;
	case On:		LL_GPIO_ResetOutputPin(POWER_LOCK_GPIO_Port, POWER_LOCK_Pin);	break;
	case Toggle:	LL_GPIO_TogglePin(POWER_LOCK_GPIO_Port, POWER_LOCK_Pin);	break;
	default: break;
	}
}

/****END OF FILE****/
