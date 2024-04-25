/**
 *  \file 		adc.c
 *  \brief		ADC function
 *  \details	File contain function to handling ADC function in coprocessor
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

/* INCLUDES */

#include <adc.h>

/* VARIABLES */
volatile uint16_t aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
volatile uint8_t ubDmaTransferStatus = 2; 				// 0 - transfer not completed ; 1 - transfer completed ; 2 - transfer not started yet
volatile uint8_t ubAdcGrpRegularSequenceConvStatus;		// Variable set into ADC interruption callback
static uint32_t ubAdcGrpRegularSequenceConvCount = 0; 	// Variable to report number of ADC group regular sequence completed. 
volatile uint16_t BatteryThermistorVoltage;       		// Battery thermistor voltage calculated from ADC (unit: mV)
volatile int16_t TemperatureADCC; 						// Temperature (unit: degree Celcius)
volatile uint16_t AnalogVrefADCmV;         				// Analog reference voltage (Vref+), connected to analog voltage supply Vdda, (unit: mV)


/* FUNCTION */

// void ThermistorLoopHandler(void){
// 	BeginAdc();
// 	uint16_t ThermistorResistance;
// 	ThermistorResistance = (BatteryThermistorVoltage * THERMISTOR_PARALLEL_RESISTANCE) / (REFERENCE_VOLTAGE - BatteryThermistorVoltage);
// 	Battery.Temperature = (TEMPERATURE_OFFSET * THERMISTOR_BETA_CONST) / (TEMPERATURE_OFFSET * )
// }

void BeginAdc(void){
	if (ubDmaTransferStatus != 0){
		ubDmaTransferStatus = 0;
	}                                           
	if ((LL_ADC_IsDisableOngoing(ADC1) == 0) && (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)){
		LL_ADC_REG_StartConversion(ADC1);
	}
	else{
		/* Error: ADC conversion start could not be performed */
	}
	//Wait for ADC conversion and DMA transfer completion to process data
	while(ubDmaTransferStatus != 1){
	}
	BatteryThermistorVoltage	=	__LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI, aADCxConvertedData[0], LL_ADC_RESOLUTION_12B);
	TemperatureADCC				=	__LL_ADC_CALC_TEMPERATURE(VDDA_APPLI, aADCxConvertedData[1], LL_ADC_RESOLUTION_12B);
	AnalogVrefADCmV				=	__LL_ADC_CALC_VREFANALOG_VOLTAGE(aADCxConvertedData[2], LL_ADC_RESOLUTION_12B);
}


/**
  * \brief	ADC Initialization Function
  * \param	None
  * \return	None
  */
void AdcInit(void){
	//timers
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	//structs
	LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
	LL_ADC_InitTypeDef ADC_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	//gpio
	GPIO_InitStruct.Pin = TEMP_SENSOR_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(TEMP_SENSOR_GPIO_Port, &GPIO_InitStruct);

	//ADC DMA INIT
	AdcDmaInit();

	//ADC NVIC configure
	NVIC_SetPriority(ADC1_COMP_IRQn, 0);
	NVIC_EnableIRQ(ADC1_COMP_IRQn);
	// Configure Regular Channel
	LL_ADC_REG_SetSequencerChAdd(ADC1, LL_ADC_CHANNEL_1);
	LL_ADC_REG_SetSequencerChAdd(ADC1, LL_ADC_CHANNEL_TEMPSENSOR);
	LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_TEMPSENSOR);
	LL_ADC_REG_SetSequencerChAdd(ADC1, LL_ADC_CHANNEL_VREFINT);
	LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT);
	// Common config
	ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
	ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
	ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
	ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
	LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

	LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_160CYCLES_5);
	LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);
	LL_ADC_REG_SetSequencerScanDirection(ADC1, LL_ADC_REG_SEQ_SCAN_DIR_FORWARD);
	LL_ADC_SetCommonFrequencyMode(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_FREQ_MODE_LOW);
	LL_ADC_DisableIT_EOC(ADC1);
	LL_ADC_DisableIT_EOS(ADC1);
	ADC_InitStruct.Clock = LL_ADC_CLOCK_SYNC_PCLK_DIV1;
	ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
	ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
	ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
	LL_ADC_Init(ADC1, &ADC_InitStruct);
	// Enable ADC internal voltage regulator
	LL_ADC_EnableInternalRegulator(ADC1);
	/* Delay for ADC internal voltage regulator stabilization.
	Compute number of CPU cycles to wait for, from delay in us.
	Note: Variable divided by 2 to compensate partially
	CPU processing cycles (depends on compilation optimization).
	Note: If system core clock frequency is below 200kHz, wait time
	is only a few CPU processing cycles. */
	uint32_t wait_loop_index;
	wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
	while(wait_loop_index != 0)
	  wait_loop_index--;
	Calibrate_ADC();
}

