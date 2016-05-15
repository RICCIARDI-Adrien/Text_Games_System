/** @file Hex_Parser.h
 * Simple parser for Intel Hexadecimal file format.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/02/2013
 */
#ifndef H_HEX_PARSER_H
#define H_HEX_PARSER_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Address of the beginning of the bootloader code. */
#define BOOTLOADER_ADDRESS 0x1800
/** Address where to remap program entry point. */
#define BOOTLOADER_ENTRY_POINT_ADDRESS 0x1FFB

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** A decoded hex instruction. */
typedef struct
{
	unsigned short Address; //! Address of the instruction.
	unsigned short Code; //! Instruction code.
	char Is_Instruction_Valid; //! Tell if the instruction is valid (can be sent to the board) or not.
	char Is_End_Of_File; //! Tell if EOF is reached or not.
} TInstruction;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Parse a whole line read from the hex file.
 * @param Hex_Line The line from the hex file (like ":A012345678").
 * @param Instructions An array holding all parsed instructions when the function returns.
 * @return The instructions count contained in Hex_Line.
 * @note The maximum instructions count in one hex line is 32, so Instructions array must be 32 entries wide.
 */
int HexParserDecodeLine(char *Hex_Line, TInstruction Instructions[]);

#endif
