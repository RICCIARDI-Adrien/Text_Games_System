/** @file Interrupts_Handler.c
 * @see Interrupts_Handler.h for description.
 * @author Adrien RICCIARDI
 */
#include <Driver_Flash.h>
#include <Interrupts_Handler.h>
#include <system.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Code to reboot the board in programming mode. */
#define INTERRUPTS_HANDLER_PROTOCOL_CODE_START_PROGRAMMING 0xFE

/** The instruction address to erase to force the bootloader mode. */
#define INTERRUPTS_HANDLER_IS_PROGRAM_PRESENT_FLAG_ADDRESS 0x1FFF

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
		if (Interrupts_Handler_UART_Received_Byte_Value == INTERRUPTS_HANDLER_PROTOCOL_CODE_START_PROGRAMMING)
		{
			// Erase the "program present" flag to force the bootloader mode
			FlashWriteWord(INTERRUPTS_HANDLER_IS_PROGRAM_PRESENT_FLAG_ADDRESS, 0x3FFF);
			
			// Do a software reboot
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