/****************************************/
/*			+-----	  /|\    .------	*/
/*          |      \  |||   /			*/
/*     ___  |       | |||  |			*/
/*    /	__\ |______/  |||  |			*/
/*    \___. |        -----  \			*/
/*			|         +|+    *------	*/
/*                     |				*/
/*                     *				*/
/*	/\  |\ \  / == |\| --- | | |\ == /  */
/* /  \ |/  \/  -- | |  |  |_| |\ -- /  */
/*										*/
/*	ePIC Adventures Project				*/
/*	Only for Strawberry PIC				*/
/*										*/
/*	File name: 	gui.h					*/
/*	Author:		Denis CARLUS			*/
/*	Date:		23/06/2013				*/
/*	Version:	1.0						*/
/****************************************/


#ifndef __GUI_HANDLER__
#define __GUI_HANDLER__

#include "Strings.h"
#include "characters.h"
#include "items.h"

#define STRAWBERRY_PIC_UPGRADED

#define endline() ScreenWriteROMString(TOOL_ENDLINE)

/**
 * @brief Write a text in the mid frame.
 * @param	EEPROMText		Address in EEPROM of the text to display
 */
	#ifdef STRAWBERRY_PIC_UPGRADED
void writeSecondaryText(unsigned short EEPROMText) {
	// Clear the line of text
	ScreenSetCursorLocation(6, 1) ;
	ScreenWriteROMString(GUI_FULL_EMPTY) ;
	// Write the text
	ScreenSetCursorLocation(6, 3) ;
	ScreenWriteROMString(EEPROMText) ;
}
	#endif

/**
 * @brief Write a text in the top frame.
 * @param	EEPROMCharacter	Address in EEPROM of the name of the character
 * @param	EEPROMText		Address in EEPROM of the text to display
 */
void writeAction(unsigned short EEPROMCharacter, unsigned short EEPROMText) {
	// Clear the line of text
	ScreenSetCursorLocation(2, 1) ;
	ScreenWriteROMString(GUI_FULL_EMPTY) ;
	// Print the character's name
	ScreenSetCursorLocation(2, 3) ;
	ScreenWriteROMString(EEPROMCharacter) ;
	// Print the action of the character (its name is normalized to 8 chars)
	ScreenSetCursorLocation(2, 12) ;
	ScreenWriteROMString(EEPROMText) ;
}

/**
 * @brief Write a text in the top frame after player's action.
 * @param	EEPROMText		Address in EEPROM of the text to display
 */
inline void writePlayerAction(unsigned short EEPROMText) {
	writeAction(PLAYER_ONE, EEPROMText) ;
}


/**
 * @brief Display the values of the statistics of the player's character.
 * @param row			Row to replace values.
 * @param value			Value to display.
 */
inline void displayPlayerStatValue(unsigned char row, unsigned char value) {
	ScreenSetCursorLocation(row, 48) ;
	ScreenWriteUnsignedInteger(value) ;
}

/**
 * @brief Display the keys and values of the statistics of the player's character.
 * @param row			Row to replace key and value values.
 * @param value			Value to display.
 */
void displayPlayerStat(unsigned row, unsigned short key, unsigned char value) {
	ScreenSetCursorLocation(row, 35) ;
	ScreenWriteROMString(key) ;
	displayPlayerStatValue(row, value) ;
}

/** brief Display the level of the player's character. */
inline void displayPlayerLevel() {
	displayPlayerStat(10, STAT_LEVEL, playerOne.level) ;
}

/** @brief Display the experience of the player's character. */
inline void displayPlayerExp() {
	displayPlayerStat(11, STAT_EXP, playerOne.exp) ;
}

/** @brief Display the strength of the player's character. */
inline void displayPlayerStr() {
	displayPlayerStat(13, STAT_STR, playerOne.str) ;
}


/** @brief Update the HP of the player in the GUI. */
void updatePlayerHP() {
	ScreenSetCursorLocation(12, 48) ;
	ScreenWriteROMString(TOOL_STAT_ERASE) ;
	ScreenSetCursorLocation(12, 48) ;
	ScreenWriteUnsignedInteger(playerOne.hp_cur) ;
	ScreenSetCursorLocation(12, 51) ;
	ScreenWriteCharacter('/') ;
	ScreenWriteUnsignedInteger(playerOne.hp_max) ;
}

