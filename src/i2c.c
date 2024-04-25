/**
 *  \file 		i2c.c
 *  \brief		I2C configuration file
 *  \details	It contain all I2C init and handling function
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

/* INCLUDES */

#include <i2c.h>

/* VARIABLES */

/* FUNCTION PROTOTYPES */

/**
 * \brief 	Function for init I2C peripherals
 * \param 	void
 * \see 	BoardInit()
 * \note 	Function should contain all action to call, before DMA using.
 * \return 	void
 */
void I2cInit(void){
	LL_I2C_InitTypeDef I2C_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	/*
	I2C2 GPIO Configuration
	PB10   ------> I2C2_SCL
	PB11   ------> I2C2_SDA
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Peripheral clock enable
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);

	// I2C Initialization
	LL_I2C_EnableAutoEndMode(I2C2);
	LL_I2C_DisableOwnAddress2(I2C2);
	LL_I2C_DisableGeneralCall(I2C2);
	LL_I2C_EnableClockStretching(I2C2);
	LL_I2C_EnableAnalogFilter(I2C2);
	I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
	// I2C_InitStruct.Timing = 0x00000708;
	I2C_InitStruct.Timing = 0x0000061E;
	I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
	I2C_InitStruct.DigitalFilter = 1;
	I2C_InitStruct.OwnAddress1 = 0;
	I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
	I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
	LL_I2C_Init(I2C2, &I2C_InitStruct);
	LL_I2C_SetOwnAddress2(I2C2, 0, LL_I2C_OWNADDRESS2_NOMASK);
	
	I2cTimeoutTimerInit();
}

/**
 * \brief 	Function for start count timeout in I2C communitcation.
 * \param 	value ratio: 0.1 ms
 * \see 	TimTimeoutStop()
 * \see 	TIM14_IRQHandler()
 * \note 	Function sets and activate timer which call interruption when
 * 			timeout is occur if do not deactivate it earlier.
 * \return 	void
 */
void I2cTimeoutTimerInit(void){
	LL_TIM_InitTypeDef TIM_InitStruct = {0};
	// Peripheral clock enable
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);
	// TIM6 interrupt Init
	NVIC_SetPriority(TIM6_IRQn, 0);
	NVIC_EnableIRQ(TIM6_IRQn);
	//TIM_InitStruct.Prescaler = 800-LL_TIM_IC_FILTER_FDIV1_N2;
	TIM_InitStruct.Prescaler = 800-1;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 10;
	LL_TIM_Init(TIM6, &TIM_InitStruct);
	LL_TIM_EnableARRPreload(TIM6);
	LL_TIM_SetTriggerOutput(TIM6, LL_TIM_TRGO_UPDATE);
	LL_TIM_DisableMasterSlaveMode(TIM6);
}

void I2cTimeoutStart(uint16_t value){
	;
}

/**
 * \brief 	Function for stop count timeout in I2C communitcation.
 * \param 	void
 * \see 	TimTimeoutStart()
 * \see 	TIM14_IRQHandler
 * \note
 * \return 	void
 */
void I2cTimeoutStop(void){
	;
}

