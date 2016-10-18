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
#include "print.h"

object sfs_eval(object in) {
restart:
    /* NULL pointer handling */
    if (!in) {
        return NULL;
    }

    if (is_AutoEvaluable(in) == True) {
        DEBUG_MSG("Evaluating auto-evaluable object");
        return in;
    } else if (is_Symbol(in) == True) {
        DEBUG_MSG("Resolving a symbol by searching for it in the symbol table");
        object *l_symb = locate_symbol(in, 0);
        if (l_symb == NULL) {
            WARNING_MSG("Unbound variable: %s", in->val.symbol);
            return NULL;
        } else {
            in = *l_symb;
        }
        goto restart;
    }
       else if (is_Pair(in) == True) {
        if (is_Symbol(car(in)) == False) {
            WARNING_MSG("Ill-formed expression: first list element can not be resolved into a primitive");
            return NULL;
        }

        object *l_symb = locate_symbol(car(in), 0);
        if (!l_symb || is_Primitive(*l_symb) == False) {
            WARNING_MSG("Primitive \"%s\" not found", car(in)->val.symbol);
            return NULL;
        }
        in = (*l_symb)->val.primitive.f(cdr(in));

        goto restart;
    }

    if (is_Quote(in) == True) {
        if (list_length(in) != 2) {
            WARNING_MSG("Wrong number of arguments on \"quote\"");
            return NULL;
        }
        return cadr(in);
    } else if (is_If(in) == True) {
        if (list_length(in) != 4) {
            WARNING_MSG("Wrong number of arguments on \"if\"");
            return NULL;
        }
        in = cdr(in);
        object result = sfs_eval(car(in));

        if (result && is_True(result) == True) {
            in = cadr(in);
        } else if (result) {
            in = caddr(in);
        } else {
            return NULL;
        }

        /* Can't have a definition inside an IF */
        if (is_Define(in) == True) {
            WARNING_MSG("Definitions not allowed in expression context");
            return NULL;
        }
        goto restart;
    } else if (is_And(in) == True) {
        in = eval_And(cdr(in));
    } else if (is_Or(in) == True) {
        in = eval_Or(cdr(in));
    } else if (is_Define(in) == True) {
        if (list_length(in) != 3) {
            WARNING_MSG("Wrong number of arguments on \"define\"");
            return NULL;
        }

        int define_result = (is_Quote(caddr(in)) == True) ?
                            define_symbol(cadr(in), caddr(in), 0) :
                            define_symbol(cadr(in), sfs_eval(caddr(in)), 0);

        if (define_result == 0) {
            return cadr(in);    /* Define returns symbol itself */
        } else { /* Could not define */
            return NULL;
        }
    } else if (is_Set(in) == True) {
        if (list_length(in) != 3) {
            WARNING_MSG("Wrong number of arguments on \"set!\"");
            return NULL;
        }

        if (set_symbol(cadr(in), caddr(in), 0) == 0) { /* Success */
            return cadr(in);
        } else {
            return NULL;
        }
    }

    goto restart;
}

object eval_And(object o) {
    /* An and with one element is the element itself */
    if (list_length(o) == 1) {
        return car(o);
    }
    object result = _true;

restart:
    /* Liste finie */
    if (is_Nil(o) == True) {
        return result;
    }

    if (is_Define(car(o)) == True) {
        WARNING_MSG("Definitions not allowed in expression context");
        return NULL;
    }
    result = sfs_eval(car(o));

    /* Court circuit si on trouve un #f */
    if (is_True(result) == False) {
        return _false;
    }

    o = cdr(o);
    goto restart;
}

object eval_Or(object o) {
    /* An or with one element is the element itself */
    if (list_length(o) == 1) {
        return car(o);
    }

    object result = _false;

restart:
    /* Liste finie */
    if (is_Nil(o) == True) {
        return result;
    }

    if (is_Define(car(o)) == True) {
        WARNING_MSG("Definitions not allowed in expression context");
        return NULL;
    }
    result = sfs_eval(car(o));

    /* Court circuit si on trouve un #t */
    if (is_True(result) == True) {
        return result;
    }

    o = cdr(o);
    goto restart;
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
