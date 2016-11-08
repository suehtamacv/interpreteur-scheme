#include <primitives.h>
#include <forms.h>
#include <limits.h>
#include <symbols.h>
#include <lists.h>
#include <print.h>
#include <math.h>
#include <mem.h>

#define TEST_NUMB_ARGUMENT_EQ(n_Arg, nomFunction) \
    if (list_length(o) != n_Arg) { \
        WARNING_MSG("Wrong number of arguments on \"" nomFunction "\""); \
        return NULL;\
    }

#define TEST_NEXT_IS_NUMBER(obj, nomFunction)\
    if(is_Number(cadr(o)) == False ){\
        WARNING_MSG("Wrong type of arguments on \"" nomFunction "\""); \
        return NULL;\
    }

void create_basic_primitives() {
    /* Those are the basic type comparison functions */
    create_primitive("boolean?", prim_is_boolean);
    create_primitive("null?", prim_is_null);
    create_primitive("string?", prim_is_string);
    create_primitive("pair?", prim_is_pair);
    create_primitive("symbol?", prim_is_symbol);
    create_primitive("char?", prim_is_char);
    create_primitive("number?", prim_is_number);
    create_primitive("list?", prim_is_list);
    create_primitive("procedure?", prim_is_procedure);

    /* Those are the number related comparison functions */
    create_primitive("real?", prim_is_real);
    create_primitive("complex?", prim_is_complex);
    create_primitive("integer?", prim_is_integer);
    create_primitive("positive?", prim_is_positive);
    create_primitive("negative?", prim_is_negative);
    create_primitive("zero?", prim_is_zero);

    /* Those are the basic list handling functions */
    create_primitive("car", prim_car);
    create_primitive("cdr", prim_cdr);
    create_primitive("set-car!", prim_set_car);
    create_primitive("set-cdr!", prim_set_cdr);
    create_primitive("cons", prim_cons);
    create_primitive("list", prim_list);

    /* Those are ordering primitives */
    create_primitive(">", prim_larger);
    create_primitive("<", prim_smaller);
    create_primitive("=", prim_equal);

    /* Those are the basic arithmetic primitives */
    create_primitive("+", prim_arith_plus);
    create_primitive("-", prim_arith_minus);
    create_primitive("*", prim_arith_times);
    create_primitive("/", prim_arith_division);
    create_primitive("remainder", prim_arith_remainder);
    create_primitive("quotient", prim_arith_quotient);
}

void create_primitive(string prim_name, object (*func)(object)) {
    define_symbol(make_symbol(prim_name), make_primitive(func, prim_name), 0);
}

object prim_arith_quotient(object o) {
    TEST_NUMB_ARGUMENT_EQ(2, "quotient");

    if (is_Integer(car(o)) == False || is_Integer(cadr(o)) == False) {
        WARNING_MSG("\"quotient\" can only be applied to numbers");
        return NULL;
    } else if (is_Zero(cadr(o)) == True) {
        WARNING_MSG("Can't divide by zero");
        return NULL;
    }

    object num = to_integer(car(o));
    object den = to_integer(cadr(o));
    return make_integer(num->val.number->val.integer /
                        den->val.number->val.integer);
}

object prim_arith_remainder(object o) {
    TEST_NUMB_ARGUMENT_EQ(2, "remainder");

    if (is_Integer(car(o)) == False || is_Integer(cadr(o)) == False) {
        WARNING_MSG("\"remainder\" can only be applied to numbers");
        return NULL;
    } else if (is_Zero(cadr(o)) == True) {
        WARNING_MSG("Can't divide by zero");
        return NULL;
    }

    object num = to_integer(car(o));
    object den = to_integer(cadr(o));
    return make_integer(num->val.number->val.integer %
                        den->val.number->val.integer);
}

object prim_equal(object o) {
    if (list_length(o) < 2) { /* (=) or (= _) ==> #t */
        return _true;
    }

restart:

    if (is_Number(car(o)) == False) {
        WARNING_MSG("Wrong type of arguments on \"=\"");
        return NULL;
    }

