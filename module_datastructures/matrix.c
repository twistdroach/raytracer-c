#include <assert.h>
#include <stdarg.h>
#include "exceptions.h"
#include "matrix.h"
#include "utilities.h"

MATRIX_Matrix* MATRIX_new(uint width, uint height) {
    MATRIX_Matrix* m = (MATRIX_Matrix*) malloc(sizeof(MATRIX_Matrix));
    if (!m)
        Throw(E_MALLOC_FAILED);

    MATRIX_init(m, width, height);
    return m;
}

uint calculate_index(const MATRIX_Matrix* matrix, uint row, uint column) {
    assert(column < matrix->width);
    assert(row < matrix->height);
    return row * matrix->width + column;
}

double MATRIX_read_cell(const MATRIX_Matrix* matrix, uint row, uint column) {
    assert(matrix);
    assert(matrix->data);
    uint ndx = calculate_index(matrix, row, column);
    return matrix->data[ndx];
}

void MATRIX_write_cell(MATRIX_Matrix* matrix, uint row, uint column, double value) {
    assert(matrix);
    uint ndx = calculate_index(matrix, row, column);
    matrix->data[ndx] = value;
}

void MATRIX_fill(MATRIX_Matrix* matrix, ...) {
    assert(matrix);
    va_list args;
    uint row = 0;
    uint column = 0;
    uint matrix_size = matrix->width * matrix->height;
    va_start(args, matrix);
    for (uint i=0; i < matrix_size; i++) {
        double d = va_arg(args, double);
        MATRIX_write_cell(matrix, row, column, d);
        column++;
        if (column == matrix->width) {
            column = 0;
            row++;
        }
    }
    va_end(args);
}
void MATRIX_copy(MATRIX_Matrix* dest, const MATRIX_Matrix* source) {
    assert(dest);
    assert(source);
    assert(source->data);
    assert(dest != source);
    MATRIX_init(dest, source->width, source->height);
    for (uint ndx=0; ndx < source->height * source->width; ndx++) {
        dest->data[ndx] = source->data[ndx];
    }
}

bool MATRIX_is_equal(const MATRIX_Matrix* m1, const MATRIX_Matrix* m2) {
    assert(m1);
    assert(m2);
    if (m1 == m2) return true;
    if (m1->width != m2->width) return false;
    if (m1->height != m2->height) return false;
    for (uint ndx=0; ndx < m1->height * m1->width; ndx++) {
        if (!double_equal(m1->data[ndx], m2->data[ndx]))
            return false;
    }
    return true;
}

void MATRIX_init(MATRIX_Matrix* matrix, uint width, uint height) {
    assert(matrix);
    assert(width > 0);
    assert(height > 0);
    matrix->width = width;
    matrix->height = height;
    matrix->data = (double*) malloc(width * height * sizeof(double));
    if (!matrix->data)
        Throw(E_MALLOC_FAILED);

    for (uint i=0; i < width * height; i++) {
        matrix->data[i] = 0.0;
    }
}

double compute_dot_product(const MATRIX_Matrix* m1, const MATRIX_Matrix* m2, uint row, uint column) {
    uint vector_column=0;
    double total = 0.0;
    for (uint vector_row=0; vector_row < m1->height; vector_row++) {
        total += MATRIX_read_cell(m1, row, vector_column) * MATRIX_read_cell(m2, vector_row, column);
        vector_column++;
    }
    return total;
}

void MATRIX_multiply(MATRIX_Matrix* dest, const MATRIX_Matrix* m1, const MATRIX_Matrix* m2) {
    assert(dest);
    assert(m1);
    assert(m2);
    assert(dest != m1);
    assert(dest != m2);
    /* matrix multiplication is only valid if the rows of the first matrix is equals
       to the columns in the second.  the resulting matrix will have the number of rows
       of the first matrix & columns of the second
    */
    assert(m1->height == m2->width);
    MATRIX_init(dest, m1->width, m2->height);
    for (uint row=0; row < m1->height; row++) {
        for (uint column=0; column < m1->width; column++) {
            MATRIX_write_cell(dest, row, column, compute_dot_product(m1, m2, row, column));
        }
    }
}

//TODO write tests & expose in interface
void convert_tuple_to_matrix(MATRIX_Matrix* dest, const TUPLES_Tuple* src) {
    assert(dest);
    assert(src);
    MATRIX_init(dest, 1, 4);
    MATRIX_fill(dest, src->x, src->y, src->z, src->w);
}

//TODO write tests & expose in interface
void convert_matrix_to_tuple(TUPLES_Tuple* dest, const MATRIX_Matrix* src) {
    assert(dest);
    assert(src);
    assert(src->width == 1);
    assert(src->height == 4);
    TUPLES_init_vector(dest, MATRIX_read_cell(src, 0, 0), MATRIX_read_cell(src, 1, 0), MATRIX_read_cell(src, 2, 0));
    //hack
    dest->w = MATRIX_read_cell(src, 3, 0);
}

double multiply_row_by_tuple(const MATRIX_Matrix* matrix, const TUPLES_Tuple* tuple, uint row) {
    return MATRIX_read_cell(matrix, row, 0) * tuple->x +
            MATRIX_read_cell(matrix, row, 1) * tuple->y +
            MATRIX_read_cell(matrix, row, 2) * tuple->z +
            MATRIX_read_cell(matrix, row, 3) * tuple->w;
}

void MATRIX_multiply_tuple(TUPLES_Tuple* dest, const MATRIX_Matrix* matrix, const TUPLES_Tuple* tuple) {
    assert(dest);
    assert(matrix);
    assert(tuple);
    assert(matrix->height == 4 && matrix->width == 4);
    TUPLES_init_point(dest, multiply_row_by_tuple(matrix, tuple, 0),
                            multiply_row_by_tuple(matrix, tuple, 1),
                            multiply_row_by_tuple(matrix, tuple, 2));
    dest->w = multiply_row_by_tuple(matrix, tuple, 3);
}

void MATRIX_destroy(MATRIX_Matrix* matrix) {
    assert(matrix);
    assert(matrix->data);
    free(matrix->data);
}

void MATRIX_delete(MATRIX_Matrix* matrix) {
    assert(matrix);
    MATRIX_destroy(matrix);
    free(matrix);
}
