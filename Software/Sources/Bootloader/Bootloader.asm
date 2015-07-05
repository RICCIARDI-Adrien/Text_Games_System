; Bootloader for Text Games System.
; @author : Adrien RICCIARDI
; @version 1.0 : 19/02/2013
; @version 1.1 : 27/02/2013, added I2C EEPROM programming section.
; @version 1.2 : 19/06/2015, added a waiting loop at the end of the firmware update, merged the release and the development bootloaders into a single file.
; @version 1.3 : 24/06/2015, improved I2C drivers to fasten download time.
#include <p16f876.inc>

;--------------------------------------------------------------------------------------------------
; Constants
;--------------------------------------------------------------------------------------------------
CODE_PROGRAMMING_START EQU h'FE'
CODE_ACKNOWLEDGE EQU h'EF'
CODE_STARTING_PROGRAM EQU 'r' ; Stands for "run"
BOOTLOADER_START_ADDRESS EQU d'8045' ; Computed manually...
EEPROM_WRITE_ADDRESS EQU h'A0' ; I2C EEPROM address with write request

;--------------------------------------------------------------------------------------------------
; Entry point
;--------------------------------------------------------------------------------------------------
	org 0
	bsf PCLATH, 4 ; Go to program page 3
	bsf PCLATH, 3
	goto Bootloader_Start

	org BOOTLOADER_START_ADDRESS
Bootloader_Start:
	;==================================================================================================
	; Initialize UART
	;==================================================================================================
	bsf STATUS, RP0 ; Go to bank 1

	movlw h'0C' ; Set 19200 bit/s rate
	movwf SPBRG

	movlw h'26' ; Asynchronous transmit enabled, 8-bit, high speed
	movwf TXSTA

	clrf TRISB ; Configure led port at the same time

	bcf STATUS, RP0 ; Go to bank 0
	movlw h'90' ; Serial port enabled, 8-bit, continuous reception enabled
	movwf RCSTA
	
	;==================================================================================================
	; Initialize I2C module
	;==================================================================================================
	bsf STATUS, RP0 ; Bank 1
	clrf SSPSTAT ; 400KHz speed mode, I2C input levels
	clrf SSPCON2 ; Reset communication flags
	movlw 2
	movwf SSPADD ; Set baud rate at approximately 333KHz (the most closer value for 400KHz)
	
	bcf STATUS, RP0 ; Bank 0
	movlw h'28'
	movwf SSPCON ; Enable I2C module in Master mode
	bcf PIR1, SSPIF ; Clear the interrupt flag
	
	; Reset registers which hold the address
	clrf h'21'
	clrf h'22'

	;==================================================================================================
	; Is the start programming code received ?
	;==================================================================================================
	; Loose approximately 250 * 3 µs to let PC send its code
	movlw d'251'
	movwf h'20' ; First available register in bank 0
@Loop_Waste_Time:
	; Check byte if one was received
	btfsc PIR1, RCIF
	goto Check_Received_Byte
	; Or continue looping
	decfsz h'20', F
	goto @Loop_Waste_Time
	
	; Start program if no byte received
	goto Start_Program

	; Check if the received byte is the good one
Check_Received_Byte:
	call UARTReadByte 
	xorlw CODE_PROGRAMMING_START
	btfss STATUS, Z
	goto Start_Program

	; Light led
	bsf PORTB, 7

	;==================================================================================================
	; Start programming flash
	;==================================================================================================
	; Send acknowledge
	movlw CODE_ACKNOWLEDGE
	call UARTWriteByte

	; Ignore all received CODE_PROGRAMMING_START until a different dummy byte is received
@Loop_Ignore_Code_Programming_Start:
	call UARTReadByte
	xorlw CODE_PROGRAMMING_START
	btfsc STATUS, Z
	goto @Loop_Ignore_Code_Programming_Start
	
	;==================================================================================================
	; Receive program instructions
	;==================================================================================================
Loop_Flash_Program_Instructions:
	; Address high byte
	call UARTReadByte 
	bsf STATUS, RP1 ; Bank 2
	movwf EEADRH

	; Check if the received byte was the end of programming code
	xorlw CODE_PROGRAMMING_START
	btfsc STATUS, Z
	goto Loop_Program_EEPROM
	
	; Address low byte
	call UARTReadByte
	bsf STATUS, RP1 ; Bank 2
	movwf EEADR
	
	; Instruction high byte
	call UARTReadByte
	bsf STATUS, RP1 ; Bank 2
	movwf EEDATH

	; Instruction low byte
	call UARTReadByte
	bsf STATUS, RP1 ; Bank 2
	movwf EEDATA
	
	;==================================================================================================
	; Flash received instruction
	;==================================================================================================
	; Reset end of write flag
	bcf STATUS, RP1 ; Bank 0
	bcf PIR2, EEIF
	
	; Configure writing
	bsf STATUS, RP1 ; Bank 3
	bsf STATUS, RP0
	bsf EECON1, EEPGD ; Select flash memory
	bsf EECON1, WREN ; Enable write

	; Special write sequence
	movlw h'55'
	movwf EECON2
	movlw h'AA'
	movwf EECON2
	bsf EECON1, WR ; Start writing
	nop
	nop
	bcf EECON1, WR ; Write is finished

	;==================================================================================================
	; Send acknowledge to PC
	;==================================================================================================
	movlw CODE_ACKNOWLEDGE
	call UARTWriteByte
	goto Loop_Flash_Program_Instructions
	
	;==================================================================================================
	; Programming loop of external EEPROM
	;==================================================================================================
