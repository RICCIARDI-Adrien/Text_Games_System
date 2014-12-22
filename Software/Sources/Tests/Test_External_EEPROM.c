/** @file Test_External_EEPROM.c
 * @see Test_External_EEPROM.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Test_External_EEPROM.h"

void TestExternalEEPROM(void)
{
	unsigned short i, Address;
	unsigned char Written_Byte, Read_Byte;
	
	ScreenWriteString("Starting external EEPROM test...\r\n");
	RandomInitializeGenerator();
	
	for (i = 0; i < TEST_EXTERNAL_EEPROM_CYCLES_COUNT; i++)
	{
		// Generate an address in the in the memory bounds
		Address = (RandomGenerateNumber() << 8) | RandomGenerateNumber();
		Address %= TEST_EXTERNAL_EEPROM_MEMORY_SIZE_BYTES;
		
		// Write a random byte at this address
		Written_Byte = RandomGenerateNumber();
		I2CWriteByte(Address, Written_Byte);
		
		// Read the memory location to check if the byte was successfully written
		I2CSetCurrentAddress(Address);
		Read_Byte = I2CReadNextByte();
		
		if (Read_Byte != Written_Byte)
		{
			// Compose error message
			ScreenSetColor(SCREEN_COLOR_CODE_TEXT_RED);
			ScreenWriteString("Error at check ");
			ScreenWriteUnsignedInteger(i + 1);
			ScreenWriteCharacter('/');
			ScreenWriteUnsignedInteger(TEST_EXTERNAL_EEPROM_CYCLES_COUNT);
			ScreenWriteString(", address = ");
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