    TEST_NEXT_IS_NUMBER(o, "=");

    switch (car(o)->val.number->numtype) {
    case NUM_PINFTY:
        if(cadr(o)->val.number->numtype != NUM_PINFTY) {
            return _false;    /* ==> #f if cadr(o) isn't +inf */
        }
        break;

    case NUM_MINFTY:
        if(cadr(o)->val.number->numtype != NUM_MINFTY) {
            return _false;
        }
        break;

    case NUM_COMPLEX:
        if (is_Zero(real_part(car(o)->val.number)) == False) {
            /* z = a + bj with b != 0 */
            if(cadr(o)->val.number->numtype != NUM_COMPLEX) {
                return _false;
            } else {
                if ((prim_equal(list(real_part(car(o)->val.number),
                                     real_part(cadr(o)->val.number))) == _false) ||
                        (prim_equal(list(imag_part(car(o)->val.number),
                                         imag_part(cadr(o)->val.number))) == _false)) {
                    return _false;
                }
            }
        } else {
            /* z = a + 0j */
            switch (cadr(o)->val.number->numtype) {
            case NUM_PINFTY:
            case NUM_MINFTY:
                return _false;
                break;

            case NUM_REAL:
            case NUM_UINTEGER:
            case NUM_INTEGER:
                if (prim_equal(list(real_part(car(o)->val.number), cadr(o))) == _false) {
                    return _false;
                }
                break;

            default:
                WARNING_MSG("Wrong type of arguments on \"=\"");
                return NULL;
            }
        }
        break;

    case NUM_INTEGER:
    case NUM_UINTEGER:
        switch (cadr(o)->val.number->numtype) {
        case NUM_INTEGER:
        case NUM_UINTEGER:
            if (cadr(o)->val.number->val.integer != car(o)->val.number->val.integer) {
                return _false;
            }
            break;

        case NUM_PINFTY:
        case NUM_MINFTY:
            return _false;
            break;

        case NUM_REAL:
            if (cadr(o)->val.number->val.real != car(o)->val.number->val.integer) {
                return _false;
            }
            break;

        case NUM_COMPLEX:
            if (is_Zero(imag_part(cadr(o)->val.number)) == False ||
                    prim_equal(list(real_part(cadr(o)->val.number), car(o))) == _false) {
                return _false;
            }
            break;

        default:
            WARNING_MSG("Wrong type of arguments on \"=\"");
            return NULL;
        }
        break;

    case NUM_REAL:
        switch (cadr(o)->val.number->numtype) {
        case NUM_UINTEGER:
        case NUM_INTEGER:
            if (car(o)->val.number->val.real != cadr(o)->val.number->val.integer) {
                return _false;    /* (= 4 4.0) evaluates to #t */
            }
            break;

        case NUM_PINFTY:
        case NUM_MINFTY:
            return _false;
            break;

        case NUM_REAL:
            if(car(o)->val.number->val.real  != cadr(o)->val.number->val.real) {
                return _false;
            }
            break;

        case NUM_COMPLEX:
            if (is_Zero(imag_part(cadr(o)->val.number)) == False) {
                return _false;
            } else {
                if (prim_equal(list(car(o), real_part(cadr(o)->val.number))) == _false) {
                    return _false;
                }
            }
            break;

        default:
            WARNING_MSG("Wrong type of arguments on \"=\"");
            return NULL;
        }
        break;

    case NUM_UNDEF:
        return _false;
        break;
    }

    o = cdr(o);
    if(list_length(o) == 1) {
        return _true;
    }
    goto restart;
}

object prim_smaller(object o) {
    if (list_length(o) < 2) {
        return _true;
    }

restart:
    if (is_Number(car(o)) == False) {
        WARNING_MSG("Wrong type of arguments on \"<\"");
        return NULL;
    }

    TEST_NEXT_IS_NUMBER(o, "<");

