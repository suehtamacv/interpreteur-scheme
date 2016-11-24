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
#include <print.h>
#include <strings.h>

object sfs_eval(object input, object env) {
    /* NULL pointer handling */
    if (!input) {
        return NULL;
    }

    if (is_AutoEvaluable(input) == True) {
        DEBUG_MSG("Evaluating auto-evaluable object");
        return input;
    } else if (is_Symbol(input) == True) {
        DEBUG_MSG("Resolving symbol \"%s\" by searching for it in the symbol table",
                  input->val.symbol);
        object *l_symb = locate_symbol(input, env);
        if (l_symb == NULL) {
            WARNING_MSG("Unbound variable: %s", input->val.symbol);
            return NULL;
        } else {
            return *l_symb;
        }
    } else if (is_Pair(input) == True) {
        object f = sfs_eval(car(input), env);
        if (is_Form(f) == False && is_Primitive(f) == False) {
            WARNING_MSG("Ill-formed expression: first list element "
                        "can not be resolved into a procedure");
            return NULL;
        }

        if (f->type == SFS_PRIMITIVE) {
            DEBUG_MSG("Evaluating primitive \"%s\"", f->val.primitive.func_name);
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
            return f->val.primitive.f(reverse(rev_eval_list));
        } else if (f->type == SFS_FORM) {
            DEBUG_MSG("Evaluating form \"%s\"", f->val.form.func_name);
            /* Calls the function */
            return f->val.form.f(cdr(input), env);
        } else if (f->type == SFS_COMPOUND) {
            object parms = f->val.compound.parms;
            object run_env = f->val.compound.env;

            object curr_parm = parms;
            object arguments = cdr(input);
            if (list_length(curr_parm) != list_length(arguments)) {
                WARNING_MSG("Wrong number of arguments on lambda expression");
                return NULL;
            }

            while (is_Nil(curr_parm) == False) {
                /* Evaluates argument in current environment */
                object val = list(make_symbol("quote"), sfs_eval(car(arguments), env));
                form_define(list(car(curr_parm), val), run_env);
                curr_parm = cdr(curr_parm);
                arguments = cdr(arguments);
            }

            return sfs_eval(f->val.compound.body, run_env);
        }
    }

    WARNING_MSG("Could not evaluate");
    return NULL;
}
