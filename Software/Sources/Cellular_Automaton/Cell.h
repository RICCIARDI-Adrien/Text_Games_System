/** @file Cell.h
 * Cell access and modification routines.
 * @author Adrien RICCIARDI
 * @version 1.0 : 12/03/2014
 */
#ifndef H_CELL_H
#define H_CELL_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** A packed row size in bytes (in a packed row a cell state is represented by 1 bit). */
#define CELL_PACKED_ROW_SIZE_BYTES (CONFIGURATION_WORLD_COLUMNS_COUNT / 8)

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Get a pointer on the specified world cells row.
 * @param Row_Number The row coordinate of the the cells row.
 * @return A pointer on the row data,
 * @return NULL if the provided Row_Number is greater than CONFIGURATION_WORLD_ROWS_COUNT
 */
unsigned char *CellGetWorldRowPointer(unsigned char Row_Number);

/** Get the state of a cell in a packed row.
 * @param Pointer_Row The packed row containing the cell.
 * @param Column_Number The column coordinate of the cell.
 * @return 1 if the cell is alive,
 * @return 0 if the cell is dead or if the provided column number is greater than CONFIGURATION_WORLD_COLUMNS_COUNT.
 */
unsigned char CellGetState(unsigned char *Pointer_Row, unsigned char Column_Number);

/** Set the state of a cell in a packed row.
 * @param Pointer_Row The packed row containing the cell.
 * @param Column_Number The column coordinate of the cell.
 * @param The new state of the cell (1 = alive, 0 = dead).
 */
void CellSetState(unsigned char *Pointer_Row, unsigned char Column_Number, unsigned char State);

#endif