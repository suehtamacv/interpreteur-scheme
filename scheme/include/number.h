/**
 * @file number.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Sat Jun 23 00:42:34 2012
 * @brief Number handling.
 *
 * Number handling.
 */

#ifndef _NUMBER_H_
#define _NUMBER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "basic.h"

typedef enum number_type_t {
    NUM_REAL, NUM_INTEGER, NUM_UINTEGER, NUM_COMPLEX, NUM_UNDEF, NUM_PINFTY, NUM_MINFTY
} number_type;

typedef struct num_t *number;
typedef struct object_t *object;

/**
 * @brief The complex_t struct is used to create the structure of a
 * complex number. A complex number is a pair of two values, its real
 * part and its imaginary part.
 */
typedef struct complex_t {
    /**
     * @brief real is the real part of the complex number.
     */
    object real;
    /**
     * @brief imag is the imaginary part of the complex number.
     */
    object imag;
} *complex;

typedef struct num_t {

    number_type numtype;
    union {
        double       real;
        int          integer;
        complex      complex;
    } val;

} *num;


object make_integer(int);
object make_uinteger(int);
object make_real(double);
object make_complex(object, object);

object to_integer(object);
object to_real(object);
object to_complex(object);

object real_part(number);
object imag_part(number);
object num_abs(object);
object num_conj(object);

#ifdef __cplusplus
}
#endif

#endif /* _NUMBER_H_ */
