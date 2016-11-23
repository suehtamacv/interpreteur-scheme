#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <object.h>

void create_basic_primitives(object);
void create_primitive(string, object (*)(object), object);

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
object prim_caar(object);
object prim_cadr(object);
object prim_cdar(object);
object prim_cddr(object);
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
object prim_string_to_list(object);

/* Those are ordering primitives */
object prim_larger(object);
object prim_smaller(object);
object prim_equal(object);

/* Those are basic arithmetic functions */
object prim_plus(object);
object prim_minus(object);
object prim_times(object);
object prim_division(object);
object prim_remainder(object);
object prim_quotient(object);
object prim_modulo(object);
object prim_abs(object);
object prim_exp(object);
object prim_sin(object);
object prim_cos(object);
object prim_tan(object);
object prim_sqrt(object);

/* Those are the primitives related to complex numbers */
object prim_make_rectangular(object);
object prim_make_polar(object);
object prim_real_part(object);
object prim_imag_part(object);
object prim_magnitude(object);
object prim_angle(object);

#endif /* PRIMITIVES_H */
