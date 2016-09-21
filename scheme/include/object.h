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
object make_nil(void);
object make_true(void);
object make_false(void);

#define SFS_NUMBER       0x00
#define SFS_CHARACTER    0x01
#define SFS_STRING       0x02
#define SFS_PAIR         0x03
#define SFS_NIL          0x04
#define SFS_BOOLEAN      0x05
#define SFS_SYMBOL       0x06


extern object nil;
extern object true;
extern object false;

#ifdef __cplusplus
}
#endif

#endif /* _OBJECT_H_ */
