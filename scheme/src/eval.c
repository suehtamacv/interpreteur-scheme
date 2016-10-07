/**
 * @file eval.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:11:30 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#include "eval.h"
#include <strings.h>

object sfs_eval(object input) {
restart:

    if (is_AutoEvaluable(input) == True) {
        return input;
    } else if (is_Symbol(input) == True) {
        object loc_symbol = locate_symbol(input->val.symbol, 0);
        if (loc_symbol == NULL) {
            ERROR_MSG("Symbol %s not found", input->val.symbol);
        } else {
            input = loc_symbol;
        }
        goto restart;
    }

    if (is_Quote(input) == True) {
        return cadr(input);
    } else if (is_If(input) == True) {
        input = cdr(input);
        if (is_True(sfs_eval(car(input))) == True) {
            input = cadr(input);
            goto restart;
        } else {
            input = caddr(input);
            goto restart;
        }
    } else if (is_And(input) == True) {
        input = eval_And(cdr(input));
    } else if (is_Or(input) == True) {
        input = eval_Or(cdr(input));
    } else if (is_Define(input) == True) {
        define_symbol(cadr(input)->val.symbol, caddr(input), 0);
        return NULL;
    } else if (is_Set(input) == True) {
        set_symbol(cadr(input)->val.symbol, caddr(input), 0);
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
