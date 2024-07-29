/**
 *  \file 		uart_lib.c
 *  \brief		UART protocol file
 *  \details	It contain all UART protocol function
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		23.07.2024
 */

/* INCLUDES */

#include <uart_lib.h>
#include <logic.h>

/* DEFINES */



/* VARIABLES */

/* FUNCTION */

/**
 * \brief 	UARTProtocol - handling of receiving data buffer
 * \param 	size - size of data to read
 * \param 	buffer - pointer for buffer to write received data
 * \see 	UsartRxIT
 * \note 	Function copy data from UART port to buffer.
 * \return 	copied - return size of copied data
 */
uint16_t UartReceiveBuffer(UartTypeDef UartHandle, uint16_t size, uint8_t* buffer){
	uint16_t copied = 0;
	while((size>0) && (UartHandle.RxFifo.length - UartHandle.RxFifo.counter != 0))
	{
		*buffer++ = UartHandle.RxFifo.buf[UartHandle.RxFifo.counter++];
		copied++;
		size--;
	}
	return copied;	//copied should be equal to size. Then - > transmission ok.
}

/**
 * \brief 	UARTProtocol - handling of sending data buffer
 * \param 	size - size of data to read
 * \param 	buffer - pointer for buffer with data to send
 * \see 	UsartTxIT()
 * \note 	Function disable UART TX interrupts. Next, it write data to UART TX FIFO buffer.
 * 			When data is wrote to TX FIFO, function enable UART TX interrupts.
 * \return 	void
 */
void UartSendBuffer(UartTypeDef UartHandle, uint16_t size, uint8_t* buffer){
	UartDisableItTxe(UartHandle);
	
	// LL_USART_DisableIT_TXE(UART);
	// LL_USART_DisableIT_TC(UART);

	for(uint16_t i=0; i<size; i++)
	{
		UartHandle.TxFifo.buf[UartHandle.TxFifo.length]=buffer[i];
		if(UartHandle.TxFifo.length < UartHandle.TxFifo.bufsize-1)
			UartHandle.TxFifo.length++;
	}
	UartEnableItTxe(UartHandle);
	// LL_USART_EnableIT_TXE(UART);
}

/**
 * \brief 	UARTProtocol - interrupt RX procedure
 * \param 	void
 * \see 	USARTx_IRQHandler()
 * \see		Uart1ReceiveBuffer
 * \note 	Function read data from UART port and write them to UART Fifo buffer.
 * 			If the buffer is empty, write from begin. If the buffer is full
 * 			- overwrite laty byte. Function is call by UART interrupt.
 * \return 	void
 */
void UsartRxIT(UartTypeDef UartHandle){
	// uint8_t byte = LL_USART_ReceiveData8(UART);
	uint8_t byte = UartReceiveData8(UartHandle);
	if(UartHandle.RxFifo.counter >= UartHandle.RxFifo.length)		//if RX buffer is empty -> return to begin
	{
		UartHandle.RxFifo.length = 0;
		UartHandle.RxFifo.counter = 0;
	}
	UartHandle.RxFifo.buf[UartHandle.RxFifo.length] = byte;
	if(UartHandle.RxFifo.length < UartHandle.RxFifo.bufsize-1)	//if buffer is full -> overwrite last byte
	{
		UartHandle.RxFifo.length++;
	}
}

/**
 * \brief 	UARTProtocol - interrupt TX procedure
 * \param 	void
 * \see 	USARTx_IRQHandler()
 * \see		Uart1SendBuffer
 * \note 	Function transmit data from TxFifo.buf byte by byte via UART port.
 * 			When all data are sent, interrupts from UART TX are disabling.
 * \return 	void
 */
void UsartTxIT(UartTypeDef UartHandle){
	if(UartHandle.TxFifo.counter < UartHandle.TxFifo.length){		//check if TX buffer contain any data
		// LL_USART_TransmitData8(UART, Uart.TxFifo.buf[Uart.TxFifo.counter++]);
		UartTransmitData8(UartHandle, UartHandle.TxFifo.buf[UartHandle.TxFifo.counter++]);
	}
	else{
		UartHandle.TxFifo.length = 0;
		UartHandle.TxFifo.counter = 0;
		UartDisableItTxe(UartHandle);
		UartClearFlags(UartHandle);
	}
}

