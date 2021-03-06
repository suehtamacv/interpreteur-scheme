/**
 * @file symbols.h
 * @author Matheus Cavalcante <matheus.de-araujo-cavalcante@phelma.grenoble-inp.fr>
 * @date Thu Oct 06 23:48:07 2016
 * @brief List of symbols and environments.
 *
 * This file contains the functions related to the symbol handling, such as the
 * table of symbols, and functions to add, remove and look for a symbol.
 */

#ifndef SYMBOLS_H
#define SYMBOLS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "basic.h"
#include "object.h"

object create_env_layer(object environment);

int define_symbol(object name, object val, object *environment);
int set_symbol(object name, object val, object environment);
object *locate_symbol(object name, object environment);
object *locate_symbol_in_env(object name, object env_layer);

#ifdef __cplusplus
}
#endif


#endif /* SYMBOLS_H */
