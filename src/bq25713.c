/**
 *  \file 		bq25713.c
 *  \brief		Library for BQ25713 handling.
 *  \details	It contain all functions for handling BQ25713.
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 *  \note
 */

/*  INCLUDES */

#include <bq25713.h>

/* FUNCTION  */


/* Start functions for I2C communication with BQ module */

uint8_t BqCheckI2cAddr(void){
	BqI2cAddr = BQ_I2C_A_VER_ADDR;
	for(uint8_t i = 0; i < BQ_CHECK_ADDR_ATEMPTS; i++){
		if(BqReadByte(DEVICE_ID) == BQ_A_VER_ID)
			return 0;
	}
	BqI2cAddr = BQ_I2C_B_VER_ADDR;
	for(uint8_t i = 0; i < BQ_CHECK_ADDR_ATEMPTS; i++){
		if(BqReadByte(DEVICE_ID) == BQ_B_VER_ID)
			return 0;
	}
	return 1;
}


/**
 * \brief 	Send register via I2C
 * \param 	RegAddr		- address of register to send data
 * \param 	*Value	 	- pointer for data array which contain bytes to send
 * \param 	Size 		- size of data to send
 * \see
 * \note 	Function send all data array to register address in one transmission.
 * \return 	If transmission error - return 1, else return 0.
 */
uint8_t BqWriteRegister(uint8_t RegAddr, uint8_t* Value, uint8_t Size){
	if(I2cWriteReg(BQ_I2C, BqI2cAddr, RegAddr, Value, Size, BQ_I2C_TIMEOUT) == 0)
		return 0;
	else 
		return BQ_RX_ERR;

}

/**
 * \brief 	Read register via I2C
 * \param 	RegAddr 	- address of data register to read
 * \param 	*Buff	 	- pointer for data array which contain bytes to send
 * \param 	Size 		- size of data to send
 * \see
 * \note 	Function read data from register address and write them to to buffer specified by pointer.
 * 			Data size is specified by 'size' parameter.
 * \return 	If transmission error - return 1, else return 0.
 */
uint8_t BqReadRegister(uint8_t RegAddr, uint8_t* Buff, uint8_t Size){
	if(I2cReadReg(BQ_I2C, BqI2cAddr, RegAddr, Buff, Size, BQ_I2C_TIMEOUT) == 0)
		return 0;
	else
		return BQ_RX_ERR;
}

/**
 * \brief 	Send byte via I2C
 * \param 	RegAddr 	- address of register to send data
 * \param 	Value		- 1 byte of data to send
 * \see
 * \note 	Function for send 1 byte of data via I2C to register specified by regAddr2.
 * \return 	If transmission error - return 1, else return 0.
 */
uint8_t BqWriteByte(uint8_t RegAddr, uint8_t Value){
	return I2cWriteByte(BQ_I2C, BqI2cAddr, RegAddr, Value, BQ_I2C_TIMEOUT);
}

/**
 * \brief 	Read byte via I2C
 * \param 	RegAddr		- address of register to read data
 * \see
 * \note 	Function for read 1 byte of data via I2C from register specified by regAddr2.
 * \return 	If transmission error - return I2C_RXTX_ERR macro (MSBit in uint16_t value is set to 1).
 * 			else - return 1 byte of data (8-15 bits in uint16_t value is equal 0)
 */
uint16_t BqReadByte(uint8_t RegAddr){
	uint8_t RetVal;
	if(I2cReadByte(BQ_I2C, BqI2cAddr, RegAddr, &RetVal, BQ_I2C_TIMEOUT) != 0x00)
		return BQ_RX_ERR;
	return RetVal;
}


/* Stop functions for I2C communication with BQ module */

/* Two bytes in one shot registers function BEGIN */

/**
 * \brief 	Function for setting Max Charge Voltage in BQ25713
 * \param 	MSByte - 1 byte of data for writting to 0x05 BQ address register
 * \param 	LSByte - 1 byte of data for writting to 0x04 BQ address register
 * \see 	BqLoopHandler()
 * \note 	This function should be call, when coprocessor detect AC Adapter connection to the robot.
 * 			It is used for setting max charge battery voltage BQ25713 when battery charging is starting
 * \return 	1 if Error, else 0
 */
uint8_t BqSetMaxChargeVoltage(uint8_t MSByte, uint8_t LSByte){
	uint8_t data [2];
	data[0] = LSByte;
	data[1] = MSByte;
	if(BqWriteRegister(MAX_CHARGE_VOLTAGE_0, data, 2) == BQ_RX_ERR)
		return 1;
	return 0;
}

/**
 * \brief 	Function for init BQ25713 when system is starting
 * \param 	MSByte - 1 byte of data for writting to 0x05 BQ address register
 * \param 	LSByte - 1 byte of data for writting to 0x04 BQ address register
 * \see 	BqLoopHandler()
 * \note 	This function should be call, when coprocessor detect AC Adapter connection to the robot.
 * \return 	1 if Error, else 0
 */
uint8_t BqSetChargeCurrentByReg(uint8_t MSByte, uint8_t LSByte){
	uint8_t data [2];
	data[0] = LSByte;
	data[1] = MSByte;
	if(BqWriteRegister(CHARGE_CURRENT_0, data, 2) == BQ_RX_ERR)
		return 1;
	return 0;
}

/**
 * \brief 	Function for set the charge current in BQ
 * \param 	ChargeCurrent - set point in uint16_t
 * \see 	BqLoopHandler()
 * \see		BqSetChargeCurrentByReg
 * \note 	This function round the ChargeCurrent setpoint to nearest accepted by BQ
 * 			and write them into BQ registers
 * \return 	1 if Error, else 0
 */
uint8_t BqSetChargeCurrentByVal(uint16_t ChargeCurrent){
	ChargeCurrent = ChargeCurrent & 0x1FC0;
	return BqSetChargeCurrentByReg(ChargeCurrent >> 8, ChargeCurrent);
}

uint8_t BqSetAcAdptMaxCurrent(uint16_t AcAdptMaxCurrent){
	uint16_t RegValue = AcAdptMaxCurrent / 50;
	uint16_t RetValue;
	uint8_t ErrorFlag = 0;
	BqWriteByte(IIN_HOST_1, RegValue);
	RetValue = BqReadByte(IIN_HOST_1);
	if(RegValue !=  RetValue)
		ErrorFlag = 1;
	return ErrorFlag;
}


/**
 * \brief 	Function for set minimum limit input voltage of AC adapter
 * \param 	MSByte - 1 byte of data for writting to 0x0B BQ address register
 * \param 	LSByte - 1 byte of data for writting to 0x0A BQ address register
 * \see 	BqLoopHandler()
 * \note 	This function should be call, when coprocessor detect AC Adapter connection to the robot.
 * \return 	1 if Error, else 0
 */
uint8_t BqSetMinAdapterInputVoltage(uint8_t MSByte, uint8_t LSByte){
	uint8_t data [2];
	data[0] = LSByte;
	data[1] = MSByte;
	if(BqWriteRegister(INPUT_VOLTAGE_0, data, 2))
		return 1;
	return 0;
}

/**
 * \brief 	Update Input Current value
 * \param 	void
 * \see 	BqAdcRegistersUpdate()
 * \note 	Function read data from BQ ADC register and convert them to real value.
 * \return 	1 if Error, else 0
 */
uint8_t BqInputCurrentInDpmUpdate(void){
	uint8_t mask[] = {1, 2, 4, 8, 16, 32, 64, 128};
	uint8_t RxBuf1[2];
	uint16_t result = 0;
	if(BqReadRegister(IIN_DPM_0, RxBuf1, 2) == BQ_RX_ERR)
		return 1;
	for(uint8_t i = 0; i < 7; i++){
			result += (BqIsNthBitSet((uint16_t)RxBuf1[1], i) * (mask[i]*50));
		}
	BqInputCurrentInDpm = result;
	return 0;
}


