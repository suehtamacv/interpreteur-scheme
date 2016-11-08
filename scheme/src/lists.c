#include "lists.h"

object reverse(object o) {
    if (is_Pair(o) == True || is_Nil(o) == True) {
        object eval_list = nil;

        /* Reverses the list */
        while (is_Nil(o) == False) {
            eval_list = cons(car(o), eval_list);
            o = cdr(o);
        }
        return eval_list;
    } else {
        WARNING_MSG("Can't reverse something that is neither a list nor nil");
        return NULL;
    }
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

object list(object a, object b) {
    return cons(a, cons(b, nil));
}
