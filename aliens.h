#ifndef ALIENS_H
#define ALIENS_H

#include <cstdint>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "images/images.h"
#include "../inc/ST7735.h"
#include "bounding_box.h"
#include "projectile.h"

class aliens
{
    private: 
        uint32_t x, y;
        uint16_t sprite [480];
        int32_t vy;
        uint32_t h, w;
        uint32_t freq_counter; // use for shoot timing and such 
    public: 
        bounding_box b1;
        projectile* a1;
        void set_speed(int, bool, bool);
        void update_position();
        void shoot();
        // projectile class probably
        aliens();
        aliens(int);
};


#endif