/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "spaceship.h"
#include <math.h>
#include <stdio.h>
#define degreesToRadians(angleDegrees) (angleDegrees * 3.1415 / 180.0)

typedef struct {
    //bool firstTime;
    float sx;
    float sy;
    float heading;
    float speed;
    int gone;
    ALLEGRO_COLOR color;
} Blast;

void draw_blast(Blast * b) {
    if(b->gone==0){
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_rotate_transform(&transform, degreesToRadians(b->heading));
    al_translate_transform(&transform, b->sx, b->sy);
    al_use_transform(&transform);
    al_draw_line(0, 0, 0, -3, b->color, 3.0f);
    al_draw_line(0, -6, 0, -9, b->color, 3.0f);
    al_draw_line(0, -11, 0, -14, b->color, 3.0f); 
    }
}

void fire(Spaceship *s, Blast *b){
        
        b->sx=s->sx;
        b->sy=s->sy;
        b->speed=s->speed+10;
        b->heading=s->heading;
        //b->gone=0;
        //b->firstTime=false;

}

void move_blast(Blast *b, float SCREEN_H, float SCREEN_W){
    if(b->gone==0){
            //printf("%f \n",b->sx);
        b->sy -= b->speed * cos(degreesToRadians(b->heading));
        b->sx += b->speed * sin(degreesToRadians(b->heading));
                   
            if (b->sx > SCREEN_W) {
                b->gone=1;
            }

            if (b->sy > SCREEN_H) {
                b->gone=1;
            }

            if (b->sy < 0) {
                b->gone=1;
            }

            if (b->sx < 0) {
                b->gone=1;
            }
           
   }
}


