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
#define TEST_CONDITION_ARGUMENT_EQ(obj, nomFunction)\
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

    /* Those are the basic arithmetic primitives */
    //define_symbol(make_symbol("+"), make_primitive(prim_arith_plus), 0);
    create_primitive(">", prim_larger);
    create_primitive("<", prim_smaller);
    create_primitive("=", prim_equal);
}

void create_primitive(string prim_name, object (*func)(object)) {
    define_symbol(make_symbol(prim_name), make_primitive(func, prim_name), 0);

}

object prim_equal(object o){
    if (list_length(o) < 2){
        return _true;
    }
restart:
    if (is_Number(car(o)) == False){
        WARNING_MSG("Wrong type of arguments on \"=\"");
        return NULL;
    }
    switch (car(o)->val.number.numtype) {
    case NUM_PINFTY:
        WARNING_MSG("ICI_+inf");
        TEST_CONDITION_ARGUMENT_EQ(o, "=");
        if(cadr(o)->val.number.numtype != NUM_PINFTY) return _false;
        break;
    case NUM_MINFTY:
        WARNING_MSG("ICI_-inf");
        TEST_CONDITION_ARGUMENT_EQ(o, "=");
        if(cadr(o)->val.number.numtype != NUM_MINFTY) return _false;
        break;
    case NUM_COMPLEX:
        TEST_CONDITION_ARGUMENT_EQ(o, "=");
        if(car(o)->val.number.val.complex.imag != 0){
            if(cadr(o)->val.number.numtype != NUM_COMPLEX) return _false;
        }else {
            if(cadr(o)->val.number.numtype == NUM_MINFTY || cadr(o)->val.number.numtype == NUM_PINFTY) return _false;
            if(cadr(o)->val.number.numtype == NUM_REAL){
                if( car(o)->val.number.val.complex.real != cadr(o)->val.number.val.real) return _false;
            }
            if(cadr(o)->val.number.numtype == NUM_UINTEGER){
                if( cadr(o)->val.number.val.integer != car(o)->val.number.val.complex.real) return _false;
            }
            if(cadr(o)->val.number.numtype == NUM_COMPLEX){
                if(car(o)->val.number.val.complex.real != cadr(o)->val.number.val.complex.real ||
                        car(o)->val.number.val.complex.imag != cadr(o)->val.number.val.complex.imag) return _false;
            }
        }
        break;
    case NUM_INTEGER:
        break;
    case NUM_UINTEGER:
         WARNING_MSG("ICI_UNITEGER");
         TEST_CONDITION_ARGUMENT_EQ(o, "=");
        if(cadr(o)->val.number.numtype == NUM_UINTEGER){
            WARNING_MSG("Compare uinteger with uniteger");
            if(car(o)->val.number.val.integer != cadr(o)->val.number.val.integer) return _false;
        }
        if(cadr(o)->val.number.numtype == NUM_REAL){
            WARNING_MSG("Compare uinteger with real");
            if( car(o)->val.number.val.integer != cadr(o)->val.number.val.real) return _false;
        }
        if(cadr(o)->val.number.numtype == NUM_COMPLEX){
            WARNING_MSG("Compare uinteger with complex");
            if( cadr(o)->val.number.val.complex.imag != 0) return _false;
            else{
                if(car(o)->val.number.val.integer != cadr(o)->val.number.val.complex.real) return _false;
            }
        }
        if(cadr(o)->val.number.numtype == NUM_MINFTY || cadr(o)->val.number.numtype == NUM_PINFTY) return _false;
        break;
    case NUM_REAL:
        WARNING_MSG("ICI_REAL");
        TEST_CONDITION_ARGUMENT_EQ(o, "=");
        if(cadr(o)->val.number.numtype == NUM_UINTEGER){
            WARNING_MSG("Compare real with uinteger");
            if(car(o)->val.number.val.real  != cadr(o)->val.number.val.integer) return _false;
        }
        if(cadr(o)->val.number.numtype == NUM_REAL){
            WARNING_MSG("Compare real with real");
            if(car(o)->val.number.val.real  != cadr(o)->val.number.val.real) return _false;
        }
        if(cadr(o)->val.number.numtype == NUM_COMPLEX){
            WARNING_MSG("Compare real with complex");
            if( cadr(o)->val.number.val.complex.imag != 0){
                return _false;
            }
            else{
                if(car(o)->val.number.val.real != cadr(o)->val.number.val.complex.real){
                    return _false;
                }
            }
        }
        if(cadr(o)->val.number.numtype == NUM_MINFTY || cadr(o)->val.number.numtype == NUM_PINFTY) return _false;
        break;
    }
    o = cdr(o);
    if(list_length(o) == 1) return _true;
    goto restart;
}

