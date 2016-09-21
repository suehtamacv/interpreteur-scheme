/**
 * @file read.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:30:11 2012
 * @brief Read routine for SFS.
 *
 * Read routine for SFS.
 */

#ifndef _READ_H_
#define _READ_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "object.h"

enum {S_OK, S_KO, S_END};

uint sfs_get_sexpr(string input, FILE *fp);

object sfs_read(char *input, uint *here);
object sfs_read_atom(char *input, uint *here);
object sfs_read_pair(char *input, uint *here);

/**
 * @brief sfs_read_bool reads a value of boolean type from the input pointer.
 * @param input is the pointer to the beginning of the character chain being read.
 * @param here is the pointer to the current position on the character chain.
 * @return a bool object.
 */
object sfs_read_bool(char *input, uint *here);
/**
 * @brief sfs_read_char reads a value of character type from the input pointer.
 * @param input is the pointer to the beginning of the character chain being read.
 * @param here is the pointer to the current position on the character chain.
 * @return a character object.
 */
object sfs_read_char(char *input, uint *here);
/**
 * @brief sfs_read_string reads a value of string type from the input pointer.
 * @param input is the pointer to the beginning of the character chain being read.
 * @param here is the pointer to the current position on the character chain.
 * @return a string object.
 */
object sfs_read_string(char *input, uint *here);
/**
 * @brief sfs_read_symbol reads a value of symbol type from the input pointer.
 * @param input is the pointer to the beginning of the character chain being read.
 * @param here is the pointer to the current position on the character chain.
 * @return a symbol object.
 */
object sfs_read_symbol(char *input, uint *here);

/**
 * @brief sfs_read_number reads a value of number type from the input pointer.
 *
 * This function shall consider negative and positive numbers, and the different
 * types of numbers (integers, reals, complex)
 * @param input is the pointer to the beginning of the character chain being read.
 * @param here is the pointer to the current position on the character chain.
 * @return a number object.
 */
object sfs_read_number(char *input, uint *here);
/**
 * @brief sfs_read_integer_number reads a value of integer type from the input pointer.
 *
 * If the number is too large to fit into INT_MAX, then the number is saved as +inf or -inf.
 * @param input is the pointer to the beginning of the character chain being read.
 * @param here is the pointer to the current position on the character chain.
 * @return a integer number (signed or unsigned) object.
 */
object sfs_read_integer_number(char *input, uint *here);
/**
 * @brief sfs_read_complex_number reads a value of complex type from the input pointer.
 *
 * The complex number should be written as [+|-]A{+|-}B{i|j}.
 * @param input is the pointer to the beginning of the character chain being read.
 * @param here is the pointer to the current position on the character chain.
 * @return a complex number object.
 */
object sfs_read_complex_number(char *input, uint *here);
/**
 * @brief sfs_read_real_number reads a value of real type from the input pointer.
 * @param input is the pointer to the beginning of the character chain being read.
 * @param here is the pointer to the current position on the character chain.
 * @return a real number object.
 */
object sfs_read_real_number(char *input, uint *here);

#ifdef __cplusplus
}
#endif

#endif /* _READ_H_ */
