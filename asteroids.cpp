
#include <ti/devices/msp/msp.h>
#include "asteroids.h"
#include "bounding_box.h"
#include "images/images.h"


void asteroids::set_speed(int gyro_y, bool turbo1, bool turbo2)
{
    // gyro comparisons here 
    if (gyro_y > 240) vy = -1;
    else if (gyro_y <= 240 && gyro_y > 180) vy = -2;
    else if (gyro_y <= 180) vy= -3;
    else vy= -1; // setting the speeds here for the asteroids based on gyro input

    if (turbo1 || turbo2) vy--;

    y += vy;
}

void asteroids::update_position()
{
    // draw the asteroid here with the new y position
    ST7735_DrawBitmap(x, y, sprite, 31, 29);
}

asteroids::asteroids()
{
    x = 0;
    y = 0;
    vy = 0;
    // set sprite and boundary box
    for (int i = 0; i < 899; i++)
    {
        sprite[i] = (uint16_t)bigAsteroid[i];
    }

    b1 = bounding_box(x + 1, y - 28, 29, 27);
    freq_counter = 0;

}

asteroids::asteroids(int start_x)
{
    x = start_x;
    y = 0;
    vy = 0;
    freq_counter = 0;
    for (int i = 0; i < 899; i++)
    {
        sprite[i] = (uint16_t)bigAsteroid[i];
    }
    b1 = bounding_box(x + 1, y - 28, 29, 27);
    // set sprite and boundary box
}