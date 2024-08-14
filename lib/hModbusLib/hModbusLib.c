#include "hModbusLib.h"


static const uint16_t hModbusCrc[] =
{
  0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
  0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
  0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
  0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
  0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
  0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
  0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
  0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
  0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
  0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
  0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
  0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
  0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
  0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
  0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
  0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
  0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
  0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
  0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
  0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
  0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
  0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
  0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
  0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
  0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
  0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
  0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
  0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
  0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
  0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
  0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
  0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 
};

uint16_t hModbusCrc16(const uint8_t *nData, uint16_t wLength)
{
  uint8_t nTemp;
  uint16_t wCRCWord = 0xFFFF;
  while (wLength--){
    nTemp = *nData++ ^ wCRCWord;
    wCRCWord >>= 8;
    wCRCWord  ^= hModbusCrc[nTemp];
  }
  return wCRCWord;
} 

void hModbusRxCallback(hModbusTypeDef Handle){
  if(hModbusGetUartRxneFlag(Handle)){
      if(Handle.rxIndex < HMODBUS_RX_SIZE - 1){
          Handle.rxBuf[Handle.rxIndex] = hModbusUsartRx8(Handle);      
          Handle.rxIndex++;
      }
      else{
          hModbusUsartRx8(Handle);
      } 
      Handle.rxTime = hModbusGetSystemClock();
  }
}

uint16_t hModbusReveiceRawData(hModbusTypeDef Handle){
  uint32_t startTime = hModbusGetSystemClock();
  while(1){
    hModbusDelay(1);  
    if(hModbusGetSystemClock() - startTime > Handle.RxTimeout)
      return 0;
    
    if(Handle.rxIndex > 0){
      if(hModbusGetUartIdleFlag(Handle))
        return Handle.rxIndex; 
    }

    else{
      hModbusClearUartIdleFlag(Handle);
    }
  }
}  

bool hModbusSendRawData(hModbusTypeDef Handle, uint8_t *Data, uint16_t size){
    while(Handle.txBusy == 1){
        hModbusDelay(1);
    }
    Handle.txBusy = 1;
    memset(Handle.rxBuf, 0, HMODBUS_RX_SIZE);
    Handle.rxIndex = 0;
    uint32_t startTime = hModbusGetSystemClock();
    
    hModbusSetGpioPin(Handle.CtrlOut);
    hModbusDelay(1);

    for (uint16_t i = 0; i < size; i++){
        while (!hModbusGetUartTxeFlag(Handle)){
            hModbusDelay(1);
            if(hModbusGetSystemClock() - startTime > Handle.TxTimeout){
                hModbusResetGpioPin(Handle.CtrlOut);
                Handle.txBusy = 0;
                return false;
            }   
        }
        hModbusClearUartTcFlag(Handle);
        hModbusUsartTx8(Handle, Data[i]);
    }

    while (!hModbusGetUartTcFlag(Handle)){
        if(hModbusGetSystemClock() - startTime > Handle.TxTimeout){
                hModbusResetGpioPin(Handle.CtrlOut);
                Handle.txBusy = 0;
                return false;
            }    
    }

    hModbusResetGpioPin(Handle.CtrlOut);
    Handle.txBusy = 0;
    return true;
}

bool hModbusInit(hModbusTypeDef Handle, UART_HANDLE_TYPE Uart, uint32_t CtrlOutPin, GPIO_TYPE_DEF CtrlOutPort, uint32_t RxTimeout, uint32_t TxTimeout){
    memset(&Handle, 0, sizeof(Handle));
    Handle.CtrlOut.Pin = CtrlOutPin;
    Handle.CtrlOut.Port = CtrlOutPort;
    hModbusResetGpioPin(Handle.CtrlOut);
    Handle.byteOrder16 = hModbus16BitOrder_AB;
    Handle.byteOrder32 = hModbus32BitOrder_ABCD;
    Handle.UartHandle = Uart;
    hModbusUsartInit(Handle);
    hModbusEnableRxneIt(Handle);
    Handle.RxTimeout = RxTimeout;
    Handle.TxTimeout = TxTimeout;
    return true;
}

void hModbusSet16BitOrder(hModbusTypeDef Handle, hModbus16BitOrderTypeDef hModbus16bitOrder){
    Handle.byteOrder16 = hModbus16bitOrder;
}

void hModbusSet32BitOrder(hModbusTypeDef Handle, hModbus32BitOrderTypeDef hModbus32bitOrder){
    Handle.byteOrder32 = hModbus32bitOrder;
}

