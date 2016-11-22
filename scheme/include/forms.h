#ifndef FORMS_H
#define FORMS_H

#include "object.h"

void create_basic_forms(object env);
void create_form(string, object (*)(object, object), object);

/* Those are the basic forms */
object form_and(object, object);
object form_or(object, object);
object form_define(object, object);
object form_quote(object, object);
object form_if(object, object);
object form_set(object, object);
object form_eval(object, object);
object form_lambda(object, object);
object form_interaction_environment(object, object);

/* Those are auxiliary functions */
uint list_length(object);

/* Those auxiliary functions verify if a certain form
 * is one of the special forms */
Bool is_If(object);
Bool is_Define(object);
Bool is_Set(object);
Bool is_Quote(object);
Bool is_And(object);
Bool is_Or(object);

#endif /* FORMS_H */
