#include <primitives.h>
#include <forms.h>
#include <limits.h>
#include <symbols.h>
#include <lists.h>
#include <print.h>
#include <math.h>
#include <stdio.h>
#include <mem.h>
#include <read.h>
#include <stdlib.h>

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

void create_basic_primitives(object env) {
    /* Those are the basic type comparison functions */
    create_primitive("boolean?", prim_is_boolean, env);
    create_primitive("null?", prim_is_null, env);
    create_primitive("string?", prim_is_string, env);
    create_primitive("pair?", prim_is_pair, env);
    create_primitive("symbol?", prim_is_symbol, env);
    create_primitive("char?", prim_is_char, env);
    create_primitive("number?", prim_is_number, env);
    create_primitive("list?", prim_is_list, env);
    create_primitive("procedure?", prim_is_procedure, env);

    /* Those are the number related comparison functions */
    create_primitive("real?", prim_is_real, env);
    create_primitive("complex?", prim_is_complex, env);
    create_primitive("integer?", prim_is_integer, env);
    create_primitive("positive?", prim_is_positive, env);
    create_primitive("negative?", prim_is_negative, env);
    create_primitive("zero?", prim_is_zero, env);
    create_primitive("equal?", prim_is_equal, env);
    create_primitive("eq?", prim_is_eq, env);
    create_primitive("not", prim_not, env);

    /* Those are the basic list handling functions */
    create_primitive("car", prim_car, env);
    create_primitive("cdr", prim_cdr, env);
    create_primitive("set-car!", prim_set_car, env);
    create_primitive("set-cdr!", prim_set_cdr, env);
    create_primitive("cons", prim_cons, env);
    create_primitive("list", prim_list, env);
    create_primitive("append", prim_append, env);

    /* Those are ordering primitives */
    create_primitive(">", prim_larger, env);
    create_primitive("<", prim_smaller, env);
    create_primitive("=", prim_equal, env);

    /* Those are conversation primitives */
    create_primitive("integer->char", prim_integer_to_char, env);
    create_primitive("char->integer", prim_char_to_integer, env);
    create_primitive("number->string", prim_number_to_string, env);
    create_primitive("string->number", prim_string_to_number, env);
    create_primitive("symbol->string", prim_symbol_to_string, env);
    create_primitive("string->symbol", prim_string_to_symbol, env);
    create_primitive("string->list", prim_string_to_list, env);

    /* Those are the basic arithmetic primitives */
    create_primitive("+", prim_plus, env);
    create_primitive("-", prim_minus, env);
    create_primitive("*", prim_times, env);
    create_primitive("/", prim_division, env);
    create_primitive("remainder", prim_remainder, env);
    create_primitive("quotient", prim_quotient, env);
    create_primitive("modulo", prim_modulo, env);
    create_primitive("abs", prim_abs, env);
    create_primitive("exp", prim_exp, env);
    create_primitive("sin", prim_sin, env);
    create_primitive("cos", prim_cos, env);
    create_primitive("log", prim_log, env);

    /* Those are the primitives related to complex numbers */
    create_primitive("make-rectangular", prim_make_rectangular, env);
    create_primitive("make-polar", prim_make_polar, env);
    create_primitive("real-part", prim_real_part, env);
    create_primitive("imag-part", prim_imag_part, env);
    create_primitive("magnitude", prim_magnitude, env);
    create_primitive("angle", prim_angle, env);
}

void create_primitive(string prim_name, object (*func)(object), object env) {
    if (is_Environment(env) == False) {
        WARNING_MSG("Can't create a primitive into something who is not an environment");
    }
    define_symbol(make_symbol(prim_name), make_primitive(func, prim_name), &env);
}

object prim_not(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "not");

    if (is_True(car(o)) == True) {
        return _false;
    } else {
        return _true;
    }
}

object prim_make_rectangular(object o) {
    TEST_NUMB_ARGUMENT_EQ(2, "make-rectangular");
    if (is_Real(car(o)) == False || is_Real(cadr(o)) == False) {
        WARNING_MSG("Wrong type of arguments on \"make-rectangular\"");
        return NULL;
    }
    return make_complex(car(o), cadr(o));
}

