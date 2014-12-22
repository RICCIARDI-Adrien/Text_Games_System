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
/*	File name: 	items.c					*/
/*	Author:		Denis CARLUS			*/
/*	Date:		23/06/2013				*/
/*	Version:	1.0						*/
/****************************************/


#ifndef __ITEMS_OBJ__
#define __ITEMS_OBJ__

#include "Strings.h"

#define STRAWBERRY_PIC_UPGRADED

#define MAX_POTION_AMOUNT	3
#define MIN_POTION_LIFE		10
#define MINPLUS_POTION_LIFE 30
#define MID_POTION_LIFE		100
#define MAX_POTION_LIFE		255

#define MIN_POTION			0
#define MINPLUS_POTION 		2
#define MID_POTION			4
#define MAX_POTION			6

unsigned char maxPotionPower = 0 ;

/**
 * @brief Inventory of the player.
 * The byte represents the quantities of the four potions as follow:
 * <b>Bits: AABBCCDD</b>
 * <ul>
 * 	<li>AA for max potion</li>
 * 	<li>BB for mid potion</li>
 * 	<li>CC for min-plus potion</li>
 * 	<li>DD for min potion</li>
 * </ul>
 *
 * The player can only have 4 items of each type at a same time.
 * Use the following functions to easily handle the inventory.
 * At the beginning, the player has 2 min potions and 1 minplus potion.
 */
unsigned char Inventory = 0b00000110 ;

/**
 * @brief Add a potion of the given type.
 * @param type	Type of the potion. Use one of the following macros:
 *					- MIN_POTION
 *					- MINPLUS_POTION
 *					- MID_POTION
 *					- MAX_POTION
 *				Otherwise, it will make bullshit...
 * @return true if the potion has been added, false if the inventory is full.
 * @return TRUE if got, FALSE if not got (too much items).
 */
bool addPotion(unsigned char type) {
	unsigned char mask = 0b00000011 << type ;
	unsigned char wanted = Inventory & (mask) ;
	unsigned char others = Inventory & (~mask) ;

	// Count the quantities of the wanted potion and add one if allowed
	if ((wanted >> type) < MAX_POTION_AMOUNT) {
		wanted++ ;
		Inventory = others | wanted ;
		return true ;
	}
	return false ;
}

/**
 * @brief Remove a potion of the given type.
 * @param type	Type of the potion. Use one of the following macros:
 *					- MIN_POTION
 *					- MINPLUS_POTION
 *					- MID_POTION
 *					- MAX_POTION
 *				Otherwise, it will make bullshit...
 * @return TRUE if used, FALSE if not used (no more item).
 */
bool usePotion(unsigned char type) {
	unsigned char mask = 0b00000011 << type ;
	unsigned char wanted = Inventory & (mask) ;
	unsigned char others = Inventory & (~mask) ;

	// Count the quantities of the wanted potion and add one if allowed
	if ((wanted >> type) > 1) {
		wanted-- ;
		Inventory = others | wanted ;
		return true ;
	}
	return false ;
}

/**
 * @brief Count how many the player has potion of the given type.
 * @param type	Type of the potion. Use one of the following macros:
 *					- MIN_POTION
 *					- MINPLUS_POTION
 *					- MID_POTION
 *					- MAX_POTION
 *				Otherwise, it will make bullshit...
 * @return The amount of potion of the given type.
 */
unsigned char countPotion(unsigned char type) {
	unsigned char mask = 0b00000011 << type ;
	unsigned char wanted = Inventory & (mask) ;
	return (wanted >> type) ;
}

	#ifdef STRAWBERRY_PIC_UPGRADED
/**
 * @brief Get the name of the item by knowing its type.
 * @return The address of the string of the item name.
 */
unsigned short itemName(unsigned char type) {
	switch(type) {
		case MIN_POTION:
			return ITEM_HEAL_MIN ;
		case MINPLUS_POTION:
			return ITEM_HEAL_MINPLUS ;
		case MID_POTION:
			return ITEM_HEAL_MID ;
		case MAX_POTION:
			return ITEM_HEAL_MAXI ;
							default: return ERROR;
				
	}
}
	#endif

#endif