/* READ COIL */

bool hModbusReadCoils(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t Startnumber, uint16_t Length, uint8_t *Data){
  uint8_t TxData[8];
  TxData[0] = SlaveAddress;
  TxData[1] = hModbusCmd_ReadCoilStatus;
  TxData[2] = (Startnumber & 0xFF00) >> 8;
  TxData[3] = (Startnumber & 0x00FF);
  TxData[4] = (Length & 0xFF00) >> 8;
  TxData[5] = (Length & 0x00FF);
  static uint16_t  Crc;
  Crc = hModbusCrc16(TxData, 6);
  TxData[6] = (Crc & 0x00FF);
  TxData[7] = (Crc & 0xFF00) >> 8;
  hModbusSendRawData(Handle, TxData, 8);
  uint16_t RecLen = hModbusReveiceRawData(Handle);
  if(RecLen == 0)
    return false;
  if(Handle.rxBuf[0] != SlaveAddress)
    return false;
  if(Handle.rxBuf[1] != hModbusCmd_ReadCoilStatus)
    return false;
  Crc = hModbusCrc16(Handle.rxBuf, Handle.rxBuf[2] + 3);
  if(((Crc & 0x00FF) != Handle.rxBuf[Handle.rxBuf[2] + 3]) || (((Crc & 0xFF00) >> 8) != Handle.rxBuf[Handle.rxBuf[2] + 4]))
    return false; 
  if(Data != NULL)
    memcpy(Data, &Handle.rxBuf[3], Handle.rxBuf[2]);   
  return true;
}

bool hModbusReadCoil(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t Number, uint8_t *Data){
  return hModbusReadCoils(Handle, SlaveAddress, Number, 1, Data);
}

/* READ DISCRETE INPUTS */

bool hModbusReadDiscreteInputs(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint8_t *Data){
  uint8_t TxData[8];
  TxData[0] = SlaveAddress;
  TxData[1] = hModbusCmd_ReadDiscreteInputs;
  TxData[2] = (StartNumber & 0xFF00) >> 8;
  TxData[3] = (StartNumber & 0x00FF);
  TxData[4] = (Length & 0xFF00) >> 8;
  TxData[5] = (Length & 0x00FF);
  static uint16_t  Crc;
  Crc = hModbusCrc16(TxData, 6);
  TxData[6] = (Crc & 0x00FF);
  TxData[7] = (Crc & 0xFF00) >> 8;
  hModbusSendRawData(Handle, TxData, 8);
  uint16_t RecLen = hModbusReveiceRawData(Handle);
  if(RecLen == 0)
    return false;
  if(Handle.rxBuf[0] != SlaveAddress)
    return false;
  if(Handle.rxBuf[1] != hModbusCmd_ReadDiscreteInputs)
    return false;
  Crc = hModbusCrc16(Handle.rxBuf, Handle.rxBuf[2] + 3);
  if(((Crc & 0x00FF) != Handle.rxBuf[Handle.rxBuf[2] + 3]) || (((Crc & 0xFF00) >> 8) != Handle.rxBuf[Handle.rxBuf[2] + 4]))
    return false; 
  if(Data != NULL)
    memcpy(Data, &Handle.rxBuf[3], Handle.rxBuf[2]);   
  return true;
}

bool hModbusReadDiscreteInput(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t Number, uint8_t *Data)
{
  return hModbusReadDiscreteInputs(Handle, SlaveAddress, Number, 1, Data); 
}

/* READ INPUT REGISTERS */

/* Read input reg 8 bit */

bool hModbusReadInputRegisters8i(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint8_t *Data){
  uint8_t TxData[8];
  TxData[0] = SlaveAddress;
  TxData[1] = hModbusCmd_ReadInputRegisters;
  TxData[2] = (StartNumber & 0xFF00) >> 8;
  TxData[3] = (StartNumber & 0x00FF);
  TxData[4] = (Length & 0xFF00) >> 8;
  TxData[5] = (Length & 0x00FF);
  static uint16_t  Crc;
  Crc = hModbusCrc16(TxData, 6);
  TxData[6] = (Crc & 0x00FF);
  TxData[7] = (Crc & 0xFF00) >> 8;
  hModbusSendRawData(Handle, TxData, 8);
  uint16_t RecLen = hModbusReveiceRawData(Handle);
  if(RecLen == 0)
    return false;
  if(Handle.rxBuf[0] != SlaveAddress)
    return false;
  if(Handle.rxBuf[1] != hModbusCmd_ReadInputRegisters)
    return false;
  Crc = hModbusCrc16(Handle.rxBuf, Handle.rxBuf[2] + 3);
  if(((Crc & 0x00FF) != Handle.rxBuf[Handle.rxBuf[2] + 3]) || (((Crc & 0xFF00) >> 8) != Handle.rxBuf[Handle.rxBuf[2] + 4]))
    return false;  
  if(Data != NULL)
    memcpy(Data, &Handle.rxBuf[3], Handle.rxBuf[2]);   
  return true;
}

