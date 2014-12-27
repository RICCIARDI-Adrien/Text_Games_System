/** @file Test_Internal_EEPROM.h
 * Test the driver accessing the microcontroller internal EEPROM.
 * @author Adrien RICCIARDI
 * @version 1.0 : 27/12/2014
 */
#ifndef H_TEST_INTERNAL_EEPROM_H
#define H_TEST_INTERNAL_EEPROM_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** The internal EEPROM size in bytes. */
#define TEST_INTERNAL_EEPROM_MEMORY_SIZE_BYTES 256

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Start the internal EEPROM test. */
void TestInternalEEPROM(void);

#endif