    switch (car(o)->val.number->numtype) {
    case NUM_UNDEF:
        return _false;

    case NUM_PINFTY:
        if(list_length(o) > 1) {
            /* (< [numbers] +inf [numbers]) is always # */
            return _false;
        }
        break;

    case NUM_MINFTY:
        if(cadr(o)->val.number->numtype == NUM_MINFTY) {
            if(list_length(o) > 1) {
                /* (< [numbers] +inf [numbers]) is always #f */
                return _false;    /* case: (<... -inf -inf ...) ==> #f */
            }
        }
        break;

    case NUM_COMPLEX:
        if(is_Zero(imag_part(car(o)->val.number)) == False) {
            /* There isn't a comparison with complex numbers having imaginary part not zero */
            WARNING_MSG("Wrong type of arguments on \"<\"");
            return NULL;
        } else {
            switch (cadr(o)->val.number->numtype) {
            /* Comparing z = a + 0j with numbers */
            case NUM_REAL:
            case NUM_UINTEGER:
            case NUM_INTEGER:
                if (prim_smaller(list(cadr(o), real_part(car(o)->val.number))) == _false) {
                    return _false;
                }
                break;

            case NUM_UNDEF:
                return _false;
                break;

            case NUM_COMPLEX:
            case NUM_PINFTY:
            case NUM_MINFTY:
                WARNING_MSG("Wrong type of arguments on \"=\"");
                return NULL;
            }
        }
        break;

    case NUM_UINTEGER:
    case NUM_INTEGER:
        switch (cadr(o)->val.number->numtype) {
        case NUM_INTEGER:
        case NUM_UINTEGER:
            if(car(o)->val.number->val.integer >= cadr(o)->val.number->val.integer) {
                return _false;
            }
            break;

        case NUM_PINFTY:
            break;

        case NUM_MINFTY:
            return _false; /* case: (< [number] -inf [number]) is always #f  */
            break;

        case NUM_REAL:
            if(car(o)->val.number->val.integer >= cadr(o)->val.number->val.real) {
                return _false;
            }
            break;

        case NUM_COMPLEX:
            if (is_Zero(imag_part(cadr(o)->val.number)) == False) {
                WARNING_MSG("Wrong type of arguments on \"<\"");
                return NULL;
            }
            if (prim_smaller(list(car(o), real_part(cadr(o)->val.number))) == _false) {
                return _false;
            }
            break;

        case NUM_UNDEF:
            return _false;
        }
        break;

    case NUM_REAL:
        switch (cadr(o)->val.number->numtype) {
        case NUM_INTEGER:
        case NUM_UINTEGER:
            if(car(o)->val.number->val.real  >= cadr(o)->val.number->val.integer) {
                return _false;
            }
            break;

        case NUM_PINFTY:
            break;

        case NUM_MINFTY:
            return _false; /* case: (< [number] -inf [number]) is always #f  */
            break;

        case NUM_REAL:
            if(car(o)->val.number->val.real  >= cadr(o)->val.number->val.real) {
                return _false;
            }
            break;

        case NUM_COMPLEX:
            /* There isn't a comparison between real and complex numbers */
            WARNING_MSG("Wrong type of arguments on \"<\"");
            return NULL;

        case NUM_UNDEF:
            return _false;
        }
    }

    o = cdr(o);
    if(list_length(o) == 1) {
        return _true;
    }
    goto restart;
}

object prim_larger(object o) {
    return prim_smaller(reverse(o));
}

object prim_is_list(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "list?");
    o = car(o);

restart:
    if (!o || (is_Pair(o) == False && is_Nil(o) == False)) {
        return _false;
    }

    if (is_Nil(o) == True) {
        return _true;
    } else {
        o = cdr(o);
        goto restart;
    }
}

object prim_is_positive(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "positive?");
    o = car(o);

    if (is_Number(o) == False) {
        WARNING_MSG("Cannot apply \"positive?\" to something who is not a number");
        return NULL;
    } else {
        switch (o->val.number->numtype) {
        case NUM_PINFTY:
            return _true;

        case NUM_MINFTY:
            return _false;

        case NUM_UINTEGER:
        case NUM_INTEGER:
            return (o->val.number->val.integer > 0 ? _true : _false);

        case NUM_REAL:
            return (o->val.number->val.real > 0 ? _true : _false);

        case NUM_COMPLEX:
            WARNING_MSG("There is no ordering relation on the complex numbers");
            return NULL;

        default:
            WARNING_MSG("Wrong number type (%d)", o->val.number->numtype);
            return NULL;
        }
    }
}