/* Read input reg 16 bit */

bool hModbusReadInputRegisters16i(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint16_t *Data){
  bool Ret = hModbusReadInputRegisters8i(Handle, SlaveAddress, StartNumber, Length * 1, (uint8_t*)Data);
  if(Ret == true)
  {
    uint8_t Tmp1[2],Tmp2[2];
    for(uint16_t i=0 ; i<Length ; i++)
    {   
      switch(Handle.byteOrder16)
      {
        case hModbus16BitOrder_AB:
          memcpy(Tmp1, &Data[i], 2);       
          Tmp2[0] = Tmp1[0];
          Tmp2[1] = Tmp1[1];
          memcpy(&Data[i], Tmp2, 2);    
        break;
        default:
          memcpy(Tmp1, &Data[i], 2);       
          Tmp2[0] = Tmp1[1];
          Tmp2[1] = Tmp1[0];
          memcpy(&Data[i], Tmp2, 2);    
        break;
      }
    }
    return true;
  }
  else
    return false;
}

bool hModbusReadInputRegister16i(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t Number, uint16_t *Data){
  return hModbusReadInputRegisters16i(Handle, SlaveAddress, Number, 1, Data); 
}

/* Read input reg 32 bit */

bool hModbusReadInputRegisters32f(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, float *Data){
  bool ret = hModbusReadInputRegisters8i(Handle, SlaveAddress, StartNumber, Length * 2, (uint8_t*)Data);
  if(ret == true)
  {
    for(uint16_t i=0 ; i<Length ; i++)
    {  
      uint8_t Tmp1[4],Tmp2[4];
      switch(Handle.byteOrder32)
      {
        case hModbus32BitOrder_DCBA:
          memcpy(Tmp1, &Data[i], 4);       
          Tmp2[0] = Tmp1[3];
          Tmp2[1] = Tmp1[2];
          Tmp2[2] = Tmp1[1];
          Tmp2[3] = Tmp1[0];
          memcpy(&Data[i], Tmp2, 4);    
        break;
        case hModbus32BitOrder_BADC:
          memcpy(Tmp1, &Data[i], 4);       
          Tmp2[0] = Tmp1[1];
          Tmp2[1] = Tmp1[0];
          Tmp2[2] = Tmp1[3];
          Tmp2[3] = Tmp1[2];
          memcpy(&Data[i], Tmp2, 4);           
        break;
        case hModbus32BitOrder_CDAB:
          memcpy(Tmp1, &Data[i], 4);       
          Tmp2[0] = Tmp1[2];
          Tmp2[1] = Tmp1[3];
          Tmp2[2] = Tmp1[0];
          Tmp2[3] = Tmp1[1];
          memcpy(&Data[i], Tmp2, 4);    
        break;
        default:
          
        break;
      }        
    }
    return true;
  }
  else
    return false;
}

bool hModbusReadInputRegister32f(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t Number, float *Data){
  return hModbusReadInputRegisters32f(Handle, SlaveAddress, Number, 1, Data); 
}

bool hModbusReadInputRegisters32i(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint32_t *Data){
  return hModbusReadInputRegisters32f(Handle, SlaveAddress, StartNumber, Length, (float*)Data);
}

bool hModbusReadInputRegister32i(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t Number, uint32_t *Data){
  return hModbusReadInputRegisters32i(Handle, SlaveAddress, Number, 1, Data); 
}

/* READ HOLDING REGISTERS */

/* Read holding reg 8 bit */

