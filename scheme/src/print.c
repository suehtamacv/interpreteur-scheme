
/**
 * @file print.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:12:17 2012
 * @brief Printing stuff for SFS.
 *
 * Printing stuff for SFS.
 */

#include "print.h"

#include <stdio.h>

void sfs_print_atom(object o) {
    switch (o->type) {
    case SFS_CHARACTER:
        printf("#\\%c ", o->val.character);
        break;

    case SFS_BOOLEAN:
        if (o->val.boolean == True) {
            printf("#t ");
        } else {
            printf("#f ");
        }
        break;
    }

    return;
}

void sfs_print_pair(object o) {
    return;
}

void sfs_print(object o) {

    if (o->type == SFS_PAIR) {
        sfs_print_pair(o);
    } else {
        sfs_print_atom(o);
    }
}