/**
  * \brief	ADC DMA Initialization Function
  * \param	None
  * \return	None
  */
void AdcDmaInit(void){
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMA_REQUEST_0);
	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_LOW);
	LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR);
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_HALFWORD);
	LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_HALFWORD);
	LL_DMA_ConfigAddresses(	DMA1, LL_DMA_CHANNEL_1, LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
							(uint32_t)&aADCxConvertedData, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, ADC_CONVERTED_DATA_BUFFER_SIZE); 	// Set DMA transfer size
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1); 									// Enable DMA transfer interruption: transfer complete	
	LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);										// Enable DMA transfer interruption: transfer error
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1); 									// Enable the DMA transfer
}

void AdcGroupConversionCompleteCallback(void){
  	ubAdcGrpRegularSequenceConvStatus = 1;
  	ubAdcGrpRegularSequenceConvCount++;
}

void AdcGroupConversionErrorCallback(void){
	LL_ADC_DisableIT_OVR(ADC1);
}

void AdcDmaTransferCompleteCallback(void){
	ubDmaTransferStatus = 1;	// Update status variable of DMA transfer
	if (ubAdcGrpRegularSequenceConvStatus != 1){
		AdcDmaTransferErrorCallback();
	}
	ubAdcGrpRegularSequenceConvStatus = 0;	// Reset status variable of ADC group regular sequence
}

void AdcDmaTransferErrorCallback(void){
	;
}

void Calibrate_ADC(void)
{
	volatile uint32_t wait_loop_index = 0;								//auxiliary variable for delay count in function
	volatile uint32_t backup_setting_adc_dma_transfer = 0;
	uint32_t Timeout = 0; 												//auxiliary variable for timeout management
    backup_setting_adc_dma_transfer = LL_ADC_REG_GetDMATransfer(ADC1);	//make backup for ADC self-calibration time
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);		//Disable ADC-DMA transfer request during calibration
    LL_ADC_StartCalibration(ADC1);										//Start ADC self-calibration
	Timeout = ADC_CALIBRATION_TIMEOUT_MS;								//Calibration Timeout
	while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0){
		if (LL_SYSTICK_IsActiveCounterFlag()){	//if SysTick flag active - > TimeOut decrement.
			if(Timeout-- == 0){
				/*Time-out occurred.*/
			}
		}
	}

	/* Delay between ADC end of calibration and ADC enable.                   */
	/* Note: Variable divided by 2 to compensate partially                    */
	/*       CPU processing cycles (depends on compilation optimization).     */
	wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
	while(wait_loop_index != 0)	{
		wait_loop_index--;
	}

	/* Delay for ADC temperature sensor stabilization time.                   */
	/* Compute number of CPU cycles to wait for, from delay in us.            */
	/* Note: Variable divided by 2 to compensate partially                    */
	/*       CPU processing cycles (depends on compilation optimization).     */
	wait_loop_index = ((LL_ADC_DELAY_TEMPSENSOR_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
	while(wait_loop_index != 0)	{
	  wait_loop_index--;
	}

	LL_ADC_REG_SetDMATransfer(ADC1, backup_setting_adc_dma_transfer);	//Restore ADC DMA transfer request after calibration
	LL_ADC_Enable(ADC1);												//Enable ADC

	/* Poll for ADC ready to convert */
	Timeout = ADC_ENABLE_TIMEOUT_MS;
	while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0){
		/* Check Systick counter flag to decrement the time-out value */
		if (LL_SYSTICK_IsActiveCounterFlag()){
			if(Timeout-- == 0){
				/* Time-out occurred */
			}
		}
	}
	/* Note: ADC flag ADRDY is not cleared here to be able to check ADC       */
	/*       status afterwards.                                               */
	/*       This flag should be cleared at ADC Deactivation, before a new    */
	/*       ADC activation, using function "LL_ADC_ClearFlag_ADRDY()".       */
}

/****END OF FILE****/