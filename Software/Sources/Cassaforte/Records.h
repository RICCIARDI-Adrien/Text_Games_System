/** @file Records.h
 * Handle low level EEPROM records interactions.
 * @author Adrien RICCIARDI
 * @version 1.0 : 01/08/2013
 */
#ifndef H_RECORDS_H
#define H_RECORDS_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Maximum number of records stored in the EEPROM. */
#define RECORD_MAXIMUM_COUNT 64
/** Length of a record name (which is stored at the beginning of the record). */
#define RECORD_NAME_LENGTH 8
/** Size of a record (including room to store the name). */
#define RECORD_TOTAL_LENGTH 64
/** Size of the record content. */
#define RECORD_CONTENT_LENGTH (RECORD_TOTAL_LENGTH - RECORD_NAME_LENGTH)

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Read the name of a record.
 * @param Record_Number Record index into EEPROM (from 0 to RECORD_MAXIMUM_COUNT - 1).
 * @param String_Record_Name On output, hold the name of the record (the provided buffer must be at least RECORD_NAME_LENGTH + 1 bytes long).
 */
void RecordReadName(unsigned char Record_Number, unsigned char *String_Record_Name);

/** Write the name of a record.
 * @param Record_Number Record index into EEPROM (from 0 to RECORD_MAXIMUM_COUNT - 1).
 * @param String_Record_Name The name to write. If the string is longer than RECORD_NAME_LENGTH, it is truncated.
 */
void RecordWriteName(unsigned char Record_Number, unsigned char *String_Record_Name);

/** Read the content of a record.
 * @param Record_Number Record index into EEPROM (from 0 to RECORD_MAXIMUM_COUNT - 1).
 * @param String_Record_Content On output, hold the content of the record (the provided buffer must be at least RECORD_CONTENT_LENGTH + 1 bytes long).
 */
void RecordReadContent(unsigned char Record_Number, unsigned char *String_Record_Content);

/** Write the content of a record.
 * @param Record_Number Record index into EEPROM (from 0 to RECORD_MAXIMUM_COUNT - 1).
 * @param String_Record_Content The content to write. If the string is longer than RECORD_CONTENT_LENGTH, it is truncated.
 */
void RecordWriteContent(unsigned char Record_Number, unsigned char *String_Record_Content);

#endif