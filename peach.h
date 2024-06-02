#ifndef PEACH_H
#define PEACH_H

#ifndef PEACH_POWF
    #include <math.h>
    #define PEACH_POWF powf
#endif

#if !defined(PEACH_MALLOC) && !defined(PEACH_FREE)
    #include <stdlib.h>

    #define PEACH_MALLOC malloc
    #define PEACH_FREE free
#endif

#ifndef PEACH_ASSERT
    #include <assert.h>

    #define PEACH_ASSERT assert
#endif

#ifndef PEACH_PRINT
    #include <stdio.h>

    #define PEACH_PRINT printf
#endif

#ifndef PEACH_RAND
    #include <stdlib.h>

    #define PEACH_RAND rand
    #define PEACH_RAND_MAX RAND_MAX 
#endif

typedef unsigned char peach_u8_t;
typedef unsigned short peach_u16_t;
typedef unsigned int peach_u32_t;
typedef unsigned long long peach_u64_t;

typedef signed char peach_i8_t;
typedef signed short peach_i16_t;
typedef signed int peach_i32_t;
typedef signed long long peach_i64_t;

#ifndef PEACH_SIZE_T
    #define PEACH_SIZE_T peach_u64_t
#endif

typedef PEACH_SIZE_T peach_size_t;
typedef float peach_float_t;

#ifndef PEACH_INLINE
    #define PEACH_INLINE static inline
#endif

#define PEACH_EULER_NUMBER 2.718281f
#define PEACH_NULL ((void*) 0)

#define PEACH_MULTIPLY(A, B) (A) * (B)
#define PEACH_MATRIX_AT(M, ROW, COL) ((M)->value[(ROW) * M->cols + (COL)])
#define PEACH_MATRIX_ROW(M, ROW) (&(M)->value[(ROW) * M->cols])

#define PEACH_RANDOM_FLOAT (PEACH_RAND() / (peach_float_t) PEACH_RAND_MAX)

typedef struct peach_matrix_t {
    peach_size_t rows;
    peach_size_t cols;

    peach_float_t* value;
} peach_matrix_t;

PEACH_INLINE peach_float_t peach_sigmoid(peach_float_t n);
PEACH_INLINE peach_float_t peach_relu(peach_float_t n);

PEACH_INLINE peach_matrix_t* paech_new_matrix(peach_size_t rows, peach_size_t cols);
PEACH_INLINE peach_matrix_t* paech_new_matrix_square(peach_size_t size);
PEACH_INLINE peach_matrix_t* paech_new_matrix_random(peach_size_t rows, peach_size_t cols, peach_float_t min, peach_float_t max);

PEACH_INLINE peach_matrix_t* paech_copy_matrix(peach_matrix_t* src);
PEACH_INLINE peach_matrix_t* paech_copy_matrix_empty(peach_matrix_t* src);

PEACH_INLINE void peach_free_matrix(peach_matrix_t* matrix);

PEACH_INLINE peach_size_t peach_get_matrix_rows(peach_matrix_t* matrix);
PEACH_INLINE peach_size_t peach_get_matrix_cols(peach_matrix_t* matrix);
PEACH_INLINE peach_float_t* peach_get_matrix_values(peach_matrix_t* matrix);

PEACH_INLINE void peach_matrix_fill(peach_matrix_t* target, peach_float_t value);
PEACH_INLINE void peach_matrix_rand(peach_matrix_t* target, peach_float_t min, peach_float_t max);
PEACH_INLINE void peach_matrix_fill_values(peach_matrix_t* dst, peach_float_t* values);
PEACH_INLINE void peach_matrix_copy_content_target(peach_matrix_t* dst, peach_matrix_t* src);
PEACH_INLINE void peach_matrix_scale(peach_matrix_t* target, peach_float_t value);

PEACH_INLINE peach_matrix_t* peach_matrix_sum(peach_matrix_t* a, peach_matrix_t* b);
PEACH_INLINE peach_matrix_t* peach_matrix_sub(peach_matrix_t* a, peach_matrix_t* b);
PEACH_INLINE peach_matrix_t* peach_matrix_mul(peach_matrix_t* a, peach_matrix_t* b);
PEACH_INLINE peach_matrix_t* peach_matrix_div(peach_matrix_t* a, peach_matrix_t* b);

PEACH_INLINE peach_matrix_t* peach_matrix_dot(peach_matrix_t* a, peach_matrix_t* b);