uint8_t BqInputCurrentUpdate(void){
	uint16_t mask[] = {50, 100, 200, 400, 800, 1600, 3200, 6400};
	uint8_t RxBuf;
	uint16_t result = 0;
	if(BqReadRegister(ADCIINCMPIN_1, &RxBuf, 1) == BQ_RX_ERR)
		return 1;
	for(uint8_t i = 0; i < 7; i++){
		result += (BqIsNthBitSet(RxBuf, i)) * mask[i];
	}
	BqInputCurrent = result;
	return 0;
}


/* Two bytes in one shot registers function END */

/**
 * \brief 	Function for init BQ25713 registers
 * \param 	void
 * \see 	BqChargerInit()
 * \note 	Function get registers addresses and the values from #define's in 'bq25713_config.h' file.
 * 			Function don't turn on charging process. If all of the registers was initialize correctly
 * 			(the read values from BQ are equal to set points and I2C transmission did not failed)
 * \return 	1 if Error, else 0
 */
uint8_t BqInit(void){
	BqFlagExtAdapterOv = 0;
	BqFlagExtAdapterOc = 0;
	BqFlagBatteryOc = 0;
	BqFlagSystemOv = 0;
	uint8_t ErrorFlag = 0;
	BqWriteByte(CHARGE_OPTION0_0, V_CHARGE_OPTION0_0);
	if(V_CHARGE_OPTION0_0 != BqReadByte(CHARGE_OPTION0_0))
		ErrorFlag = 1;
	BqWriteByte(CHARGE_OPTION0_1, V_CHARGE_OPTION0_1);
	if(V_CHARGE_OPTION0_1 != BqReadByte(CHARGE_OPTION0_1))
		ErrorFlag = 1;
	BqWriteByte(CHARGE_OPTION1_0, V_CHARGE_OPTION1_0);
	if(V_CHARGE_OPTION1_0 != BqReadByte(CHARGE_OPTION1_0))
		ErrorFlag = 1;
	BqWriteByte(CHARGE_OPTION1_1, V_CHARGE_OPTION1_1);
	if(V_CHARGE_OPTION1_1 != BqReadByte(CHARGE_OPTION1_1))
		ErrorFlag = 1;
	BqWriteByte(CHARGE_OPTION2_0, V_CHARGE_OPTION2_0);
	if(V_CHARGE_OPTION2_0 != BqReadByte(CHARGE_OPTION2_0))
		ErrorFlag = 1;
	BqWriteByte(CHARGE_OPTION2_1, V_CHARGE_OPTION2_1);
	if(V_CHARGE_OPTION2_1 != BqReadByte(CHARGE_OPTION2_1))
		ErrorFlag = 1;
	BqWriteByte(CHARGE_OPTION3_0, V_CHARGE_OPTION3_0);
	if(V_CHARGE_OPTION3_0 != BqReadByte(CHARGE_OPTION3_0))
		ErrorFlag = 1;
	BqWriteByte(CHARGE_OPTION3_1, V_CHARGE_OPTION3_1);
	if(V_CHARGE_OPTION3_1 != BqReadByte(CHARGE_OPTION3_1))
		ErrorFlag = 1;
	BqWriteByte(PROCHOT_OPTION0_0, V_PROCHOT_OPTION0_0);
	if(V_PROCHOT_OPTION0_0 != BqReadByte(PROCHOT_OPTION0_0))
		ErrorFlag = 1;
	BqWriteByte(PROCHOT_OPTION0_1, V_PROCHOT_OPTION0_1);
	if(V_PROCHOT_OPTION0_1 != BqReadByte(PROCHOT_OPTION0_1))
		ErrorFlag = 1;
	BqWriteByte(PROCHOT_OPTION1_0, V_PROCHOT_OPTION1_0);
	if(V_PROCHOT_OPTION1_0 != BqReadByte(PROCHOT_OPTION1_0))
		ErrorFlag = 1;
	BqWriteByte(PROCHOT_OPTION1_1, V_PROCHOT_OPTION1_1);
	if(V_PROCHOT_OPTION1_1 != BqReadByte(PROCHOT_OPTION1_1))
		ErrorFlag = 1;
	BqWriteByte(ADC_OPTION_0, V_ADC_OPTION_0);
	if(V_ADC_OPTION_0 != BqReadByte(ADC_OPTION_0))
		ErrorFlag = 1;
	BqWriteByte(ADC_OPTION_1, V_ADC_OPTION_1);
	if(V_ADC_OPTION_1 != BqReadByte(ADC_OPTION_1))
		ErrorFlag = 1;
	BqWriteByte(MIN_SYSTEM_VOLTAGE_0, V_MIN_SYSTEM_VOLTAGE_0);
	if(V_MIN_SYSTEM_VOLTAGE_0 != BqReadByte(MIN_SYSTEM_VOLTAGE_0))
		ErrorFlag = 1;
	BqWriteByte(MIN_SYSTEM_VOLTAGE_1, V_MIN_SYSTEM_VOLTAGE_1);
	if(V_MIN_SYSTEM_VOLTAGE_1 != BqReadByte(MIN_SYSTEM_VOLTAGE_1))
		ErrorFlag = 1;
	// BqWriteByte(IIN_HOST_0, V_IIN_HOST_0);
	// if(V_IIN_HOST_0 != BqReadByte(IIN_HOST_0))
		// ErrorFlag = 1;
	// BqWriteByte(IIN_HOST_1, V_IIN_HOST_1);
	// if(V_IIN_HOST_1 != BqReadByte(IIN_HOST_1))
	// ErrorFlag = 1;
	BqSetAcAdptMaxCurrent(IND_ADPT_MAX_CURRENT);
	if(BqSetMaxChargeVoltage(V_MAX_CHARGE_VOLTAGE_1, V_MAX_CHARGE_VOLTAGE_0))
		ErrorFlag = 1;
	//if(BqSetMinAdapterInputVoltage(V_INPUT_VOLTAGE_1, V_INPUT_VOLTAGE_0))
	//	ErrorFlag = 1;
	//if(BqSetChargeCurrent(V_CHARGE_CURRENT_1, V_CHARGE_CURRENT_0))
	//	ErrorFlag = 1;
	return ErrorFlag;
}



/**
 * \brief 	Check if AC adapter is present
 * \param 	void
 * \see 	BqLoopHandler()
 * \see 	PowerOnPbExtiProcedure()
 * \note 	This function should be call, to detect if AC Adapter is connect to the robot.
 * 			Returned value is defined by BqAdapterPresentTypeDef and BQ_RX_ERR macro.
 * \return 	1 if adapter is present, 0 if adapter not present, 0xFF if transmission error.
 */
uint16_t BqCheckAdapterPresent(void){
	uint16_t RxBuf = BqReadByte(CHARGER_STATUS_1);
	if(BqIsNthBitSet(RxBuf, 15))
		return BQ_RX_ERR;
	if(BqIsNthBitSet(RxBuf, 7))
		return 1;
	else
		return 0;
}

/**
 * \brief 	Check if battery is present
 * \param 	void
 * \see 	BqLoopHandler()
 * \note 	This function should be call, to detect if AC Adapter is connect to the robot.
 * 			Returned value is defined by BqAdapterPresentTypeDef and BQ_RX_ERR macro.
 * \return 	1 if battery is present, 0 if battery not present, 0xFF if transmission error.
 */
uint16_t BqCheckBatteryPresent(void){
	uint16_t RxBuf = BqReadByte(CHARGER_STATUS_1);
	if(BqIsNthBitSet(RxBuf, 15))
		return BQ_RX_ERR;
	if(BqIsNthBitSet(RxBuf, 7))
		return 1;
	else
		return 0;
}

/**
 * \brief 	Update Input Voltage value
 * \param 	void
 * \see 	BqAdcRegistersUpdate()
 * \note 	Function read data from BQ ADC register and convert them to real value.
 * \return 	1 if Error, else 0
 */
uint8_t BqInputVoltageUpdate(void){
	uint16_t RxBuf1 = 0;
	uint8_t mask[] = {1, 2, 4, 8, 16, 32, 64, 128};
	RxBuf1 = BqReadByte(ADCVBUS_PSYS_1);
	if(BqIsNthBitSet(RxBuf1, 15))				//check if I2C transmission error was occurred
		return 1;
	if(RxBuf1 == 0)
		BqInputVoltage = 0;
	else
		BqInputVoltage = 3200;
	for(uint8_t  i = 0; i < 8; i++){
		BqInputVoltage += (BqIsNthBitSet(RxBuf1, i) * (mask[i]*64));	//value in mV
	}
	return 0;
}

