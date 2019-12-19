#ifndef SPACESHIP_H
#define SPACESHIP_H

#define degreesToRadians(angleDegrees) (angleDegrees * 3.1415 / 180.0)
typedef struct {
float sx;
float sy;
float heading;
float speed;
int gone;
ALLEGRO_COLOR color;
} Spaceship;
void draw_ship(Spaceship *);
void move_ship(Spaceship *, float, float, float);
#endif	// SPACESHIP_H

