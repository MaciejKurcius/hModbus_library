/**
 *  \file 		Uart_Lib.h
 *  \brief		UART protocol file
 *  \details	It contain all UART protocol function
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

#ifndef __UART_LIB_H
#define __UART_LIB_H

/* INCLUDES */

// #include <main.h>
#include <Uart_Lib_Config.h>
#include <main.h>

/* DEFINES */

#define UART_H2B_ERR	0xFFFF
#define UART_FRAME_STARTBYTE 		'<'												//start sign of frame
#define UART_FRAME_STOPBYTE			'>'												//stop sign of frame

#define UART_FRAME_DATA_LENGTH(arg)	((arg + 3)*2)									//Returned frame length (without start/stop sign) frame arguments size + 1 byte of CRC + 1 byte of command + 1 byte of arguments size field in frame
#define UART_FRAME_LENGTH(arg)		(2 + UART_FRAME_DATA_LENGTH(arg))				//Returned frame length (with start/stop sign)
#define UART_MAX_DATA_LENGTH		UART_FRAME_DATA_LENGTH(UART_MAX_FRAME_ARGS_SIZE)	//Returned max size of data for UART

/* TYPE DEF ENUM */

typedef enum{
	frame		= 0,
	standard	= 1
}UartModeTypeDef;


/* STRUCTURES  */

typedef struct{
	uint8_t RawData[UART_MAX_DATA_LENGTH];
	uint16_t AccDataSize;
}UartParserTypeDef;

typedef struct{
	uint8_t* buf;			//pointer for data buffer
	uint16_t bufsize;		//buffer size (in bytes)
	uint16_t length;		//data saved in fifo
	uint16_t counter;		//data read in fifo
}UartFifoTypeDef;

typedef struct{
	UartModeTypeDef ModeOfTransmission;
	UartParserTypeDef Parser;
	UartFifoTypeDef	RxFifo;
	UartFifoTypeDef TxFifo;
}UartTypeDef;

typedef struct{
	uint8_t command;
	uint8_t size;
	uint8_t* data;
}UartFrameTypeDef;

typedef enum{
	Read	 	= 0,
	Write		= 1,
	Unknown		= 2
}ParamBuffOperationModeTypeDef;

/* VARIABLES */
volatile uint8_t UartRxBuffer[UART_RXBUF_SIZE];
volatile uint8_t UartTxBuffer[UART_TXBUF_SIZE];
volatile UartTypeDef Uart;


/* FUNCTION PROTOTYPES */
void UartLibInit(void);
void UsartRxIT(void);
void UsartTxIT(void);
extern void UartFrameExecute(UartFrameTypeDef *frame);
uint16_t UartReceiveBuffer(uint16_t size, uint8_t* buffer);
void UartSendBuffer(uint16_t size, uint8_t* buffer);
void UartSendFrame(UartFrameTypeDef* frame);
void UartSendFrameD(uint8_t command, uint8_t arg_size, uint8_t* arg);
void UartReceivedFrameHandler(void);
void UartParseFrame(uint8_t size, uint8_t* buf);
uint8_t* ByteToHex(uint8_t byte, uint8_t* buf);
uint16_t HexToByte(uint8_t* byte);
uint8_t EncodeHex(uint8_t value);
uint8_t DecodeHex(uint8_t value);
ParamBuffOperationModeTypeDef GetParamBuffOperationMode(UartFrameTypeDef *frame);


#endif
/****END OF FILE****/
