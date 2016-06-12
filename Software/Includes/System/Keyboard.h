/** @file Keyboard.h
 * Read keys from terminal keyboard through serial port.
 * @author Adrien RICCIARDI
 * @version 1.0 : 21/02/2013
 */
#ifndef H_KEYBOARD_H
#define H_KEYBOARD_H

#include "Driver_UART.h"

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** The backspace key code. */
#define KEYBOARD_KEY_CODE_BACKSPACE 8
/** The escape key code. */
#define KEYBOARD_KEY_CODE_ESCAPE 27

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Read a string from the keyboard.
 * @param String A buffer in which to store the string. Be sure that the buffer has room enough to store the whole string.
 * @param Maximum_Characters_Count Maximum number of characters allowed into the string. When this number is reached, the user can type in more characters.
 * @param Are_Only_Digits_Allowed If 1, only digits from 0 to 9 are accepted by the function. If 0, all characters are allowed.
 * @return 0 if the user canceled the string by hitting Escape (the string is not valid),
 * @return 1 if the user hit Enter (the string is valid).
 */
unsigned char KeyboardReadString(unsigned char *String, unsigned char Maximum_Characters_Count, unsigned char Are_Only_Digits_Allowed);

/** Read a character from the keyboard.
 * @return The read character.
 * @note This is a blocking function.
 */
#ifdef DOXYGEN
	unsigned char KeyboardReadChararacter(void);
#else
	#define KeyboardReadCharacter() UARTReadByte()
#endif

/** Tell if a key is available to read.
 * @return 1 if there is a key available or 0 if not.
 * @note This is a non blocking function.
 */
#ifdef DOXYGEN
	unsigned char KeyboardIsKeyAvailable(void);
#else
	#define KeyboardIsKeyAvailable() UARTIsByteAvailable()
#endif

#endif