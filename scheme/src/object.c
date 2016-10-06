
/**
 * @file object.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:02:57 2012
 * @brief Object definitions for SFS.
 *
 * Object definitions for SFS.
 */

#include "object.h"
#include "mem.h"


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

object make_symbol_table() {
    object symb = make_object(SFS_PAIR);
    symb->val.pair.car = nil;
    symb->val.pair.cdr = nil;
    return symb;
}

void make_forms() {
    _quote = make_object(SFS_SYMBOL);
    strncpy(_quote->val.symbol, "quote", sizeof(_quote->val.symbol));

    _if = make_object(SFS_SYMBOL);
    strncpy(_if->val.symbol, "if", sizeof(_if->val.symbol));

    _set = make_object(SFS_SYMBOL);
    strncpy(_set->val.symbol, "set!", sizeof(_set->val.symbol));

    _define = make_object(SFS_SYMBOL);
    strncpy(_define->val.symbol, "define", sizeof(_define->val.symbol));
}

object car(object o) {
    if (is_Pair(o) == False) {
        ERROR_MSG("Trying to get car of a object who is not a pair (actually it is %d)",
                  o->type);
    }

    return o->val.pair.car;
}

object cdr(object o) {
    /* La fin de la liste */
    if (is_Nil(o) == True) {
        return nil;
    }

    if (is_Pair(o) == False) {
        ERROR_MSG("Trying to get cdr of a object who is not a pair (actually it is %d)",
                  o->type);
    }

    return o->val.pair.cdr;
}

object cadr(object o) {
    return car(cdr(o));
}

object caddr(object o) {
    return car(cdr(cdr(o)));
}

Bool is_Boolean(object o) {
    if (o->type == SFS_BOOLEAN) {
        return True;
    }
    return False;
}

Bool is_Char(object o) {
    if (o->type == SFS_CHARACTER) {
        return True;
    }
    return False;
}

Bool is_Nil(object o) {
    if (o->type == SFS_NIL) {
        return True;
    }
    return False;
}

Bool is_Number(object o) {
    if (o->type == SFS_NUMBER) {
        return True;
    }
    return False;
}

Bool is_Pair(object o) {
    if (o->type == SFS_PAIR) {
        return True;
    }
    return False;
}

Bool is_String(object o) {
    if (o->type == SFS_STRING) {
        return True;
    }
    return False;
}

Bool is_Symbol(object o) {
    if (o->type == SFS_SYMBOL) {
        return True;
    }
    return False;
}

Bool is_AutoEvaluable(object o) {
    if (is_Boolean(o)) {
        return True;
    }
    if (is_Char(o)) {
        return True;
    }
    if (is_Nil(o)) {
        return True;
    }
    if (is_Number(o)) {
        return True;
    }
    if (is_String(o)) {
        return True;
    }

    return False;
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
