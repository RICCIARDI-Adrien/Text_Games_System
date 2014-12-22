; Bootloader for Text Games System.
; @author : Adrien RICCIARDI
; @version 1.0 : 23/06/2013, modified release bootloader code to create development version.
#include <p16f876.inc>

CODE_PROGRAMMING_START EQU h'FE'
CODE_ACKNOWLEDGE EQU h'EF'
CODE_STARTING_PROGRAM EQU 'r' ; Stands for "run"
ADDRESS_STARTUP EQU d'8030' ; Calculated manually...
EEPROM_BUS_ADDRESS EQU h'A0' ; Address for write commands

	org 0
	bsf PCLATH, 4 ; Go to program page 3
	bsf PCLATH, 3
	goto Bootloader_Start

	org ADDRESS_STARTUP
Bootloader_Start:
	;================================================================
	; Initialize UART
	;================================================================
	bsf STATUS, RP0 ; Go to bank 1

	movlw h'0C' ; Set 19200 bit/s rate
	movwf SPBRG

	movlw h'26' ; Asynchronous transmit enabled, 8-bit, high speed
	movwf TXSTA

	clrf TRISB ; Configure led port at the same time

	bcf STATUS, RP0 ; Go to bank 0
	movlw h'90' ; Serial port enabled, 8-bit, continuous reception enabled
	movwf RCSTA
	
	;================================================================
	; Initialize I2C module
	;================================================================
	bsf STATUS, RP0 ; Bank 1
	clrf SSPSTAT ; 400KHz speed mode, I2C input levels
	clrf SSPCON2 ; Reset communication flags
	movlw 2
	movwf SSPADD ; Set baud rate at approximately 333KHz (the most closer value for 400KHz)
	
	bcf STATUS, RP0 ; Bank 0
	movlw h'28'
	movwf SSPCON ; Enable I2C module in Master mode
	
	; Reset registers which hold the address
	clrf h'21'
	clrf h'22'

	;================================================================
	; Wait for the starting or the programming code
	;================================================================
Wait_Initial_Code:
	call UARTReadByte
	; Save the received byte as the comparison modifies its value
	movwf h'20' ; First available register in bank 0
	
	; Is it the starting code ?
	xorlw CODE_STARTING_PROGRAM
	btfsc STATUS, Z
	goto Start_Program
	
	; No, check with programming code
	movf h'20', W
	xorlw CODE_PROGRAMMING_START
	btfss STATUS, Z
	goto Wait_Initial_Code	

	; Light led
	bsf PORTB, 7

	;================================================================
	; Start programming flash
	;================================================================
	; Send acknowledge
	movlw CODE_ACKNOWLEDGE
	call UARTWriteByte

	; Ignore all received CODE_PROGRAMMING_START until a different dummy byte is received
@Loop_Ignore_Code_Programming_Start:
	call UARTReadByte
	xorlw CODE_PROGRAMMING_START
	btfsc STATUS, Z
	goto @Loop_Ignore_Code_Programming_Start
	
	;================================================================
	; Receive program instructions
	;================================================================
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
	
	;================================================================
	; Flash received instruction
	;================================================================
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

	;================================================================
	; Send acknowledge to PC
	;================================================================
	movlw CODE_ACKNOWLEDGE
	call UARTWriteByte
	goto Loop_Flash_Program_Instructions

	;================================================================
	; Programming loop of external EEPROM
	;================================================================
Loop_Program_EEPROM:
	; Command byte, used only to tell if the program is fully transfered or not
	bcf STATUS, RP0 ; Bank 0
	call UARTReadByte 
	xorlw CODE_PROGRAMMING_START
	btfsc STATUS, Z
	goto Programming_Finished
	
	; Save received byte into the first available register of bank 0
	call UARTReadByte
	movwf h'20'
	
	;================================================================
	; I2C write driver
	;================================================================
	; Send a start condition to the bus
	bsf STATUS, RP0 ; Bank 1
	movlw 1
	movwf SSPCON2
	; Wait for start condition completion
@Loop_Wait_Start_Condition_End:
	btfsc SSPCON2, SEN
	goto @Loop_Wait_Start_Condition_End
		
	; Send device address and write request
	bcf STATUS, RP0 ; Bank 0
	bcf PIR1, SSPIF
	movlw EEPROM_BUS_ADDRESS
	movwf SSPBUF
	; Wait for transmission end