object prim_make_polar(object o) {
    TEST_NUMB_ARGUMENT_EQ(2, "make-rectangular");
    if (is_Real(car(o)) == False || is_Real(cadr(o)) == False) {
        WARNING_MSG("Wrong type of arguments on \"make-rectangular\"");
        return NULL;
    }

    object phase = prim_times(list(
                                  make_complex(make_integer(0), make_integer(1)),
                                  cadr(o)));
    return prim_times(list(car(o), prim_exp(cons(phase, nil))));
}

object prim_real_part(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "real-part");
    o = car(o);
    if (is_Number(o) == False) {
        WARNING_MSG("Wrong type of arguments on \"real-part\"");
        return NULL;
    }

    return real_part(o->val.number);
}

object prim_imag_part(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "imag-part");
    o = car(o);
    if (is_Number(o) == False) {
        WARNING_MSG("Wrong type of arguments on \"imag-part\"");
        return NULL;
    }

    return imag_part(o->val.number);
}

object prim_magnitude(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "magnitude");
    o = car(o);
    if (is_Number(o) == False) {
        WARNING_MSG("Wrong type of arguments on \"magnitude\"");
        return NULL;
    }

    return num_abs(o);
}

object prim_angle(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "angle");
    o = car(o);
    if (is_Number(o) == False) {
        WARNING_MSG("Wrong type of arguments on \"angle\"");
        return NULL;
    }

    return num_phase(o);
}

object prim_exp(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "exp");
    o = car(o);

    if (is_Number(o) == False) {
        WARNING_MSG("\"exp\" can only be applied to numbers");
        return NULL;
    }

    switch (o->val.number->numtype) {
    case NUM_UNDEF:
        return NaN;
        break;

    case NUM_MINFTY:
        return make_integer(0);
        break;

    case NUM_PINFTY:
        return plus_inf;
        break;

    case NUM_INTEGER:
    case NUM_UINTEGER:
        return make_real(exp(o->val.number->val.integer));
        break;

    case NUM_REAL:
        return make_real(exp(o->val.number->val.real));
        break;

    case NUM_COMPLEX:
        (void) o;
        object mag = prim_exp(cons(real_part(o->val.number), nil));
        object pha = make_complex(prim_cos(cons(imag_part(o->val.number), nil)),
                                  prim_sin(cons(imag_part(o->val.number), nil)));
        return prim_times(list(mag, pha));
        break;
    }

    return NULL;
}

object prim_log(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "exp");
    o = car(o);

    if (is_Number(o) == False) {
        WARNING_MSG("\"exp\" can only be applied to numbers");
        return NULL;
    }

    if (is_Zero(o) == True) {
        WARNING_MSG("(log 0) is not defined");
        return NULL;
    }

    switch (o->val.number->numtype) {
    case NUM_UNDEF:
        return NaN;
        break;

    case NUM_PINFTY:
        return plus_inf;
        break;

    case NUM_MINFTY:
        return make_complex(plus_inf, make_real(acos(-1)));
        break;

    case NUM_INTEGER:
    case NUM_UINTEGER:
        if (is_Positive(o) == True) {
            return make_real(log(o->val.number->val.integer));
        } else {
            return make_complex(make_real(log(-o->val.number->val.integer)), make_real(acos(-1)));
        }
        break;

    case NUM_REAL:
        if (is_Positive(o) == True) {
            return make_real(log(o->val.number->val.real));
        } else {
            return make_complex(make_real(log(-o->val.number->val.real)), make_real(acos(-1)));
        }
        break;

    case NUM_COMPLEX:
        return make_complex(make_real(log(num_abs(o)->val.number->val.real)), num_phase(o));
        break;
    }

    return NULL;
}

object prim_sin(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "sin");
    o = car(o);

    if (is_Number(o) == False) {
        WARNING_MSG("\"sin\" can only be applied to numbers");
        return NULL;
    }
    switch (o->val.number->numtype) {
    case NUM_UNDEF:
    case NUM_MINFTY:
    case NUM_PINFTY:
        return NaN;
        break;

    case NUM_INTEGER:
    case NUM_UINTEGER:
        return make_real(sin((double)o->val.number->val.integer));
        break;

    case NUM_REAL:
        return make_real(sin((double)o->val.number->val.real));
        break;

    case NUM_COMPLEX:
        (void) o;
        return prim_cos(cons(prim_minus(list(make_real(acos(-1) / 2.0), o)), nil));
        break;
    }
    return NULL;
}

