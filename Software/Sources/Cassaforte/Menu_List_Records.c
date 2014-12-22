/** @file Menu_List_Records.c
 * @see Menus.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Interface.h"
#include "Menus.h"
#include "Records.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** How many rows used to display records. */
#define RECORDS_ROWS_COUNT 16
/** How many columns used to display records. */
#define RECORDS_COLUMNS_COUNT 4

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void MenuListRecords(void)
{
	unsigned char Row, Column, String_Record_Name[RECORD_NAME_LENGTH + 1], Record_Number = 1, i;
	bool Is_Empty_Color_Set = false; // Avoid changing empty and not empty record color every time (speed up printing)
	
	// Prepare screen
	ScreenHideCursor();
	ScreenClear();
	
	// Show title
	InterfaceWriteTitle(STRING_MENU_LIST_RECORDS_TITLE);
	
	// Show all records (4 columns of 16 rows)
	for (Row = 0; Row < RECORDS_ROWS_COUNT; Row++)
	{
		for (Column = 0; Column < RECORDS_COLUMNS_COUNT; Column++)
		{
			// Read file name from EEPROM
			RecordReadName(Record_Number - 1, String_Record_Name);
			
			// Change color according to record state (empty or not)
			if ((*String_Record_Name == 0) && !Is_Empty_Color_Set)
			{
				ScreenSetColor(INTERFACE_COLOR_EMPTY_RECORDS);
				Is_Empty_Color_Set = true;
			}
			else if ((*String_Record_Name != 0) && Is_Empty_Color_Set)
			{
				ScreenSetColor(INTERFACE_COLOR_NORMAL);
				Is_Empty_Color_Set = false;
			}
			
			// Show record number
			ScreenWriteString("    ");
			if (Record_Number < 10) ScreenWriteCharacter(' '); // Add a space to align 1-digit numbers
			ScreenWriteUnsignedInteger(Record_Number);
			ScreenWriteString(". ");

			// Show record name
			ScreenWriteString(String_Record_Name);
			
			// Go to next drawing slot by filling with spaces
			for (i = 8 + strlen(String_Record_Name); i < INTERFACE_SCREEN_WIDTH / RECORDS_COLUMNS_COUNT; i++) ScreenWriteCharacter(' ');
			
			Record_Number++;
		}
		ScreenWriteString("\r\n");
	}
	
	// Wait for the user to press the Escape key
	ScreenSetColor(INTERFACE_COLOR_NORMAL);
	InterfaceWaitForEscapeKey();
	
	// Reset some terminal attributes
	ScreenShowCursor();
}