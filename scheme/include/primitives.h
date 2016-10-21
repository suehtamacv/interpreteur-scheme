#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <object.h>

void create_basic_primitives(void);
void create_primitive(string, object (*)(object));

/* Basic type testing function */
object prim_is_boolean(object);


/* Those are basic arithmetic functions */
object prim_arith_plus(object);

#endif /* PRIMITIVES_H */
