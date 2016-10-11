#include "forms.h"
#include "symbols.h"

void create_basic_forms() {
    /* Those are the basic forms */
    _quote = make_object(SFS_SYMBOL);
    strncpy(_quote->val.symbol, "quote", sizeof(_quote->val.symbol));

    _if = make_object(SFS_SYMBOL);
    strncpy(_if->val.symbol, "if", sizeof(_if->val.symbol));

    _set = make_object(SFS_SYMBOL);
    strncpy(_set->val.symbol, "set!", sizeof(_set->val.symbol));

    _define = make_object(SFS_SYMBOL);
    strncpy(_define->val.symbol, "define", sizeof(_define->val.symbol));
}

void create_basic_primitives() {
    /* Those are the basic arithmetic primitives */
    object pr_Plus_Symb = make_object(SFS_SYMBOL);
    object pr_Plus = make_object(SFS_PRIMITIVE);

    pr_Plus->val.primitive.f = arith_Plus;
    strncpy(pr_Plus_Symb->val.symbol, "+", sizeof(pr_Plus_Symb->val.symbol));

    define_symbol(pr_Plus_Symb, pr_Plus, 0);
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

Bool is_Primitive(object o) {
    if (o->type == SFS_PRIMITIVE) {
        return True;
    }
    return False;
}

object arith_Plus(object o) {
    return o;
}
