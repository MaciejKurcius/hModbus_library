/**
 *  \file 		adc.h
 *  \brief		ADC function
 *  \details	File contain function to handling ADC function in coprocessor
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C" {
#endif
/* INCLUDES */

#include <main.h>

/* DEFINES */

#define TEMP_SENSOR_Pin			LL_GPIO_PIN_1
#define TEMP_SENSOR_GPIO_Port 	GPIOA
/* Value of analog reference voltage (Vref+), connected to analog voltage   */
/* supply Vdda (unit: mV).                                                  */
#define VDDA_APPLI                       ((uint32_t)3300)
#define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)   3)	//Bat_TempSensor + TempSensor + VrefInt
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)
#define ADC_CALIBRATION_TIMEOUT_MS ((uint32_t) 1)
#define ADC_ENABLE_TIMEOUT_MS ((uint32_t) 1)
#define ADC_DISABLE_TIMEOUT_MS ((uint32_t) 1)
#define ADC_STOP_CONVERSION_TIMEOUT_MS ((uint32_t) 1)
#define ADC_CONVERSION_TIMEOUT_MS ((uint32_t) 500)
/* TYPE DEF ENUM */

/* STRUCTURES */

/* VARIABLES */


/* FUNCTIONS PROTOTYPES */

void BeginAdc(void);
void AdcInit(void);
void AdcDmaInit(void);
void Calibrate_ADC(void);
void ThermistorLoopHandler(void);
void AdcDmaTransferErrorCallback(void);
void AdcDmaTransferCompleteCallback(void);
void AdcGroupConversionCompleteCallback(void);
void AdcGroupConversionErrorCallback(void);

#ifdef __cplusplus
}
#endif

#endif

/****END OF FILE****/
