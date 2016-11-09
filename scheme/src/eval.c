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

object sfs_eval(object input, object env) {
restart:
    /* NULL pointer handling */
    if (!input) {
        return NULL;
    }

    if (is_AutoEvaluable(input) == True) {
        DEBUG_MSG("Evaluating auto-evaluable object");
        return input;
    } else if (is_Symbol(input) == True) {
        DEBUG_MSG("Resolving a symbol by searching for it in the symbol table");
        object *l_symb = locate_symbol(input, 0);
        if (l_symb == NULL) {
            WARNING_MSG("Unbound variable: %s", input->val.symbol);
            return NULL;
        } else {
            input = *l_symb;
        }
        goto restart;
    } else if (is_Pair(input) == True) {
        if (is_Symbol(car(input)) == False) {
            WARNING_MSG("Ill-formed expression: first list element "
                        "can not be resolved into a procedure");
            return NULL;
        }

        object *symb = locate_symbol(car(input), 0);
        if (!symb ||
                (is_Primitive(*symb) == False &&
                 is_Form(*symb) == False)) {
            WARNING_MSG("Procedure \"%s\" not found", car(input)->val.symbol);
            return NULL;
        }

        if ((*symb)->type == SFS_PRIMITIVE) {
            /* Must evaluate the arguments */
            object rev_eval_list = nil;

            input = cdr(input);
            while (is_Nil(input) == False) {
                if (is_Define(car(input)) == True) {
                    WARNING_MSG("Definitions not allowed in expression context");
                    return NULL;
                }

                object eval_element = sfs_eval(car(input), env);
                if (eval_element) {
                    rev_eval_list = cons(eval_element, rev_eval_list);
                } else {
                    return NULL;
                }
                input = cdr(input);
            }

            /* Calls the function */
            return (*symb)->val.primitive.f(reverse(rev_eval_list));
        } else if (((*symb)->type == SFS_FORM)) {
            /* Calls the function */
            return (*symb)->val.form.f(cdr(input), env);
        }

        goto restart;
    }

    goto restart;
}
