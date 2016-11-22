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
    create_form("interaction-environment", form_interaction_environment, env);
}

void create_form(string form_name, object (*f)(object, object), object env) {
    if (is_Environment(env) == False) {
        WARNING_MSG("Can't create a form into something who is not an environment");
    }
    define_symbol(make_symbol(form_name), make_form(f, form_name), &env);
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
    object inst_list = cdr(o);
    if (is_List(o) == False) {
        WARNING_MSG("The parameters of a lambda must be lists");
        return NULL;
    }

    object body = cons(make_symbol("begin"), nil);
    while (is_Nil(inst_list) == False) {
        body = cons(car(inst_list), body);
        inst_list = cdr(inst_list);
    }
    return make_compound(parms, reverse(body), env);
}

object form_interaction_environment(object o, object env) {
    TEST_NUMB_ARGUMENT_EQ(0, "interaction-environment");

    object environment = create_env_layer(env);
    create_basic_forms(environment);
    create_basic_primitives(environment);
    define_symbol(make_symbol("NaN"), NaN, &environment);
    environment = create_env_layer(environment);

    return environment;
}

object form_eval(object o, object env) {
    (void) env;
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
    TEST_NUMB_ARGUMENT_EQ(2, "define");

    object nom = car(o);
    object val = cadr(o);
    int define_result;

restart:
    if (is_Quote(val) == True) {
        define_result = define_symbol(nom, val, &env);
    } else if (is_Symbol(val) == True) {
        object *found = locate_symbol(val, env);
        if (found) {
            val = *found;
            goto restart;
        } else {
            WARNING_MSG("Can't define something to an unbound variable");
            return NULL;
        }
    } else if (is_List(nom) == True) {
        object c = make_compound(cdr(nom), val, env);
        define_result = define_symbol(car(nom), c, &env);
    } else {
        define_result = define_symbol(nom, sfs_eval(val, env), &env);
    }

    if (define_result == 0) {
        return _void;    /* Define returns void */
    } else {
        return NULL; /* Could not define */
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