object prim_is_negative(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "negative?");
    o = car(o);

    if (is_Number(o) == False) {
        WARNING_MSG("Cannot apply \"negative?\" to something who is not a number");
        return NULL;
    } else {
        switch (o->val.number->numtype) {
        case NUM_UINTEGER:
        case NUM_PINFTY:
            return _false;

        case NUM_MINFTY:
            return _true;

        case NUM_INTEGER:
            return (o->val.number->val.integer < 0 ? _true : _false);

        case NUM_REAL:
            return (o->val.number->val.real < 0 ? _true : _false);

        case NUM_COMPLEX:
            WARNING_MSG("There is no ordering relation on the complex numbers");
            return NULL;

        default:
            WARNING_MSG("Wrong number type (%d)", o->val.number->numtype);
            return NULL;
        }
    }
}

object prim_is_zero(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "zero?");
    o = car(o);

    if (is_Number(o) == False) {
        WARNING_MSG("Cannot apply \"zero?\" to something who is not a number");
        return NULL;
    } else {
        return (is_Zero(o) == True ? _true : _false);
    }
    return _false;
}

object prim_list(object o) {
    return o;
}

object prim_cons(object o) {
    TEST_NUMB_ARGUMENT_EQ(2, "cons");
    return cons(car(o), cadr(o));
}

object prim_set_car(object o) {
    TEST_NUMB_ARGUMENT_EQ(2, "set-car!");

    if (is_Pair(car(o)) == True) { /* Can only set the car of a list */
        object old_list = car(o);
        object new_car = cadr(o);

        old_list->val.pair.car = new_car;
        return old_list;
    }
    WARNING_MSG("Wrong type of arguments on \"set-car!\"");
    return NULL;
}

object prim_set_cdr(object o) {
    TEST_NUMB_ARGUMENT_EQ(2, "set-cdr!");
    if(is_Pair(car(o)) == True) { /* Can only set the cdr of a list */
        object old_list = car(o);
        object new_cdr = cadr(o);

        if (is_Pair(new_cdr) == False && is_Nil(new_cdr) == False) {
            WARNING_MSG("Can not set the cdr to something who is neither a pair nor nil");
            return NULL;
        }

        old_list->val.pair.cdr = new_cdr;
        return old_list;
    }
    WARNING_MSG("Wrong type of arguments on \"set-cdr!\"");
    return NULL;
}

object prim_car(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "car");
    if(is_Pair(o) == True && is_Pair(car(o)) == True) {
        return car(car(o));
    }
    WARNING_MSG("Wrong type of arguments on \"car\"");
    return NULL;
}

object prim_cdr(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "cdr");
    if(is_Pair(o) == True && is_Pair(car(o)) == True) {
        return cdr(car(o));
    }
    WARNING_MSG("Wrong type of arguments on \"cdr\"");
    return NULL;
}

object prim_is_number(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "number?");
    if (is_Pair(o) == True && is_Number(car(o)) == True) {
        return _true;
    }
    return _false;
}

object prim_is_complex(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "complex?");
    if (is_Pair(o) == True && is_Complex(car(o)) == True) {
        return _true;
    }
    return _false;
}

object prim_is_real(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "real?");
    if(is_Pair(o) == True && is_Real(car(o)) == True) {
        return _true;
    }
    return _false;
}

object prim_is_procedure(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "procedure?");
    if(is_Pair(o) == True &&
            (is_Primitive(car(o)) == True ||
             is_Form(car(o)) == True)) {
        return _true;
    }
    return _false;
}

object prim_is_char(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "char?");
    if(is_Pair(o) == True && is_Char(car(o)) == True) {
        return _true;
    }
    return _false;
}

object prim_is_symbol(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "symbol?");
    if(is_Pair(o) == True && is_Symbol(car(o)) == True) {
        return _true;
    }
    return _false;
}

