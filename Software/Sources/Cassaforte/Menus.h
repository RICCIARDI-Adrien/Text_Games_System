/** @file Menus.h
 * Interface for all provided menus.
 * @author Adrien RICCIARDI
 * @version 1.0 : 01/08/2013
 */
#ifndef H_MENUS_H
#define H_MENUS_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Display the main menu from which all other sub-menus are accessible and wait for the user to select a sub-menu.
 * @return The index (starting from 1) of the sub-menu selected by the user.
 */
unsigned char MenuMain(void);

/** Display the name of all records. */
void MenuListRecords(void);

/** Display the content of a record. */
void MenuDisplayRecordContent(void);

/** Set the content of a record and its name. */
void MenuNewRecord(void);

/** Rename an existing record. */
void MenuRenameRecord(void);

/** Delete an existing record. */
void MenuDeleteRecord(void);

/** Delete all the existing records. */
void MenuDeleteAllRecords(void);

#endif