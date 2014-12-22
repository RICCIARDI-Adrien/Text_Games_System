/** @file Interrupts_Handler.h
 * Handle enabled PIC interrupts.
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/05/2013
 * @version 1.1 : 29/06/2013, the board now automatically reboots when a magic number is received (allow remote board flashing).
 */
#ifndef H_INTERRUPTS_HANDLER_H
#define H_INTERRUPTS_HANDLER_H

//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------
/** Tell if a byte was received from the UART. */
extern unsigned char Interrupts_Handler_Has_UART_Received_Byte;

/** Hold the key received by the UART. */
extern unsigned char Interrupts_Handler_UART_Received_Byte_Value;

#endif