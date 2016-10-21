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

typedef struct object_t {

    uint type;
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
        } primitive;

        struct {
            struct object_t* (*f)(struct object_t *);
        } form;
    } val;

} *object;

object make_object(uint type);
object make_pair(object car, object cdr);
object make_nil(void);
object make_true(void);
object make_false(void);
object make_primitive(object (*f)(object));
object make_form(object (*f)(object));
object make_symbol_table(void);
object make_symbol(string);
object make_string(string);
object make_number(uint type);

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
Bool is_AutoEvaluable(object o);
Bool is_Integer(object o);
Bool is_Real(object o);
#define SFS_NUMBER       0x00
#define SFS_CHARACTER    0x01
#define SFS_STRING       0x02
#define SFS_PAIR         0x03
#define SFS_NIL          0x04
#define SFS_BOOLEAN      0x05
#define SFS_SYMBOL       0x06
#define SFS_PRIMITIVE    0x07
#define SFS_FORM         0x08

extern object nil;
extern object _true;
extern object _false;
extern object symbol_table;

#ifdef __cplusplus
}
#endif

#endif /* _OBJECT_H_ */
