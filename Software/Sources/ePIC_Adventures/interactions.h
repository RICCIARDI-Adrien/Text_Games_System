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
/*	File name: 	map.h					*/
/*	Author:		Denis CARLUS			*/
/*	Date:		03/07/2013				*/
/*	Version:	1.0						*/
/****************************************/


#ifndef __MAP_H__
#define __MAP_H__

#include "../System/System.h"
#include "items.h"
#include "gui.h"
#include "tools.h"

#define STRAWBERRY_PIC_UPGRADED

extern unsigned char tmp ;


/** @brief Seed to generate a random map. */
unsigned char mapSeed ;

/** @brief Position of the player on X. */
unsigned char playerPosX ;

/** @brief Position of the player on Y. */
unsigned char playerPosY ;

/**
 * @brief 	Initialize the world map and player's location.
 *			The world map is a fictive and random array of 255 x 255
 *			available locations to the player. It is not saved in the
 *			code and so only requires three bytes to exist and be
 *			traveled. As it is random, it changes at each execution
 *			of the game and the player's location are random too.
 *			Thus, even if the seed is equal between two executions,
 *			there are little probabilities the player's the same
 *			experience from one session to another.
 *			Moreover, the way the potions locations are determined
 *			does not alter the game difficulty.
 *
 *			At each location, it is possible to find (or not) one or
 *			several items. Yet, all of them are not necessarily found
 *			and it is possible to find them later. Indeed, as there are
 *			"only" different 65,535 locations, it makes more and more
 *			possibilities by having random selector of items, so that the
 *			map can virtually be much higher than 65,535 possibilities!
 *			For example, if we have one chance on 4 to discover an item,
 *			1 chance on 4 to find one of the 4 types of available items,
 *			and 1 chance on 8 to find the lowest potion at a same location,
 *			it means we have one chance on 128 to finally find it. So, if
 *			we consider that probabilistic point of view, our map contains
 *			now 65,535 x 128 = 8,388,480 virtual locations for this kind
 *			of item. We can continue by considering the other items having
 *			lower probabilities to be found.
 *			All of this still available on only 3 bytes!
 *
 * @see		playerMoves()
 * @see 	detectItem()
 */
void initWorldMap() {
	mapSeed = RandomGenerateNumber() ;
	playerPosX = RandomGenerateNumber() ;
	playerPosY = RandomGenerateNumber() ;
}

/**
 * @brief 	Make the player move on the world map by pressing one of
 *			the direction keys.
 */
void playerActionsOnMap() {
	unsigned char action ;

	// Ask the direction to follow
	do {
		action = KeyboardReadCharacter() ;
	} while (action < '1' && action > '4') ;

	// Move the player
	switch(action) {
		case '1':
			playerPosY++ ;
			break ;

		case '2':
			playerPosY-- ;
			break ;

		case '3':
			playerPosX++ ;
			break ;

		case '4':
			playerPosX-- ;
			break ;
	}
}

/** @brief 	Detect if there is an item at the current player's location. */
void detectItem() {
	// Type of item found. By default, no item found.
	unsigned char itemAtLocation = 123 ;
	unsigned char find = modulo(RandomGenerateNumber(), POWER2_4) ;

	// Randomly find an item (according to the position and seed)
	if (find == 0) {
		find = modulo(RandomGenerateNumber(), POWER2_4) ;
		switch(find) {
			case 0:
				find = modulo(playerPosX + playerPosY + mapSeed, POWER2_4) ;
				if (find == 0)
					itemAtLocation = MIN_POTION ;
				break ;
			case 1:
				find = modulo(playerPosX + playerPosY + mapSeed, POWER2_8) ;
				if (find == 0)
					itemAtLocation = MINPLUS_POTION ;
				break ;
			case 2:
				find = modulo(playerPosX + playerPosY + mapSeed, POWER2_16) ;
				if (find == 0)
					itemAtLocation = MID_POTION ;
				break ;
			case 3:
				find = modulo(playerPosX + playerPosY + mapSeed, POWER2_32) ;
				if (find == 0)
					itemAtLocation = MAX_POTION ;
				break ;
		}
	}
	
#ifdef STRAWBERRY_PIC_UPGRADED
	// Get the item if wanted and if the inventory is not full!
	if (itemAtLocation != 123) {
		unsigned short type = itemName(itemAtLocation) ;
		writeAction(type, MISC_ITEM_FOUND) ;

		if (addPotion(itemAtLocation)) {
			if (KeyboardReadCharacter() == '0') {
				writeAction(type, MISC_ITEM_GOT) ;
			}
			else {
				writeAction(type, MISC_ITEM_NOT_GOT) ;
			}
		}
	}
#else
	addPotion(itemAtLocation) ;
#endif
}



