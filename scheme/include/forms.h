#ifndef FORMS_H
#define FORMS_H

#include "object.h"

void create_basic_forms(void);

/* Those are the basic forms */
object form_and(object);
object form_or(object);
object form_define(object);
object form_quote(object);
object form_if(object);
object form_set(object);

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