/**
 * \brief 	UARTProtocol - sending frame procedure
 * \param 	frame - UART protocol frame structure to send
 * \see 	MainLogicLoop()
 * \note 	Function send all UART protocol frame via UART port.
 * \return 	void
 */
void UartSendFrame(UartTypeDef UartHandle, UartFrameTypeDef* frame){
	static uint8_t buf[UART_FRAME_LENGTH(MAX_FRAME_ARGS_SIZE)];
	uint8_t* buffer = buf;
	uint8_t ExorCrc = frame->command ^ (frame->size);			//EXOR frame command and frame size
	*buffer++ = UART_FRAME_STARTBYTE;
	buffer = ByteToHex(frame->command, buffer);
	buffer = ByteToHex(frame->size, buffer);
	for(uint8_t i = 0; i < frame->size; i++){
		buffer = ByteToHex(frame->data[i], buffer);
		ExorCrc ^= frame->data[i];
	}
	buffer = ByteToHex(ExorCrc, buffer);
	*buffer++ = UART_FRAME_STOPBYTE;
	UartSendBuffer(UartHandle, UART_FRAME_LENGTH(frame->size), buf);
}

/**
 * \brief 	UARTProtocol - receive frame procedure
 * \param
 * \see 	MainLogicLoop()
 * \note 	Function for handle receiving data via UART port. It search frames in data stream.
 * 			If it finds a dataframe, calls a function parsing it.
 * 			Function is call by main loop of program.
 * \return 	void
 */
void UartReceivedFrameHandler(UartTypeDef UartHandle){
	uint8_t TempData;
	UartParserTypeDef Parser;
	while(UartReceiveBuffer(UartHandle, 1,&TempData)){
		if(TempData==UART_FRAME_STARTBYTE){
			Parser.AccDataSize = 0;
		}
		else{
			if(TempData != UART_FRAME_STOPBYTE){
				if(Parser.AccDataSize < UART_MAX_DATA_LENGTH)
					Parser.RawData[Parser.AccDataSize++] = TempData;
			}
			else{
				UartParseFrame(UartHandle, Parser.AccDataSize, Parser.RawData);
				return;
			}
		}
	}

}

/**
 * \brief 	UARTProtocol - parsing frame procedure
 * \param 	size - size of data frame to parse
 * \param 	buf - pointer for frame data buffer to parse
 * \see 	UartReceiveFrameHandler()
 * \see 	UartFrameExecute()
 * \note 	Function analyze data frame. If data are correct, and the counted
 * 			CRC is equal received CRC, calls function which execute this frame.
 * \return 	void
 */
void UartParseFrame(UartTypeDef UartHandle, uint8_t size, uint8_t* buf){
	uint8_t TempData = 0;
	uint8_t ExorCrc = 0;
	uint8_t* Buffer = buf;
	static UartFrameTypeDef Frame;
	if((TempData = HexToByte(Buffer)) != UART_H2B_ERR){
		Frame.command = (uint8_t)TempData;
		ExorCrc ^= (uint8_t)TempData;
	}
	else
		return;
	Buffer += 2;
	if((TempData = HexToByte(Buffer)) != UART_H2B_ERR){
		Frame.size = (uint8_t)TempData;
		ExorCrc ^= (uint8_t)TempData;
	}
	else
		return;
	if(UART_FRAME_DATA_LENGTH(Frame.size) != size)
		return;
	Buffer += 2;
	uint8_t FrameDataTemp[Frame.size];
	for(uint8_t i = 0; i < Frame.size; i++){
		if((TempData = HexToByte(Buffer)) != UART_H2B_ERR){
			FrameDataTemp[i] = (uint8_t)TempData;
			ExorCrc ^= (uint8_t)TempData;
			Buffer+=2;
		}
		else
			return;
	}
	if((TempData = HexToByte(Buffer)) != UART_H2B_ERR)
	{
		if(ExorCrc == (uint8_t)TempData){
			Frame.data = FrameDataTemp;
			if(((Frame.command >> 7) & 0xF) == 1)		//if MSB of command in frame == 1 -> resend empty frame to confirm received frame
				UartSendFrameD(UartHandle, 0, 0, 0);
			UartFrameExecute(&Frame);
		}
		else
			return;
	}
	else
		return;
}

