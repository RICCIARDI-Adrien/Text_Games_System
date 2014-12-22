/** @file Loto.c
 * @see Loto.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Loto.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private macros
//-------------------------------------------------------------------------------------------------
/** Convert a macro name into a string. */
#define MACRO_TO_STRING(X) #X
/** Convert a macro value into a string. */
#define VALUE_TO_STRING(X) MACRO_TO_STRING(X)

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Hold all the player and result numbers. */
static unsigned char Player_Numbers[LOTO_PLAYER_NUMBERS_COUNT], Result_Numbers[LOTO_PLAYER_NUMBERS_COUNT];

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Tell whether the specified number is present in the array.
 * @param Pointer_Numbers_Array The array to search into.
 * @param Number The number to check presence.
 * @return true if the number is present or false if not.
 */
static bool IsNumberPresent(unsigned char *Pointer_Numbers_Array, unsigned char Number)
{
	unsigned char i;
	
	for (i = 0; i < LOTO_PLAYER_NUMBERS_COUNT; i++)
	{
		if (Number == *Pointer_Numbers_Array) return true;
		Pointer_Numbers_Array++;
	}
	return false;
}

/** Get the player's bet numbers.
 * @return false if the player wanted to exit program or true if he wanted to continue.
 */
inline bool ReadPlayerNumbers(void)
{
	unsigned char i, j, Number, String_Player_Number[3];

	// Reset numbers
	bzero(Player_Numbers, LOTO_PLAYER_NUMBERS_COUNT); // 0 is not an allowed number
	
	// Ask for numbers
	i = 0;
	do
	{
		// Get a number
		ScreenWriteROMString(STRING_LOTO_INSERT_NUMBER_1);
		ScreenWriteUnsignedInteger(i + 1);
		ScreenWriteString("/" VALUE_TO_STRING(LOTO_PLAYER_NUMBERS_COUNT));
		ScreenWriteROMString(STRING_LOTO_INSERT_NUMBER_2);
		if (!KeyboardReadString(String_Player_Number, 2, true)) return false;
		Number = atoi(String_Player_Number);
		
		// Check if the number is in range
		if ((Number < LOTO_MINIMUM_NUMBER_VALUE) || (Number > LOTO_MAXIMUM_NUMBER_VALUE))
		{
			ScreenWriteROMString(STRING_LOTO_NUMBER_NOT_IN_RANGE);
			continue;
		}
		
		// Check if this number has been entered yet
		if (IsNumberPresent(Player_Numbers, Number))
		{
			ScreenWriteROMString(STRING_LOTO_NUMBER_ENTERED_YET);
			continue;
		}
		Player_Numbers[i] = Number;
		i++;
	} while (i < LOTO_PLAYER_NUMBERS_COUNT);
	
	ScreenWriteROMString(STRING_GENERIC_NEW_LINE);
	return true;
}

/** Pick result random numbers. */
inline void PickResultNumbers(void)
{
	unsigned char i, Number;
	
	// Reset numbers
	bzero(Result_Numbers, LOTO_PLAYER_NUMBERS_COUNT); // 0 is not an allowed number
	
	// Pick all the numbers
	for (i = 0; i < LOTO_PLAYER_NUMBERS_COUNT; i++)
	{
		// Each number must appear only one time
		do
		{
			Number = (RandomGenerateNumber() % LOTO_MAXIMUM_NUMBER_VALUE) + LOTO_MINIMUM_NUMBER_VALUE;
		} while (IsNumberPresent(Result_Numbers, Number));
		Result_Numbers[i] = Number;
	}
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void Loto(void)
{
	unsigned char i, Numbers_Found = 0;
	
	// Show title
	ScreenClear();
	ScreenWriteROMString(STRING_LOTO_TITLE);
	
	RandomInitializeGenerator();
	
	// Get player's numbers
	if (!ReadPlayerNumbers()) return;
	
	// Choose random numbers
	PickResultNumbers();
	
	// Compute and show results
	ScreenWriteROMString(STRING_LOTO_SHOW_DRAWN_NUMBERS);
	for (i = 0; i < LOTO_PLAYER_NUMBERS_COUNT; i++)
	{
		// Is the number found ?
		if (IsNumberPresent(Player_Numbers, Result_Numbers[i]))
		{
			Numbers_Found++;
			ScreenSetColor(SCREEN_COLOR_CODE_TEXT_GREEN);
		}
		else ScreenSetColor(SCREEN_COLOR_CODE_TEXT_WHITE);
		
		// Show the number
		ScreenWriteUnsignedInteger(Result_Numbers[i]);
		ScreenWriteCharacter(' ');
	}
	
	// Show results
	ScreenWriteROMString(STRING_LOTO_NUMBERS_FOUND_COUNT_1);
	ScreenWriteUnsignedInteger(Numbers_Found);
	ScreenWriteROMString(STRING_LOTO_NUMBERS_FOUND_COUNT_2);
	
	// Wait for player to quit
	KeyboardReadCharacter();
}
