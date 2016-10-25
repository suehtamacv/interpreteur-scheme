/**
 * @file lists.h
 * @author Matheus Cavalcante <matheus.de-araujo-cavalcante@phelma.grenoble-inp.fr>
 * @date Wed Oct 19 19:04:07 2016
 * @brief Functions to deal with lists.
 *
 * This file contains the functions related to the lists handling.
 */

#ifndef LISTS_H
#define LISTS_H

#include "object.h"

/**
 * @brief car returns the car of the object o, given that it is a pair.
 * @return the car of the pair.
 */
object car(object o);
/**
 * @brief cdr returns the cdr of the object o, given that it is a pair.
 * @return the cdr of the pair.
 */
object cdr(object o);
/**
 * @brief cddr returns the cdr of the cdr of the object o, given
 * that it is a pair.
 * @return the cdr of the cdr.
 */
object cddr(object o);
/**
 * @brief cadr returns the car of the cdr of the object o, i.e., the next
 * object in the list, given that it is a pair.
 * @return the car of the cdr of the pair.
 */
object cadr(object o);
/**
 * @brief caddr returns the car of the cdr of the cdr of the object o,
 * i.e., the second next object in the list, given that it is a pair.
 * @return the car of the cdr of the cdr of the pair.
 */
object caddr(object o);
/**
 * @brief cons creates a new list by prepending @a o at the beginning of @a list.
 * @param o is the object that will be prepended.
 * @param list is the ancient list.
 * @return the list (a list).
 */
object cons(object o, object list);
/* Function reverse list*/
object reverse(object o);

#endif /* LISTS_H */
