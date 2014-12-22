/****************************************/
/*			+-----	  / \    .------	*/
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
/*	File name: 	characters.c			*/
/*	Author:		Denis CARLUS			*/
/*	Date:		23/06/2013				*/
/*	Version:	1.0						*/
/****************************************/


#ifndef __CHARACTERS_OBJ__
#define __CHARACTERS_OBJ__

#include "Strings.h"
#include "tools.h"

#define STRAWBERRY_PIC_UPGRADED
#define POINTS_DISTRIBUTION	4

extern unsigned char tmp ;

/**
 * @brief Structure that represents one player.
 */
typedef struct {
    /** @brief Level of the character [1 - 100]. */
    unsigned char level ;

    /** @brief Experience points earned for the current level [0 - 200]. */
    unsigned char exp ;

	/** @brief Maximal amount of experience, increasing while the character levels up. */
	unsigned char max_exp ;

    /** @brief Maximal amount of HP [1 - 255]. */
    unsigned char hp_max ;

    /** @brief Amount of HP [0 - hp_max]. */
    unsigned char hp_cur ;

    /** @brief Damage the character can make when fighting [1 - 100]. */
    unsigned char str ;

    /** @brief Reduces the damages received by the character [1 - 100]. */
    unsigned char def ;

    /** @brief Chance the character hit its target [1 - 100]. */
    unsigned char prc ;

    /** @brief Speed of the character to fight [1 - 100]. */
    unsigned char vel ;

	/**
     * @brief Velocity-Gauge Attack bar, that is filled during a fight to attack [0 - 255].
			  When the attack is done, it is totally empty and has to be filled again.
			  The velocity is used to fill the bar.
	 */
	unsigned char vga ;
} Player ;


/**
 * @brief Structure that represents one monster at level 1.
 * The values are then adjusted to the player level when needed.
 * @see current_monster
 */
typedef struct {
    /** @brief Maximal amount of HP [1 - 255]. */
    unsigned char hp_max ;

    /** @brief Damage the character can make when fighting [1 - 100]. */
    unsigned char str ;

    /** @brief Reduces the damages received by the character [1 - 100]. */
    unsigned char def ;

    /** @brief Chance the character hit its target [1 - 100]. */
    unsigned char prc ;

    /** @brief Speed of the character to fight [1 - 100]. */
    unsigned char vel ;
} Monster ;


/**
 * @brief Structure that represents a currently fighting monster.
 * The values can be modified to create a random monster and fight it.
 */
typedef struct {
	/** @brief Type of the enemy. Helps getting the name of the monster. */
	unsigned char type ;

	/** @brief Level of the enemy. */
	unsigned char level ;

    /** @brief Maximal amount of HP [1 - 255]. */
    unsigned char hp_max ;

    /** @brief Amount of HP [0 - hp_max]. */
    unsigned char hp_cur ;

    /** @brief Damage the character can make when fighting [1 - 100]. */
    unsigned char str ;

    /** @brief Reduces the damages received by the character [1 - 100]. */
    unsigned char def ;

    /** @brief Chance the character hit its target [1 - 100]. */
    unsigned char prc ;

    /** @brief Speed of the character to fight [1 - 100]. */
    unsigned char vel ;

	/**
     * @brief Velocity-Gauge Attack bar, that is filled during a fight to attack [0 - 255].
			  When the attack is done, it is totally empty and has to be filled again.
			  The velocity is used to fill the bar.
	 */
	unsigned char vga ;
} FightingMonster ;



/* *********** INITIALIZATION OF THE PLAYERS *********** */
/** @brief The character of the first player. */
Player playerOne = {1, 0, 7, 0, 0, 0, 0, 0, 0} ;


/* *********** INITIALIZATION OF THE ENEMIES *********** */
/** @brief The enemy to kill when fighting. */
FightingMonster enemy = {0, 0, 0, 0, 0, 0} ;


