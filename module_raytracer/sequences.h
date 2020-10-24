#ifndef SIMPLE_RAYTRACER_SEQUENCES_H
#define SIMPLE_RAYTRACER_SEQUENCES_H

typedef struct SEQUENCES_Sequence SEQUENCES_Sequence;

SEQUENCES_Sequence *SEQUENCES_new(unsigned int count, double numbers[]);
SEQUENCES_Sequence *SEQUENCES_new_random(unsigned int count);
SEQUENCES_Sequence *SEQUENCES_copy(SEQUENCES_Sequence *sequence);
void SEQUENCES_delete(SEQUENCES_Sequence *sequence);
double SEQUENCES_next(SEQUENCES_Sequence *sequence);

#endif // SIMPLE_RAYTRACER_SEQUENCES_H
