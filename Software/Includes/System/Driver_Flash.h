/** @file Driver_Random.h
 * Allow read and write access to the Flash program memory.
 * @author Adrien RICCIARDI
 */ 
#ifndef H_DRIVER_FLASH_H
#define H_DRIVER_FLASH_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** The program memory 14-bit instructions count. */
#define FLASH_SIZE 8192

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Read a 14-bit data word from the specified Flash memory address.
 * @param Address The word address in Flash program memory.
 * @return 0xFFFF if the specified address is out of program memory bounds,
 * @return the data word on success.
 */
unsigned short FlashReadWord(unsigned short Address);

/** Write a 14-bit data word to the specified Flash memory address.
 * @param Address The word address in Flash program memory.
 * @param Data The data to write.
 * @note The function will have no effect if the provided address is out of memory bounds.
 */
void FlashWriteWord(unsigned short Address, unsigned short Data);
 
#endif