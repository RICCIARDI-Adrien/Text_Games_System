' How many same letters are sent successively
CONST LETTERSPERROW = 80

DIM ReadLetterCode AS UBYTE
DIM CurrentLetterCode AS UBYTE
DIM SuccessfullyReceivedCounter AS UINTEGER
DIM DisplayCounter AS INTEGER
DIM i AS INTEGER
DIM COMPort AS STRING
DIM BaudRate AS STRING
DIM Result AS INTEGER

FUNCTION UARTReadByte() AS UBYTE
	DIM ReadByte AS UBYTE
	DIM ReadBytesCount AS INTEGER
	
	DO
		GET #1,, ReadByte, 1, ReadBytesCount	
	LOOP UNTIL ReadBytesCount >= 1
	
	UARTReadByte = ReadByte
END FUNCTION

' Check parameters
IF __FB_ARGC__ <> 3 THEN
	PRINT "ERROR : bad parameters."
	PRINT "Usage : "; COMMAND$(0); " COM_Port Baud_Rate"
	END
ENDIF

' Try to open the serial port
COMPort = COMMAND$(1)
BaudRate = COMMAND$(2)
Result = OPEN COM(COMPort + ":" + BaudRate + ",N,8,1,CS0,DS0,CD0,OP0", FOR BINARY AS #1)
IF Result <> 0 THEN
	PRINT "ERROR : can't open COM port."
	END
ENDIF

CurrentLetterCode = ASC("a")
SuccessfullyReceivedCounter = 0
DisplayCounter = 0

' Send the "start" character
ReadLetterCode = ASC("@")
PUT #1,, ReadLetterCode, 1

DO
	FOR i = 1 TO LETTERSPERROW
		ReadLetterCode = UARTReadByte()
		
		IF ReadLetterCode <> CurrentLetterCode THEN
			PRINT "ERROR : row position = "; i; "; current letter is '"; CHR$(CurrentLetterCode); "' but the received letter was '"; CHR$(ReadLetterCode); "'."
			CLOSE
			END
		ENDIF
		SuccessfullyReceivedCounter = SuccessfullyReceivedCounter + 1
		
		' Display the test state each 10000 iterations
		DisplayCounter = DisplayCounter + 1
		IF DisplayCounter = 10000 THEN
			PRINT "Successfully received "; SuccessfullyReceivedCounter; " characters."
			DisplayCounter = 0
		ENDIF
	NEXT
	
	' Determine the next letter to test
	CurrentLetterCode = CurrentLetterCode + 1
	IF CurrentLetterCode > ASC("z") THEN CurrentLetterCode = ASC("a")
LOOP
	
Errors:
	PRINT "ERROR : "; ERL
	END