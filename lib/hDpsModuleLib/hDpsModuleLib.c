#include "hDpsModuleLib.h"

void hDpsModuleInit(hDpsModuleTypeDef* Handle, hModbusTypeDef* ModbusHandle, uint8_t DeviceAddr){
    Handle->ModbusHandle = ModbusHandle;
    Handle->Address = DeviceAddr;
}

bool hDpsModuleEnableOutput(hDpsModuleTypeDef* Handle){
    return hModbusWriteHoldingRegister16i(Handle->ModbusHandle, Handle->Address, 9, 1);
}

bool hDpsModuleDisableOutput(hDpsModuleTypeDef* Handle){
    return hModbusWriteHoldingRegister16i(Handle->ModbusHandle, Handle->Address, 9, 0);
}

bool hDpsmoduleSetVoltage(hDpsModuleTypeDef* Handle, uint16_t Voltage){
    return hModbusWriteHoldingRegister16i(Handle->ModbusHandle, Handle->Address, 0, Voltage);
}

bool hDpsModuleSetCurrent(hDpsModuleTypeDef* Handle, uint16_t Current){
    return hModbusWriteHoldingRegister16i(Handle->ModbusHandle, Handle->Address, 1, Current);
}

bool hDpsModuleSetVoltageCurrent(hDpsModuleTypeDef* Handle, uint16_t Voltage, uint16_t Current){
    uint16_t TxData[] = {Voltage, Current};
    return hModbusWriteHoldingRegisters16i(Handle->ModbusHandle, Handle->Address, 0, 2, TxData);
}

uint16_t hDpsModuleGetAccVoltage(hDpsModuleTypeDef* Handle){
    uint16_t RxData;
    hModbusReadHoldingRegister16i(Handle->ModbusHandle, Handle->Address, 2, &RxData);
    return RxData;
}

uint16_t hDpsModuleGetAccCurrent(hDpsModuleTypeDef* Handle){
    uint16_t RxData;
    hModbusReadHoldingRegister16i(Handle->ModbusHandle, Handle->Address, 3, &RxData);
    return RxData;
}