#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include <cstdint>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "images/images.h"
#include "../inc/ST7735.h"
#include "bounding_box.h"


class asteroids
{
    private: 
        uint32_t x, y;
        int32_t vy;
        uint32_t h, w;
        uint32_t freq_counter;
    public: 
        bounding_box b1;
        bool end;
        bool user_collided;
        bool proj_collided;
        void set_speed(int, bool, bool);
        void update_position();
        asteroids();
        asteroids(int);
        ~asteroids();
};


#endif