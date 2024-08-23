#ifndef H_DPS_MODULE_LIB_H
#define H_DPS_MODULE_LIB_H

#include "hModbusLib.h"

typedef struct{
    uint8_t Address;
    hModbusTypeDef* ModbusHandle;
}hDpsModuleTypeDef;


void hDpsModuleInit(hDpsModuleTypeDef* Handle, hModbusTypeDef* ModbusHandle, uint8_t SlaveAddr);
bool hDpsModuleEnableOutput(hDpsModuleTypeDef* Handle);
bool hDpsModuleDisableOutput(hDpsModuleTypeDef* Handle);
bool hDpsmoduleSetVoltage(hDpsModuleTypeDef* Handle, uint16_t Voltage);
bool hDpsModuleSetCurrent(hDpsModuleTypeDef* Handle, uint16_t Current);
bool hDpsModuleSetVoltageCurrent(hDpsModuleTypeDef* Handle, uint16_t Voltage, uint16_t Current);
uint16_t hDpsModuleGetAccVoltage(hDpsModuleTypeDef* Handle);
uint16_t hDpsModuleGetAccCurrent(hDpsModuleTypeDef* Handle);

#endif