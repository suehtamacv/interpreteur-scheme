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
    create_primitive("boolean?", prim_is_boolean);
    create_primitive("null?", prim_is_null);
    create_primitive("string?", prim_is_string);
    create_primitive("integer?", prim_is_integer);
    create_primitive("pair?", prim_is_pair);
    create_primitive("symbol?", prim_is_symbol);
    create_primitive("char?", prim_is_char);
    create_primitive("real?", prim_is_real);
    create_primitive("procedure?", prim_is_procedure);

    /* Those are the basic list handling function */
    create_primitive("car", prim_car);
    create_primitive("cdr", prim_cdr);
    create_primitive("set-car!", prim_set_car);
    create_primitive("set-cdr!", prim_set_cdr);
    create_primitive("cons", prim_cons);
    create_primitive("list", prim_list);

    /* Those are the basic arithmetic primitives */
    //define_symbol(make_symbol("+"), make_primitive(prim_arith_plus), 0);
}

void create_primitive(string prim_name, object (*func)(object)) {
    define_symbol(make_symbol(prim_name), make_primitive(func, prim_name), 0);
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
