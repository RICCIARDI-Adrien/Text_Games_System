/** @file Main.c
 * Show a menu presenting the available tests.
 * @author Adrien RICCIARDI
 * @version 1.0 : 01/04/2013
 */
#include <System/System.h>
#include "Test_Clock_Speed.h"
#include "Test_External_EEPROM.h"
#include "Test_Internal_EEPROM.h"
#include "Test_Random.h"
#include "Test_Screen_Speed.h"

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	unsigned char Key;
	
	// Initialize hardware
	SystemInitialize();
	SystemLedOn();
	
	while (1)
	{
		// Show main menu
		ScreenClear();
		ScreenWriteString("--- Tests menu ---\r\n\r\n");
		ScreenWriteString("    1. Random numbers generator\r\n");
		ScreenWriteString("    2. Screen printing speed\r\n");
		ScreenWriteString("    3. Clock frequency\r\n");
		ScreenWriteString("    4. External EEPROM\r\n");
		ScreenWriteString("    5. Internal EEPROM\r\n");
						
		// Wait for the user to hit a key
		do
		{
			Key = KeyboardReadCharacter();
		} while ((Key < '1') || (Key > '5'));
		
		// Start chosen test
		ScreenClear();
		switch (Key)
		{
			case '1':
				TestRandom();
				break;
			case '2':
				TestScreenSpeed();
				break;
			case '3':
				TestClockSpeed();
				break;
			case '4':
				TestExternalEEPROM();
				break;
			case '5':
				TestInternalEEPROM();
				break;
		}
	}
}