uint8_t I2cWriteByte(I2C_TypeDef *I2Cx, uint8_t DevAddr, uint8_t RegAddr, uint8_t RegVal, uint8_t Timeout){
	uint8_t RegAddrFlag = 0;
	uint8_t TimeoutCounter = 0;
	LL_I2C_HandleTransfer(I2Cx, DevAddr, LL_I2C_ADDRSLAVE_7BIT, 2, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
	while(!LL_I2C_IsActiveFlag_STOP(I2Cx)){
		if(LL_SYSTICK_IsActiveCounterFlag())
			TimeoutCounter++;
		if(RegAddrFlag == 0){
			if(LL_I2C_IsActiveFlag_TXIS(I2Cx)){
				LL_I2C_TransmitData8(I2Cx, RegAddr);
				RegAddrFlag = 1;
			}
		}
		else
			if(LL_I2C_IsActiveFlag_TXIS(I2Cx))
				LL_I2C_TransmitData8(I2Cx, RegVal);
		if(TimeoutCounter >= Timeout)
			return I2cTimeoutExceedAction(I2Cx);
	}
	LL_I2C_ClearFlag_STOP(I2Cx);
	return 0;
}

uint8_t I2cReadByte(I2C_TypeDef *I2Cx, uint8_t DevAddr, uint8_t RegAddr, uint8_t* RegVal, uint8_t Timeout){
	uint8_t TimeoutCounter = 0;
	LL_I2C_HandleTransfer(I2Cx, DevAddr, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
	while(!LL_I2C_IsActiveFlag_STOP(I2Cx)){
		if(LL_SYSTICK_IsActiveCounterFlag())
			TimeoutCounter++;
		if(LL_I2C_IsActiveFlag_TXIS(I2Cx))
			LL_I2C_TransmitData8(I2Cx, RegAddr);
		if(TimeoutCounter >= Timeout)
			return I2cTimeoutExceedAction(I2Cx);
	}
	LL_I2C_ClearFlag_STOP(I2Cx);
	TimeoutCounter = 0;
	LL_I2C_HandleTransfer(I2Cx, DevAddr, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
	while(!LL_I2C_IsActiveFlag_STOP(I2Cx)){
		if(LL_SYSTICK_IsActiveCounterFlag())
			TimeoutCounter++;
		if(LL_I2C_IsActiveFlag_RXNE(I2Cx))
			*RegVal = LL_I2C_ReceiveData8(I2Cx);
		if(TimeoutCounter >= Timeout)
			return I2cTimeoutExceedAction(I2Cx);
	}
	LL_I2C_ClearFlag_STOP(I2Cx);
	return 0;
}

uint8_t I2cTimeoutExceedAction(I2C_TypeDef *I2Cx){
	LL_I2C_ClearFlag_ARLO(I2Cx);
	LL_I2C_ClearFlag_TXE(I2Cx);
	LL_I2C_ClearFlag_STOP(I2Cx);
	return RX_ERR;
}

uint8_t I2cWriteReg(I2C_TypeDef *I2Cx, uint8_t DevAddr, uint8_t RegAddr, uint8_t* RegVal, uint8_t RegSize, uint8_t Timeout){
	uint8_t RegValIndex = 0;
	uint8_t RegAddrFlag = 0;
	uint8_t TimeoutCounter = 0;
	LL_I2C_HandleTransfer(I2Cx, DevAddr, LL_I2C_ADDRSLAVE_7BIT, (RegSize+1), LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
	while(!LL_I2C_IsActiveFlag_STOP(I2Cx)){
		if(LL_SYSTICK_IsActiveCounterFlag())
			TimeoutCounter++;
		if(RegAddrFlag == 0){
			if(LL_I2C_IsActiveFlag_TXIS(I2Cx)){
				LL_I2C_TransmitData8(I2Cx, RegAddr);
				RegAddrFlag = 1;
			}
		}
		else{
			if(LL_I2C_IsActiveFlag_TXIS(I2Cx) && RegValIndex < RegSize){
				LL_I2C_TransmitData8(I2Cx, RegVal[RegValIndex]);
				RegValIndex++;
			}
		}
		if(TimeoutCounter >= Timeout+RegSize)
			return I2cTimeoutExceedAction(I2Cx);
	}
	LL_I2C_ClearFlag_STOP(I2Cx);
	return 0;
}

uint8_t I2cReadReg(I2C_TypeDef *I2Cx, uint8_t DevAddr, uint8_t RegAddr, uint8_t* RegVal, uint8_t RegSize, uint8_t Timeout){
	uint8_t RegValIndex = 0;
	uint8_t TimeoutCounter = 0;
	LL_I2C_HandleTransfer(I2Cx, DevAddr, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
	while(!LL_I2C_IsActiveFlag_STOP(I2Cx)){
		if(LL_SYSTICK_IsActiveCounterFlag())
			TimeoutCounter++;
		if(LL_I2C_IsActiveFlag_TXIS(I2Cx))
			LL_I2C_TransmitData8(I2Cx, RegAddr);
		if(TimeoutCounter >= Timeout)
			return I2cTimeoutExceedAction(I2Cx);
	}
	LL_I2C_ClearFlag_STOP(I2Cx);
	LL_I2C_HandleTransfer(I2Cx, DevAddr, LL_I2C_ADDRSLAVE_7BIT, RegSize, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
	TimeoutCounter = 0;
	while(!LL_I2C_IsActiveFlag_STOP(I2Cx)){
		if(LL_SYSTICK_IsActiveCounterFlag())
			TimeoutCounter++;
		if(LL_I2C_IsActiveFlag_RXNE(I2Cx) && (RegValIndex < RegSize))
			RegVal[RegValIndex++] = LL_I2C_ReceiveData8(I2Cx);
		if(TimeoutCounter >= Timeout + RegSize)
			return I2cTimeoutExceedAction(I2Cx);
	}
	LL_I2C_ClearFlag_STOP(I2Cx);
	return 0;
}

/****END OF FILE****/
