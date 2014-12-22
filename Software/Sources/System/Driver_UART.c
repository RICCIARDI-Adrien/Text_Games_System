/** @file Driver_UART.c
 * @see Driver_UART.h for description.
 * @author Adrien RICCIARDI
 */
#include <Driver_UART.h>
#include <Interrupts_Handler.h>
#include <system.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void UARTInitialize(void)
{
	// Configure UART pins as inputs
	trisc.6 = 1;
	trisc.7 = 1;

	spbrg = 12; // 19200 bit/s baud rate
	txsta = 0x26; // 8-bit transmission, transmission enabled, high speed
	rcsta = 0x90; // Reception and serial port module enabled
	pie1.RCIE = 1; // Enable receive interrupt
}

unsigned char UARTReadByte(void)
{
	while (!Interrupts_Handler_Has_UART_Received_Byte);
	Interrupts_Handler_Has_UART_Received_Byte = 0;
	return Interrupts_Handler_UART_Received_Byte_Value;
}

void UARTWriteByte(unsigned char Byte)
{
	// Wait until the Tx line is free
	while (!pir1.TXIF);
	// Send data
	txreg = Byte;
}

unsigned char UARTIsByteAvailable(void)
{
	return Interrupts_Handler_Has_UART_Received_Byte;
}