/**
 * \brief 	Update System Power value
 * \param 	void
 * \see 	BqAdcRegistersUpdate()
 * \note 	Function read data from BQ ADC register and convert them to real value.
 * \return 	1 if Error, else 0
 */
uint8_t BqSystemPowerUpdate(void){
	uint16_t RxBuf0 = 0;
	uint8_t mask[] = {1, 2, 4, 8, 16, 32, 64, 128};
	uint16_t temp = 0;
	RxBuf0= BqReadByte(ADCVBUS_PSYS_0);
	if(BqIsNthBitSet(RxBuf0  , 15))				//check if I2C transmission error was occurred
		return 1;
	for(uint8_t  i = 0; i < 8; i++){
		temp += (BqIsNthBitSet(RxBuf0  , i) * (mask[i]*12));	//value in mV
	}
	BqSystemPower = temp * 1000000 / PSYS_RESISTOR;
	return 0;
}

/**
 * \brief 	Update Battery Charge Current value
 * \param 	void
 * \see 	BqAdcRegistersUpdate()
 * \note 	Function read data from BQ ADC register and convert them to real value.
 * \return 	1 if Error, else 0
 */
uint8_t BqBatteryChargeCurrentUpdate(void){
	uint8_t mask[] = {1, 2, 4, 8, 16, 32, 64, 128};
	uint16_t RxBuf1 = 0;
	uint16_t ActualValue = 0;
	static uint16_t AvgSumBuffer [AVG_BATTERY_CHARGE_CURRENT_BUFFER_SIZE];
	static uint16_t AvgChargeCurrentSum = 0;
	static uint8_t AvgSumSize = 0;
	static uint8_t Index = 0;
	//Read data from BQ25713
	RxBuf1 = BqReadByte(ADCIBAT_1);
	if(BqIsNthBitSet(RxBuf1, 15))
		return 1;
	//processing data
	for(uint8_t i = 0; i < 7; i++){
		ActualValue += (BqIsNthBitSet(RxBuf1, i) * (mask[i]*64));
	}
	BqBatteryChargeCurrent = ActualValue;
	//counting avg battery charge current
	if(Index >= AVG_BATTERY_CHARGE_CURRENT_BUFFER_SIZE)
		Index = 0;
	if(AvgSumSize < AVG_BATTERY_CHARGE_CURRENT_BUFFER_SIZE){
		AvgSumSize++;
		AvgChargeCurrentSum += ActualValue;
	}
	else{
		AvgChargeCurrentSum = AvgChargeCurrentSum - AvgSumBuffer[Index] + ActualValue;
	}
	AvgSumBuffer[Index] = ActualValue;
	Index++;
	BqAvgBatteryChargeCurrent = AvgChargeCurrentSum / AvgSumSize;
	return 0;
}

/**
 * \brief 	Update Battery Discharge Current value
 * \param 	void
 * \see 	BqAdcRegistersUpdate()
 * \note 	Function read data from BQ ADC register and convert them to real value.
 * \return 	1 if Error, else 0
 */
uint8_t BqBatteryDischargeCurrentUpdate(void){
	uint8_t mask[] = {1, 2, 4, 8, 16, 32, 64, 128};
	uint16_t RxBuf0= 0;
	RxBuf0= BqReadByte(ADCIBAT_0);
	if(BqIsNthBitSet(RxBuf0  , 15))
		return 1;
	BqBatteryDischargeCurrent = 0;
	for(uint8_t i = 0; i < 7; i++){
		BqBatteryDischargeCurrent += (BqIsNthBitSet(RxBuf0  , i) * (mask[i]*256));
	}
	return 0;
}

/**
 * \brief 	Update Battery Voltage value
 * \param 	void
 * \see 	BqAdcRegistersUpdate()
 * \note 	Function read data from BQ ADC register and convert them to real value.
 * \return 	1 if Error, else 0
 */
uint8_t BqBatteryVoltageUpdate(void){
	uint8_t mask[] = {1, 2, 4, 8, 16, 32, 64, 128};
	uint16_t RxBuf0 = 0;
	uint16_t ActualValue = 0;
	static uint16_t AvgSumBuffer[AVG_BATTERY_VOLTAGE_BUFFER_SIZE];
	static uint32_t AvgBatteryVoltageSum = 0;
	static uint8_t AvgSumSize = 0;
	static uint8_t Index = 0;
	//Read data from BQ25713
	RxBuf0= BqReadByte(ADCVSYSVBAT_0);
	if(BqIsNthBitSet(RxBuf0  , 15))
		return 1;
		
	//Processing data
	if(RxBuf0 == 0)
		ActualValue = 0;
	else
		ActualValue = 2880;
	for(uint8_t i = 0; i < 8; i++){
		ActualValue += (BqIsNthBitSet(RxBuf0  , i) * (mask[i]*64));
	}
	if(ActualValue == 0)
		ActualValue++;
	else 
		BqBatteryVoltage = ActualValue;

	//counting avg battery voltage
	if(Index >= AVG_BATTERY_VOLTAGE_BUFFER_SIZE)
		Index = 0;
	if(AvgSumSize < AVG_BATTERY_VOLTAGE_BUFFER_SIZE){
		AvgSumSize++;
		AvgBatteryVoltageSum += ActualValue;
	}
	else{
		AvgBatteryVoltageSum = AvgBatteryVoltageSum - AvgSumBuffer[Index] + ActualValue;
	}
	AvgSumBuffer[Index] = ActualValue;
	Index++;
	BqAvgBatteryVoltage = AvgBatteryVoltageSum / AvgSumSize;
	return 0;
}

/**
 * \brief 	Update System Voltage value
 * \param 	void
 * \see 	BqAdcRegistersUpdate()
 * \note 	Function read data from BQ ADC register and convert them to real value.
 * \return 	1 if Error, else 0
 */
uint8_t BqSystemVoltageUpdate(void){
	uint8_t mask[] = {1, 2, 4, 8, 16, 32, 64, 128};
	uint16_t RxBuf1 = 0;
	RxBuf1 = BqReadByte(ADCVSYSVBAT_1);
	if(BqIsNthBitSet(RxBuf1, 15))
		return 1;
	if(RxBuf1 == 0)
		BqSystemVoltage = 0;
	else
		BqSystemVoltage = 2880;
	for(uint8_t i = 0; i < 8; i++)
		BqSystemVoltage += (BqIsNthBitSet(RxBuf1, i) * (mask[i]*64));
	return 0;
}

/**
 * \brief 	Initialize ADC conversion in BQ25713
 * \param 	void
 * \see 	BqAdcRegistersUpdate()
 * \note 	If ADC in BQ25713 is configure for one-shot mode, use this function for start one
 * 			shot of ADC conversion.
 * \return 	1 if Error, else 0
 */
uint8_t BqAdcConversionStart(void){
	uint16_t RxBuf = BqReadByte(ADC_OPTION_1);
	if(BqIsNthBitSet(RxBuf, 15))
		return 1;
	if(BqWriteByte(ADC_OPTION_1, (((uint8_t)RxBuf) | 0x40)))		//bq adc one shoot start
		return 1;
	return 0;
}

/**
 * \brief 	Check ADC connversion Mode in BQ25713
 * \param 	void
 * \see 	BqAdcRegistersUpdate()
 * \note 	This function read byte from ADC Option register in BQ25713, and check mode of ADC conversion.
 * \return 	ContinousMode = 0x80 or OneShootMode = 0x00 or TransmissionErr = 0x01
 */
BqAdcModeTypeDef BqCheckAdcConversionMode(void){
	uint16_t RxBuf = 0;
	RxBuf = BqReadByte(ADC_OPTION_1);
	if(BqIsNthBitSet(RxBuf, 15))
		return TransmissionErr;
	if(BqIsNthBitSet(RxBuf, 7))
		return ContinousMode;
	else
		return OneShootMode;
}