// /* ******* DEFINTIIONS OF THE BESTIARY AT LEVEL 1 ****** */
const Monster bestiary[10] = {
							  {6, 2, 2, 48, 13},		// A mice is a small animal easy to kill
							  {7, 2, 3, 55, 4},			// Cactus is a quite dangerous plant
							  {10, 3, 2, 43, 7},		// A frog is a weak animal
							  {12, 5, 2, 30, 6},		// A pig is not so dangerous since it can often miss its target...
							  {23, 13, 8, 52, 15},		// A goblin is an horrible small humanoid
							  {22, 10, 6, 83, 22},		// A mosquito is a really precise enemy
							  {30, 10, 15, 45, 5},		// A bull is a strong and slow animal
							  {47, 39, 21, 46, 9},		// Ogre is tall and strong. Yet it has a pretty poor eyesight and agility...
							  {60, 28, 12, 68, 4},		// Mandrake is a small, obnoxious and hard to kill plant...
							  {72, 35, 48, 74, 14}		// A Valkyrie is as dangerous as beautiful
							 } ;

/**
 * @brief Select an enemy and set it to adjust the difficulty to the player's level.
 */
unsigned char selectEnemy() {
	// First select the type of enemy that best suits the player's current statistics because it's fair!

	/*
	   The more the player is strong, the more the enemies are difficult to defeat.
	   Those values correspond to the index of the monsters in the bestiary array.
	   Look at the bestiary array in characters.h to understand the values.
     */
	tmp = 2 ;									// basis: mice and cactus
	if (playerOne.level > 5) {
		tmp += 2 ;								// + frog and pig
		if (playerOne.level > 10) {
			tmp += 2 ;							// + goblin and mosquito
			if (playerOne.level > 20) {
				tmp += 2 ;						// + bull and ogre
				if (playerOne.level > 35) {
					tmp += 2 ;					// + mandrake and valkyrie
				}
			}
		}
	}

	/* Compute the index of the randomly selected monster among the ones that are compatible
	   with the player's level */
	unsigned char index = 0 ;
	for (; tmp > 0 ; tmp--) {
		index += modulo2(RandomGenerateNumber()) ;
	}
	return index ;
}

/**
 * @brief Create an enemy from its type and its level.
 */
void createEnemy() {
	unsigned char typeIndex = selectEnemy() ;
	Monster* type = &bestiary[typeIndex] ;

	// Determine the level of the monster
	unsigned char level = modulo2(RandomGenerateNumber()) ;
	if (level == 0) {
		level = modulo(RandomGenerateNumber(), POWER2_4) ;
		level = playerOne.level + level ;
		if (level > 100)
			level = 100 ;
	}
	else if (level > 1) {
		level = modulo(RandomGenerateNumber(), POWER2_4) ;
		level = playerOne.level - level ;
	}

	// Set the default attributes
	enemy.level = level ;
	enemy.type = typeIndex ;
	enemy.hp_cur = enemy.hp_max = type -> hp_max ;
	enemy.str = type -> str ;
	enemy.def = type -> def ;
	enemy.prc = type -> prc ;
	enemy.vel = type -> vel ;
	enemy.vga = RandomGenerateNumber() ;

	unsigned char random ;
	unsigned char mid = level >> 1 ;
	// Set the attributes according to the required level
	for (; level > 0 ; level--) {
		// Increase the HP of the monster on the half of the levels amount
		if (level > mid) {
			enemy.hp_max++ ;
			enemy.hp_cur++ ;
		}
		else {
			// As their are four other statistics, select one of them
			random = modulo(RandomGenerateNumber(), POWER2_4) ;
			switch (random) {
				case 0:
					enemy.str++ ;
					break ;
				case 1:
					enemy.def++ ;
					break ;
				case 2:
					enemy.prc++ ;
					break ;
				case 3:
					enemy.vel++ ;
					break ;
			}
		}
	}
}

/**
 * @brief Get the name of the enemy from its type.
 * @return The address of the corresponding string in EEPROM.
 */
unsigned short getEnemyName() {
	switch(enemy.type) {
		case 0:	return MONSTER_MICE ;
		case 1:	return MONSTER_CACTUS ;
		case 2:	return MONSTER_FROG ;
		case 3:	return MONSTER_PIG ;
		case 4:	return MONSTER_GOBLIN ;
		case 5:	return MONSTER_MOSQUITO ;
		case 6:	return MONSTER_BULL ;
		case 7:	return MONSTER_OGRE ;
		case 8:	return MONSTER_MANDRAKE ;
		case 9:	return MONSTER_VALKYRIE ;
	}
}

#endif