object prim_smaller(object o){
    if (list_length(o) < 2){
        return _true;
    }
restart:
    if (is_Number(car(o)) == False){
        WARNING_MSG("Wrong type of arguments on \"<\"");
        return NULL;
    }
    switch (car(o)->val.number.numtype) {
    case NUM_PINFTY:
        TEST_CONDITION_ARGUMENT_EQ(o, "<");
        if(list_length(o) > 1) return _false;
        break;
    case NUM_MINFTY:
        TEST_CONDITION_ARGUMENT_EQ(o, "<");
        if(list_length(o) > 1) return _false;
        break;
    case NUM_COMPLEX:
        TEST_CONDITION_ARGUMENT_EQ(o, "<");
        WARNING_MSG("Wrong type of arguments on \"<\"");
        return NULL;
        break;
    case NUM_INTEGER:
        break;
    case NUM_UINTEGER:
         WARNING_MSG("ICI_UNITEGER");
         TEST_CONDITION_ARGUMENT_EQ(o, "<");
        if(cadr(o)->val.number.numtype == NUM_UINTEGER){
            WARNING_MSG("Compare uinteger with uniteger");
            if(car(o)->val.number.val.integer >= cadr(o)->val.number.val.integer) return _false;
        }
        if(cadr(o)->val.number.numtype == NUM_REAL){
            WARNING_MSG("Compare uinteger with real");
            if(car(o)->val.number.val.integer >= cadr(o)->val.number.val.real) return _false;
        }
        if(cadr(o)->val.number.numtype == NUM_MINFTY) return _false;
        if(cadr(o)->val.number.numtype == NUM_COMPLEX){
            WARNING_MSG("Wrong type of arguments on \"<\"");
            return NULL;
        }
        break;
    case NUM_REAL:
        WARNING_MSG("ICI_REAL");
        TEST_CONDITION_ARGUMENT_EQ(o, "<");
        if(cadr(o)->val.number.numtype == NUM_UINTEGER){
            WARNING_MSG("Compare real with uinteger");
            if(car(o)->val.number.val.real  >= cadr(o)->val.number.val.integer) return _false;
        }
        if(cadr(o)->val.number.numtype == NUM_REAL){
            WARNING_MSG("Compare real with real");
            if(car(o)->val.number.val.real  >= cadr(o)->val.number.val.real) return _false;
        }
        if(cadr(o)->val.number.numtype == NUM_MINFTY) return _false;
        if(cadr(o)->val.number.numtype == NUM_COMPLEX){
            WARNING_MSG("Wrong type of arguments on \"<\"");
            return NULL;
        }
        break;
    }
    o = cdr(o);
    if(list_length(o) == 1) return _true;
    goto restart;
}

