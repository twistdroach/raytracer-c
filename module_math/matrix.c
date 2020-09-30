#define _GNU_SOURCE
#include <assert.h>
#include <stdarg.h>
#include <math.h>
#include "exceptions.h"
#include "matrix.h"
#include "utilities.h"

MATRIX_Matrix* MATRIX_new(uint width, uint height) {
    MATRIX_Matrix* m = malloc(sizeof(MATRIX_Matrix));
    if (!m)
        Throw(E_MALLOC_FAILED);

    MATRIX_init(m, width, height);
    return m;
}

inline static uint calculate_index(const MATRIX_Matrix* matrix, uint row, uint column) {
    assert(column < matrix->width);
    assert(row < matrix->height);
    return row * matrix->width + column;
}

inline double MATRIX_read_cell(const MATRIX_Matrix* matrix, uint row, uint column) {
    assert(matrix);
    assert(matrix->data);
    uint ndx = calculate_index(matrix, row, column);
    return matrix->data[ndx];
}

inline void MATRIX_write_cell(MATRIX_Matrix* matrix, uint row, uint column, double value) {
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
    assert(dest != source);
    *dest = *source;
}

bool MATRIX_is_equal(const MATRIX_Matrix* m1, const MATRIX_Matrix* m2) {
    assert(m1);
    assert(m2);
    if (m1 == m2) return true;
    if (m1->width != m2->width) return false;
    if (m1->height != m2->height) return false;
    for (uint ndx=0; ndx < m1->height * m1->width; ndx++) {
        if (!double_equal(m1->data[ndx], m2->data[ndx])){
            return false;
        }
    }
    return true;
}

void MATRIX_init(MATRIX_Matrix* matrix, uint width, uint height) {
    assert(matrix);
    assert(width > 0);
    assert(height > 0);
    assert(width < 5);
    assert(height < 5);

    matrix->width = width;
    matrix->height = height;

    for (uint i=0; i < width * height; i++) {
        matrix->data[i] = 0.0;
    }
}

MATRIX_Matrix* MATRIX_new_identity(uint width) {
    assert(width > 0);
    MATRIX_Matrix* matrix = MATRIX_new(width, width);
    for (uint column=0; column < matrix->width; column++) {
        MATRIX_write_cell(matrix, column, column, 1.0);
    }
    return matrix;
}

MATRIX_Matrix* MATRIX_new_translation(double x, double y, double z) {
    MATRIX_Matrix* matrix = MATRIX_new_identity(4);
    MATRIX_write_cell(matrix, 0, 3, x);
    MATRIX_write_cell(matrix, 1, 3, y);
    MATRIX_write_cell(matrix, 2, 3, z);
    return matrix;
}

MATRIX_Matrix* MATRIX_new_scaling(double x, double y, double z) {
    MATRIX_Matrix* matrix = MATRIX_new(4, 4);
    MATRIX_write_cell(matrix, 0, 0, x);
    MATRIX_write_cell(matrix, 1, 1, y);
    MATRIX_write_cell(matrix, 2, 2, z);
    MATRIX_write_cell(matrix, 3, 3, 1.0);
    return matrix;
}

MATRIX_Matrix* MATRIX_new_rotation_x(double radians) {
    MATRIX_Matrix* matrix = MATRIX_new(4, 4);
    double s = sin(radians);
    double c = cos(radians);
    MATRIX_write_cell(matrix, 0, 0, 1.0);
    MATRIX_write_cell(matrix, 1, 1, c);
    MATRIX_write_cell(matrix, 2, 1, s);
    MATRIX_write_cell(matrix, 1, 2, -s);
    MATRIX_write_cell(matrix, 2, 2, c);
    MATRIX_write_cell(matrix, 3, 3, 1.0);
    return matrix;
}

MATRIX_Matrix* MATRIX_new_rotation_y(double radians) {
    MATRIX_Matrix *matrix = MATRIX_new(4, 4);
    double s = sin(radians);
    double c = cos(radians);
    MATRIX_write_cell(matrix, 1, 1, 1.0);
    MATRIX_write_cell(matrix, 0, 0, c);
    MATRIX_write_cell(matrix, 2, 2, c);
    MATRIX_write_cell(matrix, 2, 0, -s);
    MATRIX_write_cell(matrix, 0, 2, s);
    MATRIX_write_cell(matrix, 3, 3, 1.0);
    return matrix;
}

