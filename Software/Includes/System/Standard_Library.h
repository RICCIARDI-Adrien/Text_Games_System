/** @file Standard_Library.h
 * Some standard libc optimized functions.
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/02/2013
 * @version 1.1 : 08/05/2013, added itoa() function.
 * @version 1.2 : 22/05/2013, made aliases for standard functions.
 * @version 1.3 : 23/10/2013, added memcpy().
 */
#ifndef H_STANDARD_LIBRARY_H
#define H_STANDARD_LIBRARY_H

#include "Driver_Random.h"
#include "Keyboard.h"
#include "Screen.h"

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Copy a whole string into another.
 * @param String_Destination Pointer to the string into which to copy.
 * @param String_Source Pointer to the string to copy.
 */
void strcpy(unsigned char *String_Destination, unsigned char *String_Source);

/** Get the length of a string in characters.
 * @param String The string to measure.
 * @return The string length.
 */
unsigned char strlen(unsigned char *String);

/** Convert a number stored in a string to its binary representation.
 * @param String The string to convert.
 * @return The binary representation of the string.
 * @warning Be sure that the string contains only digits or the result will be unpredictable.
 */
unsigned short atoi(unsigned char *String);

/** Convert a binary number into its ASCII representation.
 * @param Number The number to convert.
 * @param String_Output A pointer to the buffer in which to store the ASCII representation.
 * @warning Be sure that the output string buffer has enough room to store the whole string.
 */
void itoa(unsigned short Number, unsigned char *String_Output);

/** Set a memory area to zero.
 * @param Pointer_Buffer The buffer to fill with zeros.
 * @param Bytes_Count How many buffer bytes to set to zero.
 */
void bzero(unsigned char *Pointer_Buffer, unsigned char Bytes_Count);

/** Set a memory area to a defined value.
 * @param Pointer_Buffer The memory area to fill.
 * @param Value The value to put into the buffer.
 * @param Bytes_Count How many buffer bytes to set to Value.
 */
void memset(void *Pointer_Buffer, unsigned char Value, unsigned char Bytes_Count);

/** Copy bytes from a memory area to another.
 * @param Pointer_Destination_Buffer The destination memory area.
 * @param Pointer_Source_Buffer The source memory area.
 * @param Bytes_Count How many bytes to copy.
 */
void memcpy(void *Pointer_Destination_Buffer, void *Pointer_Source_Buffer, unsigned char Bytes_Count);

#endif