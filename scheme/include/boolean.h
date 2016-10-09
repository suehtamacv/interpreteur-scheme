/**
 * @file boolean.h
 * @author Matheus Cavalcante <matheus.de-araujo-cavalcante@phelma.grenoble-inp.fr>
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
 * @brief Bool est le type de variable utilise pour sauvegarder
 * une valeur boolean.
 */
typedef enum {
    False = FALSE,
    True = TRUE
} Bool;

#ifdef __cplusplus
}
#endif


#endif /* BOOLEAN_H */
