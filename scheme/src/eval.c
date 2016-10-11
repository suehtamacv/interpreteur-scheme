/**
 * @file eval.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:11:30 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#include "eval.h"
#include "forms.h"
#include <strings.h>

object sfs_eval(object in) {
restart:

    if (is_AutoEvaluable(in) == True) {
        return in;
    } else if (is_Symbol(in) == True) {
        object *l_symb = locate_symbol(in, 0);
        if (l_symb == NULL) {
            ERROR_MSG("Symbol %s not found", in->val.symbol);
        } else {
            in = *l_symb;
        }
        goto restart;
    } else if (is_Pair(in) == True) {
        if (is_Symbol(in) == False) {
            ERROR_MSG("Ill-formed expression: first list element is not a defined primitive");
        }

        object *l_symb = locate_symbol(in, 0);
        if (is_Primitive(*l_symb) == False) {
            ERROR_MSG("Primitive \"%s\" not found", in->val.symbol);
        }
    }

    if (is_Quote(in) == True) {
        return cadr(in);
    } else if (is_If(in) == True) {
        in = cdr(in);
        if (is_True(sfs_eval(car(in))) == True) {
            in = cadr(in);
            goto restart;
        } else {
            in = caddr(in);
            goto restart;
        }
    } else if (is_And(in) == True) {
        in = eval_And(cdr(in));
    } else if (is_Or(in) == True) {
        in = eval_Or(cdr(in));
    } else if (is_Define(in) == True) {
        define_symbol(cadr(in), sfs_eval(caddr(in)), 0);
        return NULL;
    } else if (is_Set(in) == True) {
        set_symbol(cadr(in), caddr(in), 0);
        return NULL;
    }

    goto restart;
}

object eval_And(object o) {
restart:

    /* Court circuit si on trouve un #f */
    if (is_True(sfs_eval(car(o))) == False) {
        return _false;
    }

    o = cdr(o);
    /* Liste pas encore finie */
    if (o != nil) {
        goto restart;
    }

    return _true;
}

object eval_Or(object o) {
restart:

    /* Court circuit si on trouve un #t */
    if (is_True(sfs_eval(car(o))) == True) {
        return _true;
    }

    o = cdr(o);
    /* Liste pas encore finie */
    if (o != nil) {
        goto restart;
    }

    return _false;
}

Bool is_Form(char *form, object o) {
    if (is_Symbol(o) == False) {
        return False;
    }

    if (strcasecmp(o->val.symbol, form) == 0) {
        return True;
    }
    return False;
}

Bool is_If(object o) {
    if (is_Pair(o) == False) {
        return False;
    }
    return is_Form("if", car(o));
}

Bool is_Define(object o) {
    if (is_Pair(o) == False) {
        return False;
    }
    return is_Form("define", car(o));
}

Bool is_Set(object o) {
    if (is_Pair(o) == False) {
        return False;
    }
    return is_Form("set!", car(o));
}

Bool is_Quote(object o) {
    if (is_Pair(o) == False) {
        return False;
    }
    return is_Form("quote", car(o));
}

Bool is_And(object o) {
    if (is_Pair(o) == False) {
        return False;
    }
    return is_Form("and", car(o));
}

Bool is_Or(object o) {
    if (is_Pair(o) == False) {
        return False;
    }
    return is_Form("or", car(o));
}
