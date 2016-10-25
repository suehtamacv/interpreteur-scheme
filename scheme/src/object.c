/**
 * @file object.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:02:57 2012
 * @brief Object definitions for SFS.
 *
 * Object definitions for SFS.
 */

#include "object.h"
#include "forms.h"
#include "mem.h"
#include "number.h"


object make_object(uint type) {
    object t = sfs_malloc(sizeof(*t));
    t->type = type;
    return t;
}

object make_pair(object car, object cdr) {
    object p = make_object(SFS_PAIR);
    p->val.pair.car = car;
    p->val.pair.cdr = cdr;
    return p;
}

object make_nil(void) {
    object t = make_object(SFS_NIL);
    t->val.special = t;
    return t;
}

object make_true() {
    object t = make_object(SFS_BOOLEAN);
    t->val.boolean = True;
    return t;
}

object make_false() {
    object f = make_object(SFS_BOOLEAN);
    f->val.boolean = False;
    return f;
}

object make_primitive(object (*func)(object), string func_name) {
    object f = make_object(SFS_PRIMITIVE);
    f->val.primitive.f = func;
    strcpy(f->val.primitive.func_name, func_name);
    return f;
}

object make_form(object (*func)(object), string func_name) {
    object f = make_object(SFS_FORM);
    f->val.form.f = func;
    strcpy(f->val.primitive.func_name, func_name);
    return f;
}

object make_symbol(string s) {
    object o = make_object(SFS_SYMBOL);
    strncpy(o->val.symbol,  s, sizeof(o->val.symbol));
    return o;
}

object make_string(string s) {
    object o = make_object(SFS_STRING);
    strncpy(o->val.string,  s, sizeof(o->val.symbol));
    return o;
}

object make_number(uint type) {
    object o = make_object(SFS_NUMBER);
    o->val.number.numtype = type;
    return o;
}

object make_symbol_table() {
    object symb = make_object(SFS_PAIR);
    symb->val.pair.car = nil;
    symb->val.pair.cdr = nil;
    return symb;
}

Bool is_AutoEvaluable(object o) {
    return is_Boolean(o) ||
           is_Char(o) ||
           is_Nil(o) ||
           is_Number(o) ||
           is_String(o) ||
           is_Primitive(o) ||
           is_Form(o);
}

Bool is_False(object o) {
    if (is_Boolean(o) == True && o->val.boolean == False) {
        return True;
    }
    return False;
}

Bool is_True(object o) {
    if (is_False(o) == True) {
        return False;
    }
    return True;
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

Bool is_Complex(object o) {
    if (o && o->type == SFS_NUMBER) {
        switch (o->val.number.numtype) {
        case NUM_INTEGER:
        case NUM_UINTEGER:
        case NUM_REAL:
        case NUM_COMPLEX:
            return True;

        default:
            return False;
        }
    }
    return False;
}

Bool is_Integer(object o) {
    if (o && o->type == SFS_NUMBER) {
        switch (o->val.number.numtype) {
        case NUM_INTEGER:
        case NUM_UINTEGER:
            return True;

        default:
            return False;
        }
    }
    return False;
}

Bool is_Real(object o) {
    if (o && o->type == SFS_NUMBER) {
        switch (o->val.number.numtype) {
        case NUM_INTEGER:
        case NUM_UINTEGER:
        case NUM_REAL:
            return True;
            break;

        case NUM_COMPLEX:
            return (o->val.number.val.complex.imag == 0 ? True : False);
            break;

        default:
            return False;
        }
    }
    return False;
}

Bool is_List(object o) {
    if (!o && (is_Pair(o) == False && is_Nil(o) == False)) {
        return False;
    }

restart:
    if (!o || (is_Pair(o) == False && is_Nil(o) == False)) {
        return False;
    }

    if (is_Nil(o) == True) {
        return True;
    } else {
        o = o->val.pair.cdr;
        goto restart;
    }
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

Bool is_Form(object o) {
    if (o && o->type == SFS_FORM) {
        return True;
    }
    return False;
}
