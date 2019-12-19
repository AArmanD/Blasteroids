/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   blasteroids.c
 * Author: armand
 *
 * Created on December 17, 2017, 8:26 PM
 */

#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "asteroid.h"
#include "blast.h"
#include "spaceship.h"

void draw_environment(ALLEGRO_FONT * font1, ALLEGRO_FONT * font2, int score, int kills, int level) {
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    if (kills < 4) {
        char g[255];
        char l[255];
        sprintf(l, "Level: %i", level);
        sprintf(g, "Score: %i", score);
        int width = al_get_text_width(font1, g);
        al_translate_transform(&transform, 0, 0);
        al_use_transform(&transform);
        al_draw_text(font1, al_map_rgb(255, 255, 255), ( al_get_text_width(font1, g) / 2) + 20, 10, ALLEGRO_ALIGN_CENTRE, g);



        switch (kills) {
            case 0:
                al_draw_line(-8 + 88, 9 + 60, 0 + 88, -11 + 60, al_map_rgb(0, 255, 0), 3.0f);
                al_draw_line(0 + 88, -11 + 60, 8 + 88, 9 + 60, al_map_rgb(0, 255, 0), 3.0f);
                al_draw_line(-6 + 88, 4 + 60, -1 + 88, 4 + 60, al_map_rgb(0, 255, 0), 3.0f);
                al_draw_line(6 + 88, 4 + 60, 1 + 88, 4 + 60, al_map_rgb(0, 255, 0), 3.0f);
            case 1:
                al_draw_line(-8 + 58, 9 + 60, 0 + 58, -11 + 60, al_map_rgb(0, 255, 0), 3.0f);
                al_draw_line(0 + 58, -11 + 60, 8 + 58, 9 + 60, al_map_rgb(0, 255, 0), 3.0f);
                al_draw_line(-6 + 58, 4 + 60, -1 + 58, 4 + 60, al_map_rgb(0, 255, 0), 3.0f);
                al_draw_line(6 + 58, 4 + 60, 1 + 58, 4 + 60, al_map_rgb(0, 255, 0), 3.0f);
            case 2:
                al_draw_line(-8 + 28, 9 + 60, 0 + 28, -11 + 60, al_map_rgb(0, 255, 0), 3.0f);
                al_draw_line(0 + 28, -11 + 60, 8 + 28, 9 + 60, al_map_rgb(0, 255, 0), 3.0f);
                al_draw_line(-6 + 28, 4 + 60, -1 + 28, 4 + 60, al_map_rgb(0, 255, 0), 3.0f);
                al_draw_line(6 + 28, 4 + 60, 1 + 28, 4 + 60, al_map_rgb(0, 255, 0), 3.0f);
        }

        //al_draw_text(font1, al_map_rgb(255, 255, 255), (width / 2) + 20, 10, ALLEGRO_ALIGN_CENTRE, g);
        al_draw_text(font1, al_map_rgb(0, 0, 153), (al_get_text_width(font1, l) / 2) + 20, 70, ALLEGRO_ALIGN_CENTRE, l);
    } else {
        char g[255];
        sprintf(g, "Score: %i", score);
        al_translate_transform(&transform, (1280 / 2), (720 / 2)-110);
        al_use_transform(&transform);
        al_draw_text(font2, al_map_rgb(0, 204, 204), 0 , 0, ALLEGRO_ALIGN_CENTRE, "Game Over");
        al_draw_text(font2, al_map_rgb(0, 204, 204), 0 , 80, ALLEGRO_ALIGN_CENTRE, g);
        al_draw_text(font1, al_map_rgb(0, 204, 204), 0 , 160, ALLEGRO_ALIGN_CENTRE, "Press Enter to restart");
    }
}

int size1 = 4;
int size2 = 8;

int check_won(Asteroid** a, Asteroid ** b, size_t asteroidb_len, size_t asteroids_len) {


    int check1 = 0;
    int check2 = 0;
    for (int j = 0; j < asteroids_len; j++) {
        if (b[j]->gone == 1)
            check2++;
    }

    for (int j = 0; j < asteroidb_len; j++) {
        if (a[j]->gone == 1)
            check1++;
    }


    if ((check1 == asteroidb_len) && (check2 == asteroids_len))
        return 1;
    else
        return 0;
}

int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h) {
    if ((b1_x > b2_x + b2_w - 1) || // is b1 on the right side of b2?
            (b1_y > b2_y + b2_h - 1) || // is b1 under b2?
            (b2_x > b1_x + b1_w - 1) || // is b2 on the right side of b1?
            (b2_y > b1_y + b1_h - 1)) // is b2 under b1?
    {
        // no collision
        return 0;
    }

    // collision
    return 1;
}

const float FPS = 60;

