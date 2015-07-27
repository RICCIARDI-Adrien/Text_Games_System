/** @file Driver_I2C.h
 * Driver for the I2C module optimized for the external EEPROM.
 * @author Adrien RICCIARDI
 * @version 1.0 : 26/02/2013
 * @version 1.1 : 03/09/2013, added I2CWriteByte().
 * @version 1.2 : 27/07/2015, used acknowledge polling to wait for a write cycle completion.
 */ 
#ifndef H_DRIVER_I2C_H
#define H_DRIVER_I2C_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the I2C module at 400KHz. */
void I2CInitialize(void);

/** Set the external EEPROM byte address. A following call to I2CReadNextByte() will return the byte stored at this address.
 * @param Address Address in range [0..4095].
 */
void I2CSetCurrentAddress(unsigned short Address);

/** Read the next available byte from the external EEPROM.
 * @return The next byte.
 * @note The function will wrap to address 0 if an attempt to read a byte located at an address higher than 4095 is done.
 */
unsigned char I2CReadNextByte(void);

/** Write a byte to the external EEPROM.
 * @param Address Location where to store the byte (in range [0..4095]).
 * @param Byte The value to write.
 */
void I2CWriteByte(unsigned short Address, unsigned char Byte);

#endif