/**
 * \brief 	UARTProtocol - send data by frame
 * \param 	command - value of command in frame
 * \param 	arg_size - size of data in frame
 * \param	arg - pointer for data buffer to send
 * \see 	UartSendFrame()
 * \note 	Function packing data into a frame and send them via UART port.
 * \return 	void
 */
void UartSendFrameD(UartTypeDef UartHandle, uint8_t command, uint8_t arg_size, uint8_t* arg){
	UartFrameTypeDef frame;
	frame.command = command;
	frame.size = arg_size;
	frame.data = arg;
	UartSendFrame(UartHandle, &frame);
}

/**
 * \brief 	UARTProtocol - converting function
 * \param 	byte - 1 byte with value in DEC, range (0 - 255)
 * \param 	buf - pointer for the output buffer
 * \see 	EnocdeHex()
 * \see		HexToByte()
 * \see		DecodeHex()
 * \note 	Function get one byte of value in DEC and convert them to
 * 			two byte of HEX in ASCII
 * \return 	pointer for the end of buffer
 */
uint8_t* ByteToHex(uint8_t byte, uint8_t* buf){	//1 byte of DEC to two byte of HEX in ASCII
	buf[0] = EncodeHex(byte >> 4);				//convert 0b01001100 (0x4C) - > 0b00000100 (0x04)
	buf[1] = EncodeHex(byte & 0x0F);			//convert 0b01001100 (0x4C) - > 0b00001100 (0x0C) ; logical AND operation with 0b00001111
	return buf+2;								//return pointer for the end of buffer
}

/**
 * \brief 	UARTProtocol - converting function
 * \param 	value - data to convert, range (0 - 15) in DEC
 * \see 	ByteToHex()
 * \see		HexToByte()
 * \see		DecodeHex()
 * \note 	Function get one byte of value in DEC and convert it to one byte of HEX in ASCII
 * \return 	255 if error, else 1 byte of HEX in ASCII
 */
uint8_t EncodeHex(uint8_t value){				//Input 1 byte of DEC. Output 1 byte of HEX in ASCII
	if(value < 10)
		return '0'+ value;
	else if(value < 16)
		return 'a'+ value-10;
	else
		return 255;								//if error
}

/**
 * \brief 	UARTProtocol - converting function
 * \param 	value - pointer for array with 2 values with HEX in ASCII
 * \see 	ByteToHex()
 * \see		EncodeHex()
 * \see		DecodeHex()
 * \note 	Function 2 byte of HEX in ASCII and convert them to 2bytes value in DEC
 * \return 	0xFFFF if error, else 2 byte of value in DEC
 */
uint16_t HexToByte(uint8_t* byte){				//2 byte of HEX in ASCII to 1 byte of DEC
	uint8_t value, result;						//byte[1] = '4' ; byte [0] = 'C' -> 0x4 and 0xC -> 0b00001100 + 0b00001111 -> 0b01001100 (0x4C)
	value = DecodeHex(byte[0]);
	if(value < 16){
		result = value << 4;
		value = DecodeHex(byte[1]);
		if(value < 16){
			result = result + value;
			return result;
		}
		return UART_H2B_ERR;
	}
	return UART_H2B_ERR;
}

/**
 * \brief 	UARTProtocol - converting function
 * \param 	value - one byte of data of HEX in ASCII
 * \see 	ByteToHex()
 * \see		HexToByte()
 * \see		EncodeHex()
 * \note 	Function get one byte of data in HEX in ASCII and convert it to one byte of DEC.
 * \return 	255 if error, else 1 byte of DEC
 */
uint8_t DecodeHex(uint8_t value){		//Input 1 byte of hex in ASCII. Output 1 byte of DEC.
	if(value >= '0' && value <= '9')
		return value - '0';
	else if(value >= 'a' && value <= 'f')
		return value - 'a' + 10;
	else if(value >= 'A' && value <= 'F')
		return value - 'A' + 10;
	else
		return 255;
}

ParamBuffOperationModeTypeDef GetParamBuffOperationMode(UartFrameTypeDef *frame){
	if(frame->size > 0){
		if(frame->data[0] == 0)	return Read;
		if(frame->data[0] == 1)	return Write;
	}
	else{
		return Unknown;
	}
	return Unknown;
}

/****END OF FILE****/
