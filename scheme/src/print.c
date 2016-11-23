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

    case SFS_PAIR:
        sfs_print_pair(o);
        break;

    case SFS_ENV:
        sfs_print_environment(o, 0);
        break;

    case SFS_COMPOUND:
        sfs_print_compound(o);
        break;
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

void sfs_print_pair(object o) {
    if (o->type != SFS_PAIR) {
        WARNING_MSG("Trying to print object of type %d as character (%d).", o->type,
                    SFS_PAIR);
        return sfs_print(o);
    }

    /* Le paranthèse ouvrante des listes et paires */
    printf("(");

restart:
    sfs_print(car(o));
    o = cdr(o); /* On part au prochain element de la liste */

    if (is_Pair(o) == True || is_Nil(o) == True) {
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
        printf(" . ");
        sfs_print(o);
        printf(")");
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

    switch (o->val.number->numtype) {
    case NUM_UINTEGER:
        printf("%d", o->val.number->val.integer);
        break;

    case NUM_INTEGER:
        printf("%d", o->val.number->val.integer);
        break;

    case NUM_REAL:
        printf("%Lg", o->val.number->val.real);
        break;

    case NUM_COMPLEX:
        sfs_print_number(real_part(o->val.number));
        if ((imag_part(o->val.number) == NaN ||
                is_Negative(imag_part(o->val.number)) == False) &&
                (imag_part(o->val.number)->val.number->numtype != NUM_PINFTY)) {
            printf("+");
        }
        sfs_print_number(imag_part(o->val.number));
        printf("j");
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

    printf("#<procedure %s>", o->val.primitive.func_name);
}

void sfs_print_form(object o) {
    if (o->type != SFS_FORM) {
        WARNING_MSG("Trying to print object of type %d as form (%d).", o->type,
                    SFS_FORM);
        sfs_print(o);
    }

    printf("#<procedure %s>", o->val.form.func_name);
}

void sfs_print_environment(object o, int depth) {
    if (o->type != SFS_ENV) {
        WARNING_MSG("Trying to print object of type %d as environment (%d).", o->type,
                    SFS_ENV);
        sfs_print(o);
    }

    int curr_depth = depth;
    while (curr_depth > 0) {
        printf("  ");
        curr_depth--;
    }
    printf("#<environment>\n");
    object obj = car(o);

restart:
    if (is_Nil(obj) == True) {
        if (is_Nil(cdr(o)) == False) {
            o = cdr(o);
            obj = car(o);
            sfs_print_environment(o, ++depth);
        }
        return;
    }
    curr_depth = depth;
    while (curr_depth > -2) {
        printf("  ");
        curr_depth--;
    }

    sfs_print(car(car(obj)));
    printf(" => ");
    if (is_Environment(cdr(car(obj))) == False) {
        sfs_print(cdr(car(obj)));
    } else {
        printf("#<environment>");
    }
    printf("\n");
    obj = cdr(obj);
    goto restart;
}

void sfs_print_compound(object o) {
    if (o->type != SFS_COMPOUND) {
        WARNING_MSG("Trying to print object of type %d as compound (%d).", o->type,
                    SFS_COMPOUND);
        sfs_print(o);
    }
    printf("#<procedure>");
}