object prim_larger(object o){
    if (list_length(o) < 2){
        return _true;
    }
restart:
    if (is_Number(car(o)) == False){
        WARNING_MSG("Wrong type of arguments on \">\"");
        return NULL;
    }
    switch (car(o)->val.number.numtype) {
    case NUM_PINFTY:
        if(list_length(o) > 1) return _false;
        break;
    case NUM_MINFTY:
        TEST_CONDITION_ARGUMENT_EQ(o, ">");
        if(list_length(o) > 1) return _false;
        break;
    case NUM_COMPLEX:
        TEST_CONDITION_ARGUMENT_EQ(o, ">");
        WARNING_MSG("Wrong type of arguments on \">\"");
        return NULL;
        break;
    case NUM_INTEGER:
        break;
    case NUM_UINTEGER:
         WARNING_MSG("ICI_UNITEGER");
         TEST_CONDITION_ARGUMENT_EQ(o, ">");
        if(cadr(o)->val.number.numtype == NUM_UINTEGER){
            WARNING_MSG("Compare uinteger with uniteger");
            if(car(o)->val.number.val.integer <= cadr(o)->val.number.val.integer) return _false;
        }
        if(cadr(o)->val.number.numtype == NUM_REAL){
            WARNING_MSG("Compare uinteger with real");
            if(car(o)->val.number.val.integer <= cadr(o)->val.number.val.real) return _false;
        }
        if(cadr(o)->val.number.numtype == NUM_PINFTY) return _false;
        if(cadr(o)->val.number.numtype == NUM_COMPLEX){
            WARNING_MSG("Wrong type of arguments on \">\"");
            return NULL;
        }
        break;
    case NUM_REAL:
        WARNING_MSG("ICI_REAL");
        TEST_CONDITION_ARGUMENT_EQ(o, ">");
        if(cadr(o)->val.number.numtype == NUM_UINTEGER){
            WARNING_MSG("Compare real with uinteger");
            if(car(o)->val.number.val.real  <= cadr(o)->val.number.val.integer) return _false;
        }
        if(cadr(o)->val.number.numtype == NUM_REAL){
            WARNING_MSG("Compare real with real");
            if(car(o)->val.number.val.real  <= cadr(o)->val.number.val.real) return _false;
        }
        if(cadr(o)->val.number.numtype == NUM_PINFTY) return _false;
        if(cadr(o)->val.number.numtype == NUM_COMPLEX){
            WARNING_MSG("Wrong type of arguments on \">\"");
            return NULL;
        }
        break;
    }
    o = cdr(o);
    if(list_length(o) == 1) return _true;
    goto restart;
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
        switch (o->val.number.numtype) {
        case NUM_PINFTY:
            return _true;

        case NUM_MINFTY:
            return _false;

        case NUM_UINTEGER:
        case NUM_INTEGER:
            return (o->val.number.val.integer > 0 ? _true : _false);

        case NUM_REAL:
            return (o->val.number.val.real > 0 ? _true : _false);

        case NUM_COMPLEX:
            WARNING_MSG("There is no ordering relation on the complex numbers");
            return NULL;

        default:
            WARNING_MSG("Wrong number type (%d)", o->val.number.numtype);
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
        switch (o->val.number.numtype) {
        case NUM_UINTEGER:
        case NUM_PINFTY:
            return _false;

        case NUM_MINFTY:
            return _true;

        case NUM_INTEGER:
            return (o->val.number.val.integer < 0 ? _true : _false);

        case NUM_REAL:
            return (o->val.number.val.real < 0 ? _true : _false);

        case NUM_COMPLEX:
            WARNING_MSG("There is no ordering relation on the complex numbers");
            return NULL;

        default:
            WARNING_MSG("Wrong number type (%d)", o->val.number.numtype);
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
        switch (o->val.number.numtype) {
        case NUM_PINFTY:
        case NUM_MINFTY:
            return _false;

        case NUM_INTEGER:
        case NUM_UINTEGER:
            return (o->val.number.val.integer == 0 ? _true : _false);

        case NUM_REAL:
            return (o->val.number.val.real == 0 ? _true : _false);

        case NUM_COMPLEX:
            return (o->val.number.val.complex.real == 0 &&
                    o->val.number.val.complex.imag == 0) ?
                   _true : _false;

        default:
            WARNING_MSG("Wrong number type (%d)", o->val.number.numtype);
            return NULL;
        }
    }
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
    return _false;/*faut corrriger*/
}

object prim_is_string(object o) {
    TEST_NUMB_ARGUMENT_EQ(1, "string?");
    if(is_Pair(o) == True && is_String(car(o)) == True) {
        return _true;
    }
    return _false;
}
/*
object prim_arith_plus(object o) {
    object res = make_object(SFS_NUMBER);
    res->val.number.numtype = NUM_INTEGER;
}*/