@Loop_Wait_Device_Address_Transmission:
	btfss PIR1, SSPIF
	goto @Loop_Wait_Device_Address_Transmission
	
	; Wait for device acknowledge
	bsf STATUS, RP0 ; Bank 1
@Loop_Wait_Device_Address_Acknowledge:
	btfsc SSPCON2, ACKSTAT
	goto @Loop_Wait_Device_Address_Acknowledge
	
	; Send address high byte
	bcf STATUS, RP0 ; Bank 0
	bcf PIR1, SSPIF
	; Get high address byte from memory
	movf h'22', W
	movwf SSPBUF
	; Wait for end of transmission
@Loop_Wait_Address_High_Byte_Transmission:
	btfss PIR1, SSPIF
	goto @Loop_Wait_Address_High_Byte_Transmission
	
	; Wait for device acknowledge
	bsf STATUS, RP0 ; Bank 1
@Loop_Wait_Address_High_Byte_Acknowledge:
	btfsc SSPCON2, ACKSTAT
	goto @Loop_Wait_Address_High_Byte_Acknowledge
	
	; Send address low byte
	bcf STATUS, RP0 ; Bank 0
	bcf PIR1, SSPIF
	; Get low address byte from memory
	movf h'21', W
	movwf SSPBUF
@Loop_Wait_Address_Low_Byte_Transmission:
	btfss PIR1, SSPIF
	goto @Loop_Wait_Address_Low_Byte_Transmission
	
	; Wait for device acknowledge
	bsf STATUS, RP0 ; Bank 1
@Loop_Wait_Address_Low_Byte_Acknowledge:
	btfsc SSPCON2, ACKSTAT
	goto @Loop_Wait_Address_Low_Byte_Acknowledge
	
	; Send byte to write
	bcf STATUS, RP0 ; Bank 0
	bcf PIR1, SSPIF
	; Retrieve data byte from memory
	movf h'20', W
	movwf SSPBUF
	; Wait for end of transmission
@Loop_Wait_Data_Byte_Transmission:
	btfss PIR1, SSPIF
	goto @Loop_Wait_Data_Byte_Transmission
	
	; Wait for device acknowledge
	bsf STATUS, RP0 ; Bank 1
@Loop_Wait_Data_Byte_Acknowledge:
	btfsc SSPCON2, ACKSTAT
	goto @Loop_Wait_Data_Byte_Acknowledge
	
	; Send stop condition to start write operation
	bsf SSPCON2, PEN
	; Wait for stop condition completion
@Loop_Wait_Stop_Condition_End:
	btfsc SSPCON2, PEN
	goto @Loop_Wait_Stop_Condition_End
	
	;================================================================
	; Increment address
	;================================================================²
	bcf STATUS, RP0 ; Bank 0
	incf h'21', F
	; Propagate carry to high byte only if low byte is equal 0 (so it did a roll over)
	btfsc STATUS, Z
	incf h'22', F
	
	;================================================================
	; Wait for write operation completion
	;================================================================
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
	
	;================================================================
	; Send acknowledge to PC
	;================================================================
	bcf STATUS, RP0 ; Bank 0
	movlw CODE_ACKNOWLEDGE
	call UARTWriteByte
	goto Loop_Program_EEPROM
		
	;================================================================
	; Programming finished
	;================================================================
Programming_Finished:
	; Turn led off
	bcf STATUS, RP0 ; Bank 0
	bcf PORTB, 7
	
	; "Reset" the board 
	goto Wait_Initial_Code

;----------------------------------------------------------------------------------
; Functions
;----------------------------------------------------------------------------------
;================================================================
; Wait until a byte is received from the UART.
; @return The received byte.
; @warning The function selects bank 0 and does not restore
;          previous bank when returning.
;================================================================
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

;================================================================
; Wait until the UART transmit line becomes available and send
; a byte.
; @param Put the byte to transfer in register W.
; @warning The function selects bank 0 and does not restore
;          previous bank when returning.
;================================================================
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

;----------------------------------------------------------------------------------
; Jump to the flashed program startup code (it's located at the end of the memory 
; to ease access to the remapped entry point.
;----------------------------------------------------------------------------------
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

;----------------------------------------------------------------------------------
; Configuration word
;----------------------------------------------------------------------------------
	__CONFIG _CP_OFF & _DEBUG_OFF & _WRT_ENABLE_ON & _CPD_OFF & _LVP_OFF & _BODEN_ON & _PWRTE_ON  & _WDT_OFF & _XT_OSC

	END