PEACH_INLINE void peach_matrix_sum_target(peach_matrix_t* target, peach_matrix_t* b);
PEACH_INLINE void peach_matrix_sub_target(peach_matrix_t* target, peach_matrix_t* b);
PEACH_INLINE void peach_matrix_mul_target(peach_matrix_t* target, peach_matrix_t* b);
PEACH_INLINE void peach_matrix_div_target(peach_matrix_t* target, peach_matrix_t* b);

PEACH_INLINE void peach_matrix_dot_target(peach_matrix_t* target, peach_matrix_t* a, peach_matrix_t* b);

PEACH_INLINE void peach_matrix_apply_sigmoid(peach_matrix_t* target);
PEACH_INLINE void peach_matrix_apply_relu(peach_matrix_t* target);

PEACH_INLINE void peach_matrix_print(peach_matrix_t* m);

#ifdef PEACH_IMPLEMENTATION

PEACH_INLINE peach_float_t peach_sigmoid(peach_float_t n) {
    return (1.0f / (1 + PEACH_POWF(PEACH_EULER_NUMBER, -n)));
}

PEACH_INLINE peach_float_t peach_relu(peach_float_t n) {
    if(n < 0)
        return 0;

    return n;
}

PEACH_INLINE peach_matrix_t* paech_new_matrix(peach_size_t rows, peach_size_t cols) {
    const peach_size_t size = rows * cols * sizeof(peach_float_t);

    peach_matrix_t* matrix = (peach_matrix_t*) PEACH_MALLOC(sizeof(peach_matrix_t));
    
    matrix->rows = rows;
    matrix->cols = cols;
    
    matrix->value = PEACH_MALLOC(size);

    for(peach_size_t i = 0; i < rows*cols; ++i)
        matrix->value[i] = 0.0f;

    return matrix;
}

PEACH_INLINE peach_matrix_t* paech_new_matrix_square(peach_size_t size) {
    return paech_new_matrix(size, size);
} 

PEACH_INLINE peach_matrix_t* paech_new_matrix_random(peach_size_t rows, peach_size_t cols, peach_float_t min, peach_float_t max) {
    peach_matrix_t* matrix = paech_new_matrix(rows, cols);
    peach_matrix_rand(matrix, min, max);
    return matrix;
}

PEACH_INLINE peach_matrix_t* paech_copy_matrix(peach_matrix_t* src) {
    const peach_size_t rows = src->rows;
    const peach_size_t cols = src->cols;

    peach_matrix_t* matrix = paech_new_matrix(rows, cols);
    peach_matrix_copy_content_target(matrix, src);

    return matrix;
}

PEACH_INLINE peach_matrix_t* paech_copy_matrix_empty(peach_matrix_t* src) {
    const peach_size_t rows = src->rows;
    const peach_size_t cols = src->cols;

    peach_matrix_t* matrix = paech_new_matrix(rows, cols);
    peach_matrix_fill(matrix, 0);

    return matrix;
}

PEACH_INLINE void peach_free_matrix(peach_matrix_t* matrix) {
    PEACH_FREE(matrix->value);
    PEACH_FREE(matrix);
}

PEACH_INLINE peach_size_t peach_get_matrix_rows(peach_matrix_t* matrix) {
    return matrix->rows;
}

PEACH_INLINE peach_size_t peach_get_matrix_cols(peach_matrix_t* matrix) {
    return matrix->cols;
}

PEACH_INLINE peach_float_t* peach_get_matrix_values(peach_matrix_t* matrix) {
    return matrix->value;
}

PEACH_INLINE void peach_matrix_fill(peach_matrix_t* target, peach_float_t value) {
    const peach_size_t rows = target->rows;
    const peach_size_t cols = target->cols;

    for (peach_size_t i = 0; i < rows; ++i) {
        for (peach_size_t j = 0; j < cols; ++j) {
            PEACH_MATRIX_AT(target, i, j) = value;
        }
    }
}

PEACH_INLINE void peach_matrix_rand(peach_matrix_t* target, peach_float_t min, peach_float_t max) {
    const peach_size_t rows = target->rows;
    const peach_size_t cols = target->cols;

    for (peach_size_t i = 0; i < rows; ++i) {
        for (peach_size_t j = 0; j < cols; ++j) {
            PEACH_MATRIX_AT(target, i, j) = min + PEACH_MULTIPLY(PEACH_RANDOM_FLOAT, (max - min));;
        }
    }
}

PEACH_INLINE void peach_matrix_fill_values(peach_matrix_t* dst, peach_float_t* values) {
    const peach_size_t size = dst->rows * dst->cols;

    for(peach_size_t i = 0; i < size; ++i)
        dst->value[i] = values[i];
}

