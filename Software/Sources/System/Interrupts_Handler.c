/** @file Interrupts_Handler.c
 * @see Interrupts_Handler.h for description.
 * @author Adrien RICCIARDI
 */
#include <Interrupts_Handler.h>
#include <system.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Code to begin programming (the same that the programmer uses). */
#define CODE_PROGRAMMING_START 0xFE

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
unsigned char Interrupts_Handler_Has_UART_Received_Byte = 0, Interrupts_Handler_UART_Received_Byte_Value;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void interrupt(void)
{
	// UART interrupt
	if (pir1.RCIF)
	{
		Interrupts_Handler_UART_Received_Byte_Value = rcreg;
		Interrupts_Handler_Has_UART_Received_Byte = 1;
		
		// Reboot the board if a specific magic number has been received
		if (Interrupts_Handler_UART_Received_Byte_Value == CODE_PROGRAMMING_START)
		{
			asm
			{
				// Disable interrupts
				clrf _intcon
				// Go to beginning of program memory
				clrf _pclath 
				goto 0
			}
		}
	}
}