/** @brief Update the strength of the player in the GUI. */
inline void updatePlayerStr() {
	displayPlayerStatValue(13, playerOne.str) ;
}

/** @brief Update the defence of the player in the GUI. */
inline void updatePlayerDef() {
	displayPlayerStatValue(14, playerOne.str) ;
}

/** @brief Update the precision of the player in the GUI. */
inline void updatePlayerPrc() {
	displayPlayerStatValue(15, playerOne.str) ;
}

/** @brief Update the velocity of the player in the GUI. */
inline void updatePlayerVel() {
	displayPlayerStatValue(16, playerOne.str) ;
}

/** @brief Update the VGA (Velocity Gauge of Attack) of the player in the GUI. */
void updateVGA() {
	ScreenSetCursorLocation(17, 45) ;

	unsigned char vga = playerOne.vga >> 4 ;
	for (; vga > 0 ; vga--) {
		ScreenWriteCharacter('=') ;
	}
}

/**
 * @brief Restore the informations of the player in the GUI.
 * @param extra		FALSE will only update experience, HP and strength.
 *					Use TRUE to update more informations as name and level.
 */
void restorePlayerInformations(bool extra) {
	// Put the informations on the player back in the right frame
	if (extra) {
			// Name
		ScreenSetCursorLocation(9, 35) ;
		ScreenWriteROMString(PLAYER_ONE) ;
			// Level
		displayPlayerLevel() ;
	}

	displayPlayerExp() ;
		// HP
	ScreenSetCursorLocation(12, 35) ;
	ScreenWriteROMString(STAT_HP) ;
	updatePlayerHP() ;
		// Strength
	displayPlayerStr() ;
}

/** @brief Update the HP of the enemy in the GUI. */
void updateEnemyHP() {
	ScreenSetCursorLocation(11, 8) ;
	ScreenWriteROMString(TOOL_STAT_ERASE) ;
	ScreenSetCursorLocation(11, 8) ;
	ScreenWriteUnsignedInteger(enemy.hp_cur) ;
	ScreenSetCursorLocation(11, 11) ;
	ScreenWriteCharacter('/') ;
	ScreenWriteUnsignedInteger(enemy.hp_max) ;
}

/**
 * @brief Clear a row in the actions frame (left frame) and replace with the given text.
 * @param row			Row to clear in the left frame.
 * @param EEPROMText	Text replacing the previous one.
 */
void replaceTextInLeftFrame(unsigned char row, unsigned short EEPROMText) {
	ScreenSetCursorLocation(row, 2) ;

	unsigned col = 0 ;
	for (; col < 28 ; col++) {
		ScreenWriteROMString(TOOL_BLANKSPACE) ;
	}

	ScreenSetCursorLocation(row, 3) ;
	ScreenWriteROMString(EEPROMText) ;
}

/**
 * @brief 	The "Pacific View" is the screen displayed when the player is not fighting.
 *			The player can choose the direction where to go... Maybe it will have an effect, or not!
 *			As the screen cannot be entirely redrawn because of latency (one second to refill the screen)
 *			only, few rows are redrawn, representing almost 200 characters (so that it can theoretically 
 *			take around 100 ms to be redrawn, what is perfectly correct as the screen changes are not
 *			so frequent).
 *			This is a representation of the Pacific View. The numbers on the right/top are the number of
 *			row/column. The asterisks show which lines have to be redrawn:
 *			<pre>
 *		          1 1 1 1 1 2 2 2 2 2 3 3 3 3 3 4 4 4 4 4 5 5 5 5 5 6 6
 *	    1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3
 *		# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 *
 *		+-------------------------------------------------------------+		1
 *		| LEETSPIK a trouve POTION MAG.								  |		2
 *		+-------------------------------------------------------------+		3
 *																			4
 *		+-------------------------------------------------------------+		5
 *		| Balade dans les bois...									  |		6
 *		+-------------------------------------------------------------+		7
 *		+-----------------------------+ +-----------------------------+		8
 *		| Objet trouve: POTION MAG.	  | | LEETSPIK					  |		9
 *		| [0] Ramasser				  | | Niveau :     4              |		10
 *		+-----------------------------+ | Experience : 214			  |		11	*
 *		+-----------------------------+ | HP :         10 /12		  |		12	*
 *		| ACTIONS A EFFECTUER		  | | Force :      9			  |		13	*
 *		| [1] Avancer                 | | Defense :    7			  |		14
 *		| [2] Reculer                 | | Precision :  64			  |		15
 *		| [3] Droite                  | | Rapidite :   59			  |		16
 *		| [4] Gauche                  | | Attente: [%%%%%%%%%%%%%%%%] |		17
 *		+-----------------------------+ +-----------------------------+		18
 *			</pre>
 */
