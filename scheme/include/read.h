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


#ifdef __cplusplus
}
#endif

#endif /* _READ_H_ */
