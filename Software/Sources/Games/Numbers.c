/** @file Numbers.c
 * @see Numbers.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Numbers.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Find a random number.
 * @param String The string in which to store the random number. It must be NUMBER_DIGITS_COUNT + 1 wide.
 * @note The number is NUMBER_DIGITS_COUNT digits wide.
 */
inline void FindRandomNumber(unsigned char *String)
{
	unsigned char i, Random_Number;
	
	for (i = 0; i < NUMBER_DIGITS_COUNT; i++)
	{
		Random_Number = RandomGenerateNumber() % 10;
		String[i] = Random_Number + 48; // Convert number to ASCII representation
	}
	// Terminate string
	String[NUMBER_DIGITS_COUNT] = 0;
}

/** Shift a string to the right and pad with ASCII zeros.
 * @param String The string to shift.
 * @param Amount How many characters to shift to the right.
 */
inline void ShiftStringRight(unsigned char *String, unsigned char Amount)
{
	unsigned char i, String_Length;
	
	String_Length = strlen(String);
	
	// Shift string
	for (i = String_Length - 1; i != 0xFF; i--) String[i + Amount] = String[i]; // 0xFF = -1
	
	// Pad beginning digits with ASCII zeros
	for (i = 0; i < Amount; i++) String[i] = '0'; 
	
	// Terminate string
	String[String_Length + Amount] = 0;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void Numbers(void)
{
	unsigned char String_Player_Number[NUMBER_DIGITS_COUNT + 1], String_Computer_Number[NUMBER_DIGITS_COUNT + 1], Player_Attempts = 1, i;

	// Show title
	ScreenWriteROMString(STRING_NUMBERS_TITLE);
			
	// Find number
	RandomInitializeGenerator();
	FindRandomNumber(String_Computer_Number);
	
	while (1)
	{
		// Get player number
		ScreenWriteROMString(STRING_NUMBERS_ASK_NUMBER);
		if (!KeyboardReadString(String_Player_Number, NUMBER_DIGITS_COUNT, true)) return;
		
		// Shift string to align with computer string. So it is possible to enter a number like "123" without having to enter "00123".
		if (strlen(String_Player_Number) < NUMBER_DIGITS_COUNT) ShiftStringRight(String_Player_Number, NUMBER_DIGITS_COUNT - strlen(String_Player_Number));
		
		// Does player found the number ?
		for (i = 0; i < NUMBER_DIGITS_COUNT; i++)
		{
			// Too small
			if (String_Player_Number[i] < String_Computer_Number[i])
			{
				ScreenWriteROMString(STRING_NUMBERS_TOO_SMALL);
				Player_Attempts++;
				break;
			}
			
			// Too large
			if (String_Player_Number[i] > String_Computer_Number[i])
			{
				ScreenWriteROMString(STRING_NUMBERS_TOO_LARGE);
				Player_Attempts++;
				break;
			}
		}
		
		// Does player loose ?
		if (Player_Attempts >= NUMBERS_MAXIMUM_ATTEMPTS)
		{
			ScreenWriteROMString(STRING_NUMBERS_PLAYER_LOOSE);
			
			// Show attempts
			ScreenWriteROMString(STRING_NUMBERS_ATTEMPTS);
			ScreenWriteUnsignedInteger(Player_Attempts);
			ScreenWriteROMString(STRING_GENERIC_NEW_LINE);
			
			// Show number
			ScreenWriteROMString(STRING_NUMBERS_NUMBER_VALUE);
			ScreenWriteString(String_Computer_Number);
			ScreenWriteROMString(STRING_GENERIC_NEW_LINE);
			
			KeyboardReadCharacter();
			return;
		}
		
		// Does player win ?
		if (i == NUMBER_DIGITS_COUNT)
		{
			ScreenWriteROMString(STRING_NUMBERS_PLAYER_WIN);
			
			// Show attempts
			ScreenWriteROMString(STRING_NUMBERS_ATTEMPTS);
			ScreenWriteUnsignedInteger(Player_Attempts);
			ScreenWriteROMString(STRING_GENERIC_NEW_LINE);
			
			KeyboardReadCharacter();
			return;
		}
	}
}