void updateToPacificView() {
	// Change the borders of the left frame
	ScreenSetCursorLocation(11, 1) ;
	ScreenWriteROMString(GUI_PACIFIC_UPDATE) ;

	// Put the informations on the player back in the right frame
	restorePlayerInformations(false) ;

	// Display the texts in the bottom left frame
	replaceTextInLeftFrame(14, GUI_ACTION1_PACIFIC) ;
	replaceTextInLeftFrame(15, GUI_ACTION2_PACIFIC) ;
	replaceTextInLeftFrame(16, GUI_ACTION3_PACIFIC) ;
	replaceTextInLeftFrame(17, GUI_ACTION4_PACIFIC) ;

	// Update the text in the upper frame
	#ifdef STRAWBERRY_PIC_UPGRADED
	writeSecondaryText(GUI_VIEW_PACIFIC) ;
	#endif
}


/**
 * @brief 	The "Fight View" is the screen displayed when the player is fighting.
 *			The player can choose the action to perform : attack, select an item, flee...
 *			As the screen cannot be entirely redrawn because of latency (one second to refill the screen)
 *			only, few rows are redrawn, representing almost 200 characters (so that it can theoretically 
 *			take around 100 ms to be redrawn, what is perfectly correct as the screen changes are not
 *			so frequent).
 *			This is a representation of the Fight View. The numbers on the right/top are the number of
 *			row/column. The asterisks show which lines have to be redrawn:
 *			<pre>
 *		          1 1 1 1 1 2 2 2 2 2 3 3 3 3 3 4 4 4 4 4 5 5 5 5 5 6 6
 *	    1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3
 *		# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 *
 *		+-------------------------------------------------------------+		1
 *		| LEETSPIK a trouve POTION MAG.								  |		2
 *		+-------------------------------------------------------------+		3
 *																			4
 *		+-------------------------------------------------------------+		5
 *		| Balade dans les bois...									  |		6
 *		+-------------------------------------------------------------+		7
 *		+-----------------------------+ +-----------------------------+		8
 *		| GOBELIN					  | | LEETSPIK					  |		9   *
 *		| Niveau : 5				  | | Niveau :     4              |		10  *
 *		| HP : 13 /15				  | | Experience : 214			  |		11	*
 *		+-----------------------------+ | HP : 		   10 /12		  |		12	*
 *		+-----------------------------+ | Force :      9			  |		13	*
 *		| ACTIONS A EFFECTUER         | | Defense :    7			  |		14
 *		| [1] Attaquer				  | | Precision :  64			  |		15
 *		| [2] Inventaire			  | | Rapidite :   59			  |		16
 *		| [3] Fuir le combat... 	  | | Attente: [%%%%%%%%%%%%%%%%] |		17
 *		+-----------------------------+ +-----------------------------+		18
 *			</pre>
 */
