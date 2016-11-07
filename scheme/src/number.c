#include "number.h"
#include "object.h"
#include <math.h>

object make_integer(int i) {
    object o = make_number(NUM_INTEGER);
    o->val.number->val.integer = i;
    return o;
}

object make_uinteger(int i) {
    if (i < 0) {
        return make_integer(i);
    }

    object o = make_number(NUM_UINTEGER);
    o->val.number->val.integer = i;
    return o;
}

object make_real(double r) {
    object o = make_number(NUM_REAL);
    o->val.number->val.real = r;
    return o;
}

object make_complex(object r, object i) {
    object o = make_number(NUM_COMPLEX);

    if (!r || !i) {
        WARNING_MSG("Invalid objects");
        return NULL;
    } else if (r->type != SFS_NUMBER || i->type != SFS_NUMBER) {
        WARNING_MSG("The real and imaginary parts of a complex number must be numbers");
        return NULL;
    } else if (r->val.number->numtype == NUM_COMPLEX ||
               i->val.number->numtype == NUM_COMPLEX) {
        WARNING_MSG("The real and imaginary parts of a complex number are real numbers");
        return NULL;
    }

    o->val.number->val.complex->real = r;
    o->val.number->val.complex->imag = i;
    return o;
}

object to_integer(object o) {
    if (is_Number(o) == False) {
        WARNING_MSG("Cannot convert something that is not a number to integer");
        return NULL;
    }

    switch (o->val.number->numtype) {
    case NUM_INTEGER:
    case NUM_UINTEGER:
        return o;

    case NUM_PINFTY:
    case NUM_MINFTY:
        WARNING_MSG("Cannot convert +inf or -inf to integer");
        return NULL;

    case NUM_REAL:
        if (fmod(o->val.number->val.real, 1.0) != 0) {
            WARNING_MSG("Cannot convert a real number to integer");
            return NULL;
        } else {
            return make_integer((long) o->val.number->val.real);
        }

    case NUM_COMPLEX:
        if (is_Zero(imag_part(o->val.number)) == False) {
            WARNING_MSG("Cannot convert a complex number to integer");
            return NULL;
        } else {
            return to_integer(o->val.number->val.complex->real);
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

    switch (o->val.number->numtype) {
    case NUM_REAL:
        return o;

    case NUM_INTEGER:
    case NUM_UINTEGER:
        return make_real(o->val.number->val.integer);

    case NUM_PINFTY:
    case NUM_MINFTY:
        WARNING_MSG("Cannot convert +inf or -inf to real");
        return NULL;

    case NUM_COMPLEX:
        if (is_Zero(imag_part(o->val.number)) == True) {
            return make_real(real_part(o->val.number)->val.number->val.real);
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

    switch (o->val.number->numtype) {
    case NUM_COMPLEX:
        return o;

    case NUM_REAL:
    case NUM_INTEGER:
    case NUM_UINTEGER:
        return make_complex(o, make_number(0));

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

object real_part(number n) {
    if (!n) {
        return NULL;
    }

    object o = make_object(SFS_NUMBER);

    switch (n->numtype) {
    case NUM_MINFTY:
    case NUM_PINFTY:
    case NUM_INTEGER:
    case NUM_UINTEGER:
    case NUM_REAL:
    case NUM_UNDEF:
        o->val.number = n;
        return o;
        break;

    case NUM_COMPLEX:
        return n->val.complex->real;
        break;
    }
    return NULL;
}

object imag_part(number n) {
    if (!n) {
        return NULL;
    }

    switch (n->numtype) {
    case NUM_MINFTY:
    case NUM_PINFTY:
    case NUM_INTEGER:
    case NUM_UINTEGER:
    case NUM_REAL:
    case NUM_UNDEF:
        return make_integer(0);
        break;

    case NUM_COMPLEX:
        return n->val.complex->imag;
        break;
    }
    return NULL;
}
