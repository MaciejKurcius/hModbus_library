/**
 *  \file 		uart_lib.h
 *  \brief		UART protocol file
 *  \details	It contain all UART protocol function
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		23.07.2024
 */

#ifndef UART_LIB_H
#define UART_LIB_H

/* INCLUDES */
#include <stdint.h>
#include "uart_lib_conf.h"


/* DEFINES */
#


#define HMODBUS_RTU_FRAME_START_LEN			1	// bytes (more than 3,5 char)
#define HMODBUS_RTU_FRAME_ADDR_LEN			1	// bytes
#define HMODBUS_RTU_FUN_LEN					1	// bytes
#define HMODBUS_RTU_FRAME_MAX_DATA_LENGHT	256	// bytes
#define HMODBUS_RTU_FRAME_CRC_LENGTH		2	// bytes
#define HMODBUS_RTU_FRAME_END_FRAME_LEN		1	// bytes 
#define HMODBUS_RTU_START_BYTE				0x00
#define HMODBUS_RTU_STOP_BYTE				0x00
#define HMODBUS_RTU_FRAME_LENGTH			(HMODBUS_RTU_FRAME_START_LEN + HMODBUS_RTU_FRAME_ADDR_LEN + HMODBUS_RTU_FUN_LEN + HMODBUS_RTU_FRAME_MAX_DATA_LENGHT + HMODBUS_RTU_FRAME_CRC_LENGTH + HMODBUS_RTU_FRAME_END_FRAME_LEN)


#define HMODBUS_ASCII_FRAME_START_LEN		1	// bytes (more than 3,5 char)
#define HMODBUS_ASCII_FRAME_ADDR_LEN		2	// bytes
#define HMODBUS_ASCII_FUN_LEN				2	// bytes
#define HMODBUS_ASCII_FRAME_MAX_DATA_LENGHT	256	// bytes
#define HMODBUS_ASCII_FRAME_CRC_LENGTH		2	// bytes
#define HMODBUS_ASCII_FRAME_END_FRAME_LEN	2	// bytes 
#define HMODBUS_ASCII_START_BYTE			';'
#define HMODBUS_ASCII_STOP_BYTE				'\r\n'
#define HMODBUS_ASCII_FRAME_LENGTH			(HMODBUS_ASCII_FRAME_START_LEN + HMODBUS_ASCII_FRAME_ADDR_LEN + HMODBUS_ASCII_FUN_LEN + HMODBUS_ASCII_FRAME_MAX_DATA_LENGHT + HMODBUS_ASCII_FRAME_CRC_LENGTH + HMODBUS_ASCII_FRAME_END_FRAME_LEN)

#define HMODBUS_RXTX_FIFO_MAX_SIZE			HMODBUS_RTU_FRAME_LENGTH


#define MAX_FRAME_ARGS_SIZE 50 
#define UART_MAX_DATA_LENGTH		UART_FRAME_DATA_LENGTH(MAX_FRAME_ARGS_SIZE)			//Returned max size of data for UART
#define UART_FRAME_DATA_LENGTH(arg)	((arg + 3)*2)										//Returned frame length (without start/stop sign) frame arguments size + 1 byte of CRC + 1 byte of command + 1 byte of arguments size field in frame
#define UART_FRAME_LENGTH(arg)		(2 + UART_FRAME_DATA_LENGTH(arg))					//Returned frame length (with start/stop sign)

/* TYPE DEF ENUM */

typedef enum{
	frame		= 0,
	standard	= 1
}UartModeTypeDef;


/* STRUCTURES  */

typedef struct{
	uint8_t RawData[MAX_FRAME_ARGS_SIZE];
	uint16_t AccDataSize;
}UartParserTypeDef;

typedef struct{
	uint8_t  buf[HMODBUS_RXTX_FIFO_MAX_SIZE];			
	uint16_t length;		//data saved in fifo
	uint16_t counter;		//data read in fifo
}UartFifoTypeDef;

typedef struct{
	uint8_t UartId;
	UartFifoTypeDef	RxFifo;
	UartFifoTypeDef TxFifo;
}UartTypeDef;

typedef struct{
	uint8_t Address;
	uint16_t Function;
	uint8_t DataSize;
	uint8_t* Data;
}HmodbusFrameTypeDef;

typedef enum{
	ReadCoils = 0x01,
	ReadDiscreteInputs = 0x02,
	ReadHoldingRegisters = 0x03,
	ReadInputRegisters = 0x04,
	WriteSingleCoil = 0x05,
	WriteSingleRegister = 0x06,
	Diagnostics = 0x08,
	GetCommEventCounter = 0x0B,
	WriteMultipleCoils = 0x0F,
	WriteMultipleRegisters = 0x10,
	ReportServerId = 0x11, 
	MaskWriteRegister = 0x16,
	ReadWriteMultipleRegisters = 0x17,
	ReadDeviceIndetification = 0x0E
}HmodbusFunctionFrameTypeDef;


/* EXTERN FUNCTIONS PROTOTYPES */
extern void UartLibDelay(uint32_t);
extern uint8_t UartReceiveData8(UartTypeDef);
extern void UartTransmitData8(UartTypeDef, uint8_t);
extern void UartDisableItTxe(UartTypeDef);
extern void UartEnableItTxe(UartTypeDef);
extern void UartDisableItRxne(UartTypeDef);
extern void UartEnableItRxne(UartTypeDef);
extern void UartClearFlags(UartTypeDef);


extern void UartLibInit(UartTypeDef);
extern void UartFrameExecute(UartFrameTypeDef *frame);


/* FUNCTION PROTOTYPES */
void UsartRxIT(UartTypeDef);
void UsartTxIT(UartTypeDef);
uint16_t UartReceiveBuffer(UartTypeDef UartHandle, uint16_t size, uint8_t* buffer);
void UartSendBuffer(UartTypeDef, uint16_t size, uint8_t* buffer);
void UartSendFrame(UartTypeDef, UartFrameTypeDef* frame);
void UartSendFrameD(UartTypeDef, uint8_t command, uint8_t arg_size, uint8_t* arg);
void UartReceivedFrameHandler(UartTypeDef UartHandle);
void UartParseFrame(UartTypeDef, uint8_t size, uint8_t* buf);
uint8_t* ByteToHex(uint8_t byte, uint8_t* buf);
uint16_t HexToByte(uint8_t* byte);
uint8_t EncodeHex(uint8_t value);
uint8_t DecodeHex(uint8_t value);
ParamBuffOperationModeTypeDef GetParamBuffOperationMode(UartFrameTypeDef *frame);


#endif
/****END OF FILE****/
