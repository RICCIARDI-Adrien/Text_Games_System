/** @file Interface.h
 * User interface helpers.
 * @author Adrien RICCIARDI
 * @version 1.0 : 20/10/2013
 */
#ifndef H_INTERFACE_H
#define H_INTERFACE_H

#include "Math.h"

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** Tell what operation the user chose. */
typedef enum
{
	OPERATION_TYPE_ADDITION,
	OPERATION_TYPE_SUBSTRACTION,
	OPERATION_TYPE_MULTIPLICATION,
	OPERATION_TYPE_DIVISION
} TOperationType;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Read a number from the keyboard and convert it to binary form.
 * @param Pointer_Number On output, will hold the binary representation of the number.
 * @param Is_Last_Operand_Allowed Set to 1 to allow to validate the number by pressing the Enter key, set to 0 to force the number to be validated only by typing an operator symbol.
 * @param Pointer_Operation_Type On output AND if the number is not the last of the operation, will contain the operation type. If the number is the last of the operation, discard this value.
 * @return 0 if the operand is not the last of the operation (i.e. the number insertion was terminated by pressing an operator key, like '+' or '*'),
 * @return 1 if the operand is the last of the operation (i.e. the number insertion was terminated by pressing the Enter key).
 */
unsigned char InterfaceReadNumber(TNumber *Pointer_Number, unsigned char Is_Last_Operand_Allowed, TOperationType *Pointer_Operation_Type);

/** Display a number on the screen with a preceding '=' and colors.
 * @param Pointer_Number The number to display.
 */
void InterfaceDisplayResult(TNumber *Pointer_Number);

#endif