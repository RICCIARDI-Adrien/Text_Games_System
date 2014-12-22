/** @file Math.c
 * @see Math.h for description.
 * @author Adrien RICCIARDI
 */
#include <PIC16F876.h> // The inline assembler needs the exact processor definition
#include <System/System.h>
#include "Math.h"

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Check if a bit is set or no.
 * @param Pointer_Number The number to check a bit.
 * @param Bit_Index Index of the bit.
 * @return 1 if the bit is set or 0 if not.
 */
inline unsigned char MathIsBitSet(TNumber *Pointer_Number, unsigned char Bit_Index)
{
	unsigned char Byte_Index;
	
	// Find the byte containing the concerned bit by dividing by 8
	Byte_Index = Bit_Index >> 3;
	if (Byte_Index >= MATH_NUMBERS_WIDTH_BYTES) return 0; // A non-existing bit is considered as 0
	Bit_Index -= Byte_Index << 3; // Get the remainder, which is the bit index in the byte
	
	// Create a bit mask with the index
	Bit_Index = 1 << Bit_Index; // Recycle Bit_Index variable
	
	if (Pointer_Number->Bytes[Byte_Index] & Bit_Index) return 1;
	return 0;
}

/** Set or clear a bit of a number.
 * @param Pointer_Number The number to modify.
 * @param Bit_Index The bit to modify.
 * @param Is_Bit_Set The value to affect to the bit.
 */
