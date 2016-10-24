#include "forms.h"
#include "symbols.h"
#include "eval.h"
#include "lists.h"
#include <limits.h>
#include <strings.h>

void create_basic_forms() {
    /* Create associations in the symbol table */
    create_form("and", form_and);
    create_form("or", form_or);
    create_form("define", form_define);
    create_form("quote", form_quote);
    create_form("if", form_if);
    create_form("set!", form_set);
}

void create_form(string form_name, object (*f)(object)) {
    define_symbol(make_symbol(form_name), make_form(f, form_name), 0);
}

object form_and(object o) {
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

object form_or(object o) {
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

object form_define(object o) {
    if (list_length(o) != 2) {
        WARNING_MSG("Wrong number of arguments on \"define\"");
        return NULL;
    }

    int define_result = (is_Quote(cadr(o)) == True) ?
                        define_symbol(car(o), cadr(o), 0) :
                        define_symbol(car(o), sfs_eval(cadr(o)), 0);

    if (define_result == 0) {
        return car(o);    /* Define returns symbol itself */
    } else {
        return NULL; /* Could not define */
    }
}

object form_quote(object o) {
    if (list_length(o) != 1) {
        WARNING_MSG("Wrong number of arguments on \"quote\"");
        return NULL;
    }
    return car(o);
}

object form_if(object o) {
    if (list_length(o) != 3) {
        WARNING_MSG("Wrong number of arguments on \"if\"");
        return NULL;
    }
    object result = sfs_eval(car(o));

    if (result && is_True(result) == True) {
        o = sfs_eval(cadr(o));
    } else if (result) {
        o = sfs_eval(caddr(o));
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

object form_set(object o) {
    if (list_length(o) != 2) {
        WARNING_MSG("Wrong number of arguments on \"set!\"");
        return NULL;
    }

    if (set_symbol(car(o), cadr(o), 0) == 0) { /* Success */
        return car(o);
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
