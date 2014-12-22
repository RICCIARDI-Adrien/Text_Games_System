/** @file Interface.h
 * Display records name and program appearance.
 * @author Adrien RICCIARDI
 * @version 1.0 : 31/08/2013
 */
#ifndef H_INTERFACE_H
#define H_INTERFACE_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How many columns on the terminal. */
#define INTERFACE_SCREEN_WIDTH 80
/** How many rows on the terminal. */
#define INTERFACE_SCREEN_HEIGHT 24

/** Title color. */
#define INTERFACE_COLOR_TITLE SCREEN_COLOR_CODE_TEXT_GREEN
/** Normal text color. */
#define INTERFACE_COLOR_NORMAL SCREEN_COLOR_CODE_TEXT_WHITE
/** Empty records color to make them easy to find. */
#define INTERFACE_COLOR_EMPTY_RECORDS SCREEN_COLOR_CODE_TEXT_YELLOW
/** Confirmation questions color. */
#define INTERFACE_COLOR_CONFIRMATION_QUESTION SCREEN_COLOR_CODE_TEXT_MAGENTA

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Show a centered title on the first row.
 * @param String_Title The title to display.
 * @note When terminating, the cursor location is updated in order to be ready to write strings.
 * @note When terminating, the normal text color is set.
 */
void InterfaceWriteTitle(unsigned char *String_Title);

/** Wait for the user to press the Escape key. */
void InterfaceWaitForEscapeKey(void);

/** Write a red string and go to next line.
 * @param String The error message.
 */
void InterfaceWriteErrorString(unsigned char *String);

/** Display a message asking for 'yes' or 'no'.
 * @param String_Message The yes/no question to display.
 * @return true if the user answered 'yes' or false he selected 'no'.
 * @note The 'yes' or 'no' corresponding letters are defined in the Strings.h file and are not case sensitive.
 */
bool InterfaceAskConfirmation(unsigned char *String_Message);

#endif