object prim_is_pair(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "pair?");
    if(is_Pair(o) == True && is_Pair(car(o)) == True) {
        return _true;
    }
    return _false;
}

object prim_is_integer(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "integer?");
    if(is_Pair(o) == True && is_Integer(car(o)) == True) {
        return _true;
    }
    return _false;
}
object prim_is_boolean(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "boolean?");
    if (is_Pair(o) == True && is_Boolean(car(o)) == True) {
        return _true;
    }
    return _false;
}

object prim_is_null(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "null?");
    if(is_Pair(o) == True && is_Nil(car(o)) == True) {
        return _true;
    }
    return _false;
}

object prim_is_string(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "string?");
    if(is_Pair(o) == True && is_String(car(o)) == True) {
        return _true;
    }
    return _false;
}

object prim_arith_plus(object o) {
    if (list_length(o) == 0) {
        return make_integer(0);
    } else if (list_length(o) == 1) {
        return car(o);
    }

    object result = make_integer(0);
    object next_number = NULL;

restart:
    next_number = car(o);
    if (is_Number(next_number) == False) {
        WARNING_MSG("Invalid type of argument in function \"+\"");
        return NULL;
    }

    switch (result->val.number->numtype) {
    case NUM_UNDEF:
        switch (next_number->val.number->numtype) {
        case NUM_COMPLEX:
            result = to_complex(result);
            result->val.number->val.complex->imag = imag_part(next_number->val.number);
            break;

        default:
            break;
        }
        break;

    case NUM_PINFTY:
        switch (next_number->val.number->numtype) {
        case NUM_UNDEF:
            result = NaN;
            break;

        case NUM_MINFTY:
            return NaN; /* (+ +inf -inf) = NaN */

        case NUM_COMPLEX:
            result = to_complex(result);
            result->val.number->val.complex->imag =
                prim_arith_plus(list(imag_part(result->val.number),
                                     imag_part(next_number->val.number)));
            break;

        default:
            break;
        }
        break;

    case NUM_MINFTY:
        switch (next_number->val.number->numtype) {
        case NUM_UNDEF:
        case NUM_PINFTY:
            result = NaN; /* (+ -inf +inf) = NaN */
            break;

        case NUM_COMPLEX:
            result = to_complex(result);
            result->val.number->val.complex->imag =
                prim_arith_plus(list(imag_part(result->val.number),
                                     imag_part(next_number->val.number)));
            break;

        default:
            break;
        }
        break;

    case NUM_UINTEGER:
    case NUM_INTEGER:
        switch (next_number->val.number->numtype) {
        case NUM_PINFTY:
            result = plus_inf;
            break;

        case NUM_MINFTY:
            result = minus_inf;
            break;

        case NUM_UNDEF:
            result = NaN;
            break;

        case NUM_INTEGER:
        case NUM_UINTEGER:
            result->val.number->val.integer += next_number->val.number->val.integer;
            break;

        case NUM_REAL:
            result = to_real(result);
            result->val.number->val.real += next_number->val.number->val.real;
            break;

        case NUM_COMPLEX:
            result = to_complex(result);
            result->val.number->val.complex->real = prim_arith_plus(list(
                    real_part(result->val.number),
                    real_part(next_number->val.number)));
            result->val.number->val.complex->imag = prim_arith_plus(list(
                    imag_part(result->val.number),
                    imag_part(next_number->val.number)));
            break;
        }
        break;

    case NUM_REAL:
        switch (next_number->val.number->numtype) {
        case NUM_PINFTY:
            result = plus_inf;
            break;

        case NUM_MINFTY:
            result = minus_inf;
            break;

        case NUM_UNDEF:
            result = NaN;
            break;

        case NUM_INTEGER:
        case NUM_UINTEGER:
            result->val.number->val.real += next_number->val.number->val.integer;
            break;

        case NUM_REAL:
            result->val.number->val.real += next_number->val.number->val.real;
            break;

        case NUM_COMPLEX:
            result = to_complex(result);
            result->val.number->val.complex->real = prim_arith_plus(list(
                    real_part(result->val.number),
                    real_part(next_number->val.number)));
            result->val.number->val.complex->imag = prim_arith_plus(list(
                    imag_part(result->val.number),
                    imag_part(next_number->val.number)));
            break;
        }
        break;

    case NUM_COMPLEX:
        switch (next_number->val.number->numtype) {
        case NUM_PINFTY:
        case NUM_MINFTY:
        case NUM_UNDEF:
            result->val.number->val.complex->real =
                prim_arith_plus(list(real_part(result->val.number),
                                     real_part(next_number->val.number)));
            break;

        case NUM_INTEGER:
        case NUM_UINTEGER:
        case NUM_REAL:
            result->val.number->val.complex->real = prim_arith_plus(list(
                    real_part(result->val.number),
                    next_number));
            break;

        case NUM_COMPLEX:
            result->val.number->val.complex->real = prim_arith_plus(list(
                    real_part(result->val.number),
                    real_part(next_number->val.number)));
            result->val.number->val.complex->imag = prim_arith_plus(list(
                    imag_part(result->val.number),
                    imag_part(next_number->val.number)));
            break;
        }
        break;
    }

