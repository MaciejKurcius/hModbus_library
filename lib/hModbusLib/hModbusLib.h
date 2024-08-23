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
  uint8_t SlaveAddr;
  uint8_t Cmd;
  uint8_t Data[HMODBUS_RXTX_SIZE];
  uint8_t DataLength;
  uint16_t Crc;
}hModbusFrameTypeDef;

typedef struct{
    UART_HANDLE_TYPE            UartHandle;
    uint16_t                    rxIndex;  
    uint8_t                     rxBuf[HMODBUS_RXTX_SIZE];
    uint32_t                    rxTime;
    uint8_t                     txBusy;
    uint32_t                    RxTimeout; 
    uint32_t                    TxTimeout;
    hModbus16BitOrderTypeDef    byteOrder16;
    hModbus32BitOrderTypeDef    byteOrder32;
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



void hModbusSwapU16DataByteArray(uint8_t* Data, uint8_t DataLength, hModbus16BitOrderTypeDef BitOrder);
hModbusFrameTypeDef hModbusComposeFrame8(uint8_t Addr, uint8_t Cmd, uint8_t* Data, uint8_t DataLength);
hModbusFrameTypeDef hModbusComposeFrame16(uint8_t Addr, uint8_t Cmd, uint16_t* Data, uint8_t DataLength);
void hModbusSendFrame(hModbusTypeDef* Handle, hModbusFrameTypeDef Frame);
hModbusFrameTypeDef hModbusParseFrame(hModbusTypeDef* Handle);
bool hModbusCheckRxFrame(hModbusFrameTypeDef RxFrame, hModbusFrameTypeDef TxFrame);
bool hModbusCompareFrame(hModbusFrameTypeDef RxFrame, hModbusFrameTypeDef TxFrame);
void hModbusRxCallback(hModbusTypeDef* Handle);
uint16_t hModbusReveiceRawData(hModbusTypeDef* Handle);
bool hModbusSendRawData(hModbusTypeDef* Handle, uint8_t *data, uint16_t size);
bool hModbusInit(hModbusTypeDef* Handle, UART_HANDLE_TYPE Uart, uint32_t CtrlOutPin, GPIO_TYPE_DEF CtrlOutPort, uint32_t RxTimeout, uint32_t TxTimeout);
void hModbusSet16BitOrder(hModbusTypeDef* Handle, hModbus16BitOrderTypeDef hModbus16bitOrder);
void hModbusSet32BitOrder(hModbusTypeDef* Handle, hModbus32BitOrderTypeDef hModbus32bitOrder);
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