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

void hModbusSwapU16DataByteArray(uint8_t* Data, uint8_t DataLength, hModbus16BitOrderTypeDef BitOrder){
  uint8_t TempData[DataLength];
  switch (BitOrder)
  {
  case hModbus16BitOrder_BA:
    memcpy(TempData, Data, DataLength);
    for(uint8_t i=0; i<(DataLength/2); i++){
      Data[i*2+0] = TempData[i*2+1];
      Data[i*2+1] = TempData[i*2+0];
    }
    break;
  default:
    break;
  }
}

uint16_t hModbusU8ToU16(uint8_t* Data, hModbus16BitOrderTypeDef BitOrder){
  if(BitOrder == hModbus16BitOrder_AB)
    return (Data[0] | (Data[1] << 8));
  if(BitOrder == hModbus16BitOrder_BA)
    return (Data[1] | (Data[0] << 8));
  return 0;
}


hModbusFrameTypeDef hModbusComposeFrame8(uint8_t Addr, uint8_t Cmd, uint8_t* Data, uint8_t DataLength){
  hModbusFrameTypeDef Frame = {0};
  if(DataLength > HMODBUS_RXTX_SIZE)
    return Frame;
  Frame.DeviceAddr = Addr;
  Frame.Cmd = Cmd;
  Frame.DataLength = DataLength;
  memcpy(Frame.Data, Data, DataLength);
  return Frame;
}

hModbusFrameTypeDef hModbusComposeFrame16(uint8_t Addr, uint8_t Cmd, uint16_t* Data, uint8_t DataLength){
  uint8_t Data2Send[HMODBUS_RXTX_SIZE] = {0};
  for(uint8_t i=0; i<DataLength; i++){
    Data2Send[2*i] = (Data[i] & 0xFF00) >> 8;
    Data2Send[2*i+1] = Data[i] & 0x00FF;
  }
  return hModbusComposeFrame8(Addr, Cmd, Data2Send, DataLength*2);
}

void hModbusSendFrame(hModbusTypeDef* Handle, hModbusFrameTypeDef Frame){
  uint8_t TxData[HMODBUS_FRAME_LEN(HMODBUS_RXTX_SIZE)] = {0};
  TxData[0] = Frame.DeviceAddr;
  TxData[1] = Frame.Cmd;
  memcpy(&TxData[2], Frame.Data, Frame.DataLength);
  uint16_t Crc = hModbusCrc16(TxData, Frame.DataLength+2);
  TxData[1+1+Frame.DataLength+0] = (Crc & 0x00FF);
  TxData[1+1+Frame.DataLength+1] = (Crc & 0xFF00) >> 8;
  hModbusSendRawData(Handle, TxData, HMODBUS_FRAME_LEN(Frame.DataLength));
}

