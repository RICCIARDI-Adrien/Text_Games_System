/** @file TestExternalEEPROM.h
 * Check the good functioning of the I2C driver.
 * @author Adrien RICCIARDI
 * @version 1.0 : 13/09/2014
 */
#ifndef H_TEST_EXTERNAL_EEPROM_H
#define H_TEST_EXTERNAL_EEPROM_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** The external EEPROM size in bytes. */
#define TEST_EXTERNAL_EEPROM_MEMORY_SIZE_BYTES 4096

/** How many write/read cycles to do. */
#define TEST_EXTERNAL_EEPROM_CYCLES_COUNT 1000

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Start the external EEPROM test. */
void TestExternalEEPROM(void);

#endif