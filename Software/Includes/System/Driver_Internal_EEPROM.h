/** @file Driver_Internal_EEPROM.h
 * Allow to read from and to write to the internal microcontroller 256-byte EEPROM.
 * @author Adrien RICCIARDI
 * @version 1.0 : 27/12/2014
 */
#ifndef H_DRIVER_INTERNAL_EEPROM_H
#define H_DRIVER_INTERNAL_EEPROM_H

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
/** Read a byte from the internal EEPROM.
 * @param Address The address where the byte is located.
 * @return The byte value.
 */
unsigned char InternalEEPROMReadByte(unsigned char Address);

/** Write a byte to the internal EEPROM.
 * @param Address The address to write the byte to.
 * @param Byte The byte value to write.
 */
void InternalEEPROMWriteByte(unsigned char Address, unsigned char Byte);

#endif