    o = cdr(o);
    if (is_Nil(o) == True) {
        return result;
    } else {
        goto restart;
    }
}

object prim_arith_minus(object o) {
    if (list_length(o) == 0) {
        return make_integer(0);
    } else if (list_length(o) == 1) {
        return prim_arith_minus(list(make_integer(0), car(o))); /* (- a) = (- 0 a) */
    }

    object result = car(o);

    object negative_part = prim_arith_plus(cdr(o));
    switch (negative_part->val.number->numtype) {
    case NUM_PINFTY:
        negative_part = minus_inf;
        break;

    case NUM_MINFTY:
        negative_part = plus_inf;
        break;

    case NUM_UNDEF:
        negative_part = NaN;
        break;

    case NUM_INTEGER:
    case NUM_UINTEGER:
        negative_part = make_integer(-negative_part->val.number->val.integer);
        break;

    case NUM_REAL:
        negative_part = make_real(-negative_part->val.number->val.real);
        break;

    case NUM_COMPLEX:
        negative_part = make_complex(
                            prim_arith_minus(cons(real_part(negative_part->val.number), nil)),
                            prim_arith_minus(cons(imag_part(negative_part->val.number), nil)));
        break;
    }

    return prim_arith_plus(list(result, negative_part));
}

object prim_arith_times(object o) {
    if (list_length(o) == 0) {
        return make_integer(1);
    } else if (list_length(o) == 1) {
        return car(o);
    }

    object result = make_integer(1);
    object next_number = NULL;

restart:
    next_number = car(o);
    if (is_Number(next_number) == False) {
        WARNING_MSG("Invalid type of argument in function \"*\"");
        return NULL;
    }

    object rescpy = make_number(NUM_COMPLEX);

