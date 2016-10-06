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

        struct pair_t {
            struct object_t *car;
            struct object_t *cdr;
        } pair;

        struct object_t *special;
    } val;

} *object;

object make_object(uint type);
object make_pair(object car, object cdr);
object make_nil(void);
object make_true(void);
object make_false(void);
object make_symbol_table(void);
void make_forms(void);

/**
 * @brief car returns the car of the object o, given that it is a pair.
 * @return the car of the pair.
 */
object car(object o);
/**
 * @brief cdr returns the cdr of the object o, given that it is a pair.
 * @return the cdr of the pair.
 */
object cdr(object o);
/**
 * @brief cadr returns the car of the cdr of the object o, i.e., the next
 * object in the list, given that it is a pair.
 * @return the car of the cdr of the pair.
 */
object cadr(object o);
/**
 * @brief caddr returns the car of the cdr of the cdr of the object o,
 * i.e., the second next object in the list, given that it is a pair.
 * @return the car of the cdr of the cdr of the pair.
 */
object caddr(object o);

/* The following functions are to verify the type of object */
Bool is_Number(object o);
Bool is_Char(object o);
Bool is_String(object o);
Bool is_Pair(object o);
Bool is_Nil(object o);
Bool is_Boolean(object o);
Bool is_Symbol(object o);
Bool is_True(object o);
Bool is_False(object o);
Bool is_AutoEvaluable(object o);

#define SFS_NUMBER       0x00
#define SFS_CHARACTER    0x01
#define SFS_STRING       0x02
#define SFS_PAIR         0x03
#define SFS_NIL          0x04
#define SFS_BOOLEAN      0x05
#define SFS_SYMBOL       0x06

extern object nil;
extern object _true;
extern object _false;
extern object _quote;
extern object _if;
extern object _define;
extern object _set;
extern object symbol_table;

#ifdef __cplusplus
}
#endif

#endif /* _OBJECT_H_ */
