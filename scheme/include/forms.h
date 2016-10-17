#ifndef FORMS_H
#define FORMS_H

#include "object.h"

void create_basic_forms(void);
void create_basic_primitives(void);

/* The following functions are to verify the type of object */
Bool is_Number(object o);
Bool is_Char(object o);
Bool is_String(object o);
Bool is_Pair(object o);
Bool is_Nil(object o);
Bool is_Boolean(object o);
Bool is_Symbol(object o);
Bool is_Primitive(object o);

/* Those are basic arithmetic functions */
object arith_Plus(object);

/* Those are auxiliary functions */
uint list_length(object);


#endif /* FORMS_H */
