; Bootloader for Text Games System.
; @author : Adrien RICCIARDI
; @version 1.0 : 19/02/2013
; @version 1.1 : 27/02/2013, added I2C EEPROM programming section.
; @version 1.2 : 19/06/2015, added a waiting loop at the end of the firmware update, merged the release and the development bootloaders into a single file.
; @version 1.3 : 24/06/2015, improved I2C drivers to fasten download time.
; @version 1.4 : 27/07/2015, used acknowledge polling to fasten the EEPROM write time.
; @version 2.0 : 06/08/2015, completely rewritten the bootloader to be more reliable.
; @version 2.1 : 15/05/2016, used a "program presence" flag to be more reliable when a transfer fails.
#include <p16f876.inc>

;--------------------------------------------------------------------------------------------------
; Constants
;--------------------------------------------------------------------------------------------------
; The protocol between the bootloader and the PC codes
PROTOCOL_CODE_ACKNOWLEDGE EQU h'42' ; The bootloader acknowledges data received from the PC
PROTOCOL_CODE_START_PROGRAMMING EQU h'5A' ; The PC connects to the board
PROTOCOL_CODE_PROGRAMMING_FINISHED EQU h'80' ; The PC sends an invalid high byte address (the PIC flash word highest address is 0x1FFF) to signal the end of the flash instructions
PROTOCOL_CODE_START_PROGRAM EQU 'r' ; Stands for "run"

; The bootloader location at the far end of the program memory
BOOTLOADER_START_ADDRESS EQU d'8035' ; Computed manually...
BOOTLOADER_IS_PROGRAM_PRESENT_FLAG_ADDRESS EQU h'1FFF'

; I2C EEPROM address with write request
EEPROM_WRITE_ADDRESS EQU h'A0'

; External EEPROM data value
DATA_VARIABLE_ADDRESS EQU h'70' ; The first available byte of the RAM area shared between all banks (so there is no need to change bank every time to access the variable)
; External EEPROM data address
ADDRESS_VARIABLE_LOW_BYTE_ADDRESS EQU h'71'
ADDRESS_VARIABLE_HIGH_BYTE_ADDRESS EQU h'72'

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
	; Determine whether the programming mode should be launched or not
	;==================================================================================================
	bsf STATUS, RP1 ; Bank 2
	
	; Set read address
	movlw HIGH(BOOTLOADER_IS_PROGRAM_PRESENT_FLAG_ADDRESS)
	movwf EEADRH
	movlw LOW(BOOTLOADER_IS_PROGRAM_PRESENT_FLAG_ADDRESS)
	movwf EEADR
	
	; Read the program memory word
	bsf STATUS, RP0 ; Bank 3
	bsf EECON1, EEPGD ; Select program memory
	bsf EECON1, RD ; Start read operation
	nop ; Two 'nop' are required for the read to complete
	nop
	
	; Is the flag cleared (= 0x3FFF) ?
	bcf STATUS, RP0 ; Bank 2
	; Compare high byte
	movlw h'3F'
	xorwf EEDATH, W
	btfss STATUS, Z ; Skip if equal
	goto Start_Program ; High byte is different from 0x3F, start the program
	; Compare low byte
	movlw h'FF'
	xorwf EEDATA, W
	btfss STATUS, Z ; Skip if equal
	goto Start_Program ; Low byte is different from 0xFF, start the program

	;==================================================================================================
	; Initialize the UART module
	;==================================================================================================
	bcf STATUS, RP1 ; Bank 1
	bsf STATUS, RP0

	movlw h'0C' ; Set 19200 bit/s rate
	movwf SPBRG

	movlw h'26' ; Asynchronous transmit enabled, 8-bit, high speed
	movwf TXSTA

	bcf TRISB, 7 ; Configure the led port at the same time

	bcf STATUS, RP0 ; Go to bank 0
	movlw h'90' ; Serial port enabled, 8-bit, continuous reception enabled
	movwf RCSTA
	
	;==================================================================================================
	; Initialize the I2C module
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
	
	; Reset the registers which hold the address
	clrf ADDRESS_VARIABLE_LOW_BYTE_ADDRESS
	clrf ADDRESS_VARIABLE_HIGH_BYTE_ADDRESS

	;==================================================================================================
	; Start flash programming
	;==================================================================================================
	; Light the led
	bsf PORTB, 7

	; Wait for the start programming code
@Loop_Wait_For_Start_Programming_Code:
	call UARTReadByte ; Bank 0
	xorlw PROTOCOL_CODE_START_PROGRAMMING
	btfss STATUS, Z
	goto @Loop_Wait_For_Start_Programming_Code

	; Tell the PC that the bootloader is ready
	movlw PROTOCOL_CODE_ACKNOWLEDGE
	call UARTWriteByte ; Bank 0

	;==================================================================================================
	; Receive program instructions
	;==================================================================================================
