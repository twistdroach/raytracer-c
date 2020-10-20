#include "sequences.h"

#include <stdlib.h>
#include <assert.h>
#include <exceptions.h>
#include <string.h>

typedef struct SEQUENCES_Sequence {
    unsigned int    count;
    unsigned int    next_ndx;
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
    seq->next_ndx = 0;
    return seq;
}

void SEQUENCES_delete(SEQUENCES_Sequence* sequence) {
    assert(sequence);
    free(sequence);

}
double SEQUENCES_next(SEQUENCES_Sequence* sequence) {
#pragma omp critical SEQUENCES_next
    assert(sequence);
    double r = sequence->seq[sequence->next_ndx];
    if(++sequence->next_ndx == sequence->count) {
        sequence->next_ndx = 0;
    }
    return r;
}

SEQUENCES_Sequence* SEQUENCES_copy(SEQUENCES_Sequence* sequence) {
    SEQUENCES_Sequence* seq = SEQUENCES_new(sequence->count, sequence->seq);
    seq->next_ndx = sequence->next_ndx;
    return seq;
}
