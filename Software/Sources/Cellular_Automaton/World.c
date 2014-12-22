/** @file World.c
 * @see World.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Cell.h"
#include "Configuration.h"
#include "World.h"

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Compute the next generation for the current row using the above and below rows.
 * @param Pointer_Previous_Row The row preceding the one to compute. Set to NULL if there is no preceding row.
 * @param Pointer_Current_Row The row to compute.
 * @param Pointer_Next_Row The row following the one to compute. Set to NULL if there is no following row.
 * @param Pointer_Generated_Row On output, will contain the computed new generation for the current row.
 */
static void WorldComputeNextRowGeneration(unsigned char *Pointer_Previous_Row, unsigned char *Pointer_Current_Row, unsigned char *Pointer_Next_Row, unsigned char *Pointer_Generated_Row)
{
	unsigned char Column_Number, Alive_Neighbor_Cells_Count, Current_Cell_State, New_Cell_State;
	
	for (Column_Number = 0; Column_Number < CONFIGURATION_WORLD_COLUMNS_COUNT; Column_Number++)
	{
		Alive_Neighbor_Cells_Count = 0;
		
		// Compute alive neighbor cells count
		// North cell
		if (Pointer_Previous_Row != NULL) Alive_Neighbor_Cells_Count += CellGetState(Pointer_Previous_Row, Column_Number);
		// South cell
		if (Pointer_Next_Row != NULL) Alive_Neighbor_Cells_Count += CellGetState(Pointer_Next_Row, Column_Number);
	
		// West related cells
		if (Column_Number > 0)
		{
			// North west cell
			if (Pointer_Previous_Row != NULL) Alive_Neighbor_Cells_Count += CellGetState(Pointer_Previous_Row, Column_Number - 1);
			// West cell
			Alive_Neighbor_Cells_Count += CellGetState(Pointer_Current_Row, Column_Number - 1);
			// South west cell
			if (Pointer_Next_Row != NULL) Alive_Neighbor_Cells_Count += CellGetState(Pointer_Next_Row, Column_Number - 1);
		}
		
		// East related cells
		if (Column_Number < CONFIGURATION_WORLD_COLUMNS_COUNT - 1)
		{
			// North east cell
			if (Pointer_Previous_Row != NULL) Alive_Neighbor_Cells_Count += CellGetState(Pointer_Previous_Row, Column_Number + 1);
			// East cell
			Alive_Neighbor_Cells_Count += CellGetState(Pointer_Current_Row, Column_Number + 1);
			// South east cell
			if (Pointer_Next_Row != NULL) Alive_Neighbor_Cells_Count += CellGetState(Pointer_Next_Row, Column_Number + 1);
		}
		
		// Decide new cell state according to Conway's rules
		Current_Cell_State = CellGetState(Pointer_Current_Row, Column_Number);
		if (Alive_Neighbor_Cells_Count == 3) New_Cell_State = 1; // A new cell is born
		else if ((Current_Cell_State == 1) && ((Alive_Neighbor_Cells_Count == 2) || (Alive_Neighbor_Cells_Count == 3))) New_Cell_State = 1; // Keep the cell alive
		else New_Cell_State = 0; // The cell died
		CellSetState(Pointer_Generated_Row, Column_Number, New_Cell_State);
		
		// Display the new cell state in the same time to fasten drawing
		if (New_Cell_State) ScreenWriteCharacter('O');
		else ScreenWriteCharacter(' ');
	}
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void WorldCreate(void)
{
	unsigned char *Pointer_Row, Row_Number, Column_Number, Cell_State;
	
	SystemLedOn();
	
	RandomInitializeGenerator();
	
	for (Row_Number = 0; Row_Number < CONFIGURATION_WORLD_ROWS_COUNT; Row_Number++)
	{
		// Get corresponding row
		Pointer_Row = CellGetWorldRowPointer(Row_Number);
	
		// Generate a row
		for (Column_Number = 0; Column_Number < CONFIGURATION_WORLD_COLUMNS_COUNT; Column_Number++)
		{
			// Choose a random state
			if ((RandomGenerateNumber() % 7) == 2) Cell_State = 1;
			else Cell_State = 0;
			
			CellSetState(Pointer_Row, Column_Number, Cell_State);
		}
	}
	
	SystemLedOff();
}

void WorldDisplay(void)
{
	unsigned char *Pointer_Row, Row_Number, Column_Number;
	
	// Set cursor to home
	ScreenSetCursorLocation(1, 1);
	
	for (Row_Number = 0; Row_Number < CONFIGURATION_WORLD_ROWS_COUNT; Row_Number++)
	{
		// Get row
		Pointer_Row = CellGetWorldRowPointer(Row_Number);
		
		// Show each cell state
		for (Column_Number = 0; Column_Number < CONFIGURATION_WORLD_COLUMNS_COUNT; Column_Number++)
		{
			if (CellGetState(Pointer_Row, Column_Number) == 1) ScreenWriteCharacter('O');
			else ScreenWriteCharacter(' ');
		}
	}
}

void WorldComputeNextGeneration(void)
{
	unsigned char Row_Computed_Generation[CELL_PACKED_ROW_SIZE_BYTES], Row_To_Commit[CELL_PACKED_ROW_SIZE_BYTES], *Pointer_Row_Above, *Pointer_Row_Current, *Pointer_Row_Below, i;
	
	// Set cursor to home
	ScreenSetCursorLocation(1, 1);
	
	// Compute first row separately as the upper border does not exist (it is considered as empty)
	Pointer_Row_Current = CellGetWorldRowPointer(0);
	Pointer_Row_Below = CellGetWorldRowPointer(1);			
	
	// Compute row 0
	WorldComputeNextRowGeneration(NULL, Pointer_Row_Current, Pointer_Row_Below, Row_Computed_Generation);
	
	// Compute row 1 (needing row 0 and row 2)
	Pointer_Row_Above = Pointer_Row_Current;
	Pointer_Row_Current = Pointer_Row_Below;
	Pointer_Row_Below = CellGetWorldRowPointer(2);
	WorldComputeNextRowGeneration(Pointer_Row_Above, Pointer_Row_Current, Pointer_Row_Below, Row_To_Commit); // Row_To_Commit will be committed into the loop
	
	// Row 0 can now be committed
	memcpy(Pointer_Row_Above, Row_Computed_Generation, CELL_PACKED_ROW_SIZE_BYTES);
		
	for (i = 3; i < CONFIGURATION_WORLD_ROWS_COUNT; i++)
	{
		// Fetch next row
		Pointer_Row_Above = Pointer_Row_Current;
		Pointer_Row_Current = Pointer_Row_Below;
		Pointer_Row_Below = CellGetWorldRowPointer(i);
		
		// Compute the new generation for the current row
		WorldComputeNextRowGeneration(Pointer_Row_Above, Pointer_Row_Current, Pointer_Row_Below, Row_Computed_Generation);
		
		// Commit the row computed on the previous loop as it's value won't be used anymore
		memcpy(Pointer_Row_Above, Row_To_Commit, CELL_PACKED_ROW_SIZE_BYTES);
		
		// The computed generation will commit on next loop
		memcpy(Row_To_Commit, Row_Computed_Generation, CELL_PACKED_ROW_SIZE_BYTES);
	}
	
	// Compute last row
	Pointer_Row_Above = Pointer_Row_Current;
	Pointer_Row_Current = Pointer_Row_Below;
	WorldComputeNextRowGeneration(Pointer_Row_Above, Pointer_Row_Current, NULL, Row_Computed_Generation); 
	
	// Commit the remaining rows
	memcpy(Pointer_Row_Above, Row_To_Commit, CELL_PACKED_ROW_SIZE_BYTES);
	memcpy(Pointer_Row_Current, Row_Computed_Generation, CELL_PACKED_ROW_SIZE_BYTES);
}