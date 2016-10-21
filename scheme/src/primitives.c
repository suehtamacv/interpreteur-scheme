#include <primitives.h>
#include <forms.h>
#include <limits.h>
#include <symbols.h>
#include <lists.h>

void create_basic_primitives() {
    /* Those are the basic type comparison functions */
    create_primitive("boolean?", prim_is_boolean);

    /* Those are the basic arithmetic primitives */
    create_primitive("+", prim_arith_plus);
}

void create_primitive(string prim_name, object (*func)(object)) {
    define_symbol(make_symbol(prim_name), make_primitive(func, prim_name), 0);
}

object prim_is_boolean(object o) {
    if (is_Pair(o) == True && is_Boolean(car(o)) == True) {
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
