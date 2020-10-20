#include "sequences.h"

#include <stdlib.h>
#include <assert.h>
#include <exceptions.h>
#include <string.h>
#include <utilities.h>

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
    double r = 0;
    #pragma omp critical(SEQUENCES_next)
    {
        assert(sequence);
        r = sequence->seq[sequence->next_ndx];
        sequence->next_ndx++;
        if (sequence->next_ndx == sequence->count) {
            sequence->next_ndx = 0;
        }
    }
    return r;
}

SEQUENCES_Sequence* SEQUENCES_copy(SEQUENCES_Sequence* sequence) {
    SEQUENCES_Sequence* seq = SEQUENCES_new(sequence->count, sequence->seq);
    seq->next_ndx = sequence->next_ndx;
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
