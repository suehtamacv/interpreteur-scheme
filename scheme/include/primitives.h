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



/* Those are basic arithmetic functions */
object prim_arith_plus(object);

#endif /* PRIMITIVES_H */
