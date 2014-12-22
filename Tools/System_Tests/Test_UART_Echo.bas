PRINT "RS-232 TEST"

OPEN "COM1:115200,N,8" AS #1

CharactersSent& = 0
Counter% = 0

DO
	' Choose a character between 'a' and 'z'
	SentCharacter% = INT(RND * 26) + 97

	WRITE #1, CHR$(SentCharacter%)
	'PRINT "Sent : "; CHR$(SentCharacter%)
	
	INPUT #1, ReceivedCharacter$
	'PRINT "Received : "; ReceivedCharacter$
	
	IF ASC(ReceivedCharacter$) <> (SentCharacter% - 32) THEN
		PRINT "ERROR : characters are different"
		SYSTEM
	ENDIF
	
	CharactersSent& = CharactersSent& + 1
	Counter& = Counter& + 1
	
	IF Counter& = 10000 THEN
		PRINT "Sent : "; CharactersSent&
		Counter& = 0
	ENDIF
LOOP