MATRIX_Matrix* MATRIX_new_rotation_z(double radians) {
    MATRIX_Matrix *matrix = MATRIX_new(4, 4);
    double s = sin(radians);
    double c = cos(radians);
    MATRIX_write_cell(matrix, 0, 0, c);
    MATRIX_write_cell(matrix, 1, 1, c);
    MATRIX_write_cell(matrix, 0, 1, -s);
    MATRIX_write_cell(matrix, 1, 0, s);
    MATRIX_write_cell(matrix, 2, 2, 1.0);
    MATRIX_write_cell(matrix, 3, 3, 1.0);
    return matrix;
}

MATRIX_Matrix* MATRIX_new_shearing(double xy, double xz, double yx, double yz, double zx, double zy) {
    MATRIX_Matrix *matrix = MATRIX_new_identity(4);
    MATRIX_write_cell(matrix, 0, 1, xy);
    MATRIX_write_cell(matrix, 0, 2, xz);
    MATRIX_write_cell(matrix, 1, 0, yx);
    MATRIX_write_cell(matrix, 1, 2, yz);
    MATRIX_write_cell(matrix, 2, 0, zx);
    MATRIX_write_cell(matrix, 2, 1, zy);

    return matrix;
}

static double compute_dot_product(const MATRIX_Matrix* m1, const MATRIX_Matrix* m2, uint row, uint column) {
    uint vector_column=0;
    double total = 0.0;
    for (uint vector_row=0; vector_row < m1->height; vector_row++) {
        total += MATRIX_read_cell(m1, row, vector_column) * MATRIX_read_cell(m2, vector_row, column);
        vector_column++;
    }
    return total;
}

MATRIX_Matrix* MATRIX_multiply_array(const MATRIX_Matrix* matrix[]) {
    MATRIX_Matrix* retmatrix = MATRIX_new_identity(4);
    for (uint i=0; matrix[i]; i++) {
        const MATRIX_Matrix* m = matrix[i];
        assert(m->width == 4);
        assert(m->height == 4);
        MATRIX_Matrix* new_retmatrix = MATRIX_multiply(retmatrix, m);
        MATRIX_delete(retmatrix);
        retmatrix = new_retmatrix;
    }
    return retmatrix;
}

MATRIX_Matrix* MATRIX_multiply(const MATRIX_Matrix* m1, const MATRIX_Matrix* m2) {
    assert(m1);
    assert(m2);
    /* matrix multiplication is only valid if the rows of the first matrix is equals
       to the columns in the second.  the resulting matrix will have the number of rows
       of the first matrix & columns of the second
    */
    assert(m1->height == m2->width);
    MATRIX_Matrix* dest = MATRIX_new(m1->width, m2->height);
    for (uint row=0; row < m1->height; row++) {
        for (uint column=0; column < m1->width; column++) {
            MATRIX_write_cell(dest, row, column, compute_dot_product(m1, m2, row, column));
        }
    }
    return dest;
}

#if 0
//TODO write tests & expose in interface
static void convert_tuple_to_matrix(MATRIX_Matrix* dest, const TUPLES_Tuple* src) {
    assert(dest);
    assert(src);
    MATRIX_init(dest, 1, 4);
    MATRIX_fill(dest, src->x, src->y, src->z, src->w);
}

//TODO write tests & expose in interface
static void convert_matrix_to_tuple(TUPLES_Tuple* dest, const MATRIX_Matrix* src) {
    assert(dest);
    assert(src);
    assert(src->width == 1);
    assert(src->height == 4);
    TUPLES_init_vector(dest, MATRIX_read_cell(src, 0, 0), MATRIX_read_cell(src, 1, 0), MATRIX_read_cell(src, 2, 0));
    //hack
    dest->w = MATRIX_read_cell(src, 3, 0);
}
#endif

static double multiply_row_by_tuple(const MATRIX_Matrix* matrix, const TUPLES_Tuple* tuple, uint row) {
    return MATRIX_read_cell(matrix, row, 0) * tuple->x +
            MATRIX_read_cell(matrix, row, 1) * tuple->y +
            MATRIX_read_cell(matrix, row, 2) * tuple->z +
            MATRIX_read_cell(matrix, row, 3) * tuple->w;
}

void MATRIX_multiply_tuple(TUPLES_Tuple* dest, const MATRIX_Matrix* matrix, const TUPLES_Tuple* tuple) {
    assert(matrix);
    assert(tuple);
    assert(dest);
    assert(matrix->height == 4 && matrix->width == 4);

    TUPLES_init_point(dest, multiply_row_by_tuple(matrix, tuple, 0),
                            multiply_row_by_tuple(matrix, tuple, 1),
                            multiply_row_by_tuple(matrix, tuple, 2));
    dest->w = multiply_row_by_tuple(matrix, tuple, 3);
}

