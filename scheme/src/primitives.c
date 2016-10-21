#include <primitives.h>
#include <forms.h>
#include <limits.h>
#include <symbols.h>
#include <lists.h>
#include <print.h>

#define TEST_NUMB_ARGUMENT_EQ(n_Arg, nomFunction) \
    if (list_length(o) != n_Arg) { \
        WARNING_MSG("Wrong number of arguments on \"" nomFunction "\""); \
        return NULL;\
    }

void create_basic_primitives() {
    /* Those are the basic type comparison functions */
    define_symbol(make_symbol("boolean?"), make_primitive(prim_is_boolean), 0);
    define_symbol(make_symbol("null?"), make_primitive(prim_is_null), 0);
    define_symbol(make_symbol("string?"), make_primitive(prim_is_string), 0);
    define_symbol(make_symbol("integer?"), make_primitive(prim_is_integer), 0);
    define_symbol(make_symbol("pair?"), make_primitive(prim_is_pair), 0);
    define_symbol(make_symbol("symbol?"), make_primitive(prim_is_symbol), 0);
    define_symbol(make_symbol("char?"), make_primitive(prim_is_char), 0);
    define_symbol(make_symbol("real?"), make_primitive(prim_is_real), 0);
    define_symbol(make_symbol("procedure?"), make_primitive(prim_is_procedure), 0);

    /* Those are the basic list handling function */
    define_symbol(make_symbol("car"), make_primitive(prim_car), 0);
    define_symbol(make_symbol("cdr"), make_primitive(prim_cdr), 0);
    define_symbol(make_symbol("set-car!"), make_primitive(prim_set_car), 0);
    define_symbol(make_symbol("set-cdr!"), make_primitive(prim_set_cdr), 0);
    define_symbol(make_symbol("cons"), make_primitive(prim_cons), 0);
    define_symbol(make_symbol("list"), make_primitive(prim_list), 0);

    /* Those are the basic arithmetic primitives */
    define_symbol(make_symbol("+"), make_primitive(prim_arith_plus), 0);
}

object prim_list(object o){
    return o;
}
object prim_cons(object o){
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
    return _false;/*faut corrriger*/
}

object prim_is_string(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "string?");
    if(is_Pair(o) == True && is_String(car(o)) == True) {
        return _true;
    }
    return _false;
}
object prim_arith_plus(object o) {
    object res = make_object(SFS_NUMBER);
    res->val.number.numtype = NUM_INTEGER;
    res->val.number.val.integer = 0;

restart:
    if (is_Nil(o) == True) {
        return res;
    }
    if (is_Number(car(o)) == False) {
        WARNING_MSG("Invalid argument to primitive \"+\". Was expecting a SFS_NUMBER");
        return NULL;
    }

    switch (car(o)->val.number.numtype) {
    case NUM_PINFTY:
        if (res->val.number.numtype != NUM_MINFTY &&
                res->val.number.numtype != NUM_UNDEF) {
            res->val.number.numtype = NUM_PINFTY;
        } else {
            res->val.number.numtype = NUM_UNDEF;
        }
        break;

    case NUM_MINFTY:
        if (res->val.number.numtype != NUM_PINFTY &&
                res->val.number.numtype != NUM_UNDEF) {
            res->val.number.numtype = NUM_MINFTY;
        } else {
            res->val.number.numtype = NUM_UNDEF;
        }
        break;

    case NUM_COMPLEX:
        if (res->val.number.numtype == NUM_PINFTY ||
                res->val.number.numtype == NUM_MINFTY ||
                res->val.number.numtype == NUM_UNDEF) {
            break;
        } else if (res->val.number.numtype == NUM_COMPLEX) {
            res->val.number.val.complex.real += car(o)->val.number.val.complex.real;
            res->val.number.val.complex.imag += car(o)->val.number.val.complex.imag;
        } else if (res->val.number.numtype == NUM_REAL) {
            res->val.number.numtype = NUM_COMPLEX;
            res->val.number.val.complex.real = res->val.number.val.real +
                                               car(o)->val.number.val.complex.real;
            res->val.number.val.complex.imag = car(o)->val.number.val.complex.imag;
        } else {
            res->val.number.numtype = NUM_COMPLEX;
            res->val.number.val.complex.real = res->val.number.val.integer +
                                               car(o)->val.number.val.complex.real;
            res->val.number.val.complex.imag = car(o)->val.number.val.complex.imag;
        }
        break;

    case NUM_INTEGER:
    case NUM_UINTEGER:
        if (res->val.number.numtype == NUM_PINFTY ||
                res->val.number.numtype == NUM_MINFTY ||
                res->val.number.numtype == NUM_UNDEF) {
            break;
        } else if (res->val.number.numtype == NUM_COMPLEX) {
            res->val.number.val.complex.real += car(o)->val.number.val.integer;
        } else if (res->val.number.numtype == NUM_REAL) {
            res->val.number.val.real = res->val.number.val.integer +
                                       car(o)->val.number.val.real;
        } else {
            /* Test si un overflow se produirait. */
            if (car(o)->val.number.val.integer > 0 &&
                    res->val.number.val.integer > INT_MAX - car(o)->val.number.val.integer) {
                res->val.number.numtype = NUM_PINFTY;
            }
            /* Test si un overflow se produirait. */
            else if (car(o)->val.number.val.integer < 0 &&
                     res->val.number.val.integer < INT_MIN - car(o)->val.number.val.integer) {
                res->val.number.numtype = NUM_MINFTY;
            } else {
                res->val.number.val.integer += car(o)->val.number.val.integer;
            }
        }
        break;

    case NUM_REAL:
        if (res->val.number.numtype == NUM_PINFTY ||
                res->val.number.numtype == NUM_MINFTY ||
                res->val.number.numtype == NUM_UNDEF) {
            break;
        } else if (res->val.number.numtype == NUM_COMPLEX) {
            res->val.number.val.complex.real += car(o)->val.number.val.real;
        } else if (res->val.number.numtype == NUM_REAL) {
            res->val.number.val.real += car(o)->val.number.val.real;
        } else {
            res->val.number.numtype = NUM_REAL;
            res->val.number.val.real = res->val.number.val.integer +
                                       car(o)->val.number.val.integer;
        }
        break;
    }

    o = cdr(o);
    goto restart;
}
