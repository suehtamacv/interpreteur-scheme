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

enum bool_t {
    False = 0, True = 1
};

typedef enum bool_t Bool;

#ifdef __cplusplus
}
#endif


#endif // BOOLEAN_H
