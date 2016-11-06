#include "number.h"
#include "object.h"
#include <math.h>

object make_integer(int i) {
    object o = make_number(NUM_INTEGER);
    o->val.number.val.integer = i;
    return o;
}

object make_uinteger(int i) {
    if (i < 0) {
        return make_integer(i);
    }

    object o = make_number(NUM_UINTEGER);
    o->val.number.val.integer = i;
    return o;
}

object make_real(double r) {
    object o = make_number(NUM_REAL);
    o->val.number.val.real = r;
    return o;
}

object make_complex(double r, double i) {
    object o = make_number(NUM_COMPLEX);
    o->val.number.val.complex.real = r;
    o->val.number.val.complex.imag = i;
    return o;
}

object to_integer(object o) {
    if (is_Number(o) == False) {
        WARNING_MSG("Cannot convert something that is not a number to integer");
        return NULL;
    }

    switch (o->val.number.numtype) {
    case NUM_INTEGER:
    case NUM_UINTEGER:
        return o;

    case NUM_PINFTY:
    case NUM_MINFTY:
        WARNING_MSG("Cannot convert +inf or -inf to integer");
        return NULL;

    case NUM_REAL:
        if (fmod(o->val.number.val.real, 1.0) != 0) {
            WARNING_MSG("Cannot convert a real number to integer");
            return NULL;
        } else {
            return make_integer((long) o->val.number.val.real);
        }

    case NUM_COMPLEX:
        if (o->val.number.val.complex.imag != 0 ||
                fmod(o->val.number.val.complex.real, 1.0) != 0) {
            WARNING_MSG("Cannot convert a complex number to integer");
            return NULL;
        } else {
            return make_integer((long) o->val.number.val.complex.real);
        }

    case NUM_UNDEF:
        WARNING_MSG("Cannot convert NaN to integer");
        return NULL;
    }

    return NULL;
}

object to_real(object o) {
    if (is_Number(o) == False) {
        WARNING_MSG("Cannot convert something that is not a number to real");
        return NULL;
    }

    switch (o->val.number.numtype) {
    case NUM_REAL:
        return o;

    case NUM_INTEGER:
    case NUM_UINTEGER:
        return make_real(o->val.number.val.integer);

    case NUM_PINFTY:
    case NUM_MINFTY:
        WARNING_MSG("Cannot convert +inf or -inf to real");
        return NULL;

    case NUM_COMPLEX:
        if (o->val.number.val.complex.imag == 0) {
            return make_real(o->val.number.val.complex.real);
        } else {
            WARNING_MSG("Cannot convert a complex number to real");
            return NULL;
        }

    case NUM_UNDEF:
        WARNING_MSG("Cannot convert NaN to real");
        return NULL;
    }

    return NULL;
}

object to_complex(object o) {
    if (is_Number(o) == False) {
        WARNING_MSG("Cannot convert something that is not a number to real");
        return NULL;
    }

    switch (o->val.number.numtype) {
    case NUM_COMPLEX:
        return o;

    case NUM_REAL:
        return make_complex(o->val.number.val.real, 0);

    case NUM_INTEGER:
    case NUM_UINTEGER:
        return make_complex(o->val.number.val.integer, 0);

    case NUM_PINFTY:
    case NUM_MINFTY:
        WARNING_MSG("Cannot convert +inf or -inf to complex");
        return NULL;

    case NUM_UNDEF:
        WARNING_MSG("Cannot convert NaN to complex");
        return NULL;
    }

    return NULL;
}