PEACH_INLINE void peach_matrix_copy_content_target(peach_matrix_t* dst, peach_matrix_t* src) {
    PEACH_ASSERT(dst->rows == src->rows);
    PEACH_ASSERT(dst->cols == src->cols);

    peach_matrix_fill_values(dst, src->value);
}

PEACH_INLINE void peach_matrix_scale(peach_matrix_t* target, peach_float_t value) {
    const peach_size_t rows = target->rows;
    const peach_size_t cols = target->cols;

    for (peach_size_t i = 0; i < rows; ++i)
        for (peach_size_t j = 0; j < cols; ++j)
            PEACH_MATRIX_AT(target, i, j) *= value;
}

PEACH_INLINE peach_matrix_t* peach_matrix_sum(peach_matrix_t* a, peach_matrix_t* b) {
    PEACH_ASSERT(a->rows == b->rows);
    PEACH_ASSERT(a->cols == b->cols);

    const peach_size_t rows = a->rows;
    const peach_size_t cols = a->cols;

    peach_matrix_t* result = paech_new_matrix(rows, cols);

    for (peach_size_t i = 0; i < rows; ++i)
        for (peach_size_t j = 0; j < cols; ++j)
            PEACH_MATRIX_AT(result, i, j) = PEACH_MATRIX_AT(a, i, j) + PEACH_MATRIX_AT(b, i, j);

    return result;
}

PEACH_INLINE peach_matrix_t* peach_matrix_sub(peach_matrix_t* a, peach_matrix_t* b) {
    PEACH_ASSERT(a->rows == b->rows);
    PEACH_ASSERT(a->cols == b->cols);

    const peach_size_t rows = a->rows;
    const peach_size_t cols = a->cols;

    peach_matrix_t* result = paech_new_matrix(rows, cols);

    for (peach_size_t i = 0; i < rows; ++i)
        for (peach_size_t j = 0; j < cols; ++j)
            PEACH_MATRIX_AT(result, i, j) = PEACH_MATRIX_AT(a, i, j) - PEACH_MATRIX_AT(b, i, j);

    return result;
}

PEACH_INLINE peach_matrix_t* peach_matrix_mul(peach_matrix_t* a, peach_matrix_t* b) {
    PEACH_ASSERT(a->rows == b->rows);
    PEACH_ASSERT(a->cols == b->cols);

    const peach_size_t rows = a->rows;
    const peach_size_t cols = a->cols;

    peach_matrix_t* result = paech_new_matrix(rows, cols);

    for (peach_size_t i = 0; i < rows; ++i)
        for (peach_size_t j = 0; j < cols; ++j)
            PEACH_MATRIX_AT(result, i, j) = PEACH_MATRIX_AT(a, i, j) * PEACH_MATRIX_AT(b, i, j);

    return result;
}

PEACH_INLINE peach_matrix_t* peach_matrix_div(peach_matrix_t* a, peach_matrix_t* b) {
    PEACH_ASSERT(a->rows == b->rows);
    PEACH_ASSERT(a->cols == b->cols);

    const peach_size_t rows = a->rows;
    const peach_size_t cols = a->cols;

    peach_matrix_t* result = paech_new_matrix(rows, cols);

    for (peach_size_t i = 0; i < rows; ++i)
        for (peach_size_t j = 0; j < cols; ++j)
            PEACH_MATRIX_AT(result, i, j) = PEACH_MATRIX_AT(a, i, j) / PEACH_MATRIX_AT(b, i, j);

    return result;
}

PEACH_INLINE peach_matrix_t* peach_matrix_dot(peach_matrix_t* a, peach_matrix_t* b) {
    PEACH_ASSERT(a->cols == b->rows);

    const peach_size_t n = a->cols;
    
    const peach_size_t rows = a->rows;
    const peach_size_t cols = b->cols;

    peach_matrix_t* mat = paech_new_matrix(rows, cols);

    for (peach_size_t i = 0; i < rows; ++i) {
        for (peach_size_t j = 0; j < cols; ++j) {
            PEACH_MATRIX_AT(mat, i, j) = 0;

            for (peach_size_t k = 0; k < n; ++k) {
                PEACH_MATRIX_AT(mat, i, j) += PEACH_MATRIX_AT(a, i, k) * PEACH_MATRIX_AT(b, k, j);
            }
        }
    }

    return mat;
}

