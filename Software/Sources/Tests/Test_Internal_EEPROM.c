/** @file Test_Internal_EEPROM.c
 * @see Test_Internal_EEPROM.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Test_Internal_EEPROM.h"

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void TestInternalEEPROM(void)
{
	unsigned short Address;
	unsigned char Written_Byte, Read_Byte;
	
	ScreenWriteString("Starting internal EEPROM test...\r\n");
	RandomInitializeGenerator();
	
	for (Address = 0; Address < TEST_INTERNAL_EEPROM_MEMORY_SIZE_BYTES; Address++)
	{
		// Write a random byte at the current address
		Written_Byte = RandomGenerateNumber();
		InternalEEPROMWriteByte((unsigned char) Address, Written_Byte);
		
		// Read the memory location to check if the byte was successfully written
		Read_Byte = InternalEEPROMReadByte(Address);
		
		if (Read_Byte != Written_Byte)
		{
			// Compose error message
			ScreenSetColor(SCREEN_COLOR_CODE_TEXT_RED);
			ScreenWriteString("Error at address ");
			ScreenWriteUnsignedInteger(Address);
			ScreenWriteString(", the written byte was ");
			ScreenWriteUnsignedInteger(Written_Byte);
			ScreenWriteString(" but the read byte is ");
			ScreenWriteUnsignedInteger(Read_Byte);
			ScreenWriteString(".\r\n");
			ScreenSetColor(SCREEN_COLOR_CODE_DEFAULT);
			goto Exit;
		}
	}
	
	ScreenSetColor(SCREEN_COLOR_CODE_TEXT_GREEN);
	ScreenWriteString("All tests were successful.\r\n");
	ScreenSetColor(SCREEN_COLOR_CODE_DEFAULT);
	
Exit:
	ScreenWriteString("Hit a key to return to main menu.\r\n");
	KeyboardReadCharacter();
}