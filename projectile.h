#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "projectile.h"
#include "bounding_box.h"
#include "../inc/ST7735.h"
#include <cmath>
class projectile{
    private:
        int32_t start_x, start_y;
        int32_t prev_x, prev_y;
        int32_t end_x, end_y;
        int32_t vy;
        int32_t steps;
        uint32_t counter;
        int32_t dx, dy;
        uint16_t sprite [9];
        int32_t h, w;
        bool user;

    public:
        bounding_box b1;
        bool end;
        // update the velcoties and display the new sprite position. 
        void update_position();
        projectile();
        projectile (int, int, int, int, int, bool); // start positions end positions and velocities
        ~projectile();
};
#endif