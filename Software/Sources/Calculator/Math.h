/** @file Math.h
 * Contain all low level operations.
 * @author Adrien RICCIARDI
 * @version 1.0 : 20/10/2013
 */
#ifndef H_MATH_H
#define H_MATH_H

#include <System/System.h> // Needed by memset(), memcpy()...

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How many bytes are used to represent a number. */
#define MATH_NUMBERS_WIDTH_BYTES 16 // Can hold approximately 38 digits

/** How many digits are allowed when a user is inserting a number. */
#define MATH_OPERAND_NUMBERS_WIDTH_DIGITS 37 //19 // So a big multiplication result can always be stored on a binary number
/** How many digits needed to store a result number. */
#define MATH_RESULT_NUMBERS_WIDTH_DIGITS (MATH_OPERAND_NUMBERS_WIDTH_DIGITS + 1) //(2 * MATH_OPERAND_NUMBERS_WIDTH_DIGITS)

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** A number. They are stored in little endian and always signed. */
typedef struct
{
	unsigned char Bytes[MATH_NUMBERS_WIDTH_BYTES];
} TNumber;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Set a number to zero.
 * @param Pointer_Number The number to clear.
 */
#ifdef DOXYGEN
	void MathClearNumber(TNumber *Pointer_Number);
#else
	#define MathClearNumber(Pointer_Number) memset(&(Pointer_Number)->Bytes, 0, MATH_NUMBERS_WIDTH_BYTES)
#endif

/** Copy a number into another number.
 * @param Pointer_Source_Number The number to copy.
 * @param Pointer_Destination_Number Where to store the copy of the number.
 */
#ifdef DOXYGEN
	void MathCopyNumber(TNumber *Pointer_Source_Number, TNumber *Pointer_Destination_Number);
#else
	#define MathCopyNumber(Pointer_Source_Number, Pointer_Destination_Number) memcpy(&(Pointer_Destination_Number)->Bytes, &(Pointer_Source_Number)->Bytes, MATH_NUMBERS_WIDTH_BYTES)
#endif

/** Do a logical left shift of 1 bit on a number.
 * @param Pointer_Number The number to shift to 1 bit.
 * @param Pointer_Number_Output On output, contain the shifted number.
 */
void MathLeftShift(TNumber *Pointer_Number, TNumber *Pointer_Number_Output);

/** 2-complement of a number (this has the same effect than multiplying the number by -1).
 * @param Pointer_Number The number to complement.
 * @param Pointer_Number_Output On output, contain the complemented number.
 */
void MathComplementNumber(TNumber *Pointer_Number, TNumber *Pointer_Number_Output);

/** Compare two UNSIGNED numbers.
 * @param Pointer_Number_1 The first number.
 * @param Pointer_Number_2 The second number.
 * @return -1 if Number_1 < Number_2,
 * @return 0 if Number_1 = Number_2,
 * @return 1 if Number_1 > Number_2.
 */
signed char MathCompare(TNumber *Pointer_Number_1, TNumber *Pointer_Number_2);

/** Tell if a number is negative or not.
 * @param Pointer_Number The number to check.
 * @return 1 if the number is negative or 0 if the number is positive or equal to zero.
 */
unsigned char MathIsNumberNegative(TNumber *Pointer_Number);

/** Add two signed numbers (so this function can subtract too).
 * @param Pointer_Number_1 The first number to add.
 * @param Pointer_Number_2 The second number to add.
 * @param Pointer_Number_Result On output, store the addition result.
 */
void MathAdd(TNumber *Pointer_Number_1, TNumber *Pointer_Number_2, TNumber *Pointer_Number_Result);

/** Multiply two UNSIGNED numbers.
 * @param Pointer_Number_1 The first operand.
 * @param Pointer_Number_2 The second operand.
 * @param Pointer_Number_Result Where to store the result.
 */
void MathMultiply(TNumber *Pointer_Number_1, TNumber *Pointer_Number_2, TNumber *Pointer_Number_Result);

/** Compute an Euclidean division.
 * @param Pointer_Number_Numerator The numerator.
 * @param Pointer_Number_Denominator The denominator.
 * @param Pointer_Number_Quotient On output, contain the quotient.
 * @param Pointer_Number_Remainder On output, contain the remainder.
 * @warning A division by zero will lead to an erroneous result, make sure that the denominator is different from zero.
 * @warning Be careful to not use an operand parameter as an output parameter or the result will be unpredictable.
 */
void MathDivide(TNumber *Pointer_Number_Numerator, TNumber *Pointer_Number_Denominator, TNumber *Pointer_Number_Quotient, TNumber *Pointer_Number_Remainder);

/** Return the maximum positive value that can be represented on the system.
 * @param Pointer_Number_Output On output, contain the maximum positive value.
 */
void MathComputeMaximumValue(TNumber *Pointer_Number_Output);

#endif