PEACH_INLINE void peach_matrix_sum_target(peach_matrix_t* target, peach_matrix_t* b) {
    PEACH_ASSERT(target->rows == b->rows);
    PEACH_ASSERT(target->cols == b->cols);

    const peach_size_t rows = target->rows;
    const peach_size_t cols = target->cols;

    for (peach_size_t i = 0; i < rows; ++i)
        for (peach_size_t j = 0; j < cols; ++j)
            PEACH_MATRIX_AT(target, i, j) += PEACH_MATRIX_AT(b, i, j);
}

PEACH_INLINE void peach_matrix_sub_target(peach_matrix_t* target, peach_matrix_t* b) {
    PEACH_ASSERT(target->rows == b->rows);
    PEACH_ASSERT(target->cols == b->cols);

    const peach_size_t rows = target->rows;
    const peach_size_t cols = target->cols;

    for (peach_size_t i = 0; i < rows; ++i)
        for (peach_size_t j = 0; j < cols; ++j)
            PEACH_MATRIX_AT(target, i, j) -= PEACH_MATRIX_AT(b, i, j);
}

PEACH_INLINE void peach_matrix_mul_target(peach_matrix_t* target, peach_matrix_t* b) {
    PEACH_ASSERT(target->rows == b->rows);
    PEACH_ASSERT(target->cols == b->cols);

    const peach_size_t rows = target->rows;
    const peach_size_t cols = target->cols;

    for (peach_size_t i = 0; i < rows; ++i)
        for (peach_size_t j = 0; j < cols; ++j)
            PEACH_MATRIX_AT(target, i, j) *= PEACH_MATRIX_AT(b, i, j);
}

PEACH_INLINE void peach_matrix_div_target(peach_matrix_t* target, peach_matrix_t* b) {
    PEACH_ASSERT(target->rows == b->rows);
    PEACH_ASSERT(target->cols == b->cols);

    const peach_size_t rows = target->rows;
    const peach_size_t cols = target->cols;

    for (peach_size_t i = 0; i < rows; ++i)
        for (peach_size_t j = 0; j < cols; ++j)
            PEACH_MATRIX_AT(target, i, j) /= PEACH_MATRIX_AT(b, i, j);
}

PEACH_INLINE void peach_matrix_dot_target(peach_matrix_t* target, peach_matrix_t* a, peach_matrix_t* b) {
    PEACH_ASSERT(a->cols == b->rows);

    const peach_size_t n = a->cols;
    
    const peach_size_t rows = a->rows;
    const peach_size_t cols = b->cols;

    for (peach_size_t i = 0; i < rows; ++i) {
        for (peach_size_t j = 0; j < cols; ++j) {
            PEACH_MATRIX_AT(target, i, j) = 0;

            for (peach_size_t k = 0; k < n; ++k)
                PEACH_MATRIX_AT(target, i, j) += PEACH_MATRIX_AT(a, i, k) * PEACH_MATRIX_AT(b, k, j);
        }
    }
}

PEACH_INLINE void peach_matrix_apply_sigmoid(peach_matrix_t* target) {
    const peach_size_t rows = target->rows;
    const peach_size_t cols = target->cols;

    for (peach_size_t i = 0; i < rows; ++i)
        for (peach_size_t j = 0; j < cols; ++j)
            PEACH_MATRIX_AT(target, i, j) = peach_sigmoid(PEACH_MATRIX_AT(target, i, j));
}

PEACH_INLINE void peach_matrix_apply_relu(peach_matrix_t* target) {
    const peach_size_t rows = target->rows;
    const peach_size_t cols = target->cols;

    for (peach_size_t i = 0; i < rows; ++i)
        for (peach_size_t j = 0; j < cols; ++j)
            PEACH_MATRIX_AT(target, i, j) = peach_relu(PEACH_MATRIX_AT(target, i, j));
}

PEACH_INLINE void peach_matrix_print(peach_matrix_t* m) {
    if(m == PEACH_NULL) {
        PEACH_PRINT("NULL\n");
        return;
    }

    peach_size_t rows = m->rows;
    peach_size_t cols = m->cols;

    PEACH_PRINT("(%lld %lld) = {\n", rows, cols);

    for(peach_size_t i = 0; i < rows; ++i) { 
        PEACH_PRINT("    ");

        for(peach_size_t j = 0; j < cols; ++j) {
            PEACH_PRINT("%f, ", PEACH_MATRIX_AT(m, i, j));
        }

        PEACH_PRINT("\n");
    }

    PEACH_PRINT("}\n");
}

#endif

#endif