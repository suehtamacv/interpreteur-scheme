#include "forms.h"
#include "symbols.h"
#include "eval.h"
#include "lists.h"
#include "print.h"
#include <limits.h>
#include <strings.h>
#include <primitives.h>

#define TEST_NUMB_ARGUMENT_EQ(n_Arg, nomFunction) \
    if (list_length(o) != n_Arg) { \
        WARNING_MSG("Wrong number of arguments on \"" nomFunction "\""); \
        return NULL;\
    }

void create_basic_forms(object env) {
    /* Create associations in the symbol table */
    create_form("and", form_and, env);
    create_form("or", form_or, env);
    create_form("define", form_define, env);
    create_form("quote", form_quote, env);
    create_form("if", form_if, env);
    create_form("set!", form_set, env);
    create_form("eval", form_eval, env);
    create_form("lambda", form_lambda, env);
    create_form("begin", form_begin, env);
    create_form("let", form_let, env);
    create_form("let*", form_let_star, env);
    create_form("letrec", form_letrec, env);
    create_form("cond", form_cond, env);
    create_form("interaction-environment", form_interaction_environment, env);
}

void create_form(string form_name, object (*f)(object, object), object env) {
    if (is_Environment(env) == False) {
        WARNING_MSG("Can't create a form into something who is not an environment");
    }
    define_symbol(make_symbol(form_name), make_form(f, form_name), &env);
}

object form_cond(object o, object env) {
    while (is_Nil(o) == False) {
        object curr_condition = car(o);
        if (is_List(curr_condition) == False) {
            WARNING_MSG("'cond' expects a list as its argument");
        } else if (list_length(curr_condition) != 2) {
            WARNING_MSG("'cond' expects a condition and then an instruction in each case");
            return NULL;
        }
        /* Found an else case */
        else if (is_Symbol(car(curr_condition)) == True &&
                 strcasecmp(car(curr_condition)->val.symbol,"else") == 0) {
            if (list_length(o) != 1) {
                WARNING_MSG("'else' condition should be the last in a 'cond'. Ignoring subsequent conditions");
            }
            return sfs_eval(cadr(curr_condition), env);
        } else if (is_True(sfs_eval(car(curr_condition), env)) == True) {
            return sfs_eval(cadr(curr_condition), env);
        }

        o = cdr(o);
    }

    return _void;
}

object form_let(object o, object env) {
    object parms = car(o);
    object run_env = create_env_layer(env);

    while (is_Nil(parms) == False) {
        object curr_parm = car(parms);
        if (list_length(curr_parm) != 2) {
            WARNING_MSG("Wrong let form, definition with more than two arguments found");
            return NULL;
        }

        /* Pour LET, l'evaluation se passe ici, avant de definir le variable */
        form_define(list(car(curr_parm), sfs_eval(cadr(curr_parm), env)), run_env);

        parms = cdr(parms);
    }

    object instruction_list = cdr(o);
    if (is_List(o) == False) {
        WARNING_MSG("The parameters of a lambda must be lists");
        return NULL;
    }

    object body = cons(make_symbol("begin"), nil);
    while (is_Nil(instruction_list) == False) {
        body = cons(car(instruction_list), body);
        instruction_list = cdr(instruction_list);
    }
    return sfs_eval(reverse(body), run_env);
}

object form_let_star(object o, object env) {
    object parms = car(o);
    object run_env = create_env_layer(env);

    while (is_Nil(parms) == False) {
        object curr_parm = car(parms);
        if (list_length(curr_parm) != 2) {
            WARNING_MSG("Wrong let* form, definition with more than two arguments found");
            return NULL;
        }

        /* Pour LET*, l'evaluation se passe plutôt dans la fonction define */
        form_define(list(car(curr_parm), cadr(curr_parm)), run_env);

        parms = cdr(parms);
    }

    object instruction_list = cdr(o);
    if (is_List(o) == False) {
        WARNING_MSG("The parameters of a lambda must be lists");
        return NULL;
    }

    object body = cons(make_symbol("begin"), nil);
    while (is_Nil(instruction_list) == False) {
        body = cons(car(instruction_list), body);
        instruction_list = cdr(instruction_list);
    }
    return sfs_eval(reverse(body), run_env);
}

object form_letrec(object o, object env) {
    object parms = car(o);
    object run_env = create_env_layer(env);

    /* Defining parameters as void */
    while (is_Nil(parms) == False) {
        object curr_parm = car(parms);
        form_define(list(car(curr_parm), _void), run_env);
        parms = cdr(parms);
    }

    parms = car(o);
    while (is_Nil(parms) == False) {
        object curr_parm = car(parms);
        if (list_length(curr_parm) != 2) {
            WARNING_MSG("Wrong letrec form, definition with more than two arguments found");
            return NULL;
        }

        /* In letrec we create an environment with the parameters defined
         * as void, and then redefine the variables in the resulting
         * environment */
        form_define(list(car(curr_parm), cadr(curr_parm)), run_env);
        parms = cdr(parms);
    }

    object instruction_list = cdr(o);
    if (is_List(o) == False) {
        WARNING_MSG("The parameters of a lambda must be lists");
        return NULL;
    }

    object body = cons(make_symbol("begin"), nil);
    while (is_Nil(instruction_list) == False) {
        body = cons(car(instruction_list), body);
        instruction_list = cdr(instruction_list);
    }
    return sfs_eval(reverse(body), run_env);
}

object form_begin(object o, object env) {
    object result = nil;

