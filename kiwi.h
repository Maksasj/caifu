#ifndef KIWI_H
#define KIWI_H

#include <stdint.h>

#ifndef KIWI_FRACTIONAL_BITS
    #define KIWI_FRACTIONAL_BITS 16
#endif

#ifndef KIWI_SCALE
    #define KIWI_SCALE (1 << KIWI_FRACTIONAL_BITS)
#endif

#define KIWI_INLINE static inline

typedef int32_t kiwi_fixed_t;

KIWI_INLINE kiwi_fixed_t kiwi_int_to_fixed(int32_t value);
KIWI_INLINE int32_t kiwi_fixed_to_int(kiwi_fixed_t value);

KIWI_INLINE kiwi_fixed_t kiwi_float_to_fixed(float value);
KIWI_INLINE float kiwi_fixed_to_float(kiwi_fixed_t value);

KIWI_INLINE kiwi_fixed_t kiwi_fixed_add(kiwi_fixed_t a, kiwi_fixed_t b);
KIWI_INLINE kiwi_fixed_t kiwi_fixed_sub(kiwi_fixed_t a, kiwi_fixed_t b);
KIWI_INLINE kiwi_fixed_t kiwi_fixed_mul(kiwi_fixed_t a, kiwi_fixed_t b);
KIWI_INLINE kiwi_fixed_t kiwi_fixed_div(kiwi_fixed_t a, kiwi_fixed_t b);

// Todo optimise
KIWI_INLINE kiwi_fixed_t kiwi_abs(kiwi_fixed_t v);

#ifdef PEACH_IMPLEMENTATION

KIWI_INLINE kiwi_fixed_t kiwi_int_to_fixed(int32_t value) {
    return value * KIWI_SCALE;
}

KIWI_INLINE int32_t kiwi_fixed_to_int(kiwi_fixed_t value) {
    return value / KIWI_SCALE;
}

KIWI_INLINE kiwi_fixed_t kiwi_float_to_fixed(float value) {
    return (kiwi_fixed_t)(value * KIWI_SCALE);
}

KIWI_INLINE float kiwi_fixed_to_float(kiwi_fixed_t value) {
    return (float)value / (float)KIWI_SCALE;
}

KIWI_INLINE kiwi_fixed_t kiwi_fixed_add(kiwi_fixed_t a, kiwi_fixed_t b) {
    return a + b;
}

KIWI_INLINE kiwi_fixed_t kiwi_fixed_sub(kiwi_fixed_t a, kiwi_fixed_t b) {
    return a - b;
}

KIWI_INLINE kiwi_fixed_t kiwi_fixed_mul(kiwi_fixed_t a, kiwi_fixed_t b) {
    return (kiwi_fixed_t)(((int64_t)a * b) >> KIWI_FRACTIONAL_BITS);
}

KIWI_INLINE kiwi_fixed_t kiwi_fixed_div(kiwi_fixed_t a, kiwi_fixed_t b) {
    return (kiwi_fixed_t)(((int64_t)a << KIWI_FRACTIONAL_BITS) / b);
}

// Todo optimise
KIWI_INLINE kiwi_fixed_t kiwi_abs(kiwi_fixed_t v) {
    return (v > 0 ? v : -v);
}

#endif

#endif