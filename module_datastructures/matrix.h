//
// Created by zrowitsch on 8/4/20.
//

#ifndef DATA_STRUCTURES_MATRIX_H
#define DATA_STRUCTURES_MATRIX_H
#include <stdlib.h>
#include <stdbool.h>
#include "tuples.h"

typedef struct {
    uint width, height;
    double* data;
} MATRIX_Matrix;

MATRIX_Matrix* MATRIX_new(uint width, uint height);
void MATRIX_init(MATRIX_Matrix* matrix, uint width, uint height);
void MATRIX_destroy(MATRIX_Matrix* matrix);
void MATRIX_delete(MATRIX_Matrix* matrix);

double MATRIX_read_cell(const MATRIX_Matrix* matrix, uint row, uint column);
void MATRIX_write_cell(MATRIX_Matrix* matrix, uint row, uint column, double value);
bool MATRIX_is_equal(const MATRIX_Matrix* m1, const MATRIX_Matrix* m2);

char* MATRIX_to_string(const MATRIX_Matrix* matrix);

double MATRIX_determinant(const MATRIX_Matrix* matrix);

/**
 * Returns a ptr to a copy of the matrix with the row & column removed.
 * Must call MATRIX_delete() on the returned ptr to avoid leaking memory.
 * @param matrix
 * @param row
 * @param column
 * @return
 */
MATRIX_Matrix* MATRIX_submatrix(const MATRIX_Matrix* matrix, uint row, uint column);

double MATRIX_minor(const MATRIX_Matrix* matrix, uint row, uint column);

double MATRIX_cofactor(const MATRIX_Matrix* matrix, uint row, uint column);

bool MATRIX_is_invertible(const MATRIX_Matrix* matrix);

/**
 * Should only be called on an invertible matrix.  Test with MATRIX_is_invertible!
 * Must call MATRIX_delete() on the returned ptr to avoid leaking memory.
 * @param matrix
 * @return
 */
MATRIX_Matrix* MATRIX_inverse(const MATRIX_Matrix* matrix);

/**
 * Initialize a matrix as an identity matrix.
 *
 * @param width The width & height of the matrix (identities are always square).
 */
void MATRIX_init_identity(MATRIX_Matrix* matrix, uint width);

/**
 * Transposes square matrix in place.
 * @param matrix
 */
void MATRIX_transpose(MATRIX_Matrix* matrix);

/**
 * Matrix multiplication.
 * Must call MATRIX_delete() on the returned ptr to avoid leaking memory.
 * @param m1
 * @param m2
 */
MATRIX_Matrix* MATRIX_multiply(const MATRIX_Matrix* m1, const MATRIX_Matrix* m2);

/**
 * (deep) Copy a matrix
 * @param dest  - must be uninitialized (or previously destroyed if was initialized)
 * @param source
 */
void MATRIX_copy(MATRIX_Matrix* dest, const MATRIX_Matrix* source);

/**
 * Fills matrix from left to right, row by row.  Number of args must match matrix size.
 * @param matrix
 * @param ...
 */
void MATRIX_fill(MATRIX_Matrix* matrix, ...);

/**
 * Multiplies a matrix by a tuple.  Destination tuple should be uninit'd
 * @param dest
 * @param matrix
 * @param tuple
 */
void MATRIX_multiply_tuple(TUPLES_Tuple* dest, const MATRIX_Matrix* matrix, const TUPLES_Tuple* tuple);

#endif //DATA_STRUCTURES_MATRIX_H
