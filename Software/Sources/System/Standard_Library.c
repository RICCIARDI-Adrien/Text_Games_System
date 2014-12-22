/** @file Standard_Library.c
 * @see Standard_Library.h for description.
 * @author Adrien RICCIARDI
 */
#include <Standard_Library.h>
#include <system.h> // Needed by division operator

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void strcpy(unsigned char *String_Destination, unsigned char *String_Source)
{
	while (*String_Source != 0)
	{
		*String_Destination = *String_Source;
		String_Destination++;
		String_Source++;
	}
	*String_Destination = 0; // End string
}

unsigned char strlen(unsigned char *String)
{
	unsigned char i = 0;
	
	while (*String != 0)
	{
		String++;
		i++;
	}
	return i;
}

unsigned short atoi(unsigned char *String)
{
	unsigned short Result = 0;
	
	while (*String != 0)
	{
		// Multiply last result by ten
		Result = (Result << 3) + (Result << 1);
		// Add next rank digit converted to binary
		Result += *String - 48;
		// Next digit
		String++;
	}
	return Result;
}

void itoa(unsigned short Number, unsigned char *String_Output)
{
	unsigned char String_Temp[5], i = 0;
	unsigned short Divided_By_Ten;
	
	// No need to loose cycles by converting a 1-digit number
	if (Number < 10)
	{
		String_Output[0] = Number + 48;
		String_Output[1] = 0;
		return;
	}
	
	// Convert integer to string
	while (Number > 0)
	{
		Divided_By_Ten = Number / 10;
		// Get last number digit
		String_Temp[i] = (Number - ((Divided_By_Ten << 3) + (Divided_By_Ten << 1))) + 48; // Number - Divided_By_Ten * 10 => keep only the last digit value
		Number = Divided_By_Ten;
		i++;
	}
	
	// Copy temporary string into output string starting by the temporary string end, because the conversion was done in the reverse order
	i--;
	while (i != 0xFF) // Can't use i >= 0 as i is unsigned
	{
		*String_Output = String_Temp[i];
		String_Output++;
		i--;
	}
	// Terminate string
	*String_Output = 0;
}

void bzero(unsigned char *Pointer_Buffer, unsigned char Bytes_Count)
{
	while (Bytes_Count > 0)
	{
		*Pointer_Buffer = 0;
		Pointer_Buffer++;
		Bytes_Count--;
	}
}

void memset(unsigned char *Pointer_Buffer, unsigned char Value, unsigned char Bytes_Count)
{
	while (Bytes_Count > 0)
	{
		*Pointer_Buffer = Value;
		Pointer_Buffer++;
		Bytes_Count--;
	}
}

void memcpy(unsigned char *Pointer_Destination_Buffer, unsigned char *Pointer_Source_Buffer, unsigned char Bytes_Count)
{
	while (Bytes_Count > 0)
	{
		*Pointer_Destination_Buffer = *Pointer_Source_Buffer;
		Pointer_Destination_Buffer++;
		Pointer_Source_Buffer++;
		Bytes_Count--;
	}
}