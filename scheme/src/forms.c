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

uint list_length(object o) {
    uint length = 0;
    do {
        if (is_Pair(o) == True) {
            length++;
            o = cdr(o);
        }
        if (is_Nil(o) == True && is_Pair(o) == False) {
            break;
        }
    } while(1);

    return length;
}