void updateToFightView() {
	// Change the borders of the top left frame
	ScreenSetCursorLocation(9, 1) ;
	ScreenWriteROMString(GUI_FIGHT_UPDATE) ;

	// Put the informations on the player back in the right frame
	restorePlayerInformations(true) ;

	// Display informations on the enemy
		// Enemy name
	replaceTextInLeftFrame(9, getEnemyName()) ;
		// Enemy level
	replaceTextInLeftFrame(10, STAT_LEVEL) ;
	ScreenSetCursorLocation(10, 12) ;
	ScreenWriteUnsignedInteger(enemy.level) ;
		// Enemy HP
		// Line 11 is already cleared!
	updateEnemyHP() ;
		// Line 12 is already cleared! And nothing else to display!
		// Line 13 is already cleared! And nothing else to display!

	// Display the texts in the bottom left frame
	replaceTextInLeftFrame(14, GUI_ACTION_PACIFIC) ;
	replaceTextInLeftFrame(15, GUI_ACTION1_FIGHT) ;
	replaceTextInLeftFrame(16, GUI_ACTION2_FIGHT) ;
	replaceTextInLeftFrame(17, GUI_ACTION3_FIGHT) ;

	// Update the text in the upper frame
	#ifdef STRAWBERRY_PIC_UPGRADED
	writeSecondaryText(GUI_VIEW_FIGHT) ;
	#endif
}


/**
 * @brief 	The "Inventory View" is the screen displayed when the player wants to use an item.
 *			The player can choose the item to use, if available...
 *			As the screen cannot be entirely redrawn because of latency (one second to refill the screen)
 *			only, few rows are redrawn, representing almost 200 characters (so that it can theoretically 
 *			take around 100 ms to be redrawn, what is perfectly correct as the screen changes are not
 *			so frequent).
 *			This is a representation of the Inventory View. The numbers on the right/top are the number of
 *			row/column. The asterisks show which lines have to be redrawn:
 *			<pre>
 *		          1 1 1 1 1 2 2 2 2 2 3 3 3 3 3 4 4 4 4 4 5 5 5 5 5 6 6
 *	    1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3
 *		# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 *
 *		+-------------------------------------------------------------+		1
 *		| LEETSPIK a trouve POTION MAG.								  |		2
 *		+-------------------------------------------------------------+		3
 *																			4
 *		+-------------------------------------------------------------+		5
 *		| Balade dans les bois...									  |		6
 *		+-------------------------------------------------------------+		7
 *		+-----------------------------+ +-----------------------------+		8
 *		| GOBELIN  13 /15			  | | LEETSPIK					  |		9	*
 *		+-----------------------------+ | Niveau : 4				  |		10  *
 *		+-----------------------------+ | Experience : 214			  |		11	*
 *		| OBJET A UTILISER			  | | HP : 		   10/12		  |		12	*
 *		| [0] Retour au combat        | | Force :      9			  |		13	*
 *		| [1] ODEVI	  x 2	(10 HP)   | | Defense :    7			  |		14
 *		| [2] LICOEUR x 3	(25 HP)   | | Precision :  64			  |		15
 *		| [3] POTION  x 1	(50 HP)   | | Rapidite :   59			  |		16
 *		| [4] ELIXIR  x 0	(255HP)	  | | Attente: [%%%%%%%%%%%%%%%%] |		17
 *		+-----------------------------+ +-----------------------------+		18
 *			</pre>
 */
void updateToInventoryView() {
	// Change the borders of the top left frame
	ScreenSetCursorLocation(9 ,1) ;
	ScreenWriteROMString(GUI_INVENT_UPDATE) ;

	// Put the informations on the player back in the right frame
	restorePlayerInformations(true) ;

	// Display informations on the enemy
		// Enemy name and life
	ScreenSetCursorLocation(9, 3) ;
	ScreenWriteROMString(getEnemyName()) ;
	ScreenSetCursorLocation(9, 12) ;
	ScreenWriteUnsignedInteger(enemy.hp_cur) ;
	ScreenSetCursorLocation(9, 15) ;
	ScreenWriteCharacter('/') ;
	ScreenWriteUnsignedInteger(enemy.hp_max) ;

	// Display the texts in the bottom left frame
	replaceTextInLeftFrame(12, GUI_ACTION_ITEMUSE) ;
	replaceTextInLeftFrame(13, GUI_ACTION1_ITEMUSE) ;
	replaceTextInLeftFrame(14, GUI_ACTION2_ITEMUSE) ;
	ScreenSetCursorLocation(14, 17) ;
	ScreenWriteUnsignedInteger(countPotion(MIN_POTION)) ;
	replaceTextInLeftFrame(15, GUI_ACTION3_ITEMUSE) ;
	ScreenSetCursorLocation(15, 17) ;
	ScreenWriteUnsignedInteger(countPotion(MINPLUS_POTION)) ;
	replaceTextInLeftFrame(16, GUI_ACTION4_ITEMUSE) ;
	ScreenSetCursorLocation(16, 17) ;
	ScreenWriteUnsignedInteger(countPotion(MID_POTION)) ;
	replaceTextInLeftFrame(17, GUI_ACTION5_ITEMUSE) ;
	ScreenSetCursorLocation(17, 17) ;
	ScreenWriteUnsignedInteger(countPotion(MAX_POTION)) ;
}


