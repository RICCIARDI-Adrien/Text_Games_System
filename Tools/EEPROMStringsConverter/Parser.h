/** @file Parser.h
 * Parse a resources file and generate header and EEPROM files.
 * @author Adrien RICCIARDI
 * @version 1.0 : 27/02/2013
 */
#ifndef H_PARSER_H
#define H_PARSER_H

/** Initialize the parser.
 * @param Input_File_Resources The file from which to read string resources.
 * @param Output_File_Header The file in which to write the C header (.h) file.
 * @param Output_File_EEPROM The file in which to write the EEPROM binary image.
 * @note You must provide successfully opened files, they are not opened by the function itself.
 */
void ParserInit(FILE *Input_File_Resources, FILE *Output_File_Header, FILE *Output_File_EEPROM);

/** Parse the files provided by ParserInit() function.
 * @return 1 if the parsing was done successfully or 0 if there was an error.
 */
int ParserProcessFiles(void);

#endif
