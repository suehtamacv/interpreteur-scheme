
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
        sfs_print_char(o);
        break;

    case SFS_BOOLEAN:
        sfs_print_bool(o);
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

void sfs_print_char(object o) {
    if (o->type != SFS_CHARACTER) {
        ERROR_MSG("Trying to print object of type %d as character (%d).", o->type, SFS_CHARACTER);
    }
    printf("#\\%c ", o->val.character);
}

void sfs_print_bool(object o) {
    if (o->type != SFS_BOOLEAN) {
        ERROR_MSG("Trying to print object of type %d as boolean (%d).", o->type, SFS_BOOLEAN);
    }

    if (o->val.boolean == True) {
        printf("#t ");
    } else if (o->val.boolean == False) {
        printf("#f ");
    } else {
        ERROR_MSG("Found weird boolean who is nor true neither false.");
    }
}