/**
 * @brief 	The "Level Up View" is the screen displayed when the player becomes stronger.
 *			The player can choose the elements to improve in the statistics of the character...
 *			As the screen cannot be entirely redrawn because of latency (one second to refill the screen)
 *			only, few rows are redrawn, representing almost 200 characters (so that it can theoretically 
 *			take around 100 ms to be redrawn, what is perfectly correct as the screen changes are not
 *			so frequent).
 *			This is a representation of the Level Up View. The numbers on the right/top are the number of
 *			row/column. The asterisks show which lines have to be redrawn:
 *			<pre>
 *		          1 1 1 1 1 2 2 2 2 2 3 3 3 3 3 4 4 4 4 4 5 5 5 5 5 6 6
 *	    1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3
 *		# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 *
 *		+-------------------------------------------------------------+		1
 *		| GOBELIN est KO !               							  |		2
 *		+-------------------------------------------------------------+		3
 *																			4
 *		+-------------------------------------------------------------+		5
 *		| Repartition des points d'experience...					  |		6
 *		+-------------------------------------------------------------+		7
 *		+-----------------------------+ +-----------------------------+		8
 *		| Points a distribuer: 4  	  | | LEET						  |		9
 *		| 							  | | Niveau : 4				  |		10
 *		| Augmenter les statistiques  | | Experience : 214			  |		11
 *		| [1] HP + 1        (max: 255)| | HP : 10/12				  |		12	*
 *		| [2] Force + 1     (max: 100)| | Force : 9					  |		13	*
 *		| [3] Defense + 1   (max: 100)| | Defense : 7				  |		14
 *		| [4] Precision + 1 (max: 100)| | Precision : 64			  |		15
 *		| [5] Rapidite + 1  (max: 100)| | Rapidite : 59				  |		16
 *		| [6] Aleatoire		  		  | | VGA : [%%%%%%%%%%%%%%%%]    |		17
 *		+-----------------------------+ +-----------------------------+		18
 *			</pre>
 *
 * @param points	Amount of points to distribute among the "stats"
 */
	#ifdef STRAWBERRY_PIC_UPGRADED
void updateToLevelUpView() {
	// Change the borders of the top left frame
	ScreenSetCursorLocation(12, 1) ;
	ScreenWriteROMString(GUI_LEVELUP_UPDATE) ;

	// Put the informations on the player back in the right frame
	restorePlayerInformations(false) ;

	// Display the texts in the left frame
	replaceTextInLeftFrame(9, MISC_LEVELUP_POINTS) ;
	ScreenSetCursorLocation(9, 22) ;
	ScreenWriteUnsignedInteger(POINTS_DISTRIBUTION) ;			// see POINTS_DISTRIBUTION in characters.h
	replaceTextInLeftFrame(10, TOOL_BLANKSPACE) ;	// Line 10
	replaceTextInLeftFrame(11, MISC_INCREASE_STATS) ;
	replaceTextInLeftFrame(12, GUI_ACTION1_LEVELUP) ;
	replaceTextInLeftFrame(13, GUI_ACTION2_LEVELUP) ;
	replaceTextInLeftFrame(14, GUI_ACTION3_LEVELUP) ;
	replaceTextInLeftFrame(15, GUI_ACTION4_LEVELUP) ;
	replaceTextInLeftFrame(16, GUI_ACTION5_LEVELUP) ;
	replaceTextInLeftFrame(17, GUI_ACTION6_LEVELUP) ;
}
	#endif