    switch (result->val.number->numtype) {
    case NUM_UNDEF:
        switch (next_number->val.number->numtype) {
        case NUM_COMPLEX:
            result = to_complex(result);
            result->val.number->val.complex->real = prim_arith_times(list(NaN,
                                                    real_part(result->val.number)));
            result->val.number->val.complex->imag = prim_arith_times(list(NaN,
                                                    imag_part(result->val.number)));
            break;

        default:
            break;
        }
        break;

    case NUM_PINFTY:
        switch (next_number->val.number->numtype) {
        case NUM_UNDEF:
            result = NaN;
            break;

        case NUM_PINFTY:
            result = plus_inf;
            break;

        case NUM_MINFTY:
            result = minus_inf;
            break;

        case NUM_UINTEGER:
        case NUM_INTEGER:
            if (next_number->val.number->val.integer == 0) {
                return NaN;
            }
            result = (next_number->val.number->val.integer > 0) ? plus_inf : minus_inf;
            break;

        case NUM_REAL:
            if (next_number->val.number->val.real == 0) {
                return NaN;
            }
            result = (next_number->val.number->val.real > 0) ? plus_inf : minus_inf;
            break;

        case NUM_COMPLEX:
            result->val.number->val.complex->real = prim_arith_times(list(plus_inf,
                                                    real_part(result->val.number)));
            result->val.number->val.complex->imag = prim_arith_times(list(plus_inf,
                                                    imag_part(result->val.number)));
            break;
        }
        break;

    case NUM_MINFTY:
        switch (next_number->val.number->numtype) {
        case NUM_UNDEF:
            result = NaN;
            break;

        case NUM_PINFTY:
            result = minus_inf;
            break;

        case NUM_MINFTY:
            result = plus_inf;
            break;

        case NUM_UINTEGER:
        case NUM_INTEGER:
            if (next_number->val.number->val.integer == 0) {
                return NaN;
            }
            result = (next_number->val.number->val.integer < 0) ? plus_inf : minus_inf;
            break;

        case NUM_REAL:
            if (next_number->val.number->val.real == 0) {
                return NaN;
            }
            result = (next_number->val.number->val.real < 0) ? plus_inf : minus_inf;
            break;

        case NUM_COMPLEX:
            result->val.number->val.complex->real = prim_arith_times(list(minus_inf,
                                                    real_part(result->val.number)));
            result->val.number->val.complex->imag = prim_arith_times(list(minus_inf,
                                                    imag_part(result->val.number)));
            break;
        }
        break;

    case NUM_INTEGER:
    case NUM_UINTEGER:
        switch (next_number->val.number->numtype) {
        case NUM_UNDEF:
            result = NaN;
            break;

        case NUM_PINFTY:
            if (result->val.number->val.integer == 0) {
                result = NaN;
            } else {
                result = (result->val.number->val.integer > 0) ? plus_inf : minus_inf;
            }
            break;

        case NUM_MINFTY:
            if (result->val.number->val.integer == 0) {
                result = NaN;
            } else {
                result = (result->val.number->val.integer > 0) ? minus_inf : plus_inf;
            }
            break;

        case NUM_UINTEGER:
        case NUM_INTEGER:
            result->val.number->val.integer *= next_number->val.number->val.integer;
            break;

        case NUM_REAL:
            result = to_real(result);
            result->val.number->val.real *= next_number->val.number->val.real;
            break;

        case NUM_COMPLEX:
            result = to_complex(result);
            rescpy->val.number->val.complex->real = sfs_malloc(sizeof(*
                                                    (result->val.number->val.complex->real)));
            rescpy->val.number->val.complex->imag = sfs_malloc(sizeof(*
                                                    (result->val.number->val.complex->imag)));
            memcpy(rescpy->val.number->val.complex->real,
                   result->val.number->val.complex->real,
                   sizeof(*(result->val.number->val.complex->real)));
            memcpy(rescpy->val.number->val.complex->imag,
                   result->val.number->val.complex->imag,
                   sizeof(*(result->val.number->val.complex->imag)));
            result->val.number->val.complex->real = prim_arith_times(list(real_part(
                    rescpy->val.number), real_part(next_number->val.number)));
            result->val.number->val.complex->imag = prim_arith_times(list(real_part(
                    rescpy->val.number), imag_part(next_number->val.number)));
            break;
        }
        break;

    case NUM_REAL:
        switch (next_number->val.number->numtype) {
        case NUM_UNDEF:
            result = NaN;
            break;

        case NUM_PINFTY:
            if (result->val.number->val.real == 0) {
                return NaN;
            }
            result = (result->val.number->val.real > 0) ? plus_inf : minus_inf;
            break;

        case NUM_MINFTY:
            if (result->val.number->val.real == 0) {
                return NaN;
            }
            result = (result->val.number->val.real > 0) ? minus_inf : plus_inf;
            break;

        case NUM_UINTEGER:
        case NUM_INTEGER:
            result->val.number->val.real *= next_number->val.number->val.integer;
            break;

        case NUM_REAL:
            result->val.number->val.real *= next_number->val.number->val.real;
            break;

        case NUM_COMPLEX:
            result = to_complex(result);
            rescpy->val.number->val.complex->real = sfs_malloc(sizeof(*
                                                    (result->val.number->val.complex->real)));
            rescpy->val.number->val.complex->imag = sfs_malloc(sizeof(*
                                                    (result->val.number->val.complex->imag)));
            memcpy(rescpy->val.number->val.complex->real,
                   result->val.number->val.complex->real,
                   sizeof(*(result->val.number->val.complex->real)));
            memcpy(rescpy->val.number->val.complex->imag,
                   result->val.number->val.complex->imag,
                   sizeof(*(result->val.number->val.complex->imag)));
            result->val.number->val.complex->real = prim_arith_times(list(real_part(
                    rescpy->val.number), real_part(next_number->val.number)));
            result->val.number->val.complex->imag = prim_arith_times(list(real_part(
                    rescpy->val.number), imag_part(next_number->val.number)));
            break;
        }
        break;

    case NUM_COMPLEX:
        switch (next_number->val.number->numtype) {
        case NUM_UNDEF:
        case NUM_MINFTY:
        case NUM_PINFTY:
            result->val.number->val.complex->real = prim_arith_times(list(next_number,
                                                    real_part(result->val.number)));
            result->val.number->val.complex->imag = prim_arith_times(list(next_number,
                                                    imag_part(result->val.number)));
            break;

        case NUM_UINTEGER:
        case NUM_INTEGER:
        case NUM_REAL:
            result->val.number->val.complex->real = prim_arith_times(list(real_part(
                    result->val.number), next_number));
            result->val.number->val.complex->imag = prim_arith_times(list(imag_part(
                    result->val.number), next_number));
            break;

        case NUM_COMPLEX:
            result = to_complex(result);
            rescpy->val.number->val.complex->real = sfs_malloc(sizeof(*
                                                    (result->val.number->val.complex->real)));
            rescpy->val.number->val.complex->imag = sfs_malloc(sizeof(*
                                                    (result->val.number->val.complex->imag)));
            memcpy(rescpy->val.number->val.complex->real,
                   result->val.number->val.complex->real,
                   sizeof(*(result->val.number->val.complex->real)));
            memcpy(rescpy->val.number->val.complex->imag,
                   result->val.number->val.complex->imag,
                   sizeof(*(result->val.number->val.complex->imag)));
            result->val.number->val.complex->real = prim_arith_minus(list(
                    prim_arith_times(list(real_part(next_number->val.number),
                                          real_part(rescpy->val.number))),
                    prim_arith_times(list(imag_part(next_number->val.number),
                                          imag_part(rescpy->val.number)))));
            result->val.number->val.complex->imag = prim_arith_plus(list(
                    prim_arith_times(list(real_part(next_number->val.number),
                                          imag_part(rescpy->val.number))),
                    prim_arith_times(list(imag_part(next_number->val.number),
                                          real_part(rescpy->val.number)))));
            break;
        }
        break;
    }

    o = cdr(o);
    if (is_Nil(o) == True) {
        return result;
    } else {
        goto restart;
    }
}

object prim_arith_division(object o) {
    if (list_length(o) == 0) {
        return make_integer(1);
    } else if (list_length(o) == 1) {
        return prim_arith_division(list(make_integer(1), car(o)));
        /* (/ a) = (/ 1 a) */
    }

    object result = car(o);

    object denominator = prim_arith_times(cdr(o));
    if (is_Zero(denominator) == True) {
        WARNING_MSG("Can't divide by zero");
        return NULL;
    }

    switch (denominator->val.number->numtype) {
    case NUM_PINFTY:
    case NUM_MINFTY:
        denominator = make_integer(0);
        break;

    case NUM_UNDEF:
        denominator = NaN;
        break;

    case NUM_INTEGER:
    case NUM_UINTEGER:
        denominator = make_real(1.0L / denominator->val.number->val.integer);
        break;

    case NUM_REAL:
        denominator = make_real(1.0L / denominator->val.number->val.real);
        break;

    case NUM_COMPLEX:
        denominator = prim_arith_division(list(num_conj(denominator),
                                               num_abs(denominator)));
        break;
    }

    return prim_arith_times(list(result, denominator));
}
