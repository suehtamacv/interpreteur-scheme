
/**
 * @file print.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:12:52 2012
 * @brief Printing stuff for SFS.
 *
 * Printing stuff for SFS.
 */

#ifndef _PRINT_H_
#define _PRINT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"

/**
 * @brief sfs_print prints an object of any type.
 * @param isBeginList is a flag used to indicate whether a pair is at the beginning
 * of the list. If the object is not a pair, then this flag does nothing.
 */
void sfs_print(object, Bool isBeginList);
/**
 * @brief sfs_print_atom prints an atom.
 */
void sfs_print_atom(object);
/**
 * @brief sfs_print_pair prints a pair, i.e., prints its car and then prints its cdr
 * iff it's not equal to nil.
 * @param isBeginList is a flag used to indicate whether this pair is at the beginning
 * of the list. In order to set this flag as true, the parent pair must himself have his
 * car as a pair. Another case when this flag is true is when we're dealing with the first
 * pair of the entire list, the one that does not have a parent pair.
 */
void sfs_print_pair(object, Bool isBeginList);

/**
 * @brief sfs_print_char prints an object of character type.
 */
void sfs_print_char(object);
/**
 * @brief sfs_print_bool prints an object of boolean type.
 */
void sfs_print_bool(object);
/**
 * @brief sfs_print_number prints an object of number type.
 */
void sfs_print_number(object);
/**
 * @brief sfs_print_string prints an object of string type.
 */
void sfs_print_string(object);
/**
 * @brief sfs_print_symbol prints an object of symbol type.
 */
void sfs_print_symbol(object);
/**
 * @brief sfs_print_nil prints nil.
 */
void sfs_print_nil(object);

#ifdef __cplusplus
}
#endif

#endif /* _PRINT_H_ */
