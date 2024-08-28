#ifndef hMODBUS_LIB_H
#define hMODBUS_LIB_H

#include "hModbusLibConf.h"
#include <stdbool.h>
#include  <string.h>

#define HMODBUS_FRAME_LEN(arg) (1+1+(arg)+2)

typedef enum
{
  hModbusCmd_ReadCoilStatus = 1,
  hModbusCmd_ReadDiscreteInputs = 2,
  hModbusCmd_ReadHoldingRegisters = 3,
  hModbusCmd_ReadInputRegisters = 4,
  hModbusCmd_WriteSingleCoil = 5,
  hModbusCmd_WriteSingleRegister = 6,
  hModbusCmd_WriteMultipleCoils = 15,
  hModbusCmd_WriteMultipleRegisters = 16
  
}hModubsCmdTypeDef;

typedef enum{
  hModbusMaster = 0,
  hModbusSlave = 1
}hModbusTypeTypeDef;

typedef enum
{
  hModbus16BitOrder_AB = 0,
  hModbus16BitOrder_BA,
  
}hModbus16BitOrderTypeDef;

typedef enum
{
  hModbus32BitOrder_ABCD = 0,
  hModbus32BitOrder_DCBA,
  hModbus32BitOrder_BADC,
  hModbus32BitOrder_CDAB,  
  
}hModbus32BitOrderTypeDef;

typedef struct{
  uint32_t Pin;
  GPIO_TYPE_DEF Port;
}hModbusCtrlOutTypeDef;

typedef struct{
  uint8_t DeviceAddr;
  uint8_t Cmd;
  uint8_t Data[HMODBUS_RXTX_SIZE];
  uint8_t DataLength;
  uint16_t Crc;
}hModbusFrameTypeDef;

typedef struct{
  uint8_t CoilsReg[HMODBUS_SLAVE_COIL_REG_SIZE];
  uint8_t DigitalInputsReg[HMODBUS_SLAVE_DIGITAL_INPUTS_REG_SIZE];
  uint16_t HoldingReg[HMODBUS_SLAVE_HOLDING_REG_SIZE];
  uint16_t InputsReg[HMODBUS_SLAVE_INPUT_REG_SIZE];
}hModbusSlaveDataTypeDef;

typedef struct{
    UART_HANDLE_TYPE            UartHandle;
    hModbusTypeTypeDef          Type;
    hModbusSlaveDataTypeDef*    Data;
    // RX
    uint16_t                    RxIndex;  
    uint8_t                     RxBuf[HMODBUS_RXTX_SIZE];
    uint32_t                    RxTime;
    uint32_t                    RxTimeout; 
    // TX
    uint8_t                     TxBusy;
    uint32_t                    TxTimeout;
    hModbus16BitOrderTypeDef    ByteOrder16;
    hModbus32BitOrderTypeDef    ByteOrder32;
    hModbusCtrlOutTypeDef       CtrlOut;
}hModbusTypeDef;

extern void hModbusDelay(uint32_t);
extern uint32_t hModbusGetSystemClock(void);
extern void hModbusSetGpioPin(hModbusCtrlOutTypeDef Output);
extern void hModbusResetGpioPin(hModbusCtrlOutTypeDef Output);
extern void hModbusUsartTx8(hModbusTypeDef* Handle, uint8_t TxData);
extern uint8_t hModbusUsartRx8(hModbusTypeDef* Handle);
extern uint32_t hModbusGetUartIdleFlag(hModbusTypeDef* Handle);
extern void hModbusClearUartIdleFlag(hModbusTypeDef* Handle);
extern uint32_t hModbusGetUartTxeFlag(hModbusTypeDef* Handle);
extern uint32_t hModbusGetUartTcFlag(hModbusTypeDef* Handle);
extern void hModbusClearUartTcFlag(hModbusTypeDef* Handle);
extern void hModbusUsartInit(hModbusTypeDef* Handle);
extern uint32_t hModbusGetUartRxneFlag(hModbusTypeDef* Handle);
extern void hModbusEnableRxneIt(hModbusTypeDef* Handle);


