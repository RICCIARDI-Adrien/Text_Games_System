/** @file Strings.h
 * Contain all strings used by the program in all supported languages.
 * @author Adrien RICCIARDI
 * @version 1.0 : 20/10/2013
 */
#ifndef H_STRINGS_H
#define H_STRINGS_H

// English
#ifdef STRINGS_LANGUAGE_ENGLISH

// French (default language)
#else
	#define STRING_TITLE "--- Calculatrice ---\r\n\r\n"
	#define STRING_DIVISION_BY_ZERO "Erreur : il est impossible de diviser par z\212ro.\r\n\r\n"
	#define STRING_DIVISION_QUOTIENT "Quotient "
	#define STRING_DIVISION_REMAINDER "Reste "
	#define STRING_MULTIPLICATION_OVERFLOW "Erreur : d\212passement de capacit\212 lors de la multiplication.\r\n\r\n"
#endif

#endif