hModbusFrameTypeDef hModbusParseFrame(hModbusTypeDef* Handle){
  hModbusFrameTypeDef Frame = {0};

  if(Handle->Type == hModbusMaster){
    Frame.DeviceAddr = Handle->RxBuf[0];
    Frame.Cmd = Handle->RxBuf[1];

    switch(Frame.Cmd){
      case hModbusCmd_ReadCoilStatus:
        Frame.DataLength = Handle->RxBuf[2];
        memcpy(Frame.Data, &Handle->RxBuf[3], Frame.DataLength);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_ReadDiscreteInputs:
        Frame.DataLength = Handle->RxBuf[2];
        memcpy(Frame.Data, &Handle->RxBuf[3], Frame.DataLength);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_ReadHoldingRegisters:
        Frame.DataLength = Handle->RxBuf[2];
        memcpy(Frame.Data, &Handle->RxBuf[3], Frame.DataLength);
        hModbusSwapU16DataByteArray(Frame.Data, Frame.DataLength, hModbus16BitOrder_BA);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_ReadInputRegisters:
        Frame.DataLength = Handle->RxBuf[2];
        memcpy(Frame.Data, &Handle->RxBuf[3], Frame.DataLength);
        hModbusSwapU16DataByteArray(Frame.Data, Frame.DataLength, hModbus16BitOrder_BA);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_WriteSingleCoil:
        Frame.DataLength = 4;
        memcpy(Frame.Data, &Handle->RxBuf[2], Frame.DataLength);
        hModbusSwapU16DataByteArray(Frame.Data, Frame.DataLength, hModbus16BitOrder_BA);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_WriteSingleRegister:
        Frame.DataLength = 4;
        memcpy(Frame.Data, &Handle->RxBuf[2], Frame.DataLength);
        hModbusSwapU16DataByteArray(Frame.Data, Frame.DataLength, hModbus16BitOrder_BA);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_WriteMultipleCoils:
        Frame.DataLength = 4;
        memcpy(Frame.Data, &Handle->RxBuf[2], Frame.DataLength);
        hModbusSwapU16DataByteArray(Frame.Data, Frame.DataLength, hModbus16BitOrder_BA);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_WriteMultipleRegisters:
        Frame.DataLength = 4;
        memcpy(Frame.Data, &Handle->RxBuf[2], Frame.DataLength);
        hModbusSwapU16DataByteArray(Frame.Data, Frame.DataLength, hModbus16BitOrder_BA);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
    default:
      memset(&Frame, 0, sizeof(hModbusFrameTypeDef));
      break;
    }
  }

  if(Handle->Type == hModbusMaster){
    Frame.DeviceAddr = Handle->RxBuf[0];
    Frame.Cmd = Handle->RxBuf[1];

    switch(Frame.Cmd){
      case hModbusCmd_ReadCoilStatus: // ok
        Frame.DataLength = 4;
        memcpy(Frame.Data, &Handle->RxBuf[3], Frame.DataLength);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_ReadDiscreteInputs: // ok
        Frame.DataLength = 4;
        memcpy(Frame.Data, &Handle->RxBuf[3], Frame.DataLength);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_ReadHoldingRegisters: //ok
        Frame.DataLength = 4;
        memcpy(Frame.Data, &Handle->RxBuf[3], Frame.DataLength);
        hModbusSwapU16DataByteArray(Frame.Data, Frame.DataLength, hModbus16BitOrder_BA);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_ReadInputRegisters: //ok
        Frame.DataLength = 4;
        memcpy(Frame.Data, &Handle->RxBuf[3], Frame.DataLength);
        hModbusSwapU16DataByteArray(Frame.Data, Frame.DataLength, hModbus16BitOrder_BA);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_WriteSingleCoil: //ok
        Frame.DataLength = 4;
        memcpy(Frame.Data, &Handle->RxBuf[2], Frame.DataLength);
        hModbusSwapU16DataByteArray(Frame.Data, Frame.DataLength, hModbus16BitOrder_BA);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_WriteSingleRegister: //ok
        Frame.DataLength = 4;
        memcpy(Frame.Data, &Handle->RxBuf[2], Frame.DataLength);
        hModbusSwapU16DataByteArray(Frame.Data, Frame.DataLength, hModbus16BitOrder_BA);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_WriteMultipleCoils:
        Frame.DataLength = 4 + Handle->RxBuf[6];
        memcpy(Frame.Data, &Handle->RxBuf[2], 4);
        memcpy(&Frame.Data[4], &Handle->RxBuf[7], Frame.DataLength - 4);
        hModbusSwapU16DataByteArray(Frame.Data, Frame.DataLength, hModbus16BitOrder_BA);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
      case hModbusCmd_WriteMultipleRegisters:
        Frame.DataLength = 4 + Handle->RxBuf[6];
        memcpy(Frame.Data, &Handle->RxBuf[2], 4);
        memcpy(&Frame.Data[4], &Handle->RxBuf[7], Frame.DataLength - 4);
        hModbusSwapU16DataByteArray(Frame.Data, Frame.DataLength, hModbus16BitOrder_BA);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+0] & 0x00FF);
        Frame.Crc = (Handle->RxBuf[1+1+Frame.DataLength+1] & 0xFF00) >> 8;
        break;
    default:
      memset(&Frame, 0, sizeof(hModbusFrameTypeDef));
      break;
    }
  }
  return Frame;
}



