/**
 *  \file 		eeprom.c
 *  \brief		Library for EEPROM handling.
 *  \details	
 *  \author		Maciej Kurcius
 *  \version	0.1
 *  \date		05.07.2023
 *  \note
 */


#include <eeprom.h>


uint8_t EepromWriteByte(uint8_t BlockAddr, uint8_t ByteAddr, uint8_t Value){
    return I2cWriteByte(EEPROM_I2C, EEPROM_CONTROL_BYTE(EEPROM_DEV_ID, BlockAddr), ByteAddr, Value, EEPROM_I2C_TIMEOUT);
}

/**
 * @fn uint8_t EepromReadByte(uint8_t BlockAddr, uint8_t ByteAddr, uint8_t *Value)
 * @brief This function cannot be used after an EepromWritePage() or EepromWriteByte() function before less than 4ms.
*/
uint8_t EepromReadByte(uint8_t BlockAddr, uint8_t ByteAddr, uint8_t *Value){
    return I2cReadByte(EEPROM_I2C, EEPROM_CONTROL_BYTE(EEPROM_DEV_ID, BlockAddr), ByteAddr, Value, EEPROM_I2C_TIMEOUT);
}

uint8_t EepromWritePage(uint8_t BlockAddr, uint8_t PageStartAddr, uint8_t* PageValue, uint8_t PageSize){
    return I2cWriteReg(EEPROM_I2C, EEPROM_CONTROL_BYTE(EEPROM_DEV_ID, BlockAddr), PageStartAddr, PageValue, PageSize, EEPROM_I2C_TIMEOUT);
}


/**
 * @fn EepromReadPage(uint8_t BlockAddr, uint8_t PageStartAddr, uint8_t* PageValue, uint8_t PageSize)
 * @brief This function cannot be used after an EepromWritePage() or EepromWriteByte() function before less than 4ms.
*/
uint8_t EepromReadPage(uint8_t BlockAddr, uint8_t PageStartAddr, uint8_t* PageValue, uint8_t PageSize){
    return I2cReadReg(EEPROM_I2C, EEPROM_CONTROL_BYTE(EEPROM_DEV_ID, BlockAddr), PageStartAddr, PageValue, PageSize, EEPROM_I2C_TIMEOUT);
}

char* GetBoardVersion(void){
    static char BoardVersion[BOARD_VER_MEM_SIZE+1] = "xx.x\n";
    if(strcmp(BoardVersion, "xx.x\n") == 0){
        for(uint8_t i = 0; i < BOARD_VER_READ_ATTEMPTS; i++){
            char RetVal[BOARD_VER_MEM_SIZE];
            EepromReadPage(BOARD_VER_MEM_BLOCK, BOARD_VER_MEM_ADDR, (uint8_t*) RetVal, BOARD_VER_MEM_SIZE);
            if(RetVal[0] == 'v' && RetVal[2] == '.'){
                memcpy(BoardVersion, RetVal, BOARD_VER_MEM_SIZE);
                return BoardVersion;
            }
            break;
        }
        memcpy(BoardVersion, "v1.2", BOARD_VER_MEM_SIZE);
        return BoardVersion;
    }
    return BoardVersion;
}

uint8_t WriteBoardVersion(void){
    uint8_t BoardVersion[BOARD_VER_MEM_SIZE+1] = BOARD_VERSION;
    return EepromWritePage(BOARD_VER_MEM_BLOCK, BOARD_VER_MEM_ADDR, BoardVersion, BOARD_VER_MEM_SIZE);
}