object prim_cos(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "cos");
    o = car(o);

    if (is_Number(o) == False) {
        WARNING_MSG("\"cos\" can only be applied to numbers");
        return NULL;
    }
    switch (o->val.number->numtype) {
    case NUM_UNDEF:
    case NUM_MINFTY:
    case NUM_PINFTY:
        return NaN;
        break;

    case NUM_INTEGER:
    case NUM_UINTEGER:
        return make_real(cos((double)o->val.number->val.integer));
        break;

    case NUM_REAL:
        return make_real(cos((double)o->val.number->val.real));
        break;

    case NUM_COMPLEX:
        (void) o;
        /* cos(o) = (exp(jo) + exp(-jo))/2 */
        object exp1 = prim_exp(cons(prim_times(list(
                make_complex(make_integer(0), make_integer(1)), o)), nil));
        object exp2 = prim_exp(cons(prim_times(list(
                make_complex(make_integer(0), make_integer(-1)), o)), nil));
        return prim_division(list(prim_plus(list(exp1, exp2)), make_integer(2)));
        break;
    }
    return NULL;
}

object prim_abs(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "abs");

    if (is_Number(car(o)) == False) {
        WARNING_MSG("\"abs\" can only be applied to numbers");
        return NULL;
    }
    return num_abs(car(o));
}

object prim_quotient(object o) {
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

object prim_modulo(object o) {
    TEST_NUMB_ARGUMENT_EQ(2, "modulo");

    if (is_Integer(car(o)) == False || is_Integer(cadr(o)) == False) {
        WARNING_MSG("\"modulo\" can only be applied to numbers");
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

object prim_remainder(object o) {
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

object prim_string_to_number(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "string->number");
    if(is_String(car(o)) != True) {
        WARNING_MSG("Wrong type of arguments on \"string->number\"");
        return NULL;
    } else {
        uint h = 0;
        return sfs_read_number(car(o)->val.string, &h);
    }
}

object prim_string_to_list(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "string->list");
    o = car(o);
    if (is_String(o) == False) {
        WARNING_MSG("Wrong type of arguments on \"string->symbol\"");
        return NULL;
    }

    object res = nil;
    uint i = 0;

restart:
    res = cons(make_character(o->val.string[i++]), res);
    if (o->val.string[i] != '\0') {
        goto restart;
    }
    return reverse(res);
}

object prim_string_to_symbol(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "string->symbol");
    if(is_String(car(o)) != True) {
        WARNING_MSG("Wrong type of arguments on \"string->symbol\"");
        return NULL;
    } else {
        uint h = 0;
        object symb = sfs_read(car(o)->val.string, &h);
        if (is_Symbol(symb) == True) {
            return symb;
        } else {
            WARNING_MSG("Can't create a symbol \"%s\"", car(o)->val.string);
            return NULL;
        }
    }
}
object prim_symbol_to_string(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "symbol->string");
    if(is_Symbol(car(o)) != True) {
        WARNING_MSG("Wrong type of arguments on \"symbol->string\"");
        return NULL;
    } else {
        return make_string(car(o)->val.symbol);
    }
}

