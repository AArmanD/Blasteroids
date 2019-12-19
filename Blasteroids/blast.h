#ifndef BLAST_H
#define BLAST_H

#include "spaceship.h"

typedef struct {
    //bool firstTime;
    float sx;
    float sy;
    float heading;
    float speed;
    int gone;
    ALLEGRO_COLOR color;
} Blast;
void draw_blast(Blast *);
void fire(Spaceship *, Blast *);
void move_blast(Blast *, float, float);
#endif // BLAST_H

