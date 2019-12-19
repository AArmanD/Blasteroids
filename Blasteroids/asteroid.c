/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#define degreesToRadians(angleDegrees) (angleDegrees * 3.1415 / 180.0)

bool randinit = false;


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

void draw_asteroid(Asteroid * a) {
    if(!a->gone){
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_rotate_transform(&transform, degreesToRadians(a->heading));
    al_translate_transform(&transform, a->sx, a->sy);
    al_use_transform(&transform);
    al_draw_line(-20/a->scale, 20/a->scale, -25/a->scale, 5/a->scale, a->color, 2.0f);
    al_draw_line(-25/a->scale, 5/a->scale, -25/a->scale, -10/a->scale, a->color, 2.0f);
    al_draw_line(-25/a->scale, -10/a->scale, -5/a->scale, -10/a->scale, a->color, 2.0f);
    al_draw_line(-5/a->scale, -10/a->scale, -10/a->scale, -20/a->scale, a->color, 2.0f);
    al_draw_line(-10/a->scale, -20/a->scale, 5/a->scale, -20/a->scale, a->color, 2.0f);
    al_draw_line(5/a->scale, -20/a->scale, 20/a->scale, -10/a->scale, a->color, 2.0f);
    al_draw_line(20/a->scale, -10/a->scale, 20/a->scale, -5/a->scale, a->color, 2.0f);
    al_draw_line(20/a->scale, -5/a->scale, 0/a->scale, 0/a->scale, a->color, 2.0f);
    al_draw_line(0/a->scale, 0/a->scale, 20/a->scale, 10/a->scale, a->color, 2.0f);
    al_draw_line(20/a->scale, 10/a->scale, 10/a->scale, 20/a->scale, a->color, 2.0f);
    al_draw_line(10/a->scale, 20/a->scale, 0/a->scale, 15/a->scale, a->color, 2.0f);
    al_draw_line(0/a->scale, 15/a->scale, -20/a->scale, 20/a->scale, a->color, 2.0f);
    }
}

void move_asteroid(Asteroid * a, float height, float width) {
    a->sy -= a->speed * cos(degreesToRadians(a->twist));
    a->sx += a->speed * sin(degreesToRadians(a->twist));
    switch ((int) a->rot_velocity) {
        case 0:
            break;
        case 1:
            a->heading += 0.5;
            break;
        case 2:
            a->heading++;
            break;
        case 3:
            a->heading += 2;
            break;
    }
    
    if (a->sx > width) {
                a->sy = a->sy;
                a->sx = 0;
            }

            if (a->sy > height) {
                a->sx = a->sx;
                a->sy = 0;
            }

            if (a->sy < 0) {
                a->sx = a->sx;
                a->sy = height;
            }

            if (a->sx < 0) {
                a->sy = a->sy;
                a->sx = width;
            }
}

Asteroid* create_asteroid(float wheight, float wwidth, int scale) {
    if (randinit == false) {
        srand(time(NULL));
        randinit = true;
    }

    Asteroid* a = (Asteroid*) malloc(sizeof (Asteroid));
    if(a==NULL){
        fprintf(stderr, "Error by creating Asteroid");
        exit(-1);
    }

    a->sx = rand()%(int)wwidth;
    a->sy = rand()%(int)wheight;
    a->heading = rand() % 360;
    a->twist=a->heading;
    a->rot_velocity = rand()%4;
    a->color = al_map_rgb(255, 255, 255);
    

    switch (scale) {
        case 0:
            a->scale = 1;
            a->speed = 1;
            a->gone = 0;
            break;
        case 1:
            a->scale = 2;
            a->speed = 2;
            a->gone = 1;
            break;
    }
    return a;
}

void kill_asteroid(Asteroid * a){
    free(a);
}