    while (is_Nil(o) == False) {
        result = sfs_eval(car(o), env);
        o = cdr(o);
    }

    return result;
}

object form_lambda(object o, object env) {
    object parms = car(o);

    /* Tests if the parameters are indeed symbols and if there are no repeated parameters */
    {
        object curr_parm = parms;
        if (is_Nil(curr_parm) == False && is_Symbol(car(curr_parm)) == False) {
            WARNING_MSG("Invalid type of parameter in lambda expression - symbol expected");
            return NULL;
        }
        while (is_Nil(curr_parm) == False) {
            object test_parm = cdr(curr_parm);
            while (is_Nil(test_parm) == False) {
                if (is_Symbol(car(test_parm)) == True &&
                        strcasecmp(car(curr_parm)->val.symbol, car(test_parm)->val.symbol) == 0) {
                    WARNING_MSG("Can not have repeated parameters in lambda expression");
                    return NULL;
                }
                test_parm = cdr(test_parm);
            }
            curr_parm = cdr(curr_parm);
        }
    }

    object instruction_list = cdr(o);
    if (is_List(o) == False) {
        WARNING_MSG("The parameters of a lambda must be lists");
        return NULL;
    }

    object body = cons(make_symbol("begin"), nil);
    while (is_Nil(instruction_list) == False) {
        body = cons(car(instruction_list), body);
        instruction_list = cdr(instruction_list);
    }
    return make_compound(parms, reverse(body), create_env_layer(env));
}

object form_interaction_environment(object o, object env) {
    TEST_NUMB_ARGUMENT_EQ(0, "interaction-environment");

    object environment = create_env_layer(env);
    create_basic_forms(environment);
    create_basic_primitives(environment);
    define_symbol(make_symbol("NaN"), NaN, &environment);

    return environment;
}

object form_eval(object o, object env) {
    TEST_NUMB_ARGUMENT_EQ(2, "eval");
    env = sfs_eval(cadr(o), env);
    return (env ? sfs_eval(sfs_eval(car(o), env), env) : NULL);
}

object form_and(object o, object env) {
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
    result = sfs_eval(car(o), env);

    /* Court circuit si on trouve un #f */
    if (is_True(result) == False) {
        return _false;
    }

    o = cdr(o);
    goto restart;
}

object form_or(object o, object env) {
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
    result = sfs_eval(car(o), env);

    /* Court circuit si on trouve un #t */
    if (is_True(result) == True) {
        return result;
    }

    o = cdr(o);
    goto restart;
}

object form_define(object o, object env) {
    object nom = car(o);
    object val = cdr(o);

    if (is_List(nom) == True) {
        object body = cons(make_symbol("begin"), nil);
        while (is_Nil(val) == False) {
            body = cons(car(val), body);
            val = cdr(val);
        }
        object c = make_compound(cdr(nom), reverse(body), create_env_layer(env));
        return (define_symbol(car(nom), c, &env) == 0 ? _void : NULL);
    } else {
        TEST_NUMB_ARGUMENT_EQ(2, "define");
        return (define_symbol(nom, sfs_eval(car(val), env), &env) == 0 ? _void : NULL);
    }
}

object form_quote(object o, object env) {
    TEST_NUMB_ARGUMENT_EQ(1, "quote");
    return (env ? car(o) : NULL);
}

object form_if(object o, object env) {
    if (list_length(o) < 2 || list_length(o) > 3) {
        WARNING_MSG("Wrong number of arguments on \"if\"");
        return NULL;
    }
    object result = sfs_eval(car(o), env);

    if (result && is_True(result) == True) {
        o = sfs_eval(cadr(o), env);
    } else if (result) {
        if (is_Pair(cddr(o)) == True) {
            o = sfs_eval(caddr(o), env);
        } else {
            return _void;
        }
    } else {
        return NULL;
    }

    /* Can't have a definition inside an IF */
    if (is_Define(o) == True) {
        WARNING_MSG("Definitions not allowed in expression context");
        return NULL;
    }
    return o;
}

object form_set(object o, object env) {
    TEST_NUMB_ARGUMENT_EQ(2, "set!");

    if (set_symbol(car(o), sfs_eval(cadr(o), env), env) == 0) { /* Success */
        return _void;
    } else {
        return NULL;
    }
}

Bool is_Form_Name(char *form, object o) {
    if (is_Symbol(o) == False) {
        return False;
    }

    if (strcasecmp(o->val.symbol, form) == 0) {
        return True;
    }
    return False;
}

Bool is_Define(object o) {
    if (is_Pair(o) == False) {
        return False;
    }
    return is_Form_Name("define", car(o));
}

Bool is_Set(object o) {
    if (is_Pair(o) == False) {
        return False;
    }
    return is_Form_Name("set!", car(o));
}

Bool is_Quote(object o) {
    if (is_Pair(o) == False) {
        return False;
    }
    return is_Form_Name("quote", car(o));
}

Bool is_And(object o) {
    if (is_Pair(o) == False) {
        return False;
    }
    return is_Form_Name("and", car(o));
}

Bool is_Or(object o) {
    if (is_Pair(o) == False) {
        return False;
    }
    return is_Form_Name("or", car(o));
}

uint list_length(object o) {
    uint length = 0;
    do {
        if (is_Pair(o) == True) {
            length++;
            o = cdr(o);
        }
        if (is_Nil(o) == True && is_Pair(o) == False) {
            break;
        }
    } while(1);

    return length;
}
