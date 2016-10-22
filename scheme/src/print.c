/**
 * @file print.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:12:17 2012
 * @brief Printing stuff for SFS.
 *
 * Printing stuff for SFS.
 */

#include "print.h"
#include "forms.h"
#include "lists.h"
#include <stdio.h>

void sfs_print(object o) {
    /* Le premier paranthèse ouvrante des listes */
    if (is_Pair(o) == True) {
        printf("(");
    }

restart:

    if (is_Pair(o) == True) {

        sfs_print(car(o));
        o = cdr(o); /* On part au prochain element de la liste */

        /* Le cdr vaut nil, donc il s'agit de la fin de la liste */
        if (o == nil) {
            printf(")");
            return;
        } else {
            printf(" ");
            /* Il faut encore imprimer le prochain element */
            goto restart;
        }

    } else {
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
            break;

        case SFS_NIL:
            sfs_print_nil(o);
            break;

        case SFS_PRIMITIVE:
            sfs_print_primitive(o);
            break;

        case SFS_FORM:
            sfs_print_form(o);
            break;
        }
    }
}

void sfs_print_char(object o) {
    if (o->type != SFS_CHARACTER) {
        WARNING_MSG("Trying to print object of type %d as character (%d).", o->type,
                    SFS_CHARACTER);
        return sfs_print(o);
    }


    /* Certains characteres ont une representation speciale */
    if (o->val.character == ' ') {
        printf("#\\space");
    } else if (o->val.character == '\n') {
        printf("#\\newline");
    } else {
        printf("#\\%c", o->val.character);
    }
}

void sfs_print_nil(object o) {
    if (o->type != SFS_NIL) {
        WARNING_MSG("Trying to print object of type %d as nil (%d).", o->type, SFS_NIL);
        return sfs_print(o);
    }
    printf("()");
}

void sfs_print_bool(object o) {
    if (o->type != SFS_BOOLEAN) {
        WARNING_MSG("Trying to print object of type %d as boolean (%d).", o->type,
                    SFS_BOOLEAN);
        return sfs_print(o);
    }

    if (o->val.boolean == True) {
        printf("#t");
    } else if (o->val.boolean == False) {
        printf("#f");
    } else {
        ERROR_MSG("Found weird boolean who is neither true nor false.");
    }
}

void sfs_print_number(object o) {
    if (o->type != SFS_NUMBER) {
        WARNING_MSG("Trying to print object of type %d as number (%d).", o->type,
                    SFS_NUMBER);
        sfs_print(o);
    }

    switch (o->val.number.numtype) {
    case NUM_UINTEGER:
        printf("%d", o->val.number.val.integer);
        break;

    case NUM_INTEGER:
        printf("%d", o->val.number.val.integer);
        break;

    case NUM_REAL:
        printf("%lg", o->val.number.val.real);
        break;

    case NUM_COMPLEX:
        if (o->val.number.val.complex.imag >= 0) {
            printf("%lg+%lgj", o->val.number.val.complex.real,
                   o->val.number.val.complex.imag);
        } else {
            printf("%lg%lgj", o->val.number.val.complex.real,
                   o->val.number.val.complex.imag);
        }
        break;

    case NUM_PINFTY:
        printf("+inf");
        break;

    case NUM_MINFTY:
        printf("-inf");
        break;

    case NUM_UNDEF:
        printf("NaN");
        break;
    }
}

void sfs_print_string(object o) {
    if (o->type != SFS_STRING) {
        WARNING_MSG("Trying to print object of type %d as string (%d).", o->type,
                    SFS_STRING);
        sfs_print(o);
    }

    printf("\"%s\"", o->val.string);
}

void sfs_print_symbol(object o) {
    if (o->type != SFS_SYMBOL) {
        WARNING_MSG("Trying to print object of type %d as symbol (%d).", o->type,
                    SFS_SYMBOL);
        sfs_print(o);
    }

    printf("%s", o->val.symbol);
}

void sfs_print_primitive(object o) {
    if (o->type != SFS_PRIMITIVE) {
        WARNING_MSG("Trying to print object of type %d as primitive (%d).", o->type,
                    SFS_PRIMITIVE);
        sfs_print(o);
    }

    printf("<#procedure %s>", o->val.primitive.func_name);
}

void sfs_print_form(object o) {
    if (o->type != SFS_FORM) {
        WARNING_MSG("Trying to print object of type %d as form (%d).", o->type,
                    SFS_FORM);
        sfs_print(o);
    }

    printf("<#procedure %s>", o->val.form.func_name);
}
