/** @file Cell.c
 * @see Cell.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Cell.h"
#include "Configuration.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Cell bank size in bytes. */
#define CELL_BANK_SIZE_BYTES (CELL_PACKED_ROW_SIZE_BYTES * 8)

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
// The buffer containing all the world cells must be split in 80-byte arrays in order to fit in the PIC16F876 banked memory
static unsigned char Cell_Bank_1[CELL_BANK_SIZE_BYTES], Cell_Bank_2[CELL_BANK_SIZE_BYTES], Cell_Bank_3[CELL_BANK_SIZE_BYTES];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned char *CellGetWorldRowPointer(unsigned char Row_Number)
{
	unsigned char Row_Bank_Offset, *Pointer_Bank;
	
	// Is the row number valid ?
	if (Row_Number >= CONFIGURATION_WORLD_ROWS_COUNT) return NULL;
	
	// Compute the row offset in a bank
	Row_Bank_Offset = Row_Number % 8; // There are 8 rows in a bank
	Row_Bank_Offset *= CELL_PACKED_ROW_SIZE_BYTES; // Adjust offset to take care of row size
	
	// Looks dirty but we need separate arrays to make it works
	if (Row_Number < CONFIGURATION_WORLD_ROWS_COUNT / 3) Pointer_Bank = Cell_Bank_1;
	else if (Row_Number < (2 * CONFIGURATION_WORLD_ROWS_COUNT) / 3) Pointer_Bank = Cell_Bank_2;
	else Pointer_Bank = Cell_Bank_3;	
	
	return &Pointer_Bank[Row_Bank_Offset];
}

unsigned char CellGetState(unsigned char *Pointer_Row, unsigned char Column_Number)
{
	unsigned char Cell_Byte_Index, Cell_Bit_Mask;
	
	// Is the column number valid ?
	if (Column_Number >= CONFIGURATION_WORLD_COLUMNS_COUNT) return 0;
	
	// Compute cell coordinates in the packed row
	Cell_Byte_Index = Column_Number >> 3; // Column / 8
	Cell_Bit_Mask = Column_Number - (Cell_Byte_Index << 3); // Column % 8
	Cell_Bit_Mask = 0x80 >> Cell_Bit_Mask;
	
	// Retrieve cell state
	if (Pointer_Row[Cell_Byte_Index] & Cell_Bit_Mask) return 1;
	return 0;	
}

void CellSetState(unsigned char *Pointer_Row, unsigned char Column_Number, unsigned char State)
{
	unsigned char Cell_Byte_Index, Cell_Bit_Mask;
	
	// Is the column number valid ?
	if (Column_Number >= CONFIGURATION_WORLD_COLUMNS_COUNT) return;
	
	// Compute cell coordinates in the packed row
	Cell_Byte_Index = Column_Number >> 3; // Column / 8
	Cell_Bit_Mask = Column_Number - (Cell_Byte_Index << 3); // Column % 8
	Cell_Bit_Mask = 0x80 >> Cell_Bit_Mask;
	
	// Set the new cell state
	if (State == 1) Pointer_Row[Cell_Byte_Index] |= Cell_Bit_Mask;
	else Pointer_Row[Cell_Byte_Index] &= ~Cell_Bit_Mask;
}

