/** @file Driver_Internal_EEPROM.c
 * @see Driver_Internal_EEPROM.h for description.
 * @author Adrien RICCIARDI
 */
#include <Driver_Internal_EEPROM.h>
#include <system.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned char InternalEEPROMReadByte(unsigned char Address)
{
	// Set the address to read from
	eeadr = Address;
	
	// Start the read operation
	eecon1 = 0x01; // Select the internal EEPROM and start reading
	
	return eedata;
}

void InternalEEPROMWriteByte(unsigned char Address, unsigned char Byte)
{
	// Set address and data to write
	eeadr = Address;
	eedata = Byte;
	
	// Prepare the write operation
	eecon1 = 0x04; // Select EEPROM memory and enable writing
	
	
	// Disable interrupts
	intcon.GIE = 0;
	
	// Disable writing lock with the following special sequence
	asm
	{
		movlw 0x55
		movwf _eecon2
		movlw 0xAA
		movwf _eecon2
		bsf _eecon1, WR // Start the write operation
	}
	
	// Re-enable interrupts
	intcon.GIE = 1;
	
	// Disable write operations
	eecon1 = 0;
	
	// Wait for the write operation to terminate
	while (eecon1.WR);	
}