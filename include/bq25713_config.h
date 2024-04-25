/**
 *  \file 		bq25713_config.h
 *  \brief		Library for BQ25713 config.
 *  \details	It contain all BQ25713 configuration parameters.
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 *  \note
 */

#ifndef __BQ25713_CONFIG_H
#define __BQ25713_CONFIG_H

//write I2C which is configured in i2c.h file
#define BQ_I2C					I2C2
#define BQ_I2C_DEFAULT_ADDR		0xD4	// D6 (6B + 0 on the end) - BQ25713  D4 (6A + 0 on the end) - BQ25713B
#define BQ_I2C_B_VER_ADDR		0xD4
#define BQ_I2C_A_VER_ADDR		0xD6
#define BQ_B_VER_ID				0x008A
#define BQ_A_VER_ID				0x0088
#define BQ_I2C_TIMEOUT			5		// Unit - ms
#define BQ_DEBUG_MODE			1		// Enable - 1 / Disable - 0
#define ADC_CONV_MODE			1		// Continous - 1 / Single - 0
#define BQ_CHECK_ADDR_ATEMPTS	3
//Hardware aplication values
#define PSYS_RESISTOR			30000	//Ohms

//define BQ config register values
#define V_CHARGE_OPTION0_0		0x0E
#define V_CHARGE_OPTION0_1		0x02//0x02/0x22
#define V_CHARGE_CURRENT_0		0x00
#define V_CHARGE_CURRENT_1		0x0A
#define V_MAX_CHARGE_VOLTAGE_0	0x38
#define V_MAX_CHARGE_VOLTAGE_1	0x31
#define V_CHARGE_OPTION1_0		0x01
#define V_CHARGE_OPTION1_1		0x92//0x82 - PSYSY DISABLE /0x92 - PSYS ENABLE
#define V_CHARGE_OPTION2_0		0x7A
#define V_CHARGE_OPTION2_1		0x02
#define	V_CHARGE_OPTION3_0		0x14//0x14/0x10
#define V_CHARGE_OPTION3_1		0x00//0x80 - EnHiZ / 0x00 - DisHiZ
#define V_PROCHOT_OPTION0_0		0x64//0x64/0xF1
#define V_PROCHOT_OPTION0_1		0x9E//0x9E/0x4E
#define V_PROCHOT_OPTION1_0		0x01//0x00/0xFF
#define V_PROCHOT_OPTION1_1		0x6B//0x6B/0x01
#define V_ADC_OPTION_0			0xFF//0x7F
#define V_ADC_OPTION_1			0xFF
#define V_INPUT_VOLTAGE_0		0x00
#define V_INPUT_VOLTAGE_1		0x2C
#define V_MIN_SYSTEM_VOLTAGE_0	0x00
#define V_MIN_SYSTEM_VOLTAGE_1	0x24//0x2C - 11,2V; 0x24 - 9,2V
#define V_IIN_HOST_0			0x00
#define V_IIN_HOST_1			0x38



#endif
/****END OF FILE****/
