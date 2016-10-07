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
#include <stdio.h>

void create_environment() {
    object last_env = symbol_table;
    while (cdr(last_env) != nil) {
        last_env = cdr(last_env);
    }

    object new_env = make_object(SFS_PAIR);
    new_env->val.pair.car = nil;
    new_env->val.pair.cdr = nil;

    last_env->val.pair.cdr = new_env;
}

object get_environment(int env_number) {
    if (env_number < 0) {
        ERROR_MSG("Invalid environment requested: %d", env_number);
        return NULL;
    }

    int curr_env = -1;
    object env = symbol_table;
    while (curr_env != env_number) {
        if (cdr(env) == nil) {
            ERROR_MSG("Request for an environment that does not exist: %d", env_number);
            return NULL;
        }
        env = cdr(env);
        curr_env++;
    }

    return car(env);
}

object locate_symbol(string name, int starting_env_number) {
    if (starting_env_number < 0) {
        ERROR_MSG("Invalid environment requested: %d", starting_env_number);
        return NULL;
    }

    for (int curr_env = starting_env_number; curr_env != -1; curr_env--) {
        object env = get_environment(curr_env);

        object curr_obj = env;
        while (curr_obj != nil) {
            if (strcasecmp(car(car(curr_obj))->val.string, name) == 0) {
                return car(cdr(curr_obj)); //TODO: CAR CDR?? CDR CAR??
            }
            curr_obj = cdr(curr_obj);
        }
    }

    return NULL;
}

void define_symbol(string name, object obj, int env_number) {
    object old_symbol = locate_symbol(name, env_number);

    /* Symbol already exists */
    if (old_symbol != NULL) {
        old_symbol->val.pair.cdr = obj;
    } else {

        /* Creates the object binding */
        object binding = make_pair(make_object(SFS_STRING), obj);
        strcpy(binding->val.pair.car->val.string, name);

        /* Looks for the last object */
        object last_obj = get_environment(env_number);
        while (last_obj != nil) {
            last_obj = cdr(last_obj);
        }
        last_obj = make_pair(binding, nil);
    }
}

void set_symbol(string name, object obj, int env_number) {
    object old_symbol = locate_symbol(name, env_number);

    if (old_symbol == NULL) {
        WARNING_MSG("It is not possible to use \"set!\" in a symbol that does not exist");
    } else {
        old_symbol->val.pair.cdr = obj;
    }
}
