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

    if (is_Quote(input)) {
        input = cadr(input);
    }

    if (is_AutoEvaluable(input) == False) {
        goto restart;
    }
    return input;
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
    return is_Form("if", car(o));
}

Bool is_Define(object o) {
    return is_Form("define", car(o));
}

Bool is_Set(object o) {
    return is_Form("set!", car(o));
}

Bool is_Quote(object o) {
    return is_Form("quote", car(o));
}
