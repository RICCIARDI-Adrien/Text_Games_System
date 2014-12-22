/** @file Driver_UART.h
 * Driver for the RS-232 Universal Asynchronous Receiver Transmitter.
 * @warning This driver functions are used internally by the system, you must not use them directly.
 * @author Adrien RICCIARDI
 * @version 1.0 : 21/02/2013
 * @version 1.1 : 25/05/2013, fixed overrun bug leading to no reception.
 */ 
#ifndef H_DRIVER_UART_H
#define H_DRIVER_UART_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize RS-232 port at 19200 bit/s.
 * @note This function is automatically called at system boot, you don't need to call it yourself.
 */
void UARTInitialize(void);

/** Read a byte from the UART.
 * @return The read byte.
 * @note This function waits until a byte is available.
 */
unsigned char UARTReadByte(void);

/** Write a byte to the UART.
 * @param Byte The byte to write.
 */
void UARTWriteByte(unsigned char Byte);

/** Tell if a byte was received and is available to read or not.
 * @return true if a byte can be read or false if no byte was received.
 */
unsigned char UARTIsByteAvailable(void);
 
#endif