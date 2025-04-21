
#include <cstdint>
#include <ti/devices/msp/msp.h>
#include "aliens.h"
#include "images/images.h"

void aliens::set_speed(int gyro_y, bool turbo1, bool turbo2)
{
    // gyro comparisons here 
    if (gyro_y > 240) vy = -1;
    else if (gyro_y <= 240 && gyro_y > 180) vy = -2;
    else if (gyro_y <= 180) vy= -3;
    else vy= -1; // setting the speeds here for the asteroids based on gyro input

    if (turbo1 || turbo2) vy--;

    y += vy;
}

void aliens::update_position()
{
    // draw the asteroid here with the new y position
    ST7735_DrawBitmap(x, y, sprite, 24, 20);
}

void aliens::shoot()
{
    if (freq_counter % 10 == 0)
    {
        // TODO nooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    //      if (a1 == NULL)
    //     {
    //         a1 = new projectile(x+13, y - 27, x + 13, , 4, true);
    //     }
    // }
    // if (w1 != NULL)
    // {
    //     w1->update_position();
    //     if (w1->end)
    //     {
    //         delete w1;
    //         w1 = NULL;
    //     }
    // }
    }

}

aliens::aliens()
{
    x = 0;
    y = 0;
    vy = 0;
    // set sprite and boundary box
    freq_counter = 0;

    for (int i = 0; i < 480; i++)
    {
        sprite[i] = (uint16_t) redAlien[i];
    }
    b1 = bounding_box(x +1, y - 19, 22, 18);
    a1 = NULL;

}

aliens::aliens(int start_x)
{
    x = start_x;
    y = 0;
    vy = 0;
    freq_counter = 0;
    for (int i = 0; i < 480; i++)
    {
        sprite[i] = (uint16_t) redAlien[i];
    }
    b1 = bounding_box(x +1, y - 19, 22, 18);
    a1 = NULL;
    // set sprite and boundary box
}