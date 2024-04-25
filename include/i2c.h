/**
 *  \file 		i2c.h
 *  \brief		I2C configuration file
 *  \details	It contain all I2C init and handling function
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */


#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* INCLUDES */

#include <main.h>

/* DEFINES */
#define RX_ERR				0xFF

/* TYPE DEF ENUM */

/* STRUCTURES */

/* VARIABLES */

extern uint8_t I2C_RXbuffer[2];

/* FUNCTIONS PROTOTYPES */

void I2cInit(void);
void I2cTimeoutTimerInit(void);
void I2CTimeoutStart(uint16_t);
void I2cTimeoutStop(void);
uint8_t I2cWriteByte(I2C_TypeDef *I2Cx, uint8_t DevAddr, uint8_t RegAddr, uint8_t RegVal, uint8_t Timeout);
uint8_t I2cReadByte(I2C_TypeDef *I2Cx, uint8_t DevAddr, uint8_t RegAddr, uint8_t* RegVal, uint8_t Timeout);
uint8_t I2cTimeoutExceedAction(I2C_TypeDef *I2Cx);
uint8_t I2cWriteReg(I2C_TypeDef *I2Cx, uint8_t DevAddr, uint8_t RegAddr, uint8_t* RegVal, uint8_t RegSize, uint8_t Timeout);
uint8_t I2cReadReg(I2C_TypeDef *I2Cx, uint8_t DevAddr, uint8_t RegAddr, uint8_t* RegVal, uint8_t RegSize, uint8_t Timeout);

#ifdef __cplusplus
}
#endif

#endif
/****END OF FILE****/