static void transpose_cell(MATRIX_Matrix* matrix, uint row, uint column) {
    double val = MATRIX_read_cell(matrix, row, column);
    MATRIX_write_cell(matrix, row, column, MATRIX_read_cell(matrix, column, row));
    MATRIX_write_cell(matrix, column, row, val);
}

void MATRIX_transpose(MATRIX_Matrix* matrix) {
    assert(matrix);
    // only needs to support square matrix
    assert(matrix->width == matrix->height);
    uint offset = 1;
    for (uint row=0; row < matrix->height; row++) {
        for (uint column=offset; column < matrix->width; column++) {
            transpose_cell(matrix, row, column);
        }
        offset++;
    }
}

char* MATRIX_to_string(const MATRIX_Matrix* matrix) {
    assert(matrix);
    char* str = NULL;
    Sasprintf(str, "Matrix:\n");
    for (uint row=0; row < matrix->height; row++) {
        for (uint column=0; column < matrix->width; column++) {
            Sasprintf(str, "%s %.5f", str, MATRIX_read_cell(matrix, row, column));
        }
        Sasprintf(str, "%s\n", str);
    }
    return str;
}

double MATRIX_determinant(const MATRIX_Matrix* matrix) {
    assert(matrix);
    assert(matrix->width >= 2);
    assert(matrix->height >= 2);
    double determinant = 0.0;
    if (matrix->width == 2 && matrix->height == 2) {
        determinant = (MATRIX_read_cell(matrix, 0, 0) * MATRIX_read_cell(matrix, 1, 1) -
                       MATRIX_read_cell(matrix, 0, 1) * MATRIX_read_cell(matrix, 1, 0));

    } else {
        for (uint column = 0; column < matrix->width; column++) {
            determinant += MATRIX_read_cell(matrix, 0, column) * MATRIX_cofactor(matrix, 0, column);
        }
    }
    return determinant;
}

MATRIX_Matrix* MATRIX_submatrix(MATRIX_Matrix* dest, const MATRIX_Matrix* matrix, uint row, uint column) {
    assert(matrix);
    assert(row < matrix->height);
    assert(column < matrix->width);
    assert(matrix->height >= 3);
    assert(matrix->width >= 3);

    if (dest) {
        MATRIX_init(dest, matrix->width - 1, matrix->height - 1);
    } else {
        dest = MATRIX_new(matrix->width - 1, matrix->height - 1);
    }
    for (uint write_row=0; write_row < dest->height; write_row++) {
        for (uint write_column=0; write_column < dest->width; write_column++) {
            uint read_row = write_row;
            if (write_row >= row) read_row++;
            uint read_column = write_column;
            if (write_column >= column) read_column++;
            MATRIX_write_cell(dest, write_row, write_column, MATRIX_read_cell(matrix, read_row, read_column));
        }
    }
    return dest;
}

double MATRIX_minor(const MATRIX_Matrix* matrix, uint row, uint column) {
    assert(matrix);
    assert(row < matrix->height);
    assert(column < matrix->width);
    MATRIX_Matrix submatrix;
    MATRIX_submatrix(&submatrix, matrix, row, column);
    double determinant = MATRIX_determinant(&submatrix);
    return determinant;
}

double MATRIX_cofactor(const MATRIX_Matrix* matrix, uint row, uint column) {
    assert(matrix);
    assert(row < matrix->height);
    assert(column < matrix->width);
    double cofactor = MATRIX_minor(matrix, row, column);
    if ((row + column) % 2 == 1) {
        cofactor = 0.0 - cofactor;
    }
    return cofactor;
}

bool MATRIX_is_invertible(const MATRIX_Matrix* matrix) {
    return !double_equal(MATRIX_determinant(matrix), 0.0);
}

void MATRIX_inverse(MATRIX_Matrix* dest, const MATRIX_Matrix* matrix) {
    assert(matrix);
    assert(dest);
    assert(MATRIX_is_invertible(matrix));

    MATRIX_init(dest, matrix->width, matrix->height);
    double determinant = MATRIX_determinant(matrix);
    for (uint row=0; row<matrix->height; row++) {
        for (uint column=0; column<matrix->width; column++) {
            double cofactor = MATRIX_cofactor(matrix, row, column);
            // we are transposing also so column / row are swapped below
            MATRIX_write_cell(dest, column, row, cofactor/determinant);
        }
    }
}

void MATRIX_destroy(MATRIX_Matrix* matrix) {
    assert(matrix);
    UNUSED(matrix);
}

void MATRIX_delete(MATRIX_Matrix* matrix) {
    assert(matrix);
    MATRIX_destroy(matrix);
    free(matrix);
}