Loop_Program_EEPROM:
	; Command byte, used only to tell if the program is fully transferred or not
	bcf STATUS, RP0 ; Bank 0
	call UARTReadByte 
	xorlw CODE_PROGRAMMING_START
	btfsc STATUS, Z
	goto Programming_Finished
	
	; Save received byte into the first available register of bank 0
	call UARTReadByte
	movwf h'20'
	
	;==================================================================================================
	; I2C write driver
	;==================================================================================================
	; Send a start condition to the bus
	bsf STATUS, RP0 ; Bank 1
	bsf SSPCON2, SEN
	call I2CWaitOperationCompletion
	
	; Send device address and write request
	movlw EEPROM_WRITE_ADDRESS
	movwf SSPBUF
	call I2CWaitOperationCompletion
	
	; Send address high byte
	; Get high address byte from memory
	movf h'22', W
	movwf SSPBUF
	call I2CWaitOperationCompletion
	
	; Send address low byte
	; Get low address byte from memory
	movf h'21', W
	movwf SSPBUF
	call I2CWaitOperationCompletion
	
	; Send byte to write
	; Retrieve data byte from memory
	movf h'20', W
	movwf SSPBUF
	call I2CWaitOperationCompletion
	
	; Send stop condition to start write operation
	bsf STATUS, RP0 ; Bank 1
	bsf SSPCON2, PEN
	call I2CWaitOperationCompletion
	
	;==================================================================================================
	; Increment address
	;==================================================================================================
	incf h'21', F ; We're in bank 0 yet thanks to I2CWaitOperationCompletion
	; Propagate carry to high byte only if low byte is equal 0 (so it did a roll over)
	btfsc STATUS, Z
	incf h'22', F
	
	;==================================================================================================
	; Wait for write operation completion
	;==================================================================================================
	; Each UART byte transmitted at 19200 bit/s halts the program for 520µs.
	; With 10 calls we wait almost 5.2 ms, which is more than the write time of the EEPROM.
	; The PC program will ignore received bytes different from CODE_ACKNOWLEDGE.
	movlw d'65'
	call UARTWriteByte
	call UARTWriteByte
	call UARTWriteByte
	call UARTWriteByte
	call UARTWriteByte
	call UARTWriteByte
	call UARTWriteByte
	call UARTWriteByte
	call UARTWriteByte
	call UARTWriteByte
	
	;==================================================================================================
	; Send acknowledge to PC
	;==================================================================================================
	bcf STATUS, RP0 ; Bank 0
	movlw CODE_ACKNOWLEDGE
	call UARTWriteByte
	goto Loop_Program_EEPROM
	
	;==================================================================================================
	; Programming finished
	;==================================================================================================
Programming_Finished:
	; Turn led off
	bcf STATUS, RP0 ; Bank 0
	bcf PORTB, 7
	
	; Wait for the "start program" code to be received
@Loop_Wait_Start_Program_Code:
	call UARTReadByte
	xorlw CODE_STARTING_PROGRAM
	btfss STATUS, Z
	goto @Loop_Wait_Start_Program_Code
	
	; Reset the program counter
	clrf PCLATH
	goto 0

;--------------------------------------------------------------------------------------------------
; Functions
;--------------------------------------------------------------------------------------------------
;==================================================================================================
; Wait until a byte is received from the UART.
; @return The received byte.
; @warning The function selects bank 0 and does not restore previous bank when returning.
;==================================================================================================
UARTReadByte:
	; Go to bank 0
	bcf STATUS, RP1
	bcf STATUS, RP0

	; Wait for a byte to be available
@Loop_UART_Read_Byte:
	btfss PIR1, RCIF
	goto @Loop_UART_Read_Byte

	; Read byte
	movf RCREG, W
	return

;==================================================================================================
; Wait until the UART transmit line becomes available and send
; a byte.
; @param Put the byte to transfer in register W.
; @warning The function selects bank 0 and does not restore previous bank when returning.
;==================================================================================================
UARTWriteByte:
	; Go to bank 0
	bcf STATUS, RP1
	bcf STATUS, RP0

	; Wait for for the Tx line to become available
@Loop_UART_Write_Byte:
	btfss PIR1, TXIF
	goto @Loop_UART_Write_Byte

	; Write byte
	movwf TXREG
	return

;==================================================================================================
; Wait until the current I2C operation finishes.
; @warning The function selects bank 0 and does not restore previous bank when returning.
;==================================================================================================
I2CWaitOperationCompletion:
	; Go to bank 0
	bcf STATUS, RP1
	bcf STATUS, RP0
	
	; Wait for the interrupt flag to trigger
@Loop_I2C_Wait_Operation_Completion:
	btfss PIR1, SSPIF
	goto @Loop_I2C_Wait_Operation_Completion
	bcf PIR1, SSPIF
	
	return

;--------------------------------------------------------------------------------------------------
; Jump to the flashed program startup code.
; It's located at the end of the memory to ease access to the remapped entry point.
;--------------------------------------------------------------------------------------------------
	org h'1FF8'
Start_Program:
	bcf STATUS, RP1 ; Go to bank 0
	bcf STATUS, RP0
	bcf PCLATH, 4 ; Go to program page 0
	bcf PCLATH, 3

	; Program's startup code will be stored here
	nop
	nop
	nop
	nop

;--------------------------------------------------------------------------------------------------
; Configuration word
;--------------------------------------------------------------------------------------------------
	__CONFIG _CP_OFF & _DEBUG_OFF & _WRT_ENABLE_ON & _CPD_OFF & _LVP_OFF & _BODEN_ON & _PWRTE_ON  & _WDT_OFF & _XT_OSC

	END