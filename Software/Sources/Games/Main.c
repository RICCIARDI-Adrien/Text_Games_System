/** @file Main.c
 * Initialize hardware and shows the main menu.
 * @author Adrien RICCIARDI
 * @version 1.0 : 01/03/2013
 */
#include <System/System.h>
#include "Brain_Calculation.h"
#include "Demo.h"
#include "Loto.h"
#include "Numbers.h"
#include "Strings.h"
#include "Wumpus.h"

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
		ScreenSetColor(SCREEN_COLOR_CODE_DEFAULT); // Restore the default text and background colors
		ScreenClear();
		ScreenWriteROMString(STRING_MAIN_MENU_TITLE);
		ScreenWriteROMString(STRING_MAIN_MENU_ENTRIES);
				
		// Wait for the user to hit a key
		ScreenWriteROMString(STRING_MAIN_MENU_ASK_USER);
		do
		{
			Key = KeyboardReadCharacter();
		} while ((Key < '1') || (Key > '6'));
		
		// Start chosen game
		ScreenClear();
		switch (Key)
		{
			case '1':
				Numbers();
				break;
			case '2':
				BrainCalculation();
				break;
			case '3':
				Wumpus();
				break;
			case '4':
				Loto();
				break;
			case '5':
				Demo(true);
				break;
			case '6':
				Demo(false);
				break;
		}
	}
}