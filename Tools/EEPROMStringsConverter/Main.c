/** @file Main.c
 * Tool used to automatically build strings index and allocate them into EEPROM.
 * @author Adrien RICCIARDI
 * @version 1.0 : 26/02/2013
 * @version 1.1 : 21/02/2014, check output file size and tell the user if it is bigger than the EEPROM size.
 */
#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"

/** Retrieve the size of a file.
 * @param String_File_Name The name of the file to find size.
 * @return The file size in bytes -1 if an error occurred.
 */
static int GetFileSize(char *String_File_Name)
{
	FILE *File;
	int File_Size;
	
	// Open the file
	File = fopen(String_File_Name, "rb");
	if (File == NULL) return -1;
	
	// Go to file end
	fseek(File, 0, SEEK_END);
	
	// Get size
	File_Size = (int) ftell(File);
	fclose(File);
	return File_Size;
}

int main(int argc, char *argv[])
{
	FILE *File_Resources, *File_Header, *File_EEPROM;
	char *String_Resource_File_Name, *String_Output_Header_File_Name, *String_Output_EEPROM_Image_File_Name;
	int Is_Parsing_Successful, EEPROM_Size_Bytes, Image_File_Size_Bytes;
	
	// Show title
	printf("+------------------------------------+\n");
	printf("|      EEPROM strings allocator      |\n");
	printf("+------------------------------------+\n");
	
	// Check arguments
	if (argc != 5)
	{
		printf("Usage : EEPROMStringsConverter  Resources_File_Name  Output_Header_File_Name  Output_EEPROM_Image_File_Name  EEPROM_Size_Bytes.\n\n");
	
		printf("Resources file composition :\n");
		printf("----------------------------\n");
		printf("The resources file contains all strings used into the program.\n");
		printf("Add a string to the resource file like following :\n");
		printf("STRING_IDENTIFIER String value\n");
		printf("Example file :\n");
		printf("HELLO_WORLD_STRING \"Hello, world !\"\n");
		printf("CIAO_WORLD_STRING \"Ciao a tutti !\"\n");
		printf("NB : you can use the listed escape sequences : \\n, \\r, \\\", \\\\.\n\n");
		
		printf("Header file (.h) composition :\n");
		printf("------------------------------\n");
		printf("The C header file generated from the previous example is :\n");
		printf("#define HELLO_WORLD_STRING 0\n");
		printf("#define CIAO_WORLD_STRING 14\n");
		printf("where 0 represents the address of the first string into the EEPROM and\n");
		printf("14 is the address of the second string.\n");
		printf("Strings are placed consecutively into the EEPROM.\n\n");
		
		printf("EEPROM file composition :\n");
		printf("-------------------------\n");
		printf("The corresponding EEPROM image file is :\n");
		printf("Hello, world !\\0Ciao a tutti !\\0\n");
		return -1;
	}
	
	// Get arguments value
	String_Resource_File_Name = argv[1];
	String_Output_Header_File_Name = argv[2];
	String_Output_EEPROM_Image_File_Name = argv[3];
	EEPROM_Size_Bytes = atoi(argv[4]);
	if (EEPROM_Size_Bytes <= 0)
	{
		printf("Error : invalid EEPROM size (%d bytes).\n", EEPROM_Size_Bytes);
		return -1;
	}
	
	// Try to open strings resource file
	File_Resources = fopen(String_Resource_File_Name, "r");
	if (File_Resources == NULL)
	{
		printf("Error : can't open resources file '%s'.\n", String_Resource_File_Name);
		return -2;
	}
	
	// Try to create output .h file
	File_Header = fopen(String_Output_Header_File_Name, "w");
	if (File_Header == NULL)
	{
		printf("Error : can't create output header file '%s'.\n", String_Output_Header_File_Name);
		fclose(File_Resources);
		return -3;
	}
	
	// Try to create output EEPROM file
	File_EEPROM = fopen(String_Output_EEPROM_Image_File_Name, "wb");
	if (File_EEPROM == NULL)
	{
		printf("Error : can't create output EEPROM image file '%s'.\n", String_Output_EEPROM_Image_File_Name);
		fclose(File_Resources);
		fclose(File_Header);
		return -4;
	}
	
	// Do parsing
	ParserInit(File_Resources, File_Header, File_EEPROM);
	Is_Parsing_Successful = ParserProcessFiles();

	fclose(File_Resources);
	fclose(File_Header);
	fclose(File_EEPROM);

	if (Is_Parsing_Successful)
	{
		printf("Parsing successfully done.\n");
		
		// Show image file size
		Image_File_Size_Bytes = GetFileSize(String_Output_EEPROM_Image_File_Name);
		printf("EEPROM image size : %d bytes.\n", Image_File_Size_Bytes);
		if (Image_File_Size_Bytes > EEPROM_Size_Bytes) printf("Warning : EEPROM image is bigger than EEPROM size (%d bytes).\n", EEPROM_Size_Bytes);
		return 0;
	}
	else
	{
		printf("Parsing failed. Check your resources file.\n");
		return -5;
	}
}
