/** @file Menu_Main.c
 * @see Menus.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Interface.h"
#include "Menus.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Menus starting row for display. */
#define MENUS_STARTING_ROW 8
/** Menus starting column for display. */
#define MENUS_STARTING_COLUMN 25

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned char MenuMain(void)
{
	unsigned char i = 0;
	
	ScreenHideCursor();
	ScreenClear();
	
	// Display centered title
	ScreenSetColor(INTERFACE_COLOR_TITLE);
	ScreenSetCursorLocation(1, ((INTERFACE_SCREEN_WIDTH - 18 + 1) >> 1) + 1); // Need to hardcode string size as sizeof() does not run on this compiler, +1 for string terminating zero, +1 because VT100 coordinates are 1-based
	ScreenWriteString("### Cassaforte ###");
	ScreenSetColor(INTERFACE_COLOR_NORMAL);
	
	// Display all menus (can't do a loop with an array of string pointers as it would consume too much RAM : all the strings must be loaded in memory before entering the loop)
	// List records
	ScreenSetCursorLocation(MENUS_STARTING_ROW, MENUS_STARTING_COLUMN);
	ScreenWriteString(STRING_MENU_ENTRY_LIST_RECORDS);
	// Display records
	ScreenSetCursorLocation(MENUS_STARTING_ROW + 1, MENUS_STARTING_COLUMN);
	ScreenWriteString(STRING_MENU_ENTRY_DISPLAY_RECORD_CONTENT);
	// New record
	ScreenSetCursorLocation(MENUS_STARTING_ROW + 2, MENUS_STARTING_COLUMN);
	ScreenWriteString(STRING_MENU_ENTRY_NEW_RECORD);
	// Rename a record
	ScreenSetCursorLocation(MENUS_STARTING_ROW + 3, MENUS_STARTING_COLUMN);
	ScreenWriteString(STRING_MENU_ENTRY_RENAME_RECORD);
	// Delete a record
	ScreenSetCursorLocation(MENUS_STARTING_ROW + 4, MENUS_STARTING_COLUMN);
	ScreenWriteString(STRING_MENU_ENTRY_DELETE_RECORD);
	// Delete all records
	ScreenSetCursorLocation(MENUS_STARTING_ROW + 5, MENUS_STARTING_COLUMN);
	ScreenWriteString(STRING_MENU_ENTRY_DELETE_ALL_RECORDS);

	// Wait for the user to select a menu
	do
	{
		i = KeyboardReadCharacter();
	} while ((i < '1') || (i > '6'));
	
	ScreenShowCursor();
	return i - 48; // Convert to binary value
}