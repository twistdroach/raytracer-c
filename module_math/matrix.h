#ifndef DATA_STRUCTURES_MATRIX_H
#define DATA_STRUCTURES_MATRIX_H

#include "tuples.h"
#include "utilities.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct MATRIX_Matrix {
  uint width, height;
  double data[16];
} MATRIX_Matrix;

MATRIX_Matrix *MATRIX_new(uint width, uint height);
void MATRIX_init(MATRIX_Matrix *matrix, uint width, uint height);
void MATRIX_destroy(MATRIX_Matrix *matrix);
#define MATRIX_destroy_all(...) Fn_apply(MATRIX_Matrix, MATRIX_destroy, __VA_ARGS__);
void MATRIX_delete(MATRIX_Matrix *matrix);
#define MATRIX_delete_all(...) Fn_apply(MATRIX_Matrix, MATRIX_delete, __VA_ARGS__);

double MATRIX_read_cell(const MATRIX_Matrix *matrix, uint row, uint column);
void MATRIX_write_cell(MATRIX_Matrix *matrix, uint row, uint column, double value);
bool MATRIX_is_equal(const MATRIX_Matrix *m1, const MATRIX_Matrix *m2);

char *MATRIX_to_string(const MATRIX_Matrix *matrix);

double MATRIX_determinant(const MATRIX_Matrix *matrix);

/**
 * Returns a ptr to a copy of the matrix with the row & column removed.
 * Must call MATRIX_delete() on the returned ptr to avoid leaking memory (unless
 * you provided dest)
 * @param matrix
 * @param row
 * @param column
 * @return
 */
MATRIX_Matrix *MATRIX_submatrix(MATRIX_Matrix *dest, const MATRIX_Matrix *matrix, uint row, uint column);

double MATRIX_minor(const MATRIX_Matrix *matrix, uint row, uint column);

double MATRIX_cofactor(const MATRIX_Matrix *matrix, uint row, uint column);

bool MATRIX_is_invertible(const MATRIX_Matrix *matrix);

/**
 * Should only be called on an invertible matrix.  Test with
 * MATRIX_is_invertible!
 * @param matrix
 * @return
 */
void MATRIX_inverse(MATRIX_Matrix *dest, const MATRIX_Matrix *matrix);

/**
 * Allocates and initializes a an identity matrix.
 * Must call MATRIX_delete() on the returned ptr to avoid leaking memory.
 *
 * @param width The width & height of the matrix (identities are always square).
 */
MATRIX_Matrix *MATRIX_new_identity(uint width);
MATRIX_Matrix *MATRIX_new_translation(double x, double y, double z);
MATRIX_Matrix *MATRIX_new_scaling(double x, double y, double z);
MATRIX_Matrix *MATRIX_new_rotation_x(double radians);
MATRIX_Matrix *MATRIX_new_rotation_y(double radians);
MATRIX_Matrix *MATRIX_new_rotation_z(double radians);
MATRIX_Matrix *MATRIX_new_shearing(double xy, double xz, double yx, double yz, double zx, double zy);

/**
 * Transposes square matrix in place.
 * @param matrix
 */
void MATRIX_transpose(MATRIX_Matrix *matrix);

/**
 * Matrix multiplication.
 * Must call MATRIX_delete() on the returned ptr to avoid leaking memory.
 * @param m1
 * @param m2
 */
MATRIX_Matrix *MATRIX_multiply(const MATRIX_Matrix *m1, const MATRIX_Matrix *m2);
MATRIX_Matrix *MATRIX_multiply_array(const MATRIX_Matrix *matrix[]);

/**
 * example:
 * \code{.c}
 * MATRIX_Matrix* m1 = MATRIX_new_identity(4);
 * MATRIX_Matrix* m2 = MATRIX_new_scaling(2, 2, 2);
 * MATRIX_Matrix* m3 = MATRIX_new_translation(0, 0, 1);
 * MATRIX_Matrix* result = MATRIX_multiply_many(m1, m2, m3);
 * \endcode
 */
#define MATRIX_multiply_many(...) MATRIX_multiply_array((const MATRIX_Matrix *[]){__VA_ARGS__, NULL})
/**
 * (deep) Copy a matrix
 * @param dest - must be allocated
 * @param source
 */
void MATRIX_copy(MATRIX_Matrix *dest, const MATRIX_Matrix *source);

/**
 * Fills matrix from left to right, row by row.  Number of args must match
 * matrix size.
 * @param matrix
 * @param ...
 */
void MATRIX_fill(MATRIX_Matrix *matrix, ...);

/**
 * Multiplies a matrix by a tuple and places the answer in dest.
 * @param dest
 * @param matrix
 * @param tuple
 */
void MATRIX_multiply_tuple(TUPLES_Tuple *dest, const MATRIX_Matrix *matrix, const TUPLES_Tuple *tuple);

#endif // DATA_STRUCTURES_MATRIX_H