bool hModbusCheckRxFrame(hModbusFrameTypeDef RxFrame, hModbusFrameTypeDef TxFrame){
  if(RxFrame.Cmd != TxFrame.Cmd) return false;
  if(RxFrame.DeviceAddr != TxFrame.DeviceAddr) return false;
  if(RxFrame.Crc != TxFrame.Crc) return false;
  return true;
}

bool hModbusCompareFrame(hModbusFrameTypeDef RxFrame, hModbusFrameTypeDef TxFrame){
  if(hModbusCheckRxFrame(RxFrame, TxFrame) == false) return false;
  if(RxFrame.DataLength != TxFrame.DataLength) return false;
  if(memcmp(RxFrame.Data, TxFrame.Data, RxFrame.DataLength)== 0) 
    return true;
  else 
    return false;
}

bool hModbusRxFrameExecute(hModbusTypeDef* Handle, hModbusFrameTypeDef RxFrame){
  if(RxFrame.Cmd == 0 || Handle->Type == hModbusMaster) 
    return false;
  
  // check CRC value of RX frame

  hModbusFrameTypeDef TxFrame = {0};

  if(RxFrame.Cmd == hModbusCmd_ReadCoilStatus){
    uint16_t TxRegLength = hModbusU8ToU16(&RxFrame.Data[5], hModbus16BitOrder_AB);
    uint16_t TxRegStart = hModbusU8ToU16(&RxFrame.Data[3], hModbus16BitOrder_AB);
    uint8_t TxData[TxRegLength+1];
    if(TxRegLength > HMODBUS_SLAVE_COIL_REG_SIZE)  
      return false;
    TxData[0] = TxRegLength;
    memcpy(&TxData[1], &Handle->Data->CoilsReg[TxRegStart], TxRegLength);
    TxFrame = hModbusComposeFrame8(RxFrame.DeviceAddr, RxFrame.Cmd, TxData, TxRegLength+1);
  }

  else if(RxFrame.Cmd == hModbusCmd_ReadDiscreteInputs){
    uint16_t TxRegLength = hModbusU8ToU16(&RxFrame.Data[5], hModbus16BitOrder_AB);
    uint16_t TxRegStart = hModbusU8ToU16(&RxFrame.Data[3], hModbus16BitOrder_AB);
    uint8_t TxData[TxRegLength+1];
    if(TxRegLength > HMODBUS_SLAVE_DIGITAL_INPUTS_REG_SIZE)  
      return false;
    TxData[0] = TxRegLength;
    memcpy(&TxData[1], &Handle->Data->DigitalInputsReg[TxRegStart], TxRegLength);
    TxFrame = hModbusComposeFrame8(RxFrame.DeviceAddr, RxFrame.Cmd, TxData, TxRegLength+1);
  }

  else if(RxFrame.Cmd == hModbusCmd_ReadHoldingRegisters){
    uint16_t TxRegLength = hModbusU8ToU16(&RxFrame.Data[5], hModbus16BitOrder_AB);
    uint16_t TxRegStart = hModbusU8ToU16(&RxFrame.Data[3], hModbus16BitOrder_AB);
    uint8_t TxData[TxRegLength+1];
    if(TxRegLength > HMODBUS_SLAVE_HOLDING_REG_SIZE)  
      return false;
    TxData[0] = TxRegLength;
    memcpy(&TxData[1], &Handle->Data->HoldingReg[TxRegStart], TxRegLength);
    TxFrame = hModbusComposeFrame8(RxFrame.DeviceAddr, RxFrame.Cmd, TxData, TxRegLength+1);
  }

  else if(RxFrame.Cmd == hModbusCmd_ReadInputRegisters){
    uint16_t TxRegLength = hModbusU8ToU16(&RxFrame.Data[5], hModbus16BitOrder_AB);
    uint16_t TxRegStart = hModbusU8ToU16(&RxFrame.Data[3], hModbus16BitOrder_AB);
    uint8_t TxData[TxRegLength+1];
    if(TxRegLength > HMODBUS_SLAVE_INPUT_REG_SIZE)  
      return false;
    TxData[0] = TxRegLength;
    memcpy(&TxData[1], &Handle->Data->InputsReg[TxRegStart], TxRegLength);
    TxFrame = hModbusComposeFrame8(RxFrame.DeviceAddr, RxFrame.Cmd, TxData, TxRegLength+1);
  }

  else if(RxFrame.Cmd == hModbusCmd_WriteSingleCoil){
    uint16_t CoilRegData = hModbusU8ToU16(&RxFrame.Data[5], hModbus16BitOrder_AB);
    uint16_t CoilRegIndex = hModbusU8ToU16(&RxFrame.Data[3], hModbus16BitOrder_AB);
    if(CoilRegIndex > HMODBUS_SLAVE_COIL_REG_SIZE)  
      return false;
    memcpy(&Handle->Data->CoilsReg[CoilRegIndex], RxFrame.Data[5], 2);
    TxFrame = RxFrame;
  }

  else if(RxFrame.Cmd == hModbusCmd_WriteSingleRegister){
    uint16_t HoldingRegData = hModbusU8ToU16(&RxFrame.Data[5], hModbus16BitOrder_AB);
    uint16_t HoldingRegIndex = hModbusU8ToU16(&RxFrame.Data[3], hModbus16BitOrder_AB);
    if(HoldingRegIndex > HMODBUS_SLAVE_HOLDING_REG_SIZE)  
      return false;
    Handle->Data->HoldingReg[HoldingRegIndex] = HoldingRegData;
    TxFrame = RxFrame;
  }

  else if(RxFrame.Cmd == hModbusCmd_WriteMultipleCoils){
    uint16_t CoilRegSize = hModbusU8ToU16(&RxFrame.Data[5], hModbus16BitOrder_AB);
    uint16_t CoilRegIndex = hModbusU8ToU16(&RxFrame.Data[3], hModbus16BitOrder_AB);
    uint8_t BytesSize = RxFrame.Data[7];
    if(CoilRegIndex + BytesSize > HMODBUS_SLAVE_COIL_REG_SIZE)  
      return false;
    memcpy(&Handle->Data->CoilsReg[CoilRegIndex], RxFrame.Data[8], BytesSize);
    uint16_t TxData[] = {CoilRegIndex, CoilRegSize};
    TxFrame = hModbusComposeFrame16(RxFrame.DeviceAddr, RxFrame.Cmd, TxData, 2);
  }

  else if(RxFrame.Cmd == hModbusCmd_WriteMultipleRegisters){
    uint16_t HoldingRegSize = hModbusU8ToU16(&RxFrame.Data[5], hModbus16BitOrder_AB);
    uint16_t HoldingRegIndex = hModbusU8ToU16(&RxFrame.Data[3], hModbus16BitOrder_AB);
    uint8_t BytesSize = RxFrame.Data[7];
    if(HoldingRegIndex + BytesSize > HMODBUS_SLAVE_HOLDING_REG_SIZE)  
      return false;
    memcpy(&Handle->Data->HoldingReg[HoldingRegIndex], RxFrame.Data[8], BytesSize);
    uint16_t TxData[] = {HoldingRegIndex, HoldingRegSize};
    TxFrame = hModbusComposeFrame16(RxFrame.DeviceAddr, RxFrame.Cmd, TxData, 2);
  }

  else
    return false;

  hModbusSendFrame(Handle, TxFrame);
  return true;
}

