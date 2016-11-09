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
#include "forms.h"
#include "lists.h"

object create_env_layer(object environment) {
    if (is_Environment(environment) == False) {
        WARNING_MSG("Can't create a subenvironment into something who is not an environment");
        return NULL;
    }
    return make_pair(nil, environment);
}

object* locate_symbol(object name, object environment) {
    if (!environment) {
        ERROR_MSG("Invalid environment requested");
        return NULL;
    }
    if (is_Environment(environment) == False) {
        WARNING_MSG("Invalid environment");
        return NULL;
    }

    object *env_layer = &((*environment).val.pair.car);
    do {
        object* obj = locate_symbol_in_env(name, *env_layer);
        if (obj != NULL) {
            return obj;
        }
        environment = cdr(environment);
        env_layer = &((*environment).val.pair.car);
        if (*env_layer == nil) {
            break;
        }
    } while(1);

    return NULL;
}

object* locate_symbol_in_env(object name, object env_layer) {
    if (is_Symbol(name) == False) {
        ERROR_MSG("An object can not be bound to another who is not a symbol");
    }

    object* curr_obj = &env_layer;
    while (is_Nil(*curr_obj) == False) {
        if (strcasecmp(car(car(*curr_obj))->val.string, name->val.symbol) == 0) {
            return &((*curr_obj)->val.pair.car->val.pair.cdr);
        }
        curr_obj = &((*curr_obj)->val.pair.cdr);
    }

    return NULL;
}

int define_symbol(object name, object val, object *environment) {
    if (is_Environment(*environment) == False) {
        WARNING_MSG("Can't define a symbol into something who is not an environment");
    }

    DEBUG_MSG("Defining object '%s'", name->val.string);
    object env_layer = (*environment)->val.pair.car;
    object *old_symbol = locate_symbol_in_env(name, env_layer);

    if (!val) {
        WARNING_MSG("Must specify a value for the symbol. Symbol not defined");
        return -1;
    }

    /* Symbol already exists */
    if (old_symbol != NULL) {
        DEBUG_MSG("Symbol %s already exists", name->val.string);
        *old_symbol = val;
    } else {
        /* Creates the object binding */
        object binding = make_pair(make_object(SFS_SYMBOL), val);
        strcpy(binding->val.pair.car->val.symbol, name->val.symbol);

        /* Looks for the last object */
        (*environment)->val.pair.car = cons(binding, env_layer);
    }
    return 0;
}

int set_symbol(object name, object val, object environment) {
    object *old_symbol = locate_symbol(name, environment);

    if (old_symbol == NULL) {
        WARNING_MSG("It is not possible to use \"set!\" in a symbol that does not exist");
        return -1; /* Error */
    } else if (!val) {
        WARNING_MSG("Must specify a value for the symbol. Symbol not set");
        return -1;
    } else {
        *old_symbol = val;
    }
    return 0;
}
