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
    } else if (is_Pair(in) == True) {
        if (is_Symbol(car(in)) == False) {
            WARNING_MSG("Ill-formed expression: first list element "
                        "can not be resolved into a primitive");
            return NULL;
        }

        object *l_symb = locate_symbol(car(in), 0);
        if (!l_symb ||
                (is_Primitive(*l_symb) == False &&
                 is_Form(*l_symb) == False)) {
            WARNING_MSG("Procedure \"%s\" not found", car(in)->val.symbol);
            return NULL;
        }

        if ((*l_symb)->type == SFS_PRIMITIVE) {
            return (*l_symb)->val.primitive.f(cdr(in));
        } else if (((*l_symb)->type == SFS_FORM)) {
            return (*l_symb)->val.form.f(cdr(in));
        }

        goto restart;
    }

    goto restart;
}