void hModbusRxCallback(hModbusTypeDef* Handle){
  if(Handle->Type == hModbusMaster){
    if(hModbusGetUartRxneFlag(Handle)){
        if(Handle->RxIndex < HMODBUS_RXTX_SIZE - 1){
            Handle->RxBuf[Handle->RxIndex] = hModbusUsartRx8(Handle);      
            Handle->RxIndex++;
        }
        else{
            hModbusUsartRx8(Handle);
        } 
        Handle->RxTime = hModbusGetSystemClock();
    }
  }
  if(Handle->Type == hModbusSlave){
    hModbusReveiceRawData(Handle);
    if(hModbusGetUartRxneFlag(Handle)){
        if(Handle->RxIndex < HMODBUS_RXTX_SIZE - 1){
            Handle->RxBuf[Handle->RxIndex] = hModbusUsartRx8(Handle);      
            Handle->RxIndex++;
        }
        else{
            hModbusUsartRx8(Handle);
        } 
        Handle->RxTime = hModbusGetSystemClock();
    }
  }
}

uint16_t hModbusReveiceRawData(hModbusTypeDef* Handle){
  uint32_t startTime = hModbusGetSystemClock();
  if(Handle->Type == hModbusMaster){
    while(1){
      hModbusDelay(1);  
      if(hModbusGetSystemClock() - startTime > Handle->RxTimeout)
        return 0;
      
      if(Handle->RxIndex > 0){
        if(hModbusGetUartIdleFlag(Handle))
          return Handle->RxIndex; 
      }
      else{
        hModbusClearUartIdleFlag(Handle);
      }
    }
  }
  if(Handle->Type == hModbusSlave){
    while(1){
      hModbusDelay(1);  
      if(hModbusGetSystemClock() - startTime > Handle->RxTimeout)
        return 0;
      
      if(Handle->RxIndex > 0){
        if(hModbusGetUartIdleFlag(Handle)){
          hModbusFrameTypeDef RxFrame;
          RxFrame = hModbusParseFrame(Handle);
          hModbusRxFrameExecute(Handle, RxFrame);
          return Handle->RxIndex; 
          }
      }
      else{
        hModbusClearUartIdleFlag(Handle);
      }
    }
  }
  return 0;
}  

