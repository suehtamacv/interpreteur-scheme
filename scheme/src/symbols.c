/**
 * @file symbols.c
 * @author Matheus Cavalcante <matheus.de-araujo-cavalcante@phelma.grenoble-inp.fr>
 * @date Thu Oct 06 23:48:07 2016
 * @brief List of symbols and environments.
 *
 * This file contains the functions related to the symbol handling, such as the
 * table of symbols, and functions to add, remove and look for a symbol.
 */

#include "include/symbols.h"
#include "include/print.h"
#include <stdio.h>

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

object* locate_symbol(string name, int starting_env_number) {
    if (starting_env_number < 0) {
        ERROR_MSG("Invalid environment requested: %d", starting_env_number);
        return NULL;
    }

    for (int curr_env = starting_env_number; curr_env != -1; curr_env--) {
        object* env = get_environment(curr_env);

        object* curr_obj = env;
        while (is_Nil(*curr_obj) == False) {
            if (strcasecmp(car(car(*curr_obj))->val.string, name) == 0) {
                return &((*curr_obj)->val.pair.car->val.pair.cdr);
            }
            curr_obj = &((*curr_obj)->val.pair.cdr);
        }
    }

    return NULL;
}

void define_symbol(string name, object obj, int env_number) {
    object *old_symbol = locate_symbol(name, env_number);

    /* Symbol already exists */
    if (old_symbol != NULL) {
        *old_symbol = obj;
    } else {

        /* Creates the object binding */
        object binding = make_pair(make_object(SFS_STRING), obj);
        strcpy(binding->val.pair.car->val.string, name);

        /* Looks for the last object */
        object* last_obj = get_environment(env_number);
        while (is_Nil(*last_obj) == False) {
            last_obj = &((*last_obj)->val.pair.cdr);
        }
        *last_obj = make_pair(binding, nil);
    }
}

void set_symbol(string name, object obj, int env_number) {
    object *old_symbol = locate_symbol(name, env_number);

    if (old_symbol == NULL) {
        WARNING_MSG("It is not possible to use \"set!\" in a symbol that does not exist");
    } else {
        *old_symbol = obj;
    }
}
