#ifndef PLAYER_H
#define PLAYER_H

#include <cstdint>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "images/images.h"
#include "../inc/ST7735.h"
#include "bounding_box.h"
#include "projectile.h"


class Player{
    private: 
        int32_t x, y;
        int16_t vx, vy;
        uint16_t sprite [810];
        uint32_t h, w;
        uint32_t weapon_select;
        int8_t freq_count;
        int8_t freq_mod;
        int8_t radial_counter;
        uint16_t aim_cursor [25];
        uint16_t aim_black [25];
        uint32_t cursor_position;
        uint32_t prev_cursor_position;
        int32_t aim_x, aim_y; // used for 
        int8_t shoot_counter;
        uint8_t update_score_counter;
        // define cursor sprite??
        bool turbo1, turbo2, weapon1, weapon2;
        bool prev_collided;

        int32_t score_adder;
        


        public:
            bounding_box b1 [3]; // for collision detection later on 
            projectile* w1;
            bool collided;
            int32_t score;
            bool radial_happening;
            bool left_collided;
            bool right_collided;
            void gyro_controls(int, int);
            void shoot();
            void radial();
            void draw_player();
            void update_cursor(int);
            void draw_cursor();
            void boom();
            void recieveButton(bool, bool, bool, bool);
            int player_speed();
            Player();
};

#endif