bool hModbusSendRawData(hModbusTypeDef* Handle, uint8_t *Data, uint16_t size){
  while(Handle->TxBusy == 1){
      hModbusDelay(1);
  }
  Handle->TxBusy = 1;
  memset(Handle->RxBuf, 0, HMODBUS_RXTX_SIZE);
  Handle->RxIndex = 0;
  uint32_t startTime = hModbusGetSystemClock();
  
  if(Handle->CtrlOut.Port != NULL)
    hModbusSetGpioPin(Handle->CtrlOut);

  hModbusDelay(1);

  for (uint16_t i = 0; i < size; i++){
      while (!hModbusGetUartTxeFlag(Handle)){
          hModbusDelay(1);
          if(hModbusGetSystemClock() - startTime > Handle->TxTimeout){
              if(Handle->CtrlOut.Port != NULL)
                hModbusResetGpioPin(Handle->CtrlOut);
              Handle->TxBusy = 0;
              return false;
          }   
      }
      hModbusClearUartTcFlag(Handle);
      hModbusUsartTx8(Handle, Data[i]);
  }

  while (!hModbusGetUartTcFlag(Handle)){
      if(hModbusGetSystemClock() - startTime > Handle->TxTimeout){
              if(Handle->CtrlOut.Port != NULL)
                hModbusResetGpioPin(Handle->CtrlOut);
              Handle->TxBusy = 0;
              return false;
          }    
  }

  if(Handle->CtrlOut.Port != NULL)
    hModbusResetGpioPin(Handle->CtrlOut);

  Handle->TxBusy = 0;
  return true;
}

