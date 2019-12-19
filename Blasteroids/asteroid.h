#ifndef ASTEROID_H
#define ASTEROID_H

typedef struct {
float sx;
float sy;
float heading;
float twist;
float speed;
float rot_velocity;
float scale;
int gone;
ALLEGRO_COLOR color;
} Asteroid;
void draw_asteroid(Asteroid *);
void move_asteroid(Asteroid *,float, float);
Asteroid* create_asteroid(float, float, int);
void kill_asteroid(Asteroid * a);

#endif	// ASTEROID_H

