#include "forms.h"
#include "symbols.h"
#include <limits.h>

void create_basic_forms() {
    /* Those are the basic forms */
    _quote = make_symbol("quote");
    _if = make_symbol("if");
    _set = make_symbol("set!");
    _define = make_symbol("define");
}

void create_basic_primitives() {
    /* Those are the basic arithmetic primitives */
    define_symbol(make_symbol("+"), make_primitive(arith_Plus), 0);
}

Bool is_Boolean(object o) {
    if (o && o->type == SFS_BOOLEAN) {
        return True;
    }
    return False;
}

Bool is_Char(object o) {
    if (o && o->type == SFS_CHARACTER) {
        return True;
    }
    return False;
}

Bool is_Nil(object o) {
    if (o && o->type == SFS_NIL) {
        return True;
    }
    return False;
}

Bool is_Number(object o) {
    if (o && o->type == SFS_NUMBER) {
        return True;
    }
    return False;
}

Bool is_Pair(object o) {
    if (o && o->type == SFS_PAIR) {
        return True;
    }
    return False;
}

Bool is_String(object o) {
    if (o && o->type == SFS_STRING) {
        return True;
    }
    return False;
}

Bool is_Symbol(object o) {
    if (o && o->type == SFS_SYMBOL) {
        return True;
    }
    return False;
}

Bool is_Primitive(object o) {
    if (o && o->type == SFS_PRIMITIVE) {
        return True;
    }
    return False;
}

object arith_Plus(object o) {
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
                       res->val.number.val.integer < INT_MIN - car(o)->val.number.val.integer){
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