/** @brief The player choose the action to perform. */
void chooseAction() {
Label_backToFight:
	do {
		tmp = KeyboardReadCharacter() ;
	#ifdef STRAWBERRY_PIC_UPGRADED
	} while (tmp < '1' && tmp > '3') ;
	#else
	} while (tmp < '1' && tmp > '2') ;
	#endif

	switch(tmp) {
		case '1': // Attack the enemy
			#ifdef STRAWBERRY_PIC_UPGRADED
			if (modulo(RandomGenerateNumber(), POWER2_128) < playerOne.prc) {
			#endif
				// Not missed ! Compute the damages made to the enemy
				if (playerOne.str < enemy.def) {
					tmp = 1 ;
				}
				else {
					tmp = playerOne.str - enemy.def ;
				}

				#ifdef STRAWBERRY_PIC_UPGRADED
				// Critical hit comes with a chance of 5% (13 ~= 5% of 255)
				if (RandomGenerateNumber() < 13) {
					// Critical hit = double damages
					tmp = tmp + tmp ;
				}
				#endif

				// Player is damaged
				if (enemy.hp_cur > tmp) {
					enemy.hp_cur -= tmp ;
				}
				else {
					enemy.hp_cur = 0 ;
				}

				// Display the message of the attack
				updateEnemyHP() ;
				writeAction(getEnemyName(), FIGHT_DAMAGED) ;
			#ifdef STRAWBERRY_PIC_UPGRADED
			}
			else {
				// Missed, just print a sentence to signal it...
				writePlayerAction(FIGHT_MISSED) ;
			}
			

			tmp = 0 ;
			#endif
			break ;

		case '2': // Go to inventory
			updateToInventoryView() ;
			do {
				tmp = KeyboardReadCharacter() ;
			} while (tmp < '0' && tmp > '4') ;
			updateToFightView() ;

			switch(tmp) {
				case '0':	// Cancel inventory
					goto Label_backToFight ;
					break ;
				case '1':	// Use minimal potion
					tmp = usePotion(MIN_POTION) ;
					break ;
				case '2':	// Use minimal+ potion
					tmp = usePotion(MINPLUS_POTION) ;
					break ;
				case '3':	// Use middle potion
					tmp = usePotion(MID_POTION) ;
					break ;
				case '4':	// Use maximal potion
					tmp = usePotion(MAX_POTION) ;
					break ;
			}

			if (tmp != 0) {
				// As an object has been used...
				updatePlayerHP() ;
				#ifdef STRAWBERRY_PIC_UPGRADED
				tmp = 0 ;
				#endif
			}
			break ;

		#ifdef STRAWBERRY_PIC_UPGRADED
		case '3': // Flee (50%)
			if (modulo(RandomGenerateNumber(), POWER2_2) == 0)
				tmp = 123 ;	// Magic code to say the player flees
			else
				tmp = 0 ;
		#endif
	}
}

/** @brief The computer selects the distribution of one point. */
#ifdef STRAWBERRY_PIC_UPGRADED
unsigned char randomDistribution() {
	unsigned char keys[5] ;
	tmp = 5 ;
	// Is HP a good option ?
	if (playerOne.hp_max < 255) keys[0] = '1' ;
	else tmp-- ;
	// Is STR a good option ?
	if (playerOne.str < 100) keys[1] = '2' ;
	else tmp-- ;
	// Is DEF a good option ?
	if (playerOne.def < 100) keys[2] = '3' ;
	else tmp-- ;
	// Is PRC a good option ?
	if (playerOne.prc < 100) keys[3] = '4' ;
	else tmp-- ;
	// Is VEL a good option ?
	if (playerOne.vel < 100) keys[4] = '5' ;
	else tmp-- ;

	if (tmp > 1) {
		return keys[RandomGenerateNumber() % tmp] ;
	}
	else if (tmp == 1)
		return keys[0] ;
	else
		return 0 ;
}
#endif

	#ifdef STRAWBERRY_PIC_UPGRADED
/** @brief The player selects the distribution of its "level up points". */
void distributeLevelUpPoints() {
	unsigned char points = POINTS_DISTRIBUTION ;

	while (points > 0) {
		do {
			tmp = KeyboardReadCharacter() ;
		} while(tmp < '1' && tmp > '6') ;

Label_pointDistribution:
		switch(tmp) {
			case '1':
				if (playerOne.hp_max < 255) {
					playerOne.hp_max++ ;
					updatePlayerHP() ;
					points-- ;
				}
				break ;

			case '2':
				if (playerOne.str < 100) {
					playerOne.str++ ;
					updatePlayerStr() ;
					points-- ;
				}
				break ;

			case '3':
				if (playerOne.def < 100) {
					playerOne.def++ ;
					updatePlayerDef() ;
					points-- ;
				}
				break ;

			case '4':
				if (playerOne.prc < 100) {
					playerOne.prc++ ;
					updatePlayerPrc() ;
					points-- ;
				}
				break ;

			case '5':
				if (playerOne.vel < 100) {
					playerOne.vel++ ;
					updatePlayerVel() ;
					points-- ;
				}
				break ;

			case '6':
				tmp = randomDistribution() ;
				if (tmp != 0) goto Label_pointDistribution ;
		}
	}
}
	#endif

#endif