object prim_number_to_string(object o) {
    if (list_length(o) > 2) {
        WARNING_MSG("Wrong number of arguments on \"number->string\"");
        return NULL;
    }
    if (is_Number(car(o)) == False) {
        WARNING_MSG("Wrong type of arguments on \"number->string\"");
        return NULL;
    } else if (list_length(o) == 2 && is_Integer(cadr(o)) == False) {
        WARNING_MSG("Wrong type of arguments on \"number->string\"");
        return NULL;
    } else {
        string str = "";
        switch (car(o)->val.number->numtype) {
        case NUM_INTEGER:
        case NUM_UINTEGER:
            if (list_length(o) == 2) {
                switch (cadr(o)->val.number->val.integer) {
                case 2:
                    (void) o;
                    int s;
#define bitN(arg,n) (((arg)>>(n))&1)
                    for (s = sizeof(int) * CHAR_BIT - 1; s >= 0; s--) {
                        str[sizeof(int) * CHAR_BIT - s - 1] =
                            bitN(car(o)->val.number->val.integer, s) == 1 ? '1' : '0';
                    }
                    str[sizeof(int) * CHAR_BIT] = '\0';
#undef bitN
                    break;

                case 8:
                    sprintf(str, "%o", car(o)->val.number->val.integer);
                    break;

                case 16:
                    sprintf(str, "%X", car(o)->val.number->val.integer);
                    break;

                case 10:
                    sprintf(str, "%d", car(o)->val.number->val.integer);
                    break;

                default:
                    WARNING_MSG("Invalid radix (must be 2, 8, 10 or 16)");
                    return NULL;
                }
            } else {
                sprintf(str, "%d", car(o)->val.number->val.integer);
            }
            break;

        case NUM_REAL:
            sprintf(str, "%Lg", car(o)->val.number->val.real);
            break;

        case NUM_UNDEF:
            sprintf(str, "NaN");
            break;

        case NUM_PINFTY:
            sprintf(str, "+inf");
            break;

        case NUM_MINFTY:
            sprintf(str, "-inf");
            break;

        case NUM_COMPLEX:
            (void) o;
            object realpart = prim_number_to_string(cons(real_part(car(o)->val.number),
                                                    nil));
            object imagpart = prim_number_to_string(cons(imag_part(car(o)->val.number),
                                                    nil));
            if (imag_part(car(o)->val.number) == NaN ||
                    is_Negative(imag_part(car(o)->val.number)) == False) {
                sprintf(str, "%s+%sj", realpart->val.string, imagpart->val.string);
            } else {
                sprintf(str, "%s%sj", realpart->val.string, imagpart->val.string);
            }
            break;
        }
        return make_string(str);
    }
    return NULL;
}

object prim_char_to_integer(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "char->integer");
    if(is_Char(car(o)) != True) {
        WARNING_MSG("Wrong type of arguments on \"char->integer\"");
        return NULL;
    } else {
        return make_integer((int) car(o)->val.character);
    }
}

object prim_integer_to_char(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "integer->char");
    if (car(o)->val.number->numtype != NUM_UINTEGER &&
            car(o)->val.number->numtype != NUM_INTEGER) {
        WARNING_MSG("Wrong type of arguments on \"integer->char\"");
        return NULL;
    } else if (car(o)->val.number->val.integer < 0) {
        WARNING_MSG("Can not convert negative number to char");
        return NULL;
    } else {
        return make_character((char) car(o)->val.number->val.integer);
    }
}

object prim_is_eq(object o) {
    if (list_length(o) < 2) { /* (eq?) or (eq? argument1 ) ==> #t */
        return _true;
    }
restart:
    if (is_Symbol(car(o)) == True && is_Symbol(cadr(o)) == True) {
        return (strcmp(car(o)->val.string, cadr(o)->val.string) == 0 ? _true : _false);
    } else if (car(o) != cadr(o)) {
        return _false;
    } else {
        o = cdr(o);
        if(list_length(o) == 1) {
            return _true;
        }
    }
    goto restart;

}
object prim_is_equal(object o) {
    if (list_length(o) < 2) { /* (equal?) or (equal? argument1 ) ==> #t */
        return _true;
    }
restart:
    if (car(o)->val.primitive.f != cadr(o)->val.primitive.f) {
        return _false;
    } else {
        o = cdr(o);
        if(list_length(o) == 1) {
            return _true;
        }
    }
    goto restart;
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

object prim_append(object o) {
    object res = nil;
    if (is_List(o) == False) {
        WARNING_MSG("Wrong type of arguments on \"append\": expecting lists");
        return NULL;
    }

restart:
    if (is_Nil(o) == True) {
        return reverse(res);
    }

    object curr_list = car(o);
    if (is_List(curr_list) == False) {
        WARNING_MSG("Wrong type of arguments on \"append\": expecting lists");
        return NULL;
    }
    while (is_Nil(curr_list) == False) {
        res = cons(car(curr_list), res);
        curr_list = cdr(curr_list);
    }

