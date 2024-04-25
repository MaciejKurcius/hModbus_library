/**
 *  \file 		config.h
 *  \brief		Firmware config file
 *  \details	Contains all config defines and flag for firmware that can be changed by user.
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */



#ifndef __CONFIG_H
#define __CONFIG_H

#include <main.h>
/* ============================================= */


#define FIRMWARE_VERSION    "v1.0.1"
#define SBC_POWER_OFF_PBLED_BLINK_DELAY 500
#define SBC_POWER_OFF_DELAY 60000
#define POWER_ON_DELAY 	1500
#define POWER_OFF_DELAY 500
#define MAIN_LOOP_DELAY	750

/* INVERTERS AND POWER SWITCH DATA */

#define VS_SW_CH1_SLEW_RATE		32	//ms
#define VS_SW_CH2_SLEW_RATE		13	//ms
#define DC12V_INV_SLEW_RATE		16	//ms
#define DC5V_INV_SLEW_RATE		6	//ms
#define INV_TIMEOUT_OFFSET		5	//ms
#define VS_SW_DEFAULT_STATE     1

/* UART */
#define UART_BAUD_RATE			            38400
#define BATTERY_FULL_VOLTAGE_TRESHOLD	    12500	// [mV] <- charge current treshold. Below this value, PB signalized that battery is full
#define BATTERY_FULL_CURRENT_TRESHOLD	    300		// [mA] <- charge current treshold. Below this value, PB signalized that battery is full
#define BATTERY_DISCHARGED_VOLTAGE_TRESHOLD	9800	// [mV]
#define USBC_CHARGE_CURRENT		            2560	// 64-8128 [mA]
#define IND_ADPT_CHARGE_CURRENT	            1472	// 64-8128 [mA]
#define USBC_ADPT_MAX_CURRENT               2800    // 3250/2800 [mA] optional 4150 for 100W charger
#define IND_ADPT_MAX_CURRENT                1000    // 3250/2800 [mA] optional 4150 for 100W charger

/* THERMISTOR */
#define BATTERY_TEMP_OFFSET                 (-2)    //Celsius degree
#define BATTERY_MAX_TEMP                    55      //Celsius degree
#define BATTERY_DISCONNECTED_ADC_TRESHOLD   3200    //Battery thermistor ADC voltage in mV

/* ARM INVERTER */
#define ARM_BOARD_DEFAULT_STATE 1       // 1 - 'On' or 0 - 'Off'

/* BOARD VERSION */
#define BOARD_VER_MEM_BLOCK			0x00
#define BOARD_VER_MEM_ADDR			0x00
#define BOARD_VER_MEM_SIZE			0x04
#define BOARD_VER_READ_ATTEMPTS		5
#define BOARD_VERSION               "v1.3\n"

#endif
/****END OF FILE****/