// Auxliary functions
void hModbusSwapU16DataByteArray(uint8_t* Data, uint8_t DataLength, hModbus16BitOrderTypeDef BitOrder);
uint16_t hModbusU8ToU16(uint8_t* Data, hModbus16BitOrderTypeDef BitOrder);
// Frames handling
hModbusFrameTypeDef hModbusComposeFrame8(uint8_t Addr, uint8_t Cmd, uint8_t* Data, uint8_t DataLength);
hModbusFrameTypeDef hModbusComposeFrame16(uint8_t Addr, uint8_t Cmd, uint16_t* Data, uint8_t DataLength);
void hModbusSendFrame(hModbusTypeDef* Handle, hModbusFrameTypeDef Frame);
hModbusFrameTypeDef hModbusParseFrame(hModbusTypeDef* Handle);
bool hModbusCheckRxFrame(hModbusFrameTypeDef RxFrame, hModbusFrameTypeDef TxFrame);
bool hModbusCompareFrame(hModbusFrameTypeDef RxFrame, hModbusFrameTypeDef TxFrame);
bool hModbusRxFrameExecute(hModbusTypeDef* Handle, hModbusFrameTypeDef RxFrame);
// Callbacks
void hModbusRxCallback(hModbusTypeDef* Handle);
// Raw Data
uint16_t hModbusReveiceRawData(hModbusTypeDef* Handle);
bool hModbusSendRawData(hModbusTypeDef* Handle, uint8_t *data, uint16_t size);
// Config functions
bool hModbusInit(hModbusTypeDef* Handle, UART_HANDLE_TYPE Uart, hModbusTypeTypeDef Type);
void hModbusCtrlOutConfig(hModbusTypeDef* Handle, uint32_t CtrlOutPin, GPIO_TYPE_DEF CtrlOutPort);
void hModbusSetRxTimeout(hModbusTypeDef* Handle, uint32_t RxTimeout);
void hModbusSetTxTimeout(hModbusTypeDef* Handle, uint32_t TxTimeout);
void hModbusSet16BitOrder(hModbusTypeDef* Handle, hModbus16BitOrderTypeDef hModbus16bitOrder);
void hModbusSet32BitOrder(hModbusTypeDef* Handle, hModbus32BitOrderTypeDef hModbus32bitOrder);
void hModbusInitSlaveData(hModbusTypeDef* Handle, hModbusSlaveDataTypeDef* SlaveData);
// void hModbusInitSlaveData(uint8_t* CoilsReg, uint16_t CoilsRegSize, uint8_t* DigitalInputsReg, uint16_t DigitalInputsRegSize, uint16_t* HoldingReg, uint16_t HoldingRegSize, uint16_t* InputReg, uint16_t InputRegSize);
// Modbus functions
bool hModbusReadCoils(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Startnumber, uint16_t Length, uint8_t *Data);
bool hModbusReadCoil(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint8_t *Data);
bool hModbusReadDiscreteInputs(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint8_t *Data);
bool hModbusReadDiscreteInput(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint8_t *Data);
bool hModbusReadInputRegisters8i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint8_t *Data);
bool hModbusReadInputRegisters16i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint16_t *Data);
bool hModbusReadInputRegister16i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint16_t *Data);
bool hModbusReadInputRegisters32f(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, float *Data);
bool hModbusReadInputRegister32f(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, float *Data);
bool hModbusReadInputRegisters32i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint32_t *Data);
bool hModbusReadInputRegister32i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint32_t *Data);
bool hModbusReadHoldingRegisters8i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint8_t *Data);
bool hModbusReadHoldingRegisters16i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint16_t *Data);
bool hModbusReadHoldingRegister16i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint16_t *Data);
bool hModbusReadHoldingRegisters32f(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, float *Data);
bool hModbusReadHoldingRegister32f(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, float *Data);
bool hModbusReadHoldingRegisters32i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint32_t *Data);
bool hModbusReadHoldingRegister32i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint32_t *Data);
bool hModbusWriteCoil(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint16_t Data);
bool hModbusWriteHoldingRegister16i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint16_t Data);
bool hModbusWriteHoldingRegisters16i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint16_t *Data);

#endif