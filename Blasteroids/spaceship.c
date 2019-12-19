/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#define degreesToRadians(angleDegrees) (angleDegrees * 3.1415 / 180.0)

typedef struct {
float sx;
float sy;
float heading;
float speed;
int gone;
ALLEGRO_COLOR color;
} Spaceship;

void draw_ship(Spaceship* s){
ALLEGRO_TRANSFORM transform;
al_identity_transform(&transform);
al_rotate_transform(&transform, degreesToRadians(s->heading));
al_translate_transform(&transform, s->sx, s->sy);
al_use_transform(&transform);
al_draw_line(-8, 9, 0, -11, s->color, 3.0f);
al_draw_line(0, -11, 8, 9, s->color, 3.0f);
al_draw_line(-6, 4, -1, 4, s->color, 3.0f);
al_draw_line(6, 4, 1, 4, s->color, 3.0f);
}

void move_ship(Spaceship *s,float SCREEN_H, float SCREEN_W, float heading){
    if (s->sx > SCREEN_W) {
                s->sy = s->sy;
                s->sx = 0;
            }

            if (s->sy > SCREEN_H) {
                s->sx = s->sx;
                s->sy = 0;
            }

            if (s->sy < 0) {
                s->sx = s->sx;
                s->sy = SCREEN_H;
            }

            if (s->sx < 0) {
                s->sy = s->sy;
                s->sx = SCREEN_W;
            }
    s->sy -= s->speed * cos(degreesToRadians(heading));
    s->sx += s->speed * sin(degreesToRadians(heading));
}




