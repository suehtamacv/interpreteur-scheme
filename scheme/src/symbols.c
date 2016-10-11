/**
 * @file symbols.c
 * @author Matheus Cavalcante <matheus.de-araujo-cavalcante@phelma.grenoble-inp.fr>
 * @date Thu Oct 06 23:48:07 2016
 * @brief List of symbols and environments.
 *
 * This file contains the functions related to the symbol handling, such as the
 * table of symbols, and functions to add, remove and look for a symbol.
 */

#include <stdio.h>
#include <strings.h>
#include "symbols.h"
#include "print.h"
#include "forms.h"

void create_environment() {
    object* last_env = &symbol_table;
    while (cdr(*last_env) != nil) {
        last_env = &((*last_env)->val.pair.cdr);
    }

    (*last_env)->val.pair.cdr = make_pair(nil, nil);
}

object *get_environment(int env_number) {
    if (env_number < 0) {
        ERROR_MSG("Invalid environment requested: %d", env_number);
        return NULL;
    }

    int curr_env = -1;
    object* env = &symbol_table;
    while (curr_env != env_number) {
        if (is_Nil(cdr(*env)) == True) {
            ERROR_MSG("Request for an environment that does not exist: %d", env_number);
            return NULL;
        }
        env = &((*env)->val.pair.cdr);
        curr_env++;
    }

    return &((*env)->val.pair.car);
}

object* locate_symbol(object symbol_name, int starting_env_number) {
    if (starting_env_number < 0) {
        ERROR_MSG("Invalid environment requested: %d", starting_env_number);
        return NULL;
    }
    if (is_Symbol(symbol_name) == False) {
        ERROR_MSG("An object can not be bound to another who is not a symbol");
    }

    int curr_env = starting_env_number;
    for (; curr_env != -1; curr_env--) {
        object* env = get_environment(curr_env);

        object* curr_obj = env;
        while (is_Nil(*curr_obj) == False) {
            if (strcasecmp(car(car(*curr_obj))->val.string, symbol_name->val.symbol) == 0) {
                return &((*curr_obj)->val.pair.car->val.pair.cdr);
            }
            curr_obj = &((*curr_obj)->val.pair.cdr);
        }
    }

    return NULL;
}

void define_symbol(object symbol_name, object obj, int env_number) {
    object *old_symbol = locate_symbol(symbol_name, env_number);

    /* Symbol already exists */
    if (old_symbol != NULL) {
        *old_symbol = obj;
    } else {

        /* Creates the object binding */
        object binding = make_pair(make_object(SFS_SYMBOL), obj);
        strcpy(binding->val.pair.car->val.symbol, symbol_name->val.symbol);

        /* Looks for the last object */
        object* last_obj = get_environment(env_number);
        while (is_Nil(*last_obj) == False) {
            last_obj = &((*last_obj)->val.pair.cdr);
        }
        *last_obj = make_pair(binding, nil);
    }
}

void set_symbol(object symbol_name, object obj, int env_number) {
    object *old_symbol = locate_symbol(symbol_name, env_number);

    if (old_symbol == NULL) {
        WARNING_MSG("It is not possible to use \"set!\" in a symbol that does not exist");
    } else {
        *old_symbol = obj;
    }
}
