/** @file Strings.h
 * Contain all strings used by the program in all supported languages.
 * @author Adrien RICCIARDI
 * @version 1.0 : 31/08/2013
 */
#ifndef H_STRINGS_H
#define H_STRINGS_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
// English
#ifdef STRINGS_LANGUAGE_ENGLISH

// French (default language)
#else
	// Main menu entries
	#define STRING_MENU_ENTRY_LIST_RECORDS "1. Lister les enregistrements"
	#define STRING_MENU_ENTRY_DISPLAY_RECORD_CONTENT "2. Consulter un enregistrement"
	#define STRING_MENU_ENTRY_NEW_RECORD "3. Cr" SCREEN_LETTER_SMALL_E_ACUTE "er un nouvel enregistrement"
	#define STRING_MENU_ENTRY_RENAME_RECORD "4. Renommer un enregistrement"
	#define STRING_MENU_ENTRY_DELETE_RECORD "5. Supprimer un enregistrement"
	#define STRING_MENU_ENTRY_DELETE_ALL_RECORDS "6. Effacer tous les enregistrements"
	
	// List record names menu strings
	#define STRING_MENU_LIST_RECORDS_TITLE "Liste des enregistrements"
	
	// Display record content menu strings
	#define STRING_MENU_DISPLAY_RECORD_CONTENT_TITLE "Consultation d'un enregistrement"
	#define STRING_MENU_DISPLAY_RECORD_CONTENT_ASK_USER_FOR_RECORD_NUMBER "Num" SCREEN_LETTER_SMALL_E_ACUTE "ro de l'enregistrement " SCREEN_LETTER_SMALL_A_GRAVE " consulter : "
	#define STRING_MENU_DISPLAY_RECORD_CONTENT_RECORD_NAME_1 "\r\nContenu de l'enregistrement '"
	#define STRING_MENU_DISPLAY_RECORD_CONTENT_RECORD_NAME_2 "' :\r\n"
	
	// New record menu strings
	#define STRING_MENU_NEW_RECORD_TITLE "Cr" SCREEN_LETTER_SMALL_E_ACUTE "ation d'un enregistrement"
	#define STRING_MENU_NEW_RECORD_ASK_USER_FOR_RECORD_NUMBER "Num" SCREEN_LETTER_SMALL_E_ACUTE "ro de l'enregistrement " SCREEN_LETTER_SMALL_A_GRAVE " cr" SCREEN_LETTER_SMALL_E_ACUTE "er : "
	#define STRING_MENU_NEW_RECORD_ASK_OVERWRITE_RECORD "Cet enregistrement n'est pas vide. Voulez-vous l'" SCREEN_LETTER_SMALL_E_ACUTE "craser (O/N) ? "
	#define STRING_MENU_NEW_RECORD_ASK_RECORD_NAME "\r\nEntrez le nom de l'enregistrement : "
	#define STRING_MENU_NEW_RECORD_ASK_RECORD_CONTENT "Entrez le contenu de l'enregistrement :\r\n"
	#define STRING_MENU_NEW_RECORD_SUCCESS "Le nouvel enregistrement a " SCREEN_LETTER_SMALL_E_ACUTE "t" SCREEN_LETTER_SMALL_E_ACUTE " sauvegard" SCREEN_LETTER_SMALL_E_ACUTE " avec succ" SCREEN_LETTER_SMALL_E_GRAVE "s."
	
	// Rename record menu strings
	#define STRING_MENU_RENAME_RECORD_TITLE "Renommer un enregistrement"
	#define STRING_MENU_RENAME_RECORD_ASK_USER_FOR_RECORD_NUMBER "Num" SCREEN_LETTER_SMALL_E_ACUTE "ro de l'enregistrement " SCREEN_LETTER_SMALL_A_GRAVE " renommer : "
	#define STRING_MENU_RENAME_RECORD_SHOW_PREVIOUS_NAME "\r\nLe nom actuel de l'enregistrement est : "
	#define STRING_MENU_RENAME_RECORD_ASK_NEW_RECORD_NAME "Entrez le nouveau nom de l'enregistrement : "
	#define STRING_MENU_RENAME_RECORD_SUCCESS "L'enregistrement a " SCREEN_LETTER_SMALL_E_ACUTE "t" SCREEN_LETTER_SMALL_E_ACUTE " renomm" SCREEN_LETTER_SMALL_E_ACUTE " avec succ" SCREEN_LETTER_SMALL_E_GRAVE "s."
	
	// Delete record menu strings
	#define STRING_MENU_DELETE_RECORD_TITLE "Supprimer un enregistrement"
	#define STRING_MENU_DELETE_RECORD_ASK_USER_FOR_RECORD_NUMBER "Num" SCREEN_LETTER_SMALL_E_ACUTE "ro de l'enregistrement " SCREEN_LETTER_SMALL_A_GRAVE " supprimer : "
	#define STRING_MENU_DELETE_RECORD_SHOW_RECORD_NAME_1 "\r\nLe nom de l'enregistrement est '"
	#define STRING_MENU_DELETE_RECORD_SHOW_RECORD_NAME_2 "'.\r\n"
	#define STRING_MENU_DELETE_RECORD_ASK_CONFIRMATION "Voulez-vous vraiment supprimer cet enregistrement (O/N) ? "
	#define STRING_MENU_DELETE_RECORD_SUCCESS "L'enregistrement a " SCREEN_LETTER_SMALL_E_ACUTE "t" SCREEN_LETTER_SMALL_E_ACUTE " supprim" SCREEN_LETTER_SMALL_E_ACUTE " avec succ" SCREEN_LETTER_SMALL_E_GRAVE "s."
	
	// Delete all records menu strings
	#define STRING_MENU_DELETE_ALL_RECORDS_TITLE "Supprimer tous les enregistrements"
	#define STRING_MENU_DELETE_ALL_RECORDS_ASK_CONFIRMATION "Voulez-vous vraiment supprimer tout le contenu de la m" SCREEN_LETTER_SMALL_E_ACUTE "moire (O/N) ? "
	#define STRING_MENU_DELETE_ALL_RECORDS_SUCCESS "Tous les enregistrements ont " SCREEN_LETTER_SMALL_E_ACUTE "t" SCREEN_LETTER_SMALL_E_ACUTE " supprim" SCREEN_LETTER_SMALL_E_ACUTE "s avec succ" SCREEN_LETTER_SMALL_E_GRAVE "s."
	
	// Strings shared by several parts of the program
	#define STRING_WAIT_FOR_ESCAPE_KEY "Appuyez sur Echap pour continuer."
	#define STRING_ERROR_BAD_RECORD_NUMBER "Le num" SCREEN_LETTER_SMALL_E_ACUTE "ro d'enregistrement n'est pas valide."
	#define STRING_ERROR_EMPTY_RECORD "Cet enregistrement est vide."
	#define STRING_CONFIRMATION_LETTER_YES 'O'
	#define STRING_CONFIRMATION_LETTER_NO 'N'
#endif

#endif