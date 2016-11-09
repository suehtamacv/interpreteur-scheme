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
#include <math.h>

object make_object(object_type type) {
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

object make_form(object (*func)(object,object), string func_name) {
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

object make_character (char i){
    object o = make_object(SFS_CHARACTER);
    o->val.character = i;
    return o;
}

object make_number(uint type) {
    object o = make_object(SFS_NUMBER);
    o->val.number = sfs_malloc(sizeof(*o->val.number));
    o->val.number->numtype = type;

    if (type == NUM_COMPLEX) {
        o->val.number->val.complex = sfs_malloc(sizeof(*o->val.number->val.complex));
    }

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
    if (o && o == nil) {
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
        switch (o->val.number->numtype) {
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

Bool is_Zero(object o) {
    if (o && is_Number(o) == True) {
        switch (o->val.number->numtype) {
        case NUM_UNDEF:
        case NUM_PINFTY:
        case NUM_MINFTY:
            return False;

        case NUM_INTEGER:
        case NUM_UINTEGER:
            return (o->val.number->val.integer == 0 ? True : False);

        case NUM_REAL:
            return (o->val.number->val.real == 0 ? True : False);

        case NUM_COMPLEX:
            return (is_Zero(real_part(o->val.number)) == True &&
                    is_Zero(imag_part(o->val.number)) == True) ?
                   True : False;
        }
    }
    return False;
}

Bool is_Positive(object o) {
    if (!o) {
        WARNING_MSG("Invalid object");
        return -1;
    }
    if (o->type != SFS_NUMBER) {
        WARNING_MSG("Only numbers can be said to be positive");
        return -1;
    }

    switch (o->val.number->numtype) {
    case NUM_PINFTY:
        return True;

    case NUM_MINFTY:
        return False;

    case NUM_UINTEGER:
    case NUM_INTEGER:
        return (o->val.number->val.integer > 0 ? True : False);

    case NUM_REAL:
        return (o->val.number->val.real > 0 ? True : False);

    case NUM_COMPLEX:
        WARNING_MSG("There is no ordering relation on the complex numbers");
        return -1;

    default:
        WARNING_MSG("Wrong number type (%d)", o->val.number->numtype);
        return -1;
    }
}

Bool is_Negative(object o) {
    if (is_Zero(o) == True) {
        return False;
    }
    if (is_Positive(o) == True) {
        return False;
    }

    return (is_Zero(o) != (unsigned) - 1 &&
            is_Positive(o) != (unsigned) - 1 ? True : (unsigned) - 1);
}

Bool is_Integer(object o) {
    if (o && o->type == SFS_NUMBER) {
        switch (o->val.number->numtype) {
        case NUM_INTEGER:
        case NUM_UINTEGER:
            return True;

        case NUM_REAL:
            return (fmod(o->val.number->val.real, 1.0) == 0 ? True : False);

        case NUM_COMPLEX:
            if (is_Zero(imag_part(o->val.number)) == False) {
                return False;
            } else {
                switch (real_part(o->val.number)->val.number->numtype) {
                case NUM_PINFTY:
                case NUM_MINFTY:
                case NUM_COMPLEX:
                case NUM_UNDEF:
                    return False;
                    break;

                case NUM_INTEGER:
                case NUM_UINTEGER:
                    return True;
                    break;

                case NUM_REAL:
                    return (fmod(real_part(o->val.number)->val.number->val.real,
                                 1.0) == 0 ? True : False);
                    break;
                }
            }

        default:
            return False;
        }
    }
    return False;
}

Bool is_Real(object o) {
    if (o && o->type == SFS_NUMBER) {
        switch (o->val.number->numtype) {
        case NUM_INTEGER:
        case NUM_UINTEGER:
        case NUM_REAL:
            return True;
            break;

        case NUM_COMPLEX:
            return is_Zero(imag_part(o->val.number));
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