/**
 * \brief 	Update all values from BQ25713 ADC registers
 * \param 	void
 * \see 	BqLoopHandler()
 * \see 	BqCheckAdcConversionMode
 * \see 	BqBatteryDischargeCurrentUpdate
 * \see 	BqBatteryChargeCurrentUpdate
 * \see 	BqInputVoltageUpdate
 * \see 	BqBatteryVoltageUpdate
 * \see 	BqSystemVoltageUpdate
 * \see 	BqInputCurrentInDpmUpdate
 * \note 	If ADC conversion in BQ25713 is set as one-shoot mode, it first
 * 			udpates all values from ADC registers in BQ25713, and after it, call ADCStart function.
 * 			The new values will be available in next call of function because ADC conversion time is 10ms.
 * 			This function is call in main loop of program so it don't contain any delay's.
 * \return 	1 if Error, else 0
 */
uint8_t BqAdcRegistersUpdate(void){
	uint8_t ErrorFlag = 0;
	//uint8_t AdcConvMode = BqCheckAdcConversionMode();
	if(BqBatteryDischargeCurrentUpdate())
		ErrorFlag = 1;
	if(BqBatteryChargeCurrentUpdate())
		ErrorFlag = 1;
	if(BqInputVoltageUpdate())
		ErrorFlag = 1;
	if(BqBatteryVoltageUpdate())
		ErrorFlag = 1;
	if(BqSystemVoltageUpdate())
		ErrorFlag = 1;
	if(BqInputCurrentInDpmUpdate())
		ErrorFlag = 1;
	if(BqSystemPowerUpdate())
		ErrorFlag = 1;
	//if(AdcConvMode == TransmissionErr)		// check if BQ ADC is set for one-shot update
	//	ErrorFlag = 1;						// if one-shot - > Start ADC Conversion
	//else if( AdcConvMode == OneShootMode)	// ADC start conversion is at the end to make sure
	//	if(BqAdcConversionStart())			// that the conversion will end after registers reading
	//		ErrorFlag = 1;					// The registers values will be update in next call
	return ErrorFlag;
}

/**
 * \brief 	Check if N-th bit in value is set
 * \param 	value - value to check
 * \param 	BitNumber	- number of bit to check
 * \see
 * \note 	This is auxiliary function to analyze data from BQ25713 registers.
 * \return 	1 if bit is set, 0 if reset
 */
uint8_t BqIsNthBitSet(uint16_t value, uint8_t BitNumber){
	return ((value >> BitNumber) & 0x0001);
}

/**
 * \brief 	Reset all registers
 * \param 	void
 * \see 	BqChargerInit()
 * \note 	Function reset all registers in BQ25713 to default values.
 * \return 	1 if Error, else 0
 */
uint8_t BqResetAllReg(void){
	uint16_t RxBuf = 0;
	RxBuf =  BqReadByte(CHARGE_OPTION3_1);
	if(BqIsNthBitSet(RxBuf, 15))
		return 1;
	if(BqWriteByte(CHARGE_OPTION3_1, (RxBuf| (0x01<<6))))
		return 1;
	return 0;
}

/**
 * \brief 	Set power mode of BQ25713
 * \param 	'LowPowerModeEnable' or 'LowPowerModeDisable'
 * \see
 * \note 	Function for set power mode of BQ25713. In low power mode some functions are not available.
 * 			To check details, go to BQ25713 datasheet - page 37.
 * \return 	1 if Error, else 0
 */
uint8_t BqSetPowerMode(BqPowerModeTypeDef PwrMode){
	uint16_t RxBuf = 0;
	RxBuf =  BqReadByte(CHARGE_OPTION0_1);
	if(BqIsNthBitSet(RxBuf, 15))
		return 1;
	if(BqWriteByte(CHARGE_OPTION0_1, ( ((uint8_t)RxBuf& ~(1 << 7)) | (PwrMode << 7) )))
		return 1;
	return 0;
}

/**
 * \brief 	Set power IBAT pin mode in BQ25713
 * \param 	'ChargeCurrentMode' or DischargeCurrentMode'
 * \see
 * \note 	Function for change value source of IBAT analog output in BQ25713.
 * \return 	1 if Error, else 0
 */
uint8_t BqSetIbatMode(BqIbatModeTypeDef IbatMode){
	uint16_t RxBuf =  0;
	RxBuf =  BqReadByte(CHARGE_OPTION2_0);
	if(BqIsNthBitSet(RxBuf, 15))
		return 1;
	if(BqWriteByte(CHARGE_OPTION2_0, ( ((uint8_t)RxBuf& ~(1 << 6)) | (IbatMode << 6) )))
		return 1;
	return 0;
}

/**
 * \brief 	Enable/Disable IBAT pin in BQ25713
 * \param 	'Enable' or 'Disable'
 * \see
 * \note 	Function for enable/disable IBAT analog output pin in BQ25713
 * \return 	1 if Error, else 0
 */
uint8_t BqSetIbat(BqOptionStateTypeDef State){
	uint16_t RxBuf =  0;
	RxBuf = BqReadByte(CHARGE_OPTION1_1);
	if(BqIsNthBitSet(RxBuf, 15))
		return 1;
	if(BqWriteByte(CHARGE_OPTION1_1, ( ((uint8_t)RxBuf& ~(1 << 7)) | (State << 7) )))
		return 1;
	return 0;
}

/**
 * \brief	BQ25713 Error Handler function
 * \param	void
 * \see		BqLoopHandler()
 * \see		BqClearSystemOverVoltageFlag
 * \note	Function check ChargerStatur register (0x20 address) and updates data
 * 			in PowerStatus structure. If any error is occur, proper procedure is
 * 			executed.
 * \return	if Error 1, else 0
 */
uint8_t BqErrorHandler(void){
	uint16_t RxBuf = 0;
	RxBuf = BqReadByte(CHARGER_STATUS_0);
	if(BqIsNthBitSet(RxBuf, 15))
		return 1;
	if(BqIsNthBitSet(RxBuf, 7))			//External Adapter Over Voltage Protection Active
		BqFlagExtAdapterOv = 1;
	else
		BqFlagExtAdapterOv = 0;
	if(BqIsNthBitSet(RxBuf, 6))		//Battery Over Current Protection Active
		BqFlagBatteryOc = 1;
	else
		BqFlagBatteryOc = 0;
	if(BqIsNthBitSet(RxBuf, 5))		//External Adapter Over Current Protection Active
		BqFlagExtAdapterOc = 1;
	else
		BqFlagExtAdapterOc = 0;
	if(BqIsNthBitSet(RxBuf, 4)){		//System Voltage Over Voltage Protection Active
		BqFlagSystemOv = 1;
		BqClearSystemOverVoltageFlag();
	}
	else
		BqFlagSystemOv = 0;
	return 0;
}

/**
 * \brief	BQ25713 Loop Handler function
 * \param	void
 * \see		BqErrorHandler()
 * \note	Function contain all action which should be execute in main loop of program.
 * \return	if Error 1, else 0
 */
uint8_t BqLoopHandler(void){
	BqErrorHandler();
	BqAdcRegistersUpdate();
	BqInputCurrentUpdate();
	BqInputVoltageUpdate();
	BqSystemPowerUpdate();
	BqBatteryChargeCurrentUpdate();
	BqBatteryDischargeCurrentUpdate();
	BqBatteryVoltageUpdate();
	BqSystemVoltageUpdate();

//#if BQ_DEBUG_MODE == 1
//	BqAllRegUpdate();
//#endif
	return 0;
}

/**
 * \brief	System Overvoltage Alarm clear
 * \param	void
 * \see		BqErrorHandler()
 * \note	Function reset 4 bit in ChargerStatus (0x20) register in BQ25713.
 * \return	void
 */
void BqClearSystemOverVoltageFlag(void){
	BqWriteByte(CHARGER_STATUS_0, 0x00);
}

#if BQ_DEBUG_MODE == 1

/* VARIABLES */

/* FUNCTION PROTOTYPES */

/**
 * \brief 	Function for update all BQ2571 registers content in coprocessor structures.
 * \param 	void
 * \see 	MainLogicLoop()
 * \note 	This function should be call, before processor enters to main loop program.
 * \return 	1 if Error, else 0
 */
