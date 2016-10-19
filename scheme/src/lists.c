#include "lists.h"

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

object cddr(object o) {
    object obj_temp = cdr(o);
    if (obj_temp) {
        return cdr(obj_temp);
    } else {
        return NULL;
    }
}

object caddr(object o) {
    return car(cddr(o));
}

object cons(object o, object list) {
    return make_pair(o, list);
}
