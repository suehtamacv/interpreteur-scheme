/**
 * @file object.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 17:49:46 2012
 * @brief Object definition for SFS.
 *
 * Object definition for SFS.
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "boolean.h"
#include "number.h"

typedef enum object_type_t {
    SFS_NUMBER, SFS_CHARACTER, SFS_STRING, SFS_PAIR, SFS_NIL, SFS_BOOLEAN, SFS_SYMBOL, SFS_PRIMITIVE, SFS_FORM, SFS_ENV, SFS_COMPOUND
} object_type;

typedef struct object_t {

    object_type type;
    union {
        num              number;
        Bool             boolean;
        char             character;
        string           string;
        string           symbol;
        struct object_t *special;

        struct pair_t {
            struct object_t *car;
            struct object_t *cdr;
        } pair;

        struct {
            struct object_t* (*f)(struct object_t *);
            string func_name;
        } primitive;

        struct {
            struct object_t* (*f)(struct object_t *, struct object_t *);
            string func_name;
        } form;

        struct {
            struct object_t* parms;
            struct object_t* body;
            struct object_t* env;
        } compound;
    } val;

} *object;

object make_object(object_type type);
object make_pair(object car, object cdr);
object make_nil(void);
object make_true(void);
object make_false(void);
object make_primitive(object (*f)(object), string func_name);
object make_form(object (*f)(object, object), string func_name);
object make_env_list(void);
object make_symbol(string);
object make_string(string);
object make_number(uint type);
object make_character(char);
object make_integer(int);
object make_compound(object parms, object body, object env);


/* The following functions are to verify the type of object */
Bool is_True(object o);
Bool is_False(object o);
Bool is_Number(object o);
Bool is_Char(object o);
Bool is_String(object o);
Bool is_Pair(object o);
Bool is_Nil(object o);
Bool is_Boolean(object o);
Bool is_Symbol(object o);
Bool is_Primitive(object o);
Bool is_Form(object o);
Bool is_Integer(object o);
Bool is_Real(object o);
Bool is_List(object o);
Bool is_Complex(object o);
Bool is_Void(object o);
Bool is_Zero(object o);
Bool is_Positive(object o);
Bool is_Negative(object o);
Bool is_Environment(object o);

Bool is_AutoEvaluable(object o);

extern object nil;
extern object _void;
extern object _true;
extern object _false;
extern object master_environment;
extern object plus_inf;
extern object minus_inf;
extern object NaN;

#ifdef __cplusplus
}
#endif

#endif /* _OBJECT_H_ */