Loop_Program_Flash:
	; Receive the address high byte
	call UARTReadByte ; Bank 0
	bsf STATUS, RP1 ; Bank 2
	movwf EEADRH

	; Check if the received byte was the end of programming code
	xorlw PROTOCOL_CODE_PROGRAMMING_FINISHED
	btfsc STATUS, Z
	goto Loop_Program_EEPROM
	
	; Receive the address low byte
	call UARTReadByte ; Bank 0
	bsf STATUS, RP1 ; Bank 2
	movwf EEADR
	
	; Receive the instruction high byte
	call UARTReadByte ; Bank 0
	bsf STATUS, RP1 ; Bank 2
	movwf EEDATH

	; Receive the instruction low byte
	call UARTReadByte ; Bank 0
	bsf STATUS, RP1 ; Bank 2
	movwf EEDATA

	;==================================================================================================
	; Flash the received instruction
	;==================================================================================================
	; Reset the end of write flag
	bcf STATUS, RP1 ; Bank 0
	bcf PIR2, EEIF

	; Configure writing
	bsf STATUS, RP1 ; Bank 3
	bsf STATUS, RP0
	bsf EECON1, EEPGD ; Select the flash memory
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
	movlw PROTOCOL_CODE_ACKNOWLEDGE
	call UARTWriteByte ; Bank 0
	goto Loop_Program_Flash
	
	;==================================================================================================
	; Programming loop of the external EEPROM
	;==================================================================================================
Loop_Program_EEPROM:
	; Command byte, used only to tell if the program is fully transferred or not
	call UARTReadByte ; Bank 0
	xorlw PROTOCOL_CODE_PROGRAMMING_FINISHED
	btfsc STATUS, Z
	goto Programming_Finished
	
	; Save received byte to RAM
	call UARTReadByte ; Bank 0
	movwf DATA_VARIABLE_ADDRESS
	
	;==================================================================================================
	; Write to the I2C EEPROM
	;==================================================================================================
	; Send a start condition to the bus
	bsf STATUS, RP0 ; Bank 1
	bsf SSPCON2, SEN
	call I2CWaitOperationCompletion ; Bank 0
	
	; Send the device address with a write request
	movlw EEPROM_WRITE_ADDRESS
	movwf SSPBUF
	call I2CWaitOperationCompletion ; Bank 0
	
	; Send the address high byte
	movf ADDRESS_VARIABLE_HIGH_BYTE_ADDRESS, W
	movwf SSPBUF
	call I2CWaitOperationCompletion ; Bank 0
	
	; Send the address low byte
	movf ADDRESS_VARIABLE_LOW_BYTE_ADDRESS, W
	movwf SSPBUF
	call I2CWaitOperationCompletion ; Bank 0
	
	; Send the data to write
	movf DATA_VARIABLE_ADDRESS, W
	movwf SSPBUF
	call I2CWaitOperationCompletion ; Bank 0
	
	; Send a stop condition to start the write operation
	bsf STATUS, RP0 ; Bank 1
	bsf SSPCON2, PEN
	call I2CWaitOperationCompletion ; Bank 0
	
	;==================================================================================================
	; Increment the EEPROM address
	;==================================================================================================
	incf ADDRESS_VARIABLE_LOW_BYTE_ADDRESS, F
	; Propagate carry to high byte only if low byte is equal 0 (so it did a roll over)
	btfsc STATUS, Z
	incf ADDRESS_VARIABLE_HIGH_BYTE_ADDRESS, F
	
	;==================================================================================================
	; Wait for the write operation completion
	;==================================================================================================
@Loop_EEPROM_Acknowledge_Polling:
	; Send a repeated start to avoid hanging the I2C hardware module
	bsf STATUS, RP0 ; Bank 1
	bsf SSPCON2, RSEN
	call I2CWaitOperationCompletion ; Bank 0
	
	; Send the device address with a write request
	movlw EEPROM_WRITE_ADDRESS
	movwf SSPBUF
	call I2CWaitOperationCompletion ; Bank 0
	
	; Did the device acknowledge ?
	bsf STATUS, RP0 ; Bank 1
	btfsc SSPCON2, ACKSTAT
	goto @Loop_EEPROM_Acknowledge_Polling
	
	; Send a stop (not required by the EEPROM protocol) to avoid hanging the I2C hardware module
	bsf SSPCON2, PEN
	call I2CWaitOperationCompletion ; Bank 0
	
	;==================================================================================================
	; Send acknowledge to PC
	;==================================================================================================
	movlw PROTOCOL_CODE_ACKNOWLEDGE
	call UARTWriteByte ; Bank 0
	goto Loop_Program_EEPROM
	
	;==================================================================================================
	; Both memories were successfully programmed
	;==================================================================================================
Programming_Finished:
	; Disable the "programming mode" flag
	bsf STATUS, RP0 ; Bank 1
	bsf TRISB, 0

	; Turn the led off
	bcf STATUS, RP0 ; Bank 0
	bcf PORTB, 7

	; Wait for the "start program" code to be received
@Loop_Wait_Start_Program_Code:
	call UARTReadByte ; Bank 0
	xorlw PROTOCOL_CODE_START_PROGRAM
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
	org h'1FF7'
Start_Program:
	bcf STATUS, RP1 ; Go to bank 0
	bcf STATUS, RP0
	bcf PCLATH, 4 ; Go to program page 0
	bcf PCLATH, 3

	; Program startup code will be stored here
	nop
	nop
	nop
	nop

	; Program present flag
	dw h'3FFF' ; Tell the flash is empty to force bootloader mode on first boot
	
;--------------------------------------------------------------------------------------------------
; Configuration word
;--------------------------------------------------------------------------------------------------
	__CONFIG _CP_OFF & _DEBUG_OFF & _WRT_ENABLE_ON & _CPD_OFF & _LVP_OFF & _BODEN_ON & _PWRTE_ON  & _WDT_OFF & _XT_OSC

	END