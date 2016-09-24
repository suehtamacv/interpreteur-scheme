/**
 * @file boolean.h
 * @author Matheus Cavalcante <matheus.de-araujo-cavalcante@minatec.grenoble-inp.fr>
 * @date Dom Set 11 22:18:07 2016
 * @brief Boolean handling.
 *
 * Boolean handling.
 */

#ifndef BOOLEAN_H
#define BOOLEAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "basic.h"

/**
 * @brief The bool_t enum est utiliser pour decrire les valeurs Vrai et Faux qui n'existent
 * pas dans la langage C.
 */
enum bool_t {
    False = FALSE,
    True = TRUE
};

/**
 * @brief Bool est le type de variable utilise pour sauvegarder
 * une valeur boolean.
 */
typedef enum bool_t Bool;

#ifdef __cplusplus
}
#endif


#endif /* BOOLEAN_H */
