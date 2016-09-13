
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

    case SFS_NUMBER:
        sfs_print_number(o);
        break;

    case SFS_STRING:
        sfs_print_string(o);
        break;

    case SFS_SYMBOL:
        sfs_print_symbol(o);
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
    if (o->val.character == ' ') {
        printf("#\\space");
    } else if (o->val.character == '\n') {
        printf("#\\newline");
    } else {
        printf("#\\%c", o->val.character);
    }
}

void sfs_print_bool(object o) {
    if (o->type != SFS_BOOLEAN) {
        ERROR_MSG("Trying to print object of type %d as boolean (%d).", o->type, SFS_BOOLEAN);
    }

    if (o->val.boolean == True) {
        printf("#t");
    } else if (o->val.boolean == False) {
        printf("#f");
    } else {
        ERROR_MSG("Found weird boolean who is nor true neither false.");
    }
}

void sfs_print_number(object o) {
    if (o->type != SFS_NUMBER) {
        ERROR_MSG("Trying to print object of type %d as number (%d).", o->type, SFS_NUMBER);
    }

    switch (o->val.number.numtype) {
    case NUM_UINTEGER:
        printf("%d", o->val.number.val.integer);
        break;

    case NUM_INTEGER:
        if (o->val.number.val.integer > 0) {
            printf("+%d", o->val.number.val.integer);
        } else {
            printf("%d", o->val.number.val.integer);
        }
        break;

    case NUM_REAL:
        printf("%lg", o->val.number.val.real);
        break;
    }
}

void sfs_print_string(object o) {
    if (o->type != SFS_STRING) {
        ERROR_MSG("Trying to print object of type %d as string (%d).", o->type, SFS_STRING);
    }

    printf("\"%s\"", o->val.string);
}

void sfs_print_symbol(object o) {
    if (o->type != SFS_SYMBOL) {
        ERROR_MSG("Trying to print object of type %d as symbol (%d).", o->type, SFS_SYMBOL);
    }

    printf("%s", o->val.symbol);
}
