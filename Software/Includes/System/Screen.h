/** @file Screen.h
 * Print text formatted for a terminal through serial port.
 * @author Adrien RICCIARDI
 * @version 1.0 : 21/02/2013
 * @version 1.1 : 27/02/2013, added ScreenWriteROMString() function.
 * @version 1.2 : 08/05/2013, added ScreenSetCursorLocation() function.
 * @version 1.3 : 22/06/2013, added ScreenSetColor() function.
 */
#ifndef H_SCREEN_H
#define H_SCREEN_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Black text. */
#define SCREEN_COLOR_CODE_TEXT_BLACK 30
/** Red text. */
#define SCREEN_COLOR_CODE_TEXT_RED 31
/** Green text. */
#define SCREEN_COLOR_CODE_TEXT_GREEN 32
/** Yellow text. */
#define SCREEN_COLOR_CODE_TEXT_YELLOW 33
/** Blue text. */
#define SCREEN_COLOR_CODE_TEXT_BLUE 34
/** Magenta text. */
#define SCREEN_COLOR_CODE_TEXT_MAGENTA 35
/** Cyan text. */
#define SCREEN_COLOR_CODE_TEXT_CYAN 36
/** White text. */
#define SCREEN_COLOR_CODE_TEXT_WHITE 37

/** Black background. */
#define SCREEN_COLOR_CODE_BACKGROUND_BLACK 40
/** Red background. */
#define SCREEN_COLOR_CODE_BACKGROUND_RED 41
/** Green background. */
#define SCREEN_COLOR_CODE_BACKGROUND_GREEN 42
/** Yellow background. */
#define SCREEN_COLOR_CODE_BACKGROUND_YELLOW 43
/** Blue background. */
#define SCREEN_COLOR_CODE_BACKGROUND_BLUE 44
/** Magenta background. */
#define SCREEN_COLOR_CODE_BACKGROUND_MAGENTA 45
/** Cyan background. */
#define SCREEN_COLOR_CODE_BACKGROUND_CYAN 46
/** White background. */
#define SCREEN_COLOR_CODE_BACKGROUND_WHITE 47

/** Restore text and background default colors. */
#define SCREEN_COLOR_CODE_DEFAULT 0

/** The 'à' letter. */
#define SCREEN_LETTER_SMALL_A_GRAVE "\xE0"
/** The 'è' letter. */
#define SCREEN_LETTER_SMALL_E_GRAVE "\xE8"
/** The 'é' letter. */
#define SCREEN_LETTER_SMALL_E_ACUTE "\xE9"

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Write a string to the screen.
 * @param String The string to write.
 */
void ScreenWriteString(unsigned char *String);

/** Write a character to the screen.
 * @param Character The character to write.
 */
void ScreenWriteCharacter(unsigned char Character);

/** Write an unsigned integer (16-bit wide on BoostC) value to the screen.
 * @param Integer The integer to write.
 */
void ScreenWriteUnsignedInteger(unsigned int Integer);

/** Write a string stored into the external EEPROM.
 * @param Address Address of the beginning of the string into the EEPROM.
 */
void ScreenWriteROMString(unsigned short Address);

/** Clear the terminal screen. */
void ScreenClear(void);

/** Set the terminal screen cursor location.
 * @param Row The row (Y) where to put the cursor.
 * @param Column The column (X) where to put the cursor.
 */
void ScreenSetCursorLocation(unsigned char Row, unsigned char Column);

/** Hide the terminal cursor. */
void ScreenHideCursor(void);

/** Show the terminal cursor. */
void ScreenShowCursor(void);

/** Move the cursor down for 1 row. */
void ScreenMoveCursorDown(void);

/** Disable terminal scrolling to use the full screen mode. */
void ScreenDisableScrolling(void);

/** Enable normal terminal scrolling. */
void ScreenEnableScrolling(void);

/** Set the text font or background color.
 * @param Color_Code The color to use (the value must come from the SCREEN_COLOR_CODE_XXX set).
 */
void ScreenSetColor(unsigned char Color_Code);

#endif