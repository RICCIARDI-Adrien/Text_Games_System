/** @file Main.c
 * Compute simple operations.
 * @author Adrien RICCIARDI
 * @version 1.0 : 20/10/2013
 * @version 1.1 : 08/09/2014, speeded up the InterfaceConvertStringToNumber() function, improved the interface (it is now possible to reuse the last result as the first operand for the next calculation) and handled multiplications overflow.
 */
#include <System/System.h>
#include "Interface.h"
#include "Math.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	TNumber Number_Operand_1, Number_Operand_2, Number_Result, Number_Remainder, Number_Maximum_Value;
	TOperationType Current_Operation_Type, Next_Operation_Type;
	unsigned char Is_Operand_1_Negative, Is_Operand_2_Negative, Negative_Signs_Count, Is_Last_Operand;
	
	// Initialize the system
	SystemInitialize();
	MathComputeMaximumValue(&Number_Maximum_Value);
	
	ScreenWriteString(STRING_TITLE);
	
Start:
	while (1)
	{
		// Read the first one operand (to be sure there are always almost 2 operands per operation)
		InterfaceReadNumber(&Number_Operand_1, 0, &Current_Operation_Type);
		Is_Operand_1_Negative = MathIsNumberNegative(&Number_Operand_1);
			
		// Read the other operands until the last one is inserted
		do
		{
			Is_Last_Operand = InterfaceReadNumber(&Number_Operand_2, 1, &Next_Operation_Type);
			Is_Operand_2_Negative = MathIsNumberNegative(&Number_Operand_2);
		
			// Do the selected operation
			SystemLedOn();
			switch (Current_Operation_Type)
			{
				case OPERATION_TYPE_ADDITION:
					MathAdd(&Number_Operand_1, &Number_Operand_2, &Number_Result);
					break;
					
				case OPERATION_TYPE_SUBSTRACTION:
					MathComplementNumber(&Number_Operand_2, &Number_Operand_2);
					MathAdd(&Number_Operand_1, &Number_Operand_2, &Number_Result);
					break;
					
				case OPERATION_TYPE_MULTIPLICATION:
					// Convert negative numbers to positive ones as the multiplication is unsigned
					Negative_Signs_Count = 0;
					// Check operand 1 sign
					if (Is_Operand_1_Negative)
					{
						MathComplementNumber(&Number_Operand_1, &Number_Operand_1);
						Negative_Signs_Count++;
					}
					// Check operand 2 sign
					if (Is_Operand_2_Negative)
					{
						MathComplementNumber(&Number_Operand_2, &Number_Operand_2);
						Negative_Signs_Count++;
					}
					
					// Check for overflow using the following formula : a * b > MAX => a > MAX / b
					MathDivide(&Number_Maximum_Value, &Number_Operand_2, &Number_Result, &Number_Remainder);
					if (MathCompare(&Number_Operand_1, &Number_Result) > 0)
					{
						ScreenSetColor(SCREEN_COLOR_CODE_TEXT_RED);
						ScreenWriteString(STRING_MULTIPLICATION_OVERFLOW);
						ScreenSetColor(SCREEN_COLOR_CODE_DEFAULT);
						goto Start; // Abort every operation
					}
					
					MathMultiply(&Number_Operand_1, &Number_Operand_2, &Number_Result);
						
					// Convert result to negative if needed
					if (Negative_Signs_Count == 1) MathComplementNumber(&Number_Result, &Number_Result);
					break;
					
				case OPERATION_TYPE_DIVISION:
					// Convert negative numbers to positive ones as the division is unsigned
					Negative_Signs_Count = 0;
					// Check operand 1 sign
					if (Is_Operand_1_Negative)
					{
						MathComplementNumber(&Number_Operand_1, &Number_Operand_1);
						Negative_Signs_Count++;
					}
					
					// Is the denominator equal to zero ?
					MathClearNumber(&Number_Result); // Recycle Number_Result
					if (MathCompare(&Number_Operand_2, &Number_Result) == 0)
					{
						ScreenSetColor(SCREEN_COLOR_CODE_TEXT_RED);
						ScreenWriteString(STRING_DIVISION_BY_ZERO);
						ScreenSetColor(SCREEN_COLOR_CODE_DEFAULT);
						goto Start; // Abort every operation
					}
					
					// Check operand 2 sign
					if (Is_Operand_2_Negative)
					{
						MathComplementNumber(&Number_Operand_2, &Number_Operand_2);
						Negative_Signs_Count++;
					}
					
					MathDivide(&Number_Operand_1, &Number_Operand_2, &Number_Result, &Number_Remainder);
						
					// Convert result to negative if needed
					if (Negative_Signs_Count == 1) MathComplementNumber(&Number_Result, &Number_Result);
					break;
			}
			SystemLedOff();
			
			// Show the result ?
			if (Is_Last_Operand)
			{
				if (Current_Operation_Type == OPERATION_TYPE_DIVISION)
				{
					ScreenWriteString(STRING_DIVISION_QUOTIENT);
					InterfaceDisplayResult(&Number_Result);
					ScreenWriteString(STRING_DIVISION_REMAINDER);
					InterfaceDisplayResult(&Number_Remainder);
				}
				else InterfaceDisplayResult(&Number_Result);
				
				ScreenWriteCharacter('\r');
				ScreenWriteCharacter('\n');
			}
			// Continue the operations
			else
			{
				MathCopyNumber(&Number_Result, &Number_Operand_1); // Set the current result as the new first operand
				Is_Operand_1_Negative = MathIsNumberNegative(&Number_Operand_1);
				Current_Operation_Type = Next_Operation_Type;
			}			
		} while (!Is_Last_Operand);
	}
}