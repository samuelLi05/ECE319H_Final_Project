
#include <ti/devices/msp/msp.h>
#include "asteroids.h"
#include "bounding_box.h"
#include "images/images.h"


void asteroids::set_speed(int gyro_y, bool turbo1, bool turbo2)
{
    // gyro comparisons here 
    if (gyro_y > 240) vy = 1;
    else if (gyro_y <= 240 && gyro_y > 180) vy =1;
    else if (gyro_y <= 180) vy=2;
    else vy= -1; // setting the speeds here for the asteroids based on gyro input

    if (turbo1 || turbo2) vy++;

    if (!user_collided)
    {
        if (freq_counter % 1 == 0) y += vy;
        freq_counter++;
        if (y >= 200) end = true;
    }

    b1 = bounding_box(x, y - 28, 30, 27);
}

void asteroids::update_position()
{
    // draw the asteroid here with the new y position
    ST7735_DrawBitmap(x, y, bigAsteroidFast, 27, 32);
}

asteroids::asteroids()
{
    x = 0;
    y = 0;
    vy = 0;
    // set sprite and boundary box
    b1 = bounding_box(x, y - 28, 30, 27);
    freq_counter = 0;
    end = false;
    user_collided  = false;
    proj_collided = false;

}

asteroids::asteroids(int start_x)
{
    x = start_x;
    y = 0;
    vy = 0;
    freq_counter = 0;
    b1 = bounding_box(x, y - 28, 30, 27);
    end = false;
    user_collided = false;
    proj_collided = false;
    // set sprite and boundary box
}

asteroids::~asteroids()
{
    ST7735_DrawBitmap(x, y, radial_clear, 43, 37);
}