bool hModbusInit(hModbusTypeDef* Handle, UART_HANDLE_TYPE Uart, hModbusTypeTypeDef Type){
  memset(Handle, 0, sizeof(*Handle));
  Handle->ByteOrder16 = hModbus16BitOrder_AB;
  Handle->ByteOrder32 = hModbus32BitOrder_ABCD;
  Handle->UartHandle = Uart;
  Handle->Type = Type;
  Handle->CtrlOut.Pin = 0;
  Handle->CtrlOut.Port = NULL;
  hModbusUsartInit(Handle);
  hModbusEnableRxneIt(Handle);
  Handle->RxTimeout = HMODBUS_DEFAULT_RX_TIMEOUT;
  Handle->TxTimeout = HMODBUS_DEFAULT_TX_TIMEOUT;
  return true;
}

void hModbusCtrlOutConfig(hModbusTypeDef* Handle, uint32_t CtrlOutPin, GPIO_TYPE_DEF CtrlOutPort){
  Handle->CtrlOut.Pin = CtrlOutPin;
  Handle->CtrlOut.Port = CtrlOutPort;
  hModbusResetGpioPin(Handle->CtrlOut);
}
 
void hModbusSetRxTimeout(hModbusTypeDef* Handle, uint32_t RxTimeout){
  Handle->RxTimeout = RxTimeout;
}

void hModbusSeTxTimeout(hModbusTypeDef* Handle, uint32_t TxTimeout){
  Handle->TxTimeout = TxTimeout;
}

void hModbusSet16BitOrder(hModbusTypeDef* Handle, hModbus16BitOrderTypeDef hModbus16bitOrder){
  Handle->ByteOrder16 = hModbus16bitOrder;
}

void hModbusSet32BitOrder(hModbusTypeDef* Handle, hModbus32BitOrderTypeDef hModbus32bitOrder){
  Handle->ByteOrder32 = hModbus32bitOrder;
}

void hModbusInitSlaveData(hModbusTypeDef* Handle, hModbusSlaveDataTypeDef* SlaveData){
  Handle->Data = SlaveData;
}

/* READ COIL */

bool hModbusReadCoils(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Startnumber, uint16_t Length, uint8_t *Data){
  // Compose and sen TX frame
  uint16_t TxData[] = {Startnumber, Length};
  hModbusFrameTypeDef TxFrame = hModbusComposeFrame16(SlaveAddress, hModbusCmd_ReadCoilStatus, TxData, 2);
  hModbusSendFrame(Handle, TxFrame);

  // Receive and parse RX frame
  if(hModbusReveiceRawData(Handle) == 0) // check if Rx data length is empty
    return false;
  hModbusFrameTypeDef RxFrame = hModbusParseFrame(Handle);

  // Check RX data
  if(hModbusCheckRxFrame(RxFrame, TxFrame)){
    if(Data != NULL){
      memcpy(Data, &RxFrame.Data, RxFrame.DataLength);
      return true;
    }
  }
  // if error - return false
  return false;
}

bool hModbusReadCoil(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint8_t *Data){
  return hModbusReadCoils(Handle, SlaveAddress, Number, 1, Data);
}

/* READ DISCRETE INPUTS */

bool hModbusReadDiscreteInputs(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint8_t *Data){
  uint16_t TxData[] = {StartNumber, Length};
  hModbusFrameTypeDef TxFrame = hModbusComposeFrame16(SlaveAddress, hModbusCmd_ReadDiscreteInputs, TxData, 2);
  hModbusSendFrame(Handle, TxFrame);

  if(hModbusReveiceRawData(Handle) == 0)
    return false;
  hModbusFrameTypeDef RxFrame = hModbusParseFrame(Handle);
  
  if(hModbusCheckRxFrame(RxFrame, TxFrame)){
    if(Data != NULL){
      memcpy(Data, RxFrame.Data, RxFrame.DataLength);
      return true;
    }
  }
  return false;
}

bool hModbusReadDiscreteInput(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint8_t *Data)
{
  return hModbusReadDiscreteInputs(Handle, SlaveAddress, Number, 1, Data); 
}

