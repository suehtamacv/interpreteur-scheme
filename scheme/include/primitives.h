#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <object.h>

void create_basic_primitives(void);

/* Basic type testing function */
object prim_is_boolean(object);
object prim_is_null(object);
object prim_is_string(object);
object prim_is_integer(object);
object prim_is_pair(object);
object prim_is_symbol(object);
object prim_is_char(object);
object prim_is_real(object);
object prim_is_procedure(object);

object prim_is_greater_than(object);

/* Thpose are the basic list handling functions */
object prim_car(object);
object prim_cdr(object);
object prim_set_car(object);
object prim_set_cdr(object);


/* Those are basic arithmetic functions */
object prim_arith_plus(object);

#endif /* PRIMITIVES_H */
