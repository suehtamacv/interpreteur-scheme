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
#include "lists.h"
#include <strings.h>

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
                        "can not be resolved into a procedure");
            return NULL;
        }

        object *symb = locate_symbol(car(in), 0);
        if (!symb ||
                (is_Primitive(*symb) == False &&
                 is_Form(*symb) == False)) {
            WARNING_MSG("Procedure \"%s\" not found", car(in)->val.symbol);
            return NULL;
        }

        if ((*symb)->type == SFS_PRIMITIVE) {//faut corriger
            /* Must evaluate the arguments */
            object evaluated_list = nil;
            in = cdr(in);
            while (is_Nil(in) == False) {
                object eval_element = sfs_eval(car(in));
                if (eval_element) {
                    evaluated_list = cons(sfs_eval(car(in)), evaluated_list);
                } else {
                    return NULL;
                }
                in = cdr(in);
            }

            /* Calls the function */
            return (*symb)->val.primitive.f(evaluated_list);
        } else if (((*symb)->type == SFS_FORM)) {
            /* Calls the function */
            return (*symb)->val.form.f(cdr(in));
        }

        goto restart;
    }

    goto restart;
}