inline void MathSetBitValue(TNumber *Pointer_Number, unsigned char Bit_Index, unsigned char Is_Bit_Set)
{
	unsigned char Byte_Index;
	
	// Find the byte containing the concerned bit by dividing by 8
	Byte_Index = Bit_Index >> 3;
	if (Byte_Index >= MATH_NUMBERS_WIDTH_BYTES) return; // Do nothing if the bit is not existing
	Bit_Index -= Byte_Index << 3; // Get the remainder, which is the bit index in the byte
	Bit_Index = 1 << Bit_Index; // Create a bit mask with the index (recycle Bit_Index variable)
	
	// Affect bit value
	if (Is_Bit_Set) Pointer_Number->Bytes[Byte_Index] |= Bit_Index;
	else Pointer_Number->Bytes[Byte_Index] &= ~Bit_Index;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
/** Left shift a number by one bit.
 * @param Pointer_Number The number to shift.
 * @param Pointer_Number_Output The number where to store the result.
 */
void MathLeftShift(TNumber *Pointer_Number, TNumber *Pointer_Number_Output)
{
	unsigned char i, Leftmost_Bit_Value = 0, Byte;
	
	for (i = 0; i < MATH_NUMBERS_WIDTH_BYTES; i++)
	{
		Byte = Pointer_Number->Bytes[i];
		
		asm
		{
			// Left shift by one bit (most significant bit is stored in carry flag)
			rlf _Byte, W
			// Fill least significant bit with previous rank value
			iorwf _Leftmost_Bit_Value, W // Do not affect carry flag
			// Store shifted byte in temporary variable
			movwf _Byte // Do not affect carry flag
			
			// Store leftmost bit of this rank
			btfss _status, C
			goto Clear_Bit
			
		Set_Bit:
			bsf _Leftmost_Bit_Value, 0
			goto End
			
		Clear_Bit:
			bcf _Leftmost_Bit_Value, 0
			
		End:
		}
		
		Pointer_Number_Output->Bytes[i] = Byte;
	}
}

void MathComplementNumber(TNumber *Pointer_Number, TNumber *Pointer_Number_Output)
{
	unsigned char i;
	TNumber Number_One;
	
	// Invert bits
	for (i = 0; i < MATH_NUMBERS_WIDTH_BYTES; i++) Pointer_Number_Output->Bytes[i] = ~Pointer_Number->Bytes[i];
	
	// Add 1
	MathClearNumber(&Number_One);
	Number_One.Bytes[0] = 1;
	MathAdd(Pointer_Number_Output, &Number_One, Pointer_Number_Output);
}

signed char MathCompare(TNumber *Pointer_Number_1, TNumber *Pointer_Number_2)
{
	unsigned char i, Value_1, Value_2;
	
	for (i = MATH_NUMBERS_WIDTH_BYTES - 1; i != 255; i--) // Start from the most significant byte
	{
		Value_1 = Pointer_Number_1->Bytes[i];
		Value_2 = Pointer_Number_2->Bytes[i];
		
		if (Value_1 < Value_2) return -1;
		if (Value_1 > Value_2) return 1;
	}
	// Numbers are equal
	return 0;
}

unsigned char MathIsNumberNegative(TNumber *Pointer_Number)
{
	// Check most significant bit value
	if (Pointer_Number->Bytes[MATH_NUMBERS_WIDTH_BYTES - 1] & 0x80) return 1;
	return 0;
}

void MathAdd(TNumber *Pointer_Number_1, TNumber *Pointer_Number_2, TNumber *Pointer_Number_Result)
{
	unsigned char i, Operand_1, Operand_2, Carry_Flag = 0;
		
	for (i = 0; i < MATH_NUMBERS_WIDTH_BYTES; i++) // Numbers are stored in little endian
	{
		// "Cache" operands to avoid compiler calculus disturbing the carry flag
		Operand_1 = Pointer_Number_1->Bytes[i];
		Operand_2 = Pointer_Number_2->Bytes[i];
		
		asm
		{
			// Do addition with operands
			movf _Operand_2, W // Initialize accumulator with Operand_2 value
			addwf _Operand_1, F // Add Operand_1 and store result in Operand_1
			
			// Check carry flag
			btfss _status, C
			goto Operands_Addition_Carry_Clear
			
		Operands_Addition_Carry_Set:
			movlw 1
			movwf _Operand_2 // Recycle Operand_2 and set it to 1
			goto Carry_Addition
			
		Operands_Addition_Carry_Clear:
			clrf _Operand_2 // Recycle Operand_2 and set it to 0
			
		// Add previous rank carry to the operands
		Carry_Addition:
			movf _Carry_Flag, W
			addwf _Operand_1, F
			
			// Check carry flag
			btfss _status, C
			goto Carry_Addition_Carry_Clear
			
		Carry_Addition_Carry_Set:
			movlw 1
			movwf _Carry_Flag
			goto End
			
		Carry_Addition_Carry_Clear:
			clrf _Carry_Flag
			
		// OR carries from operands addition and carry addition to be sure that all set carry flags are took into account
		End:
			movf _Operand_2, W
			iorwf _Carry_Flag, F
		}
		
		// Store result
		Pointer_Number_Result->Bytes[i] = Operand_1;
	}
}

void MathMultiply(TNumber *Pointer_Number_1, TNumber *Pointer_Number_2, TNumber *Pointer_Number_Result)
{
	unsigned char i;
	TNumber Number_Accumulator;
	
	MathClearNumber(&Number_Accumulator);
	
	// Need to start from the most significant bit to double the result at each loop
	for (i = (MATH_NUMBERS_WIDTH_BYTES * 8) - 1; i != 255; i--) // i != 255 is equivalent to i >= 0 when using signed numbers
	{
		// Always double the accumulator
		MathLeftShift(&Number_Accumulator, &Number_Accumulator);
		
		// Add value only if the factor bit is set
		if (MathIsBitSet(Pointer_Number_2, i)) MathAdd(&Number_Accumulator, Pointer_Number_1, &Number_Accumulator);
	}
	
	MathCopyNumber(&Number_Accumulator, Pointer_Number_Result);
}

void MathDivide(TNumber *Pointer_Number_Numerator, TNumber *Pointer_Number_Denominator, TNumber *Pointer_Number_Quotient, TNumber *Pointer_Number_Remainder)
{
	TNumber Number_Complemented_Denominator;
	unsigned char i, Bit_Value;
	
	MathClearNumber(Pointer_Number_Quotient);
	MathClearNumber(Pointer_Number_Remainder);
	
	// Pre compute -Denominator to avoid recalculating it every time
	MathComplementNumber(Pointer_Number_Denominator, &Number_Complemented_Denominator);

	// Need to start from the most significant bit
	for (i = (MATH_NUMBERS_WIDTH_BYTES * 8) - 1; i != 255; i--) // i != 255 is equivalent to i >= 0 when using signed numbers
	{
		// Get the 'i' bit of the numerator
		Bit_Value = MathIsBitSet(Pointer_Number_Numerator, i);
		
		// Multiply remainder by 2
		MathLeftShift(Pointer_Number_Remainder, Pointer_Number_Remainder); 
		// Set the last significant bit of the remainder to the value of the numerator 'i' bit
		Pointer_Number_Remainder->Bytes[0] |= Bit_Value; // Little hack to gain better performances than calling MathSetBitValue()
		
		if (MathCompare(Pointer_Number_Remainder, Pointer_Number_Denominator) >= 0)
		{
			// Compute remainder := remainder - denominator
			MathAdd(Pointer_Number_Remainder, &Number_Complemented_Denominator, Pointer_Number_Remainder);
			
			// Set quotient 'i' bit
			MathSetBitValue(Pointer_Number_Quotient, i, 1);
		}
	}
}

void MathComputeMaximumValue(TNumber *Pointer_Number_Output)
{
	unsigned char i;
	
	// Set all bits to 1
	for (i = 0; i < MATH_NUMBERS_WIDTH_BYTES; i++) Pointer_Number_Output->Bytes[i] = 0xFF;
	
	// Reset the sign bit
	Pointer_Number_Output->Bytes[MATH_NUMBERS_WIDTH_BYTES - 1] &= 0x7F;
}
	