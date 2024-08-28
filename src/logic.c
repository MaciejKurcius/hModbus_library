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
#include "config.h"
#include <hModbusLib.h>
#include <hDpsModuleLib.h>

/* VARIABLES */
hModbusTypeDef hModbusMaster1Handle;
hModbusTypeDef hModbusSlave1Handle;
hDpsModuleTypeDef DpsModule;
hModbusSlaveDataTypeDef Slave1Data;

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
	LL_mDelay(2000);
	PowerOnInitProcedure();

	// Modbus functions for init
	hModbusMaster1UartInit();
	// hModbusInit(&hModbusMaster1Handle, USART1, hModbusMaster);
	hModbusInit(&hModbusSlave1Handle, USART1, hModbusSlave);
	hModbusInitSlaveData(&hModbusSlave1Handle, &Slave1Data);
}

void MainLogicLoop(void){
	uint16_t delay = 1;

	/* Input read test */
	uint8_t static RxInput = 7;

	hModbusReadDiscreteInput(&hModbusMaster1Handle, 1, 3, &RxInput);
	LL_mDelay(delay);

	// /* Input register read test */
	// uint16_t static RxInputReg = 7;

	// hModbusReadInputRegister16i(&hModbusMaster1Handle, 1, 5, &RxInputReg);
	// LL_mDelay(delay);

	// /* Coil register read write test */
	// uint8_t static RxCoil = 0;

	// hModbusWriteCoil(&hModbusMaster1Handle, 1, 5, 0);
	// LL_mDelay(delay);
	// hModbusReadCoil(&hModbusMaster1Handle, 1, 5, &RxCoil); //ok
	// LL_mDelay(delay);
	// hModbusWriteCoil(&hModbusMaster1Handle, 1, 5, 1);
	// LL_mDelay(delay);
	// hModbusReadCoil(&hModbusMaster1Handle, 1, 5, &RxCoil); //ok
	// LL_mDelay(delay);


	// /* Coil registers read write test */
	// uint8_t TxCoilZero[5] = {0};
	// uint8_t TxCoil[5] = {1, 0, 0, 1, 1};
	// uint8_t static RxCoils;

	// hModbusWriteCoil(&hModbusMaster1Handle, 1, 10, TxCoilZero[0]);
	// LL_mDelay(delay);
	// hModbusWriteCoil(&hModbusMaster1Handle, 1, 11, TxCoilZero[1]);
	// LL_mDelay(delay);
	// hModbusWriteCoil(&hModbusMaster1Handle, 1, 12, TxCoilZero[2]);
	// LL_mDelay(delay);
	// hModbusWriteCoil(&hModbusMaster1Handle, 1, 13, TxCoilZero[3]);
	// LL_mDelay(delay);
	// hModbusWriteCoil(&hModbusMaster1Handle, 1, 14, TxCoilZero[4]);
	// LL_mDelay(delay);

	// hModbusReadCoils(&hModbusMaster1Handle, 1, 10, 5, &RxCoils);
	// LL_mDelay(delay);

	// hModbusWriteCoil(&hModbusMaster1Handle, 1, 10, TxCoil[0]);
	// LL_mDelay(delay);
	// hModbusWriteCoil(&hModbusMaster1Handle, 1, 11, TxCoil[1]);
	// LL_mDelay(delay);
	// hModbusWriteCoil(&hModbusMaster1Handle, 1, 12, TxCoil[2]);
	// LL_mDelay(delay);
	// hModbusWriteCoil(&hModbusMaster1Handle, 1, 13, TxCoil[3]);
	// LL_mDelay(delay);
	// hModbusWriteCoil(&hModbusMaster1Handle, 1, 14, TxCoil[4]);
	// LL_mDelay(delay);

	// hModbusReadCoils(&hModbusMaster1Handle, 1, 10, 5, &RxCoils); //ok - but the rx data length is to small. 
	// LL_mDelay(delay);

	// /* Holding 16i register read write test */
	// uint16_t static RxHolding16iData = 0;

	// hModbusWriteHoldingRegister16i(&hModbusMaster1Handle, 1, 9, 0);			
	// LL_mDelay(delay);
	// hModbusReadHoldingRegister16i(&hModbusMaster1Handle, 1, 9, &RxHolding16iData);
	// LL_mDelay(delay);
	// hModbusWriteHoldingRegister16i(&hModbusMaster1Handle, 1, 9, 5678);
	// LL_mDelay(delay);
	// hModbusReadHoldingRegister16i(&hModbusMaster1Handle, 1, 9, &RxHolding16iData);  
	// LL_mDelay(delay);


	// /* Holding registers read write test */
	// uint16_t static TxHolding16iDataZero[5] = {0};
	// uint16_t static TxHolding16iData[5] = {21, 37, 7, 2137, 458};
	// uint16_t static RxHolding16iDataArr[5] = {0};

	// hModbusWriteHoldingRegisters16i(&hModbusMaster1Handle, 1, 9, 5, TxHolding16iDataZero);
	// LL_mDelay(delay);
	// hModbusReadHoldingRegisters16i(&hModbusMaster1Handle, 1, 9, 5, RxHolding16iDataArr);
	// LL_mDelay(delay);
	// hModbusWriteHoldingRegisters16i(&hModbusMaster1Handle, 1, 9, 5, TxHolding16iData);
	// LL_mDelay(delay);
	// hModbusReadHoldingRegisters16i(&hModbusMaster1Handle, 1, 9, 5, RxHolding16iDataArr); 
	// LL_mDelay(delay);

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

void DebugGpio1(OutputModeTypeDef Mode){
	switch(Mode){
	case On:		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_5);	break;
	case Off:		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);	break;
	case Toggle:	LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_5);	break;
	default: break;
	}
}

void DebugGpio2(OutputModeTypeDef Mode){
	switch(Mode){
	case On:		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_8);	break;
	case Off:		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8);	break;
	case Toggle:	LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_8);	break;
	default: break;
	}
}



/****END OF FILE****/