enum MYKEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE
};
const int SCREEN_W = 1280;
const int SCREEN_H = 720;

int main(int argc, char **argv) {

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    bool redraw = true;
    bool key[5] = {false, false, false, false, false};
    bool doexit = false;
    float heading = 0;
    int shotcounter = 0;
    int score = 0;
    int kills = 0;
    int counter = 0;
    size_t asteroidb_len = 4;
    size_t asteroids_len = 8;
    int level = 1;



    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return -1;
    }

    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_FONT *font1 = al_load_ttf_font("FreeSerif.ttf", 30, 0);
    ALLEGRO_FONT *font2 = al_load_ttf_font("FreeSerif.ttf", 70, 0);

    if (!font1) {
        fprintf(stderr, "Could not load 'FreeSerif.ttf'.\n");
        return -1;
    }

    if (!font2) {
        fprintf(stderr, "Could not load 'FreeSerif.ttf'.\n");
        return -1;
    }


    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(0, 0, 0));

    Spaceship s = {(SCREEN_W / 2 + 8), (SCREEN_H / 2 + 8), 0, 1, 0, al_map_rgb(0, 255, 0)};



    Blast c [3];

    for (int i = 0; i < (sizeof (c) / sizeof (c[0])); i++) {
        c[i].color = al_map_rgb(0, 255, 0);
        c[i].gone = 1;
        c[i].heading = 0;
        c[i].speed = 0;
        c[i].sx = 0;
        c[i].sy = 0;
    }

    Asteroid * big[255];
      //Asteroid ** big = malloc(asteroids_len * sizeof (Asteroid *));
    for (int j = 0; j < asteroidb_len; j++) {
        big[j] = create_asteroid(SCREEN_H, SCREEN_W, 0);
        //*(sarray+j)=create_asteroid(SCREEN_H, SCREEN_W, 0);
    }

    Asteroid * small[255];
    //Asteroid ** small = malloc(asteroids_len * sizeof (Asteroid *));
    for (int j = 0; j < asteroids_len; j++) {
        small[j] = create_asteroid(SCREEN_H, SCREEN_W, 1);
    }
    int scount = 0;

    al_flip_display();



    al_start_timer(timer);

    while (!doexit) {
        counter++;
        int won = check_won(big, small, asteroidb_len, asteroids_len);
        if (won == 1) {
            for (int j = 0; j < asteroidb_len; j++) {
                //kill_asteroid(big[j]);
                free(big[j]);
            }

            for (int j = 0; j < asteroids_len; j++) {
                free(small[j]);
                //kill_asteroid(small[j]);
            }
            level++;
            asteroidb_len += 4;
            asteroids_len += 8;
            //big = (Asteroid **) realloc(big, (asteroidb_len) * sizeof (Asteroid *));
            //small = (Asteroid **) realloc(small, (asteroidb_len) * sizeof (Asteroid *));
            

            for (int j = 0; j < asteroids_len; j++) {
                small[j] = create_asteroid(SCREEN_H, SCREEN_W, 1);
            }
            

            for (int j = 0; j < asteroidb_len; j++) {
                big[j] = create_asteroid(SCREEN_H, SCREEN_W, 0);
            }
            
            //puts("Lol");

            s.sx = (SCREEN_W / 2 + 8);
            s.sy = (SCREEN_H / 2 + 8);
            s.speed = 0.5;
            s.heading = 0;
            counter = 0;
            scount = 0;
        }

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {


            if (key[KEY_UP]) {
                s.speed+=0.3;
                if(s.speed == 0)
                s.speed=0.3;

            }

            if (key[KEY_DOWN]) {
                s.speed-=0.3;
                if(s.speed == 0)
                s.speed= -0.3;
            }

            if (key[KEY_RIGHT]) {
                if (s.heading < 358)
                    s.heading += 2;
                else
                    s.heading = 0;
            }

            if (key[KEY_LEFT]) {
                if (s.heading > 0)
                    s.heading -= 2;
                else
                    s.heading = 358;
            }

            if (key[KEY_UP] || key[KEY_DOWN]) {
                heading = s.heading;
            }

            if (key[KEY_SPACE]) {

                if (c[shotcounter].gone == 1) {
                    c[shotcounter].gone = 0;
                    fire(&s, &c[shotcounter]);
                    shotcounter++;
                }

                if (shotcounter >= (sizeof (c) / sizeof (c[0]))) {
                    shotcounter = 0;
                }

            }

            move_ship(&s, SCREEN_H, SCREEN_W, heading);


            for (int j = 0; j < asteroidb_len; j++) {
                move_asteroid(big[j], SCREEN_H, SCREEN_W);
            }

            for (int j = 0; j < asteroids_len; j++) {
                move_asteroid(small[j], SCREEN_H, SCREEN_W);
            }

            for (int i = 0; i < (sizeof (c) / sizeof (c[0])); i++) {
                move_blast(&c[i], SCREEN_H, SCREEN_W);
            }

            redraw = true;
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN&&kills < 4) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = true;
                    break;

                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = true;
                    break;

                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = true;
                    break;

                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = true;
                    break;

                case ALLEGRO_KEY_SPACE:
                    key[KEY_SPACE] = true;
                    break;
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP&&kills < 4) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = false;
                    break;

                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = false;
                    break;

                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = false;
                    break;

                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = false;
                    break;

                case ALLEGRO_KEY_SPACE:
                    key[KEY_SPACE] = false;
                    break;

                case ALLEGRO_KEY_ESCAPE:
                    doexit = true;
                    break;
            }
        }else if((ev.type == ALLEGRO_EVENT_KEY_UP)&&(kills == 4)){
            if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
            kills=0;
            score=0;
            level=1;
            heading = 0;
            s.sx = (SCREEN_W / 2 + 8);
            s.sy = (SCREEN_H / 2 + 8);
            s.speed = 0.5;
            s.heading = 0;
            counter = 0;
            scount=0;
            
            for (int j = 0; j < asteroidb_len; j++) {
                free(big[j]);
                //kill_asteroid(big[j]);
            }

            for (int j = 0; j < asteroids_len; j++) {
                free(small[j]);
                //kill_asteroid(small[j]);
            }
            asteroidb_len = 4;
            asteroids_len = 8;
            
            for (int j = 0; j < asteroids_len; j++) {
                small[j] = create_asteroid(SCREEN_H, SCREEN_W, 1);
            }

            for (int j = 0; j < asteroidb_len; j++) {
                big[j] = create_asteroid(SCREEN_H, SCREEN_W, 0);
            }
            
            }
        }



        for (int i = 0; i < asteroidb_len; i++) {
            if ((bounding_box_collision(big[i]->sx, big[i]->sy, 40, 40, s.sx, s.sy, 14, 20) == 1) && (counter > 200) && (big[i]->gone == 0) && (kills <4)) {
                s.sx = (SCREEN_W / 2 + 8);
                s.sy = (SCREEN_H / 2 + 8);
                s.speed = 0.5;
                s.heading = 0;
                heading = 0;
                counter = 0;
                kills++;
            }
            for (int j = 0; j < (sizeof (c) / sizeof (c[0])); j++) {
                if ((bounding_box_collision(c[j].sx, c[j].sy, 1, 14, big[i]->sx, big[i]->sy, 40, 40) == 1)&& (big[i]->gone == 0)) {
                    if (c[j].gone == 0) {
                        c[j].gone = 1;
                        score += 100;
                        big[i]->gone = 1;
                        small[scount]->sx = big[i]->sx;
                        small[scount]->sy = big[i]->sy + 100;
                        small[scount]->gone = 0;
                        scount++;
                        small[scount]->sx = big[i]->sx;
                        small[scount]->sy = big[i]->sy - 100;
                        small[scount]->gone = 0;
                    }
                }
            }
        }

        for (int i = 0; i < asteroids_len; i++) {
            if ((bounding_box_collision(small[i]->sx, small[i]->sy, 20, 20, s.sx, s.sy, 14, 20) == 1) && (counter > 200) && (small[i]->gone == 0) && (kills <4)) {
                s.sx = (SCREEN_W / 2 + 8);
                s.sy = (SCREEN_H / 2 + 8);
                s.speed = 0.5;
                s.heading = 0;
                counter = 0;
                heading = 0;
                kills++;
            }
            for (int j = 0; j < (sizeof (c) / sizeof (c[0])); j++) {
                if ((bounding_box_collision(c[j].sx, c[j].sy, 1, 14, small[i]->sx, small[i]->sy, 40, 40) == 1)&& (small[i]->gone == 0)) {
                    if (c[j].gone == 0) {
                        c[j].gone = 1;
                        score += 100;
                        small[i]->gone = 1;
                    }
                }
            }
        }





        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));


            for (int j = 0; j < asteroidb_len; j++) {
                draw_asteroid(big[j]);
            }

            for (int j = 0; j < asteroids_len; j++) {
                draw_asteroid(small[j]);
            }
            draw_ship(&s);
            for (int i = 0; i < (sizeof (c) / sizeof (c[0])); i++) {
                draw_blast(&c[i]);
            }
            draw_environment(font1, font2, score, kills, level);
            al_flip_display();
        }
    }
    
    al_destroy_timer(timer);



    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    for (int j = 0; j < asteroidb_len; j++) {
        //kill_asteroid(big[j]);
        free(big[j]);
    }

    for (int j = 0; j < asteroids_len; j++) {
        free(small[j]);
       // kill_asteroid(small[j]);

    }



    return 0;
}