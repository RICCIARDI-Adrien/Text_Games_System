/** @file Interface.c
 * @see Interface.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Interface.h"
#include "Math.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Convert a string UNSIGNED representation of a number into the corresponding UNSIGNED binary representation.
 * @param String The string to convert.
 * @param Pointer_Output_Number On output, hold the binary value of the string.
 * @warning User must ensure that the string can't contain more digits than the binary representation can hold, as no check is done by this function.
 */
inline void InterfaceConvertStringToNumber(unsigned char *String, TNumber *Pointer_Output_Number)
{
	TNumber Number_Temp_1, Number_Temp_Shifted_By_2;
	unsigned char i;
	
	// Initialize variables
	MathClearNumber(&Number_Temp_1);
	MathClearNumber(Pointer_Output_Number);
	
	while (*String != 0)
	{
		// Multiply the previous result by 10 (as entered number is in base 10)
		// Left shift by 3 to multiply the number by 8
		MathLeftShift(Pointer_Output_Number, &Number_Temp_1);
		MathCopyNumber(&Number_Temp_1, &Number_Temp_Shifted_By_2); // Keep the number << 1 to avoid recalculating it
		MathLeftShift(&Number_Temp_1, Pointer_Output_Number); // Can't use the same operand as source and destination
		MathLeftShift(Pointer_Output_Number, &Number_Temp_1); // Can't use the same operand as source and destination
		// Add the two numbers to get it multiplied by 10
		MathAdd(&Number_Temp_1, &Number_Temp_Shifted_By_2, Pointer_Output_Number); // Can't use the same operand as source and destination
		
		// Add the most significant digit of the string to the result
		MathClearNumber(&Number_Temp_1);
		Number_Temp_1.Bytes[0] = *String - 48;
		MathAdd(Pointer_Output_Number, &Number_Temp_1, Pointer_Output_Number);
		
		String++;
	}
}

/** Convert a number into it's corresponding string representation.
 * @param Pointer_Number The number to convert.
 * @param String_Output The string where to store the converted representation.
 * @warning Be sure that the string is large enough to hold all number digits.
 */