uint8_t BqAllRegUpdate(void){
	BoolTypeDef ErrorFlag = False;
	if(BqChargeOption0RegUpdate())
		ErrorFlag = True;
	if(BqChargeOption1RegUpdate())
		ErrorFlag = True;
	if(BqChargeOption2RegUpdate())
		ErrorFlag = True;
	if(BqChargeOption3RegUpdate())
		ErrorFlag = True;
	if(BqProchotOption0RegUpdate())
		ErrorFlag = True;
	if(BqProchotOption1RegUpdate())
		ErrorFlag = True;
	if(BqAdcOptionRegUpdate())
		ErrorFlag = True;
	if(BqChargerStatusRegUpdate())
		ErrorFlag = True;
	if(BqProchotStatusRegUpdate())
		ErrorFlag = True;
	if(BqChargeCurrentRegUpdate())
		ErrorFlag = True;
	if(BqMaxChargeVoltageRegUpdate())
		ErrorFlag = True;
	if(BqMinSystemVoltageRegUpdate())
		ErrorFlag = True;
	if(BqIinHostRegUpdate())
		ErrorFlag = True;
	if(BqIinDpmRegUpdate())
		ErrorFlag = True;
	if(BqInputVoltageRegUpdate())
		ErrorFlag = True;
	if(BqOtgVoltageRegUpdate())
		ErrorFlag = True;
	if(BqOtgCurrentRegUpdate())
		ErrorFlag = True;
	if(BqAdcVbusPsysRegUpdate())
		ErrorFlag = True;
	if(BqAdcIbatRegUpdate())
		ErrorFlag = True;
	if(BqAdcIinCmpinRegUpdate())
		ErrorFlag = True;
	if(AdcVsysVbatRegUpdate())
		ErrorFlag = True;
	return ErrorFlag;
}

/**
 * \brief 	Function for update data form ChargeOption0 registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from CHARGE_OPTION_0 registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t BqChargeOption0RegUpdate(void){
	uint16_t temp = 0;
	ChargeOption0.RxErr = False;
	temp = BqReadByte(CHARGE_OPTION0_1);
	ChargeOption0.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ChargeOption0.RxErr = True;
	ChargeOption0.EnLwpwr 			= BqIsNthBitSet(temp, 7);
	ChargeOption0.WdtmrAdj_1		= BqIsNthBitSet(temp, 6);
	ChargeOption0.WdtmrAdj_0		= BqIsNthBitSet(temp, 5);
	ChargeOption0.IdpmAutoDisable	= BqIsNthBitSet(temp, 4);
	ChargeOption0.OtgOnChargOk		= BqIsNthBitSet(temp, 3);
	ChargeOption0.EnOoa				= BqIsNthBitSet(temp, 2);
	ChargeOption0.PwmFreq			= BqIsNthBitSet(temp, 1);
	ChargeOption0.LowPtmRipple		= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(CHARGE_OPTION0_0);
	ChargeOption0.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ChargeOption0.RxErr = True;
	ChargeOption0.SysShortDisable	= BqIsNthBitSet(temp, 6);
	ChargeOption0.EnLearn			= BqIsNthBitSet(temp, 5);
	ChargeOption0.IadptGain			= BqIsNthBitSet(temp, 4);
	ChargeOption0.IbatGain			= BqIsNthBitSet(temp, 3);
	ChargeOption0.EnLdo				= BqIsNthBitSet(temp, 2);
	ChargeOption0.EnIpdm			= BqIsNthBitSet(temp, 1);
	ChargeOption0.ChrgInhibit		= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form ChargeOption1 registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from ChargeOption1 registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t BqChargeOption1RegUpdate(void){
	uint16_t temp = 0;
	ChargeOption1.RxErr = False;
	temp = BqReadByte(CHARGE_OPTION1_1);
	ChargeOption1.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ChargeOption1.RxErr = True;
	ChargeOption1.EnIbat 			= BqIsNthBitSet(temp, 7);
	ChargeOption1.EnProchotLpwr_1	= BqIsNthBitSet(temp, 6);
	ChargeOption1.EnProchotLpwr_0	= BqIsNthBitSet(temp, 5);
	ChargeOption1.EnPsys			= BqIsNthBitSet(temp, 4);
	ChargeOption1.RsnsRac			= BqIsNthBitSet(temp, 3);
	ChargeOption1.RsnsRsr			= BqIsNthBitSet(temp, 2);
	ChargeOption1.PsysRatio			= BqIsNthBitSet(temp, 1);
	ChargeOption1.PtmPinsel			= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(CHARGE_OPTION1_0);
	ChargeOption1.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ChargeOption1.RxErr = True;
	ChargeOption1.CmpRef			= BqIsNthBitSet(temp, 7);
	ChargeOption1.CmpPol			= BqIsNthBitSet(temp, 6);
	ChargeOption1.CmpDeg_1			= BqIsNthBitSet(temp, 5);
	ChargeOption1.CmpDeg_0			= BqIsNthBitSet(temp, 4);
	ChargeOption1.ForceLatchoff		= BqIsNthBitSet(temp, 3);
	ChargeOption1.EnPtm				= BqIsNthBitSet(temp, 2);
	ChargeOption1.EnShipDchg		= BqIsNthBitSet(temp, 1);
	ChargeOption1.AutoWakeUpEn		= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form ChargeOption2 registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from ChargeOption2 registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t BqChargeOption2RegUpdate(void){
	uint16_t temp = 0;
	ChargeOption2.RxErr = False;
	temp = BqReadByte(CHARGE_OPTION2_1);
	ChargeOption2.RegMSByte = (uint8_t)temp;
	if(BqIsNthBitSet(temp, 15))
		return ChargeOption2.RxErr = True;
	ChargeOption2.PkpwrTovldDeg_1	= BqIsNthBitSet(temp, 7);
	ChargeOption2.PkpwrTovldDeg_0	= BqIsNthBitSet(temp, 6);
	ChargeOption2.EnPkpwrIdpm	 	= BqIsNthBitSet(temp, 5);
	ChargeOption2.EnPkpwrVsys		= BqIsNthBitSet(temp, 4);
	ChargeOption2.PkpwrOvldStat		= BqIsNthBitSet(temp, 3);
	ChargeOption2.PkpwrRelaxStat	= BqIsNthBitSet(temp, 2);
	ChargeOption2.PkpwrTmax_1		= BqIsNthBitSet(temp, 1);
	ChargeOption2.PkpwrTmax_0		= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(CHARGE_OPTION2_0);
	ChargeOption2.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ChargeOption2.RxErr = True;
	ChargeOption2.EnExtilim	 		= BqIsNthBitSet(temp, 7);
	ChargeOption2.EnIchgIdchg		= BqIsNthBitSet(temp, 6);
	ChargeOption2.Q2Ocp				= BqIsNthBitSet(temp, 5);
	ChargeOption2.AcxOcp			= BqIsNthBitSet(temp, 4);
	ChargeOption2.EnAcoc			= BqIsNthBitSet(temp, 3);
	ChargeOption2.AcocVth			= BqIsNthBitSet(temp, 2);
	ChargeOption2.EnBatoc			= BqIsNthBitSet(temp, 1);
	ChargeOption2.BatocVth			= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form ChargeOption3 registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from ChargeOption3 registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t BqChargeOption3RegUpdate(void){
	uint16_t temp = 0;
	ChargeOption3.RxErr = False;
	temp = BqReadByte(CHARGE_OPTION3_1);
	ChargeOption3.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ChargeOption3.RxErr = True;
	ChargeOption3.EnHiz 			= BqIsNthBitSet(temp, 7);
	ChargeOption3.ResetReg			= BqIsNthBitSet(temp, 6);
	ChargeOption3.ResetVindpm		= BqIsNthBitSet(temp, 5);
	ChargeOption3.EnOtg				= BqIsNthBitSet(temp, 4);
	ChargeOption3.EnIcoMode			= BqIsNthBitSet(temp, 3);
	temp = BqReadByte(CHARGE_OPTION3_0);
	ChargeOption3.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ChargeOption3.RxErr = True;
	ChargeOption3.EnConVap			= BqIsNthBitSet(temp, 6);
	ChargeOption3.OtgVapMode		= BqIsNthBitSet(temp, 5);
	ChargeOption3.IlAvg_1			= BqIsNthBitSet(temp, 4);
	ChargeOption3.IlAvg_0			= BqIsNthBitSet(temp, 3);
	ChargeOption3.OtgRangeLow		= BqIsNthBitSet(temp, 2);
	ChargeOption3.BatfetoffHiz		= BqIsNthBitSet(temp, 1);
	ChargeOption3.PsysOthIdchg		= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form ProchotOption0 registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from ProchotOption0 registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t BqProchotOption0RegUpdate(void){
	uint16_t temp = 0;
	ProchotOption0.RxErr = False;
	temp = BqReadByte(PROCHOT_OPTION0_1);
	ProchotOption0.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ProchotOption0.RxErr = True;
	ProchotOption0.Ilim2Vth_4		= BqIsNthBitSet(temp, 7);
	ProchotOption0.Ilim2Vth_3		= BqIsNthBitSet(temp, 6);
	ProchotOption0.Ilim2Vth_2		= BqIsNthBitSet(temp, 5);
	ProchotOption0.Ilim2Vth_1		= BqIsNthBitSet(temp, 4);
	ProchotOption0.Ilim2Vth_0		= BqIsNthBitSet(temp, 3);
	ProchotOption0.IcritDeg_1		= BqIsNthBitSet(temp, 2);
	ProchotOption0.IcritDeg_0		= BqIsNthBitSet(temp, 1);
	ProchotOption0.ProchotVdpm_80_90= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(PROCHOT_OPTION0_0);
	ProchotOption0.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ProchotOption0.RxErr = True;
	ProchotOption0.VsysTh1_3		= BqIsNthBitSet(temp, 7);
	ProchotOption0.VsysTh1_2		= BqIsNthBitSet(temp, 6);
	ProchotOption0.VsysTh1_1		= BqIsNthBitSet(temp, 5);
	ProchotOption0.VsysTh1_0		= BqIsNthBitSet(temp, 4);
	ProchotOption0.VsysTh2_1		= BqIsNthBitSet(temp, 3);
	ProchotOption0.VsysTh2_0		= BqIsNthBitSet(temp, 2);
	ProchotOption0.InomDeg			= BqIsNthBitSet(temp, 1);
	ProchotOption0.LowerProchotVdpm	= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief Function for update data form ProchotOption1 registers in BQ25713
 * \param void
 * \see BqAllRegUpdate()
 * \note Function read data from ProchotOption1 registers in BQ25713
 * and write them to the proper structure in coprocessor.
 * \return 1 if Error, else 0
 */
