#include "sequences.h"

#include <stdlib.h>
#include <assert.h>
#include <exceptions.h>
#include <string.h>
#include <utilities.h>

#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_num_threads() 1
#define omp_get_thread_num() 0
#define omp_get_max_threads() 1
#endif

typedef struct SEQUENCES_Sequence {
    unsigned int    count;
    unsigned int*   next_ndx;
    double          seq[];
} SEQUENCES_Sequence;

SEQUENCES_Sequence* SEQUENCES_new(unsigned int count, double numbers[]) {
    assert(count > 0);
    SEQUENCES_Sequence *seq = malloc(sizeof(SEQUENCES_Sequence) + count * sizeof(numbers[0]));
    if (!seq) {
        Throw(E_MALLOC_FAILED);
    }
    seq->count = count;
    memcpy(seq->seq, numbers, count * sizeof(numbers[0]));
    seq->next_ndx = calloc(omp_get_max_threads(), sizeof(unsigned int));

    return seq;
}

void SEQUENCES_delete(SEQUENCES_Sequence* sequence) {
    assert(sequence);
    free(sequence->next_ndx);
    free(sequence);

}
double SEQUENCES_next(SEQUENCES_Sequence* sequence) {
    assert(sequence);
    int thread_id = omp_get_thread_num();
    unsigned int ndx = sequence->next_ndx[thread_id];
    sequence->next_ndx[thread_id] = (ndx + 1) % sequence->count;
    return sequence->seq[ndx];
}

SEQUENCES_Sequence* SEQUENCES_copy(SEQUENCES_Sequence* sequence) {
    SEQUENCES_Sequence* seq = SEQUENCES_new(sequence->count, sequence->seq);
    memcpy(seq->next_ndx, sequence->next_ndx, omp_get_max_threads());
    return seq;
}

SEQUENCES_Sequence* SEQUENCES_new_random(unsigned int count) {
    assert(count >= 1);
    double rands[count];
    for (unsigned int ndx = 0; ndx < count; ndx++) {
        rands[ndx] = UTILITIES_random_double(0, 1);
    }
    return SEQUENCES_new(count, rands);
}
