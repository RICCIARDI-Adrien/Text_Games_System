/** @file Brain_Calculation.c
 * @see Brain_Calculation.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Brain_Calculation.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Hold selected difficulty level. */
static bool Is_Difficulty_Normal;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Return a random number in the range [Minimum ; Maximum].
 * @param Minimum The minimum value the number can take.
 * @param Maximum The maximum value the number can take.
 * @return The random number.
 */
static signed short RandomNumberRange(signed short Minimum, signed short Maximum)
{
	signed short Number;
	
	do
	{
		// Create a 16-bit integer from two 8-bit values
		Number = (RandomGenerateNumber() << 8) | RandomGenerateNumber();
		Number %= Maximum;
	}
	while (Number < Minimum);
	return Number;
}

/** Choose a calculus according to current difficulty level. The result is always positive.
 * @param First_Number The first number of the calculus.
 * @param Operator The operator (+, -, *).
 * @param Second_Number The second number.
 * @param Result The result of the chosen calculus.
 */
inline void ChooseCalculus(signed short *First_Number, unsigned char *Operator, signed short *Second_Number, signed short *Result)
{
	do
	{
		// Choose numbers according to difficulty level
		if (Is_Difficulty_Normal)
		{
			*First_Number = RandomNumberRange(10, 99);
			*Second_Number = RandomNumberRange(10, 99);
		}
		// Easy
		else
		{
			*First_Number = RandomNumberRange(0, 9);
			*Second_Number = RandomNumberRange(0, 9);
		}
		
		// Choose operator and compute calculus
		switch (RandomGenerateNumber() % 3)
		{
			case 0:
				*Operator = '+';
				*Result = *First_Number + *Second_Number;
				break;
			case 1:
				*Operator = '-';
				*Result = *First_Number - *Second_Number;
				break;
			case 2:
				*Operator = '*';
				*Result = *First_Number * *Second_Number;
				break;
		}
	} while (*Result < 0);		
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void BrainCalculation(void)
{
	signed short First_Number, Second_Number, Result, Player_Number;
	unsigned char Correct_Results_Count = 0, Operator, String_Player_Number[MAXIMUM_DIGITS_COUNT + 1];
	
	// Show title
	ScreenWriteROMString(STRING_BRAIN_CALCULATION_TITLE);
	
	// Choose difficulty level
	ScreenWriteROMString(STRING_BRAIN_CALCULATION_CHOOSE_DIFFICULTY);
	// Wait for user's choice (recycle Operator variable)
	do
	{
		Operator = KeyboardReadCharacter();
		// Exit program if Escape key is hit
		if (Operator == 27) return;
	} while ((Operator != 'f') && (Operator != 'm'));
	// Set difficulty
	if (Operator == 'm') Is_Difficulty_Normal = true;
	else Is_Difficulty_Normal = false;
	
	ScreenWriteROMString(STRING_GENERIC_NEW_LINE);
	ScreenWriteROMString(STRING_GENERIC_NEW_LINE);
	
	RandomInitializeGenerator();
	
	while (1)
	{
		// Show calculus
		ChooseCalculus(&First_Number, &Operator, &Second_Number, &Result);
		ScreenWriteUnsignedInteger(First_Number);
		ScreenWriteCharacter(' ');
		ScreenWriteCharacter(Operator);
		ScreenWriteCharacter(' ');
		ScreenWriteUnsignedInteger(Second_Number);
		ScreenWriteCharacter(' ');
		ScreenWriteCharacter('=');
		ScreenWriteCharacter(' ');
				
		// Get user's number
		if (!KeyboardReadString(String_Player_Number, MAXIMUM_DIGITS_COUNT, true))
		{
			// Show correct answers count
			ScreenWriteROMString(STRING_BRAIN_CALCULATION_CORRECT_ANSWERS);
			ScreenWriteUnsignedInteger(Correct_Results_Count);
			ScreenWriteROMString(STRING_GENERIC_NEW_LINE);
			KeyboardReadCharacter();
			return;
		}
		Player_Number = atoi(String_Player_Number);
		
		// Check if user's result is correct
		if (Player_Number == Result)
		{
			ScreenWriteROMString(STRING_BRAIN_CALCULATION_GOOD_RESULT);
			Correct_Results_Count++;
			
			// End game if enough answers were correct
			if (Correct_Results_Count == BRAIN_CALCULATION_WINNING_ANSWERS_COUNT)
			{
				ScreenWriteROMString(STRING_BRAIN_CALCULATION_PLAYER_WIN);
				KeyboardReadCharacter();
				return;
			}
		}
		else
		{
			ScreenWriteROMString(STRING_BRAIN_CALCULATION_BAD_RESULT);
			ScreenWriteUnsignedInteger(Result);
			
			// Show correct answers count
			ScreenWriteROMString(STRING_BRAIN_CALCULATION_CORRECT_ANSWERS);
			ScreenWriteUnsignedInteger(Correct_Results_Count);
			ScreenWriteROMString(STRING_GENERIC_NEW_LINE);
			KeyboardReadCharacter();
			return;
		}
	}
}