bool hModbusReadHoldingRegisters8i(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint8_t *Data){
  uint8_t TxData[8];
  TxData[0] = SlaveAddress;
  TxData[1] = hModbusCmd_ReadHoldingRegisters;
  TxData[2] = (StartNumber & 0xFF00) >> 8;
  TxData[3] = (StartNumber & 0x00FF);
  TxData[4] = (Length & 0xFF00) >> 8;
  TxData[5] = (Length & 0x00FF);
  static uint16_t  Crc;
  Crc = hModbusCrc16(TxData, 6);
  TxData[6] = (Crc & 0x00FF);
  TxData[7] = (Crc & 0xFF00) >> 8;
  hModbusSendRawData(Handle, TxData, 8);
  uint16_t RecLen = hModbusReveiceRawData(Handle);
  if(RecLen == 0)
    return false;
  if(Handle.rxBuf[0] != SlaveAddress)
    return false;
  if(Handle.rxBuf[1] != hModbusCmd_ReadHoldingRegisters)
    return false;
  Crc = hModbusCrc16(Handle.rxBuf, Handle.rxBuf[2] + 3);
  if(((Crc & 0x00FF) != Handle.rxBuf[Handle.rxBuf[2] + 3]) || (((Crc & 0xFF00) >> 8) != Handle.rxBuf[Handle.rxBuf[2] + 4]))
    return false; 
  if(Data != NULL)
  {
    for(uint8_t i=0 ; i<Handle.rxBuf[2] ; i+=2)
    {    
      uint8_t H = Handle.rxBuf[i+3];
      Handle.rxBuf[i+3] = Handle.rxBuf[i+3+1];
      Handle.rxBuf[i+3+1] = H;      
    }
    memcpy(Data, &Handle.rxBuf[3], Handle.rxBuf[2]);   
  }
  return true;
}

/* Read holding reg 16 bit */

bool hModbusReadHoldingRegisters16i(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint16_t *Data){
  bool Ret = hModbusReadHoldingRegisters8i(Handle, SlaveAddress, StartNumber, Length * 1, (uint8_t*)Data);
  if(Ret == true)
  {
    uint8_t Tmp1[2],Tmp2[2];
    for(uint16_t i=0 ; i<Length ; i++)
    {   
      switch(Handle.byteOrder16)
      {
        case hModbus16BitOrder_AB:
          memcpy(Tmp1, &Data[i], 2);       
          Tmp2[0] = Tmp1[0];
          Tmp2[1] = Tmp1[1];
          memcpy(&Data[i], Tmp2, 2);    
        break;
        default:
          memcpy(Tmp1, &Data[i], 2);       
          Tmp2[0] = Tmp1[1];
          Tmp2[1] = Tmp1[0];
          memcpy(&Data[i], Tmp2, 2);    
        break;
      }
    }
    return true;
  }
  else
    return false;
}

bool hModbusReadHoldingRegister16i(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t Number, uint16_t *Data){
  return hModbusReadHoldingRegisters16i(Handle, SlaveAddress, Number, 1, Data); 
}

/* Read holding reg 32bit */

bool hModbusReadHoldingRegisters32f(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, float *Data){
  bool Ret = hModbusReadHoldingRegisters8i(Handle, SlaveAddress, StartNumber, Length * 2, (uint8_t*)Data);
  if(Ret == true)
  {
    for(uint16_t i=0 ; i<Length ; i++)
    {  
      uint8_t Tmp1[4],Tmp2[4];
      switch(Handle.byteOrder32)
      {
        case hModbus32BitOrder_DCBA:
          memcpy(Tmp1, &Data[i], 4);       
          Tmp2[0] = Tmp1[3];
          Tmp2[1] = Tmp1[2];
          Tmp2[2] = Tmp1[1];
          Tmp2[3] = Tmp1[0];
          memcpy(&Data[i], Tmp2, 4);    
        break;
        case hModbus32BitOrder_BADC:
          memcpy(Tmp1, &Data[i], 4);       
          Tmp2[0] = Tmp1[1];
          Tmp2[1] = Tmp1[0];
          Tmp2[2] = Tmp1[3];
          Tmp2[3] = Tmp1[2];
          memcpy(&Data[i], Tmp2, 4);           
        break;
        case hModbus32BitOrder_CDAB:
          memcpy(Tmp1, &Data[i], 4);       
          Tmp2[0] = Tmp1[2];
          Tmp2[1] = Tmp1[3];
          Tmp2[2] = Tmp1[0];
          Tmp2[3] = Tmp1[1];
          memcpy(&Data[i], Tmp2, 4);    
        break;
        default:
        break;
      }        
    }
    return true;
  }
  else
    return false;
}

bool hModbusReadHoldingRegister32f(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t Number, float *Data){
  return hModbusReadHoldingRegisters32f(Handle, SlaveAddress, Number, 1, Data); 
}

bool hModbusReadHoldingRegisters32i(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint32_t *Data){
  return hModbusReadHoldingRegisters32f(Handle, SlaveAddress, StartNumber, Length, (float*)Data);
}

