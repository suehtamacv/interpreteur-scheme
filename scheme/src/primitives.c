#include <primitives.h>
#include <forms.h>
#include <limits.h>
#include <symbols.h>
#include <lists.h>

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
    define_symbol(make_symbol("car"), make_primitive(prim_is_car), 0);


    //define_symbol(make_symbol(">"), make_primitive(prim_is_greater_than), 0);
    /* Those are the basic arithmetic primitives */
    define_symbol(make_symbol("+"), make_primitive(prim_arith_plus), 0);
}
/*
object prim_is_greater_than(object o){
restart:
    if (is_Nil(o) == True) {
        return _true;
    }
    if(is_Pair(o) != True || is_Number(car(o)) != True)
        WARNING_MSG("Wrong!");
    //else{
           //if(sfs_eval(car(o)) < sfs_eval(cadr(o)) )return _false;
     else{
         goto restart;
     }
}*/

object prim_is_car(object o){
    if (list_length(o) != 1) {
        WARNING_MSG("Wrong number of arguments on \"car\"");
        return NULL;
    }
   if(is_Pair(o)== True && is_Pair(car(o)) == True) {
       return car(car(o));
   }
   WARNING_MSG("Wrong type of arguments on \"car\"");
   return NULL;
}


object prim_is_real(object o){
    if (list_length(o) != 1) {
        WARNING_MSG("Wrong number of arguments on \"real?\"");
        return NULL;
    }
    if(is_Pair(o)== True && is_Real(car(o)) == True) return _true;
    return _false;
}


object prim_is_char(object o){
    if (list_length(o) != 1) {
        WARNING_MSG("Wrong number of arguments on \"Char\"");
        return NULL;
    }
    if(is_Pair(o)== True && is_Char(car(o)) == True) return _true;
    return _false;
}

object prim_is_symbol(object o){
    if (list_length(o) != 1) {
        WARNING_MSG("Wrong number of arguments on \"Symbol\"");
        return NULL;
    }
    if(is_Pair(o)== True && is_Symbol(car(o)) == True) return _true;
    return _false;
}

object prim_is_pair(object o){
    if (list_length(o) != 1) {
        WARNING_MSG("Wrong number of arguments on \"Pair\"");
        return NULL;
    }
    if(is_Pair(o)== True && is_Pair(car(o)) == True) return _true;
    return _false;
}

object prim_is_integer(object o){
    if (list_length(o) != 1) {
        WARNING_MSG("Wrong number of arguments on \"Integer\"");
        return NULL;
    }
    if(is_Pair(o)== True && is_Integer(car(o)) == True) return _true;
    return _false;
}
object prim_is_boolean(object o) {
    if (list_length(o) != 1) {
        WARNING_MSG("Wrong number of arguments on \"Boolean\"");
        return NULL;
    }
    if (is_Pair(o) == True && is_Boolean(car(o)) == True) return _true;
    return _false;
}

object prim_is_null(object o){
    if(is_Pair(o) == True && is_Nil(car(o))== True) return _true;
    return _false;/*faut corrriger*/
}

object prim_is_string(object o){
    if (list_length(o) != 1) {
        WARNING_MSG("Wrong number of arguments on \"String\"");
        return NULL;
       }
    if(is_Pair(o)== True && is_String(car(o))== True) return _true;
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
