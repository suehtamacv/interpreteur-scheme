
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
 */
void sfs_print(object);

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
/**
 * @brief sfs_print_primitive prints the definition of a primitive.
 */
void sfs_print_primitive(object);
/**
 * @brief sfs_print_form prints the definition of a form.
 */
void sfs_print_form(object);
/**
 * @brief sfs_print_pair prints a pair.
 */
void sfs_print_pair(object);
/**
 * @brief sfs_print_pair prints an environment.
 */
void sfs_print_environment(object);
/**
 * @brief sfs_print_compound prints a compound.
 */
void sfs_print_compound(object);

#ifdef __cplusplus
}
#endif

#endif /* _PRINT_H_ */
