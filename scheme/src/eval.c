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
restart:
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
            input = *l_symb;
        }
        goto restart;
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
            object run_env;
            if (f->val.compound.env) {
                 run_env = create_env_layer(f->val.compound.env);
            } else {
                run_env = create_env_layer(env);
            }

            object cur_list = parms;
            object cur_vals = cdr(input);
            if (list_length(cur_list) != list_length(cur_vals)) {
                WARNING_MSG("Wrong number of arguments on lambda expression");
                return NULL;
            }

            while (is_Nil(cur_list) == False) {
                define_symbol(car(cur_list), car(cur_vals), &run_env);
                cur_list = cdr(cur_list);
                cur_vals = cdr(cur_vals);
            }

            sfs_print(run_env);
            printf("\n");

            return sfs_eval(f->val.compound.body, run_env);
        }

        goto restart;
    }

    goto restart;
}
