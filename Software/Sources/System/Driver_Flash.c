/** @file Driver_Flash.c
 * @see Driver_Flash.h for description.
 * @author Adrien RICCIARDI
 */ 
#include <Driver_Flash.h>
#include <system.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned short FlashReadWord(unsigned short Address)
{
	// Check address validity
	if (Address >= FLASH_SIZE) return 0xFFFF;
	
	// Set the address
	eeadrh = Address >> 8;
	eeadr = (unsigned char) Address;
	
	// Choose which memory to read
	eecon1 = 0x80; // Select program memory, do not initiate any operation
	
	// Start reading
	eecon1.RD = 1;
	asm nop; // Two 'nop' are required for the core to fetch data from the program memory
	asm nop;
	
	return (eedath << 8) | eedata;
}

void FlashWriteWord(unsigned short Address, unsigned short Data)
{
	// Check address validity
	if (Address >= FLASH_SIZE) return;
	
	// Set the address
	eeadrh = Address >> 8;
	eeadr = (unsigned char) Address;
	
	// Set the data to write
	eedath = Data >> 8;
	eedata = (unsigned char) Data;
	
	// Choose which memory to write
	eecon1 = 0x84; // Select program memory, allow write cycles, do not initiate any operation
	
	// Start writing
	asm
	{
		movlw 0x55 // Execute the special write sequence
		movwf _eecon2
		movlw 0xAA
		movwf _eecon2
	}
	eecon1.WR = 1; // Initiate write operation
	asm nop; // // Two 'nop' are required for the core to write data to the program memory
	asm nop;
	
	// Disable write operations
	eecon1.WREN = 0;
}