/**
 *  \file 		eeprom.h
 *  \brief		Library for EEPROM handling.
 *  \details	
 *  \author		Maciej Kurcius
 *  \version	0.1
 *  \date		05.07.2023
 *  \note
 */

#ifndef EEPROM_H
#define EEPROM_D

#include <i2c.h>
#include <string.h>

#define EEPROM_I2C          I2C2
#define EEPROM_DEV_ID       0xA0
#define EEPROM_I2C_TIMEOUT      2
#define EEPROM_CONTROL_BYTE(DevId, BlockAddr) (DevId | BlockAddr)

uint8_t EepromWriteByte(uint8_t BlockAddr, uint8_t ByteAddr, uint8_t Value);
uint8_t EepromReadByte(uint8_t BlockAddr, uint8_t ByteAddr, uint8_t *Value);
uint8_t EepromWritePage(uint8_t BlockAddr, uint8_t PageStartAddr, uint8_t* PageValue, uint8_t PageSize);
uint8_t EepromReadPage(uint8_t BlockAddr, uint8_t PageStartAddr, uint8_t* PageValue, uint8_t PageSize);
char* GetBoardVersion(void);
uint8_t WriteBoardVersion(void);

// memory organization

#define EEPROM_BLOCK_ADDR_0     (0x00 << 1)
#define EEPROM_BLOCK_ADDR_1     (0x01 << 1)
#define EEPROM_BLOCK_ADDR_2     (0x02 << 1)
#define EEPROM_BLOCK_ADDR_3     (0x03 << 1)
#define EEPROM_BLOCK_ADDR_4     (0x04 << 1)
#define EEPROM_BLOCK_ADDR_5     (0x05 << 1)
#define EEPROM_BLOCK_ADDR_6     (0x06 << 1)
#define EEPROM_BLOCK_ADDR_7     (0x07 << 1)

#endif