/* READ INPUT REGISTERS */

/* Read input reg 8 bit */

bool hModbusReadInputRegisters8i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint8_t *Data){
  uint16_t TxData[] = {StartNumber, Length};
  hModbusFrameTypeDef TxFrame = hModbusComposeFrame16(SlaveAddress, hModbusCmd_ReadInputRegisters, TxData, 2);
  hModbusSendFrame(Handle, TxFrame);

  if(hModbusReveiceRawData(Handle) == 0)
    return false;

  hModbusFrameTypeDef RxFrame = hModbusParseFrame(Handle);

  if(hModbusCheckRxFrame(RxFrame, TxFrame)){
    if(Data != NULL){
      memcpy(Data, &RxFrame.Data, RxFrame.DataLength);
      return true;
    }
  }

  return false;
}

/* Read input reg 16 bit */

bool hModbusReadInputRegisters16i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint16_t *Data){
  bool Ret = hModbusReadInputRegisters8i(Handle, SlaveAddress, StartNumber, Length * 1, (uint8_t*)Data);
  if(Ret == true)
  {
    uint8_t Tmp1[2],Tmp2[2];
    for(uint16_t i=0 ; i<Length ; i++)
    {   
      switch(Handle->ByteOrder16)
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

bool hModbusReadInputRegister16i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint16_t *Data){
  return hModbusReadInputRegisters16i(Handle, SlaveAddress, Number, 1, Data); 
}

/* Read input reg 32 bit */

bool hModbusReadInputRegisters32f(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, float *Data){
  bool ret = hModbusReadInputRegisters8i(Handle, SlaveAddress, StartNumber, Length * 2, (uint8_t*)Data);
  if(ret == true)
  {
    for(uint16_t i=0 ; i<Length ; i++)
    {  
      uint8_t Tmp1[4],Tmp2[4];
      switch(Handle->ByteOrder32)
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

bool hModbusReadInputRegister32f(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, float *Data){
  return hModbusReadInputRegisters32f(Handle, SlaveAddress, Number, 1, Data); 
}

bool hModbusReadInputRegisters32i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint32_t *Data){
  return hModbusReadInputRegisters32f(Handle, SlaveAddress, StartNumber, Length, (float*)Data);
}

bool hModbusReadInputRegister32i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint32_t *Data){
  return hModbusReadInputRegisters32i(Handle, SlaveAddress, Number, 1, Data); 
}

/* READ HOLDING REGISTERS */

/* Read holding reg 8 bit */

bool hModbusReadHoldingRegisters8i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint8_t *Data){
  uint16_t TxData[] = {StartNumber, Length};
  hModbusFrameTypeDef TxFrame = hModbusComposeFrame16(SlaveAddress, hModbusCmd_ReadHoldingRegisters, TxData, 2);
  hModbusSendFrame(Handle, TxFrame);

  if(hModbusReveiceRawData(Handle) == 0)
    return false;
  hModbusFrameTypeDef RxFrame = hModbusParseFrame(Handle);

  if(hModbusCheckRxFrame(RxFrame, TxFrame)){
    if(Data != NULL){
      memcpy(Data, RxFrame.Data, RxFrame.DataLength);
      return true;
    }
  }
  return false;
}

/* Read holding reg 16 bit */

bool hModbusReadHoldingRegisters16i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint16_t *Data){
  bool Ret = hModbusReadHoldingRegisters8i(Handle, SlaveAddress, StartNumber, Length * 1, (uint8_t*)Data);
  if(Ret == true)
  {
    uint8_t Tmp1[2],Tmp2[2];
    for(uint16_t i=0 ; i<Length ; i++)
    {   
      switch(Handle->ByteOrder16)
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

bool hModbusReadHoldingRegister16i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint16_t *Data){
  return hModbusReadHoldingRegisters16i(Handle, SlaveAddress, Number, 1, Data); 
}

/* Read holding reg 32bit */

bool hModbusReadHoldingRegisters32f(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, float *Data){
  bool Ret = hModbusReadHoldingRegisters8i(Handle, SlaveAddress, StartNumber, Length * 2, (uint8_t*)Data);
  if(Ret == true)
  {
    for(uint16_t i=0 ; i<Length ; i++)
    {  
      uint8_t Tmp1[4],Tmp2[4];
      switch(Handle->ByteOrder32)
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

bool hModbusReadHoldingRegister32f(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, float *Data){
  return hModbusReadHoldingRegisters32f(Handle, SlaveAddress, Number, 1, Data); 
}

bool hModbusReadHoldingRegisters32i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint32_t *Data){
  return hModbusReadHoldingRegisters32f(Handle, SlaveAddress, StartNumber, Length, (float*)Data);
}

bool hModbusReadHoldingRegister32i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint32_t *Data){
  return hModbusReadHoldingRegisters32i(Handle, SlaveAddress, Number, 1, Data); 
}

/* WRITE COIL */

bool hModbusWriteCoil(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint16_t Data){
  uint16_t TxData[] = {Number, 0};
  if(Data == 0) 
    TxData[1] = 0x0000;
  else  
    TxData[1] = 0xFF00;
  hModbusFrameTypeDef TxFrame = hModbusComposeFrame16(SlaveAddress, hModbusCmd_WriteSingleCoil, TxData, 2);
  hModbusSendFrame(Handle, TxFrame);

  if(hModbusReveiceRawData(Handle) == 0)
    return false;
  hModbusFrameTypeDef RxFrame = hModbusParseFrame(Handle);

  if(hModbusCompareFrame(RxFrame, TxFrame))
    return true;

  return false;
}

/* WRITE HODLING REGISTERS */

bool hModbusWriteHoldingRegister16i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t Number, uint16_t Data){
  uint16_t TxData[] = {Number, Data};
  hModbusFrameTypeDef TxFrame = hModbusComposeFrame16(SlaveAddress, hModbusCmd_WriteSingleRegister, TxData, 2);
  hModbusSendFrame(Handle, TxFrame);

  if(hModbusReveiceRawData(Handle) == 0)
    return false;
  hModbusFrameTypeDef RxFrame = hModbusParseFrame(Handle);

  if(hModbusCompareFrame(RxFrame, TxFrame))
    return true;

  return false;
}

bool hModbusWriteHoldingRegisters16i(hModbusTypeDef* Handle, uint8_t SlaveAddress, uint16_t StartNumber, uint16_t Length, uint16_t *Data){
  if (Length==1){
		return hModbusWriteHoldingRegister16i(Handle, SlaveAddress, StartNumber, Data[0]);
	}
  uint8_t TxData[5+Length*2];
  TxData[0] = (StartNumber & 0xFF00) >> 8;
  TxData[1] = (StartNumber & 0x00FF);
  TxData[2] = (Length & 0xFF00) >> 8;
  TxData[3] = (Length & 0x00FF);
  TxData[4] = (Length * 2);

  memcpy(&TxData[5], Data, Length*2);
  if(Handle->ByteOrder16 == hModbus16BitOrder_AB){
    hModbusSwapU16DataByteArray(&TxData[5], Length*2, hModbus16BitOrder_BA);
  }

  hModbusFrameTypeDef TxFrame = hModbusComposeFrame8(SlaveAddress, hModbusCmd_WriteMultipleRegisters, TxData, 5+Length*2);
  hModbusSendFrame(Handle, TxFrame);

  if(hModbusReveiceRawData(Handle) == 0)
    return false;
  hModbusFrameTypeDef RxFrame = hModbusParseFrame(Handle);

  if(hModbusCheckRxFrame(RxFrame, TxFrame) && Data != NULL){
    if(memcmp(RxFrame.Data, TxFrame.Data, 4) == 0)
      return true;
  }
  return false;
}