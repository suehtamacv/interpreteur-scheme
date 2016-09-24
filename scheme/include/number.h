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

#define NUM_REAL     0x00
#define NUM_INTEGER  0x01
#define NUM_UINTEGER 0x02
#define NUM_COMPLEX  0x03
#define NUM_UNDEF    0x04
#define NUM_PINFTY   0x05
#define NUM_MINFTY   0x06

/**
 * @brief The complex_t struct is used to create the structure of a
 * complex number. A complex number is a pair of two values, its real
 * part and its imaginary part.
 */
typedef struct complex_t {
    /**
     * @brief real is the real part of the complex number.
     */
    double real;
    /**
     * @brief imag is the imaginary part of the complex number.
     */
    double imag;
} complex;

typedef struct num_t {

    uint numtype;
    union {
        double       real;
        int          integer;
        complex      complex;
    } val;

} num;

#ifdef __cplusplus
}
#endif

#endif /* _NUMBER_H_ */
