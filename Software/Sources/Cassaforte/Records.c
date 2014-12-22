/** @file Records.c
 * @see Records.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Records.h"

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void RecordReadName(unsigned char Record_Number, unsigned char *String_Record_Name)
{
	unsigned char i;
	
	// Return empty value if the record does not exist to avoid damaging the EEPROM with a bad address
	if (Record_Number >= RECORD_MAXIMUM_COUNT)
	{
		*String_Record_Name = 0;
		return;
	}
	
	// Go to specified record name into EEPROM
	I2CSetCurrentAddress(Record_Number * RECORD_TOTAL_LENGTH);
	
	// Read the record name
	for (i = 0; i < RECORD_NAME_LENGTH; i++)
	{
		*String_Record_Name = I2CReadNextByte();
		if (*String_Record_Name == 0) return;
		String_Record_Name++;
	}
	// Terminate string
	*String_Record_Name = 0;
}

void RecordWriteName(unsigned char Record_Number, unsigned char *String_Record_Name)
{
	unsigned short Address;
	unsigned char i;
	
	// Do nothing if the record does not exist to avoid damaging the EEPROM with a bad address
	if (Record_Number >= RECORD_MAXIMUM_COUNT) return;
	
	// Compute record name base address into EEPROM
	Address = Record_Number * RECORD_TOTAL_LENGTH;
	
	// Write record data
	for (i = 0; i < RECORD_NAME_LENGTH; i++)
	{
		I2CWriteByte(Address, *String_Record_Name);
		if (*String_Record_Name == 0) return; // Exit once the end of the data to write is reached
		Address++;
		String_Record_Name++;
	}
}

void RecordReadContent(unsigned char Record_Number, unsigned char *String_Record_Content)
{
	unsigned char i;
	
	// Return empty value if the record does not exist to avoid damaging the EEPROM with a bad address
	if (Record_Number >= RECORD_MAXIMUM_COUNT)
	{
		*String_Record_Content = 0;
		return;
	}
	
	// Go to specified record content into EEPROM
	I2CSetCurrentAddress(Record_Number * RECORD_TOTAL_LENGTH + RECORD_NAME_LENGTH);
	
	// Read the record content
	for (i = 0; i < RECORD_CONTENT_LENGTH; i++)
	{
		*String_Record_Content = I2CReadNextByte();
		if (*String_Record_Content == 0) return;
		String_Record_Content++;
	}
	// Terminate string
	*String_Record_Content = 0;
}

void RecordWriteContent(unsigned char Record_Number, unsigned char *String_Record_Content)
{
	unsigned short Address;
	unsigned char i;
	
	// Do nothing if the record does not exist to avoid damaging the EEPROM with a bad address
	if (Record_Number >= RECORD_MAXIMUM_COUNT) return;
	
	// Compute record name base address into EEPROM
	Address = Record_Number * RECORD_TOTAL_LENGTH + RECORD_NAME_LENGTH;
	
	// Write record data
	for (i = 0; i < RECORD_CONTENT_LENGTH; i++)
	{
		I2CWriteByte(Address, *String_Record_Content);
		if (*String_Record_Content == 0) return; // Exit once the end of the data to write is reached
		Address++;
		String_Record_Content++;
	}
}