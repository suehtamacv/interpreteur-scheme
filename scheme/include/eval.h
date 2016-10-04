/**
 * @file eval.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:10:38 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#ifndef _EVAL_H_
#define _EVAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"

object sfs_eval(object);

/**
 * @brief is_Form is used to verify whether the object is equal to
 * a certain form.
 * @return True iff the object is equal to the form.
 */
Bool is_Form(char *, object);

Bool is_If(object);
Bool is_Define(object);
Bool is_Set(object);
Bool is_Quote(object);
Bool is_And(object);
Bool is_Or(object);

object eval_And(object);
object eval_Or(object);

#ifdef __cplusplus
}
#endif

#endif /* _EVAL_H_ */