inline void InterfaceConvertNumberToString(TNumber *Pointer_Number, unsigned char *String_Output)
{
	TNumber Number_Zero, Number_Ten, Number_Divided_By_Ten, Number_Temp;
	unsigned char i = 0, j = 0, Loops_Count, Temp;
	
	// Initialize variables
	MathClearNumber(&Number_Zero);
	MathClearNumber(&Number_Ten);
	Number_Ten.Bytes[0] = 10;
	
	// Handle zero special case
	if (MathCompare(Pointer_Number, &Number_Zero) == 0)
	{
		String_Output[0] = '0';
		String_Output[1] = 0;
		return;
	}
	
	// Retrieve digits from binary number
	while (MathCompare(Pointer_Number, &Number_Zero) > 0)
	{
		// Divide number by ten to extract units value
		MathDivide(Pointer_Number, &Number_Ten, &Number_Divided_By_Ten, &Number_Temp);
		MathCopyNumber(&Number_Divided_By_Ten, &Number_Temp);
		
		// Get last number digit using the following calculation : Digit = Number - Divided_By_Ten * 10 => keep only the last digit value
		MathMultiply(&Number_Temp, &Number_Ten, &Number_Temp);
		MathComplementNumber(&Number_Temp, &Number_Temp);
		MathAdd(Pointer_Number, &Number_Temp, &Number_Temp);
		String_Output[i] = Number_Temp.Bytes[0] + 48;
		
		MathCopyNumber(&Number_Divided_By_Ten, Pointer_Number);
		i++;
	}
	// Terminate string
	String_Output[i] = 0;
	
	// Reverse digits as they have been found in the wrong order
	Loops_Count = i >> 1; // We need String_Size / 2 loops to invert the string
	i--; // Last offset of the string is String_Size - 1
	
	while (Loops_Count > 0)
	{
		Temp = String_Output[i];
		String_Output[i] = String_Output[j];
		String_Output[j] = Temp;
		i--;
		j++;
		Loops_Count--;
	}
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned char InterfaceReadNumber(TNumber *Pointer_Number, unsigned char Is_Last_Operand_Allowed, TOperationType *Pointer_Operation_Type)
{
	unsigned char i = 0, Key, String_Number[MATH_OPERAND_NUMBERS_WIDTH_DIGITS + 1], Is_Number_Negative = 0, Is_Last_Operand;
	TOperationType Operation_Type;
	
	while (1)
	{
		// Wait for a key
		Key = KeyboardReadCharacter();
		
		// Handle key action
		switch (Key)
		{
			// Validate number (only if it is the last of the operation)
			case '\r':
			case '\n':
				if (Is_Last_Operand_Allowed && (i > 0)) // Can't accept an empty number
				{
					Is_Last_Operand = 1;
					goto Convert_Number;
				}
				break;
				
			// Erase the last entered character
			case '\b':
				// Remove the sign, which is not part of the string
				if (Is_Number_Negative && (i == 0))
				{
					// Erase character on screen
					ScreenWriteCharacter('\b');
					ScreenWriteCharacter(' ');
					ScreenWriteCharacter('\b');
					Is_Number_Negative = 0;
					break;
				}
				
				// Can't erase a non existing character
				if (i == 0) break;
				
				// Erase character on screen
				ScreenWriteCharacter('\b');
				ScreenWriteCharacter(' ');
				ScreenWriteCharacter('\b');
				
				// Erase character in string
				i--;
				String_Number[i] = 0;
				break;
				
			// Negative sign or subtraction operator
			case '-':
				// The sign must be the first character of the string
				if (i == 0)
				{
					// Show sign
					ScreenWriteCharacter('-');
					Is_Number_Negative = 1;
				}
				// This is the subtraction operator
				else
				{
					*Pointer_Operation_Type = OPERATION_TYPE_SUBSTRACTION;
					Is_Last_Operand = 0;
					goto Show_Operation;
				}
				break;
				
			// Addition operator
			case '+':
				if (i > 0) // // Can't accept an empty number
				{
					*Pointer_Operation_Type = OPERATION_TYPE_ADDITION;
					Is_Last_Operand = 0;
					goto Show_Operation;
				}
				break;
				
			// Multiplication operator
			case '*':
				if (i > 0) // // Can't accept an empty number
				{
					*Pointer_Operation_Type = OPERATION_TYPE_MULTIPLICATION;
					Is_Last_Operand = 0;
					goto Show_Operation;
				}
				break;
				
			// Division operator
			case '/':
				if (i > 0) // // Can't accept an empty number
				{
					*Pointer_Operation_Type = OPERATION_TYPE_DIVISION;
					Is_Last_Operand = 0;
					goto Show_Operation;
				}
				break;
				
			default:
				// Only digits are allowed
				if ((Key < '0') || (Key > '9')) break;
				// Can't write more characters than allowed
				if (i >= MATH_OPERAND_NUMBERS_WIDTH_DIGITS) break;
				
				// Show character
				ScreenWriteCharacter(Key);
				
				// Add character to string
				String_Number[i] = Key;
				i++;
				break;
		}
	}
	
Show_Operation:
	ScreenWriteCharacter('\r');
	ScreenWriteCharacter('\n');
	ScreenSetColor(SCREEN_COLOR_CODE_TEXT_GREEN);
	ScreenWriteCharacter(Key);
	ScreenSetColor(SCREEN_COLOR_CODE_DEFAULT);
	
Convert_Number:
	// Convert string to number
	String_Number[i] = 0;
	InterfaceConvertStringToNumber(String_Number, Pointer_Number);
					
	// Handle negative numbers
	if (Is_Number_Negative) MathComplementNumber(Pointer_Number, Pointer_Number);
	
	ScreenWriteCharacter('\r');
	ScreenWriteCharacter('\n');
	
	return Is_Last_Operand;
}

void InterfaceDisplayResult(TNumber *Pointer_Number)
{
	unsigned char String[MATH_RESULT_NUMBERS_WIDTH_DIGITS + 1];
	
	ScreenWriteString("= ");
	// Show a colored result
	ScreenSetColor(SCREEN_COLOR_CODE_TEXT_YELLOW);
	
	// Only positive numbers can be converted
	if (MathIsNumberNegative(Pointer_Number))
	{
		MathComplementNumber(Pointer_Number, Pointer_Number);
		ScreenWriteCharacter('-');
	}	
	
	InterfaceConvertNumberToString(Pointer_Number, String);
	ScreenWriteString(String);
	ScreenSetColor(SCREEN_COLOR_CODE_DEFAULT);
	
	ScreenWriteCharacter('\r');
	ScreenWriteCharacter('\n');
}