    o = cdr(o);
    goto restart;
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
    o = car(o);
    if(is_Pair(o) == True) {
        return car(o);
    }
    WARNING_MSG("Wrong type of arguments on \"car\"");
    return NULL;
}

object prim_cdr(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "cdr");
    o = car(o);
    if (is_Pair(o) == True) {
        return cdr(o);
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

object prim_plus(object o) {
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
                prim_plus(list(imag_part(result->val.number),
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
                prim_plus(list(imag_part(result->val.number),
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
            result->val.number->val.complex->real = prim_plus(list(
                    real_part(result->val.number),
                    real_part(next_number->val.number)));
            result->val.number->val.complex->imag = prim_plus(list(
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
            result->val.number->val.complex->real = prim_plus(list(
                    real_part(result->val.number),
                    real_part(next_number->val.number)));
            result->val.number->val.complex->imag = prim_plus(list(
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
                prim_plus(list(real_part(result->val.number),
                               real_part(next_number->val.number)));
            break;

        case NUM_INTEGER:
        case NUM_UINTEGER:
        case NUM_REAL:
            result->val.number->val.complex->real = prim_plus(list(
                    real_part(result->val.number),
                    next_number));
            break;

        case NUM_COMPLEX:
            result->val.number->val.complex->real = prim_plus(list(
                    real_part(result->val.number),
                    real_part(next_number->val.number)));
            result->val.number->val.complex->imag = prim_plus(list(
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

object prim_minus(object o) {
    if (list_length(o) == 0) {
        return make_integer(0);
    } else if (list_length(o) == 1) {
        return prim_minus(list(make_integer(0), car(o))); /* (- a) = (- 0 a) */
    }

    object result = car(o);

    object negative_part = prim_plus(cdr(o));
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
                            prim_minus(cons(real_part(negative_part->val.number), nil)),
                            prim_minus(cons(imag_part(negative_part->val.number), nil)));
        break;
    }

    return prim_plus(list(result, negative_part));
}

object prim_times(object o) {
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
            result->val.number->val.complex->real = prim_times(list(NaN,
                                                    real_part(result->val.number)));
            result->val.number->val.complex->imag = prim_times(list(NaN,
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
            result = to_complex(result);
            result->val.number->val.complex->real = prim_times(list(plus_inf,
                                                    real_part(result->val.number)));
            result->val.number->val.complex->imag = prim_times(list(plus_inf,
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
            result = to_complex(result);
            result->val.number->val.complex->real = prim_times(list(minus_inf,
                                                    real_part(result->val.number)));
            result->val.number->val.complex->imag = prim_times(list(minus_inf,
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
            result->val.number->val.complex->real = prim_times(list(real_part(
                    rescpy->val.number), real_part(next_number->val.number)));
            result->val.number->val.complex->imag = prim_times(list(real_part(
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
            result->val.number->val.complex->real = prim_times(list(real_part(
                    rescpy->val.number), real_part(next_number->val.number)));
            result->val.number->val.complex->imag = prim_times(list(real_part(
                    rescpy->val.number), imag_part(next_number->val.number)));
            break;
        }
        break;

    case NUM_COMPLEX:
        switch (next_number->val.number->numtype) {
        case NUM_UNDEF:
        case NUM_MINFTY:
        case NUM_PINFTY:
            result->val.number->val.complex->real = prim_times(list(next_number,
                                                    real_part(result->val.number)));
            result->val.number->val.complex->imag = prim_times(list(next_number,
                                                    imag_part(result->val.number)));
            break;

        case NUM_UINTEGER:
        case NUM_INTEGER:
        case NUM_REAL:
            result->val.number->val.complex->real = prim_times(list(real_part(
                    result->val.number), next_number));
            result->val.number->val.complex->imag = prim_times(list(imag_part(
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
            result->val.number->val.complex->real = prim_minus(list(
                    prim_times(list(real_part(next_number->val.number),
                                    real_part(rescpy->val.number))),
                    prim_times(list(imag_part(next_number->val.number),
                                    imag_part(rescpy->val.number)))));
            result->val.number->val.complex->imag = prim_plus(list(
                    prim_times(list(real_part(next_number->val.number),
                                    imag_part(rescpy->val.number))),
                    prim_times(list(imag_part(next_number->val.number),
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

object prim_division(object o) {
    if (list_length(o) == 0) {
        return make_integer(1);
    } else if (list_length(o) == 1) {
        return prim_division(list(make_integer(1), car(o)));
        /* (/ a) = (/ 1 a) */
    }

    object result = car(o);

    object denominator = prim_times(cdr(o));
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
        denominator = prim_division(list(num_conj(denominator),
                                         prim_times(list(num_abs(denominator), num_abs(denominator)))));
        break;
    }

    return prim_times(list(result, denominator));
}