bool hModbusReadHoldingRegister32i(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t Number, uint32_t *Data){
  return hModbusReadHoldingRegisters32i(Handle, SlaveAddress, Number, 1, Data); 
}

/* WRITE COIL */

bool hModbusWriteCoil(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t Number, uint8_t Data){
  uint8_t TxData[8];
  TxData[0] = SlaveAddress;
  TxData[1] = hModbusCmd_WriteSingleCoil;
  TxData[2] = (Number & 0xFF00) >> 8;
  TxData[3] = (Number & 0x00FF);
  if(Data == 0)
    TxData[4] = 0;
  else
    TxData[4] = 0xFF;
  TxData[5] = 0;
  static uint16_t  Crc;
  Crc = hModbusCrc16(TxData, 6);
  TxData[6] = (Crc & 0x00FF);
  TxData[7] = (Crc & 0xFF00) >> 8;
  hModbusSendRawData(Handle, TxData, 8);
  uint16_t RecLen = hModbusReveiceRawData(Handle);
  if(RecLen == 0)
    return false;
  if(memcmp(TxData, Handle.rxBuf, 8) == 0)
    return true;
  else
    return false;
}

/* WRITE HODLING REGISTERS */

bool hModbusWriteHoldingRegister16i(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t Number, uint16_t Data){
  uint8_t TxData[8];
  TxData[0] = SlaveAddress;
  TxData[1] = hModbusCmd_WriteMultipleRegisters;
  TxData[2] = (Number & 0xFF00) >> 8;
  TxData[3] = (Number & 0x00FF);
  TxData[4] = (Data & 0xFF00) >> 8;
  TxData[5] = Data & 0x00FF;
  static uint16_t  Crc;
  Crc = hModbusCrc16(TxData, 6);
  TxData[6] = (Crc & 0x00FF);
  TxData[7] = (Crc & 0xFF00) >> 8;
  hModbusSendRawData(Handle, TxData, 8);
  uint16_t RecLen = hModbusReveiceRawData(Handle);
  if(RecLen == 0)
    return false;
  if(memcmp(TxData, Handle.rxBuf, 8) == 0)
    return true;
  else
    return false;
}

bool hModbusWriteHoldingRegisters16i(hModbusTypeDef Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint16_t *Data){
	if (Length==1){
		return hModbusWriteHoldingRegister16i(Handle, SlaveAddress, StartNumber, Data[0]);
	}
	else{
	  uint8_t TxData[7 + Length * 2 + 2];
	  TxData[0] = SlaveAddress;
	  TxData[1] = hModbusCmd_WriteMultipleRegisters;
	  TxData[2] = (StartNumber & 0xFF00) >> 8;
	  TxData[3] = (StartNumber & 0x00FF);
	  TxData[4] = (Length & 0xFF00) >> 8;
	  TxData[5] = (Length & 0x00FF);
	  TxData[6] = (Length * 2);
	  uint8_t Tmp1[2],Tmp2[2];
	  for(uint16_t i=0 ; i<Length ; i++){   
      switch(Handle.byteOrder16){
        case hModbus16BitOrder_AB:
          memcpy(Tmp1, &Data[i], 2);       
          Tmp2[0] = Tmp1[1];
          Tmp2[1] = Tmp1[0];
          memcpy(&TxData[7 + i * 2], Tmp2, 2);    
          break;
        default:
          memcpy(Tmp1, &Data[i], 2);       
          Tmp2[0] = Tmp1[0];
          Tmp2[1] = Tmp1[1];
          memcpy(&TxData[7 + i * 2], Tmp2, 2);    
          break;
      }
	  }    
	  static uint16_t Crc;
	  Crc = hModbusCrc16(TxData, 7 + Length * 2);
	  TxData[7 + Length * 2 + 0] = (Crc & 0x00FF);
	  TxData[7 + Length * 2 + 1] = (Crc & 0xFF00) >> 8;
	  hModbusSendRawData(Handle, TxData, 7 + Length * 2 + 2);
	  uint16_t RecLen = hModbusReveiceRawData(Handle);
	  if(RecLen == 0)
      return false;
	  Crc = hModbusCrc16(TxData, 6);
	  TxData[6] = (Crc & 0x00FF);
	  TxData[7] = (Crc & 0xFF00) >> 8;  
	  if(memcmp(TxData, Handle.rxBuf, 8) == 0)
      return true;
	  else
      return false;
	}
}