/**
 * @brief 	Setup of the Graphic User Interface. Its made as a full "Pacific View".
 *			The screen is 80x24 characters.
 */
void setupGUI() {
								/*** DRAW THE EMPTY GUI ***/
	ScreenWriteROMString(GUI_SETUP_PART1) ;
	ScreenWriteROMString(GUI_SETUP_PART2) ;
	ScreenWriteROMString(GUI_SETUP_PART3) ;

	// Display the informations on the player's character
	displayPlayerLevel() ;
	displayPlayerExp() ;
	displayPlayerStr() ;
	displayPlayerStat(14, STAT_DEF, playerOne.def) ;
	displayPlayerStat(15, STAT_PRC, playerOne.prc) ;
	displayPlayerStat(16, STAT_VEL, playerOne.vel) ;

		// HP
	updatePlayerHP() ;
		
		// Velocity Gauge Attack
	ScreenSetCursorLocation(17, 35) ;
	ScreenWriteROMString(STAT_VGA) ;
}



/**
 * @brief Create a character to begin a new adventure !
 */
void character_wizard() {
	#ifdef STRAWBERRY_PIC_UPGRADED
	unsigned char valid ;

    do {
        ScreenClear() ;
		
		// Display the game title
		endline() ;
		ScreenWriteROMString(GAME_TITLE) ;
		endline() ;

        // Ask for its rank
		ScreenWriteROMString(CREATE_RANK) ;
		endline() ;
		ScreenWriteROMString(RANKS_LIST) ;

        // Process the choice
        unsigned char rank = KeyboardReadCharacter() ;
        switch(rank) {
            case '1':
	#endif
                playerOne.str = 8 ;
                playerOne.def = 10 ;
                playerOne.prc = 52 ;
				playerOne.vel = 6 ;
                playerOne.hp_max = 15 ;
	#ifdef STRAWBERRY_PIC_UPGRADED
                break ;

            case '2':
                playerOne.str = 5 ;
                playerOne.def = 8 ;
                playerOne.prc = 73 ;
				playerOne.vel = 8 ;
                playerOne.hp_max = 10 ;
                break ;

            case '3':
                playerOne.str = 5 ;
                playerOne.def = 15 ;
                playerOne.prc = 64 ;
				playerOne.vel = 11 ;
                playerOne.hp_max = 12 ;
                break ;

            case '4':
                playerOne.str = 6 ;
                playerOne.def = 5 ;
                playerOne.prc = 92 ;
				playerOne.vel = 10 ;
                playerOne.hp_max = 12 ;
                break ;
        }
	#endif
        playerOne.hp_cur = playerOne.hp_max ;
		playerOne.vga = RandomGenerateNumber() ;

	#ifdef STRAWBERRY_PIC_UPGRADED
        // Display the statistics of the player's character
		ScreenClear() ;
        ScreenWriteROMString(STATISTICS) ;
        endline() ;
        ScreenWriteROMString(STAT_LEVEL) ;
		ScreenWriteUnsignedInteger(playerOne.level) ;
		endline() ;
        ScreenWriteROMString(STAT_HP) ;
		ScreenWriteUnsignedInteger(playerOne.hp_cur) ;
		ScreenWriteCharacter('/') ;
		ScreenWriteUnsignedInteger(playerOne.hp_max) ;
		endline() ;
        ScreenWriteROMString(STAT_STR) ;
		ScreenWriteUnsignedInteger(playerOne.str) ;
		endline() ;
        ScreenWriteROMString(STAT_DEF) ;
		ScreenWriteUnsignedInteger(playerOne.def) ;
		endline() ;
        ScreenWriteROMString(STAT_PRC) ;
		ScreenWriteUnsignedInteger(playerOne.prc) ;
        endline() ;
        ScreenWriteROMString(STAT_VEL) ;
		ScreenWriteUnsignedInteger(playerOne.vel) ;
        endline() ;

		// Ask for confirmation
        ScreenWriteROMString(CREATE_VALID) ;
        valid = KeyboardReadCharacter() ;
    } while (valid != 'o' && valid != 'O') ;
	#endif
}

#endif