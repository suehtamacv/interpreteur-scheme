#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <object.h>

void create_basic_primitives(void);
void create_primitive(string, object (*)(object));

/* Those are the basic type testing functions */
object prim_is_boolean(object);
object prim_is_null(object);
object prim_is_string(object);
object prim_is_pair(object);
object prim_is_symbol(object);
object prim_is_char(object);
object prim_is_procedure(object);
object prim_is_list(object);
object prim_is_number(object);

/* Those are number handling functions */
object prim_is_integer(object);
object prim_is_real(object);
object prim_is_complex(object);
object prim_is_positive(object);
object prim_is_negative(object);
object prim_is_zero(object);
object prim_is_eq(object);
object prim_is_equal(object);

/* Those are the basic list handling functions */
object prim_car(object);
object prim_cdr(object);
object prim_set_car(object);
object prim_set_cdr(object);
object prim_cons(object);
object prim_list(object);

/* Those are the conversation primitives */
object prim_integer_to_char(object);
object prim_char_to_integer(object);
object prim_number_to_string(object);
object prim_string_to_number(object);
object prim_symbol_to_string(object);
object prim_string_to_symbol(object);

/* Those are ordering primitives */
object prim_larger(object);
object prim_smaller(object);
object prim_equal(object);


/* Those are basic arithmetic functions */
object prim_arith_plus(object);
object prim_arith_minus(object);
object prim_arith_times(object);
object prim_arith_division(object);


#endif /* PRIMITIVES_H */