uint8_t BqProchotOption1RegUpdate(void){
	uint16_t temp = 0;
	ProchotOption1.RxErr = False;
	temp = BqReadByte(PROCHOT_OPTION1_1);
	ProchotOption1.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ProchotOption1.RxErr = True;
	ProchotOption1.IdchgVth_5		= BqIsNthBitSet(temp, 7);
	ProchotOption1.IdchgVth_4		= BqIsNthBitSet(temp, 6);
	ProchotOption1.IdchgVth_3		= BqIsNthBitSet(temp, 5);
	ProchotOption1.IdchgVth_2		= BqIsNthBitSet(temp, 4);
	ProchotOption1.IdchgVth_1		= BqIsNthBitSet(temp, 3);
	ProchotOption1.IdchgVth_0		= BqIsNthBitSet(temp, 2);
	ProchotOption1.IdchgDeg_1		= BqIsNthBitSet(temp, 1);
	ProchotOption1.IdchgDeg_0		= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(PROCHOT_OPTION1_0);
	ProchotOption1.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ProchotOption1.RxErr = True;
	ProchotOption1.ProchotProfileVdpm	= BqIsNthBitSet(temp, 7);
	ProchotOption1.ProchotProfileComp	= BqIsNthBitSet(temp, 6);
	ProchotOption1.ProchotProfileIcrit	= BqIsNthBitSet(temp, 5);
	ProchotOption1.ProchotProfileInom	= BqIsNthBitSet(temp, 4);
	ProchotOption1.ProchotProfileIdchg	= BqIsNthBitSet(temp, 3);
	ProchotOption1.ProchotProfileVsys	= BqIsNthBitSet(temp, 2);
	ProchotOption1.ProchotProfileBatpres= BqIsNthBitSet(temp, 1);
	ProchotOption1.ProchotProfileAcok	= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form ADCOption registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from ADCOption registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t BqAdcOptionRegUpdate(void){
	uint16_t temp;
	AdcOption.RxErr = False;
	temp = BqReadByte(ADC_OPTION_1);
	AdcOption.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return AdcOption.RxErr = True;
	AdcOption.AdcConv		= BqIsNthBitSet(temp, 7);
	AdcOption.AdcStart		= BqIsNthBitSet(temp, 6);
	AdcOption.AdcFullscale	= BqIsNthBitSet(temp, 5);
	temp = BqReadByte(ADC_OPTION_0);
	AdcOption.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return AdcOption.RxErr = True;
	AdcOption.EnAdcCmpin	= BqIsNthBitSet(temp, 7);
	AdcOption.EnAdcVbus		= BqIsNthBitSet(temp, 6);
	AdcOption.EnAdcPsys		= BqIsNthBitSet(temp, 5);
	AdcOption.EnAdcIin		= BqIsNthBitSet(temp, 4);
	AdcOption.EnAdcIdchg	= BqIsNthBitSet(temp, 3);
	AdcOption.EnAdcIchg		= BqIsNthBitSet(temp, 2);
	AdcOption.EnAdcVsys		= BqIsNthBitSet(temp, 1);
	AdcOption.EnAdcVbat		= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form ChargerStatus registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from ChargerStatus registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t BqChargerStatusRegUpdate(void){
	uint16_t temp = 0;
	ChargerStatus.RxErr = False;
	temp = BqReadByte(CHARGER_STATUS_1);
	ChargerStatus.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ChargerStatus.RxErr = True;
	ChargerStatus.AcStat 	= BqIsNthBitSet(temp, 7);
	ChargerStatus.IcoDone 	= BqIsNthBitSet(temp, 6);
	ChargerStatus.InVap 	= BqIsNthBitSet(temp, 5);
	ChargerStatus.InVindpm 	= BqIsNthBitSet(temp, 4);
	ChargerStatus.InIindpm 	= BqIsNthBitSet(temp, 3);
	ChargerStatus.InFchrg 	= BqIsNthBitSet(temp, 2);
	ChargerStatus.InPchrg 	= BqIsNthBitSet(temp, 1);
	ChargerStatus.InOtg 	= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(CHARGER_STATUS_0);
	ChargerStatus.RegLSByte = temp;
	if(BqIsNthBitSet(temp,15))
		return ChargerStatus.RxErr = True;
	ChargerStatus.FaultAcov 	= BqIsNthBitSet(temp, 7);
	ChargerStatus.FaultBatoc 	= BqIsNthBitSet(temp, 6);
	ChargerStatus.FaultAcoc 	= BqIsNthBitSet(temp, 5);
	ChargerStatus.SysOvpStat 	= BqIsNthBitSet(temp, 4);
	ChargerStatus.FaultSysShort = BqIsNthBitSet(temp, 3);
	ChargerStatus.FaultLatchoff = BqIsNthBitSet(temp, 2);
	ChargerStatus.FaultOtgOvp 	= BqIsNthBitSet(temp, 1);
	ChargerStatus.FaultOtgUvp 	= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form ProchotStatus registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from ProchotStatus registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t BqProchotStatusRegUpdate(void){
	uint16_t temp = 0;
	ProchotStatus.RxErr = False;
	temp = BqReadByte(PROCHOT_STATUS_1);
	ProchotStatus.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ProchotStatus.RxErr = True;
	ProchotStatus.EnProchotExit		= BqIsNthBitSet(temp, 6);
	ProchotStatus.ProchotWidth_1	= BqIsNthBitSet(temp, 5);
	ProchotStatus.ProchotWidth_0	= BqIsNthBitSet(temp, 4);
	ProchotStatus.ProchotClear		= BqIsNthBitSet(temp, 3);
	ProchotStatus.StatVapFail		= BqIsNthBitSet(temp, 1);
	ProchotStatus.StatExitVap		= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(PROCHOT_STATUS_0);
	ProchotStatus.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ProchotStatus.RxErr = True;
	ProchotStatus.StatVpdm				= BqIsNthBitSet(temp, 7);
	ProchotStatus.StatComp				= BqIsNthBitSet(temp, 6);
	ProchotStatus.StatIcrit				= BqIsNthBitSet(temp, 5);
	ProchotStatus.StatInom				= BqIsNthBitSet(temp, 4);
	ProchotStatus.StatIdchg				= BqIsNthBitSet(temp, 3);
	ProchotStatus.StatVsys				= BqIsNthBitSet(temp, 2);
	ProchotStatus.StatBatteryRemoval 	= BqIsNthBitSet(temp, 1);
	ProchotStatus.StatAdapterRemoval	= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form ChargeCurrent registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from ChargeCurrent registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t BqChargeCurrentRegUpdate(void){
	uint16_t temp = 0;
	ChargeCurrent.RxErr = False;
	temp = BqReadByte(CHARGE_CURRENT_1);
	ChargeCurrent.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ChargeCurrent.RxErr = True;
	ChargeCurrent.InvalidWrite_2	= BqIsNthBitSet(temp, 7);
	ChargeCurrent.InvalidWrite_1	= BqIsNthBitSet(temp, 6);
	ChargeCurrent.InvalidWrite_0	= BqIsNthBitSet(temp, 5);
	ChargeCurrent.Bit6				= BqIsNthBitSet(temp, 4);
	ChargeCurrent.Bit5				= BqIsNthBitSet(temp, 3);
	ChargeCurrent.Bit4				= BqIsNthBitSet(temp, 2);
	ChargeCurrent.Bit3				= BqIsNthBitSet(temp, 1);
	ChargeCurrent.Bit2				= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(CHARGE_CURRENT_0);
	ChargeCurrent.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return ChargeCurrent.RxErr = True;
	ChargeCurrent.Bit1				= BqIsNthBitSet(temp, 7);
	ChargeCurrent.Bit0				= BqIsNthBitSet(temp, 6);
	return 0;
}

/**
 * \brief 	Function for update data form MaxChargeVoltage registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from MaxChargeVoltage registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t BqMaxChargeVoltageRegUpdate(void){
	uint16_t temp = 0;
	MaxChargeVoltage.RxErr = False;
	temp = BqReadByte(MAX_CHARGE_VOLTAGE_1);
	MaxChargeVoltage.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return MaxChargeVoltage.RxErr = True;
	MaxChargeVoltage.InvalidWrite	= BqIsNthBitSet(temp, 7);
	MaxChargeVoltage.Bit11		 	= BqIsNthBitSet(temp, 6);
	MaxChargeVoltage.Bit10			= BqIsNthBitSet(temp, 5);
	MaxChargeVoltage.Bit9			= BqIsNthBitSet(temp, 4);
	MaxChargeVoltage.Bit8			= BqIsNthBitSet(temp, 3);
	MaxChargeVoltage.Bit7			= BqIsNthBitSet(temp, 2);
	MaxChargeVoltage.Bit6			= BqIsNthBitSet(temp, 1);
	MaxChargeVoltage.Bit5			= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(MAX_CHARGE_VOLTAGE_0);
	MaxChargeVoltage.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return MaxChargeVoltage.RxErr = True;
	MaxChargeVoltage.Bit4			= BqIsNthBitSet(temp, 7);
	MaxChargeVoltage.Bit3			= BqIsNthBitSet(temp, 6);
	MaxChargeVoltage.Bit2			= BqIsNthBitSet(temp, 5);
	MaxChargeVoltage.Bit1			= BqIsNthBitSet(temp, 4);
	MaxChargeVoltage.Bit0			= BqIsNthBitSet(temp, 3);
	return 0;
}

/**
 * \brief 	Function for update data form MinSystemVoltage registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from MinSystemVoltage registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t BqMinSystemVoltageRegUpdate(void){
	uint16_t temp = 0;
	MinSystemVoltage.RxErr = False;
	temp = BqReadByte(MIN_SYSTEM_VOLTAGE_1);
	MinSystemVoltage.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return MinSystemVoltage.RxErr = True;
	MinSystemVoltage.InvalidWrite_1	= BqIsNthBitSet(temp, 7);
	MinSystemVoltage.InvalidWrite_0	= BqIsNthBitSet(temp, 6);
	MinSystemVoltage.Bit5			= BqIsNthBitSet(temp, 5);
	MinSystemVoltage.Bit4			= BqIsNthBitSet(temp, 4);
	MinSystemVoltage.Bit3			= BqIsNthBitSet(temp, 3);
	MinSystemVoltage.Bit2			= BqIsNthBitSet(temp, 2);
	MinSystemVoltage.Bit1			= BqIsNthBitSet(temp, 1);
	MinSystemVoltage.Bit0			= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form IIN_HOST registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from IIN_HOST registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t BqIinHostRegUpdate(void){
	uint16_t temp = 0;
	IinHost.RxErr = False;
	temp = BqReadByte(IIN_HOST_1);
	IinHost.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return IinHost.RxErr = True;
	IinHost.InvalidWrite	= BqIsNthBitSet(temp, 7);
	IinHost.Bit6			= BqIsNthBitSet(temp, 6);
	IinHost.Bit5			= BqIsNthBitSet(temp, 5);
	IinHost.Bit4			= BqIsNthBitSet(temp, 4);
	IinHost.Bit3			= BqIsNthBitSet(temp, 3);
	IinHost.Bit2			= BqIsNthBitSet(temp, 2);
	IinHost.Bit1			= BqIsNthBitSet(temp, 1);
	IinHost.Bit0			= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form IIN_DPM registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from IIN_DPM registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t	BqIinDpmRegUpdate(void){
	uint16_t temp = 0;
	IinDpm.RxErr = False;
	temp = BqReadByte(IIN_DPM_1);
	IinDpm.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return IinDpm.RxErr = True;
	IinDpm.InvalidWrite		= BqIsNthBitSet(temp, 7);
	IinDpm.Bit6				= BqIsNthBitSet(temp, 6);
	IinDpm.Bit5				= BqIsNthBitSet(temp, 5);
	IinDpm.Bit4				= BqIsNthBitSet(temp, 4);
	IinDpm.Bit3				= BqIsNthBitSet(temp, 3);
	IinDpm.Bit2				= BqIsNthBitSet(temp, 2);
	IinDpm.Bit1				= BqIsNthBitSet(temp, 1);
	IinDpm.Bit0				= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form InputVoltage registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from InputVoltage registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t	BqInputVoltageRegUpdate(void){
	uint16_t temp = 0;
	InputVoltage.RxErr = False;
	temp = BqReadByte(INPUT_VOLTAGE_1);
	InputVoltage.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return InputVoltage.RxErr = True;
	InputVoltage.InvalidWrite_1 = BqIsNthBitSet(temp, 7);
	InputVoltage.InvalidWrite_0 = BqIsNthBitSet(temp, 6);
	InputVoltage.Bit7			= BqIsNthBitSet(temp, 5);
	InputVoltage.Bit6			= BqIsNthBitSet(temp, 4);
	InputVoltage.Bit5			= BqIsNthBitSet(temp, 3);
	InputVoltage.Bit4			= BqIsNthBitSet(temp, 2);
	InputVoltage.Bit3			= BqIsNthBitSet(temp, 1);
	InputVoltage.Bit2			= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(INPUT_VOLTAGE_0);
	InputVoltage.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return InputVoltage.RxErr = True;
	InputVoltage.Bit1			= BqIsNthBitSet(temp, 7);
	InputVoltage.Bit0			= BqIsNthBitSet(temp, 6);
	return 0;
	return 0;
}

/**
 * \brief 	Function for update data form OTGVoltage registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function prepare for read data from OTGVoltage registers in BQ25713.
 * 			Actually function is empty, becouse OTG function is not used.
 * \return 	always 0
 */
uint8_t	BqOtgVoltageRegUpdate(void){
	return 0;
}

/**
 * \brief 	Function for update data form OTGCurrent registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function prepare for read data from OTGCurrent registers in BQ25713.
 * 			Actually function is empty, becouse OTG function is not used.
 * \return 	always 0
 */
uint8_t BqOtgCurrentRegUpdate(void){
	return 0;
}

/**
 * \brief 	Function for update data form ADCVBUS/PSYS registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from ADCVBUS/PSYS registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t BqAdcVbusPsysRegUpdate(void){
	uint16_t temp = 0;
	AdcVbusPsys.RxErr = False;
	temp = BqReadByte(ADCVBUS_PSYS_1);
	AdcVbusPsys.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return AdcVbusPsys.RxErr = True;
	AdcVbusPsys.VbusBit7	= BqIsNthBitSet(temp, 7);
	AdcVbusPsys.VbusBit6	= BqIsNthBitSet(temp, 6);
	AdcVbusPsys.VbusBit5	= BqIsNthBitSet(temp, 5);
	AdcVbusPsys.VbusBit4	= BqIsNthBitSet(temp, 4);
	AdcVbusPsys.VbusBit3	= BqIsNthBitSet(temp, 3);
	AdcVbusPsys.VbusBit2	= BqIsNthBitSet(temp, 2);
	AdcVbusPsys.VbusBit1	= BqIsNthBitSet(temp, 1);
	AdcVbusPsys.VbusBit0	= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(ADCVBUS_PSYS_0);
	AdcVbusPsys.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return AdcVbusPsys.RxErr = True;
	AdcVbusPsys.PsysBit7	= BqIsNthBitSet(temp, 7);
	AdcVbusPsys.PsysBit6	= BqIsNthBitSet(temp, 6);
	AdcVbusPsys.PsysBit5	= BqIsNthBitSet(temp, 5);
	AdcVbusPsys.PsysBit4	= BqIsNthBitSet(temp, 4);
	AdcVbusPsys.PsysBit3	= BqIsNthBitSet(temp, 3);
	AdcVbusPsys.PsysBit2	= BqIsNthBitSet(temp, 2);
	AdcVbusPsys.PsysBit1	= BqIsNthBitSet(temp, 1);
	AdcVbusPsys.PsysBit0	= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form ADCIBAT registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from ADCIBAT registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t	BqAdcIbatRegUpdate(void){
	uint16_t temp = 0;
	AdcIbat.RxErr = False;
	temp = BqReadByte(ADCIBAT_1);
	AdcIbat.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return AdcIbat.RxErr = True;
	AdcIbat.IchgBit6	= BqIsNthBitSet(temp, 6);
	AdcIbat.IchgBit5	= BqIsNthBitSet(temp, 5);
	AdcIbat.IchgBit4	= BqIsNthBitSet(temp, 4);
	AdcIbat.IchgBit3	= BqIsNthBitSet(temp, 3);
	AdcIbat.IchgBit2	= BqIsNthBitSet(temp, 2);
	AdcIbat.IchgBit1	= BqIsNthBitSet(temp, 1);
	AdcIbat.IchgBit0	= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(ADCIBAT_0);
	AdcIbat.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return AdcIbat.RxErr = True;
	AdcIbat.IdchgBit6	= BqIsNthBitSet(temp, 6);
	AdcIbat.IdchgBit5	= BqIsNthBitSet(temp, 5);
	AdcIbat.IdchgBit4	= BqIsNthBitSet(temp, 4);
	AdcIbat.IdchgBit3	= BqIsNthBitSet(temp, 3);
	AdcIbat.IdchgBit2	= BqIsNthBitSet(temp, 2);
	AdcIbat.IdchgBit1	= BqIsNthBitSet(temp, 1);
	AdcIbat.IdchgBit0	= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form ADCIINCMPIN registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from ADCIINCMPIN registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t	BqAdcIinCmpinRegUpdate(void){
	uint16_t temp = 0;
	AdcIinCmpin.RxErr = False;
	temp = BqReadByte(ADCIINCMPIN_1);
	AdcIinCmpin.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return AdcIinCmpin.RxErr = True;
	AdcIinCmpin.IinBit7		= BqIsNthBitSet(temp, 7);
	AdcIinCmpin.IinBit6		= BqIsNthBitSet(temp, 6);
	AdcIinCmpin.IinBit5		= BqIsNthBitSet(temp, 5);
	AdcIinCmpin.IinBit4		= BqIsNthBitSet(temp, 4);
	AdcIinCmpin.IinBit3		= BqIsNthBitSet(temp, 3);
	AdcIinCmpin.IinBit2		= BqIsNthBitSet(temp, 2);
	AdcIinCmpin.IinBit1		= BqIsNthBitSet(temp, 1);
	AdcIinCmpin.IinBit0		= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(ADCIINCMPIN_0);
	AdcIinCmpin.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return AdcIinCmpin.RxErr = True;
	AdcIinCmpin.CmpinBit7	= BqIsNthBitSet(temp, 7);
	AdcIinCmpin.CmpinBit6	= BqIsNthBitSet(temp, 6);
	AdcIinCmpin.CmpinBit5	= BqIsNthBitSet(temp, 5);
	AdcIinCmpin.CmpinBit4	= BqIsNthBitSet(temp, 4);
	AdcIinCmpin.CmpinBit3	= BqIsNthBitSet(temp, 3);
	AdcIinCmpin.CmpinBit2	= BqIsNthBitSet(temp, 2);
	AdcIinCmpin.CmpinBit1	= BqIsNthBitSet(temp, 1);
	AdcIinCmpin.CmpinBit0	= BqIsNthBitSet(temp, 0);
	return 0;
}

/**
 * \brief 	Function for update data form ADCVSYSVBAT registers in BQ25713
 * \param 	void
 * \see 	BqAllRegUpdate()
 * \note 	Function read data from ADCVSYSVBAT registers in BQ25713
 * 			and write them to the proper structure in coprocessor.
 * \return 	1 if Error, else 0
 */
uint8_t AdcVsysVbatRegUpdate(void){
	uint16_t temp = 0;
	AdcVsysVbat.RxErr = False;
	temp = BqReadByte(ADCVSYSVBAT_1);
	AdcVsysVbat.RegMSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return AdcVsysVbat.RxErr = True;
	AdcVsysVbat.VsysBit7	= BqIsNthBitSet(temp, 7);
	AdcVsysVbat.VsysBit6	= BqIsNthBitSet(temp, 6);
	AdcVsysVbat.VsysBit5	= BqIsNthBitSet(temp, 5);
	AdcVsysVbat.VsysBit4	= BqIsNthBitSet(temp, 4);
	AdcVsysVbat.VsysBit3	= BqIsNthBitSet(temp, 3);
	AdcVsysVbat.VsysBit2	= BqIsNthBitSet(temp, 2);
	AdcVsysVbat.VsysBit1	= BqIsNthBitSet(temp, 1);
	AdcVsysVbat.VsysBit0	= BqIsNthBitSet(temp, 0);
	temp = BqReadByte(ADCVSYSVBAT_0);
	AdcVsysVbat.RegLSByte = temp;
	if(BqIsNthBitSet(temp, 15))
		return AdcVsysVbat.RxErr = True;
	AdcVsysVbat.VbatBit7	= BqIsNthBitSet(temp, 7);
	AdcVsysVbat.VbatBit6	= BqIsNthBitSet(temp, 6);
	AdcVsysVbat.VbatBit5	= BqIsNthBitSet(temp, 5);
	AdcVsysVbat.VbatBit4	= BqIsNthBitSet(temp, 4);
	AdcVsysVbat.VbatBit3	= BqIsNthBitSet(temp, 3);
	AdcVsysVbat.VbatBit2	= BqIsNthBitSet(temp, 2);
	AdcVsysVbat.VbatBit1	= BqIsNthBitSet(temp, 1);
	AdcVsysVbat.VbatBit0	= BqIsNthBitSet(temp, 0);
	return 0;
}

#endif
/****END OF FILE****/
