#include <cmath>
#include <ti/devices/msp/msp.h>
#include "projectile.h"
#include "bounding_box.h"
#include "images/images.h"


void projectile::update_position()
{
    if (!user)
    {
        start_y += vy;
        ST7735_DrawBitmap(start_x, start_y,sprite, 3, 3);
    } else
    {
        if (counter <= steps)
        {
            start_x = start_x + (int)(counter * ((float)dx / (float)steps));
            start_y = start_y + (int)(counter * ((float)dy / (float)steps));
            counter++;
            b1 = bounding_box(start_x - 1, start_y + 3, 4, 4);
            ST7735_DrawBitmap(prev_x, prev_y, Black3x3, 3, 3);
            ST7735_DrawBitmap(start_x, start_y, sprite, 3, 3);
            prev_x = start_x;
            prev_y = start_y;
        }
        else {
            ST7735_DrawBitmap(start_x, start_y,Black3x3, 3, 3); // then desttruct this here. Get rid of bounding box
            end = true;
        }
        
    }
}

projectile::projectile()
{
    start_x = 0;
    start_y = 0;
    prev_x = 0;
    prev_y = 0;
    end_x= 0; 
    end_y = 0;
    vy = 0;
    // set spritt
    for (int i = 0; i < 6; i++)
    {
        sprite[i] = (uint16_t)user_projectile[i];
    }
    h= 3;
    w = 2;
    user = true;
    end = false;
    b1 = bounding_box(start_x - 1, start_y + 3, 4, 4);
}

projectile::projectile(int s_x, int s_y, int e_x, int e_y, int v_y, bool select)
{
    start_x = s_x;
    start_y = s_y;
    end_x = e_x;
    end_y = e_y;
    prev_x = 0;
    prev_y = 0;
    vy = v_y;
    user = select;
    dx = end_x - start_x;
    dy = end_y - start_y;
    float length = sqrt(dx*dx + dy*dy);
    steps = length / 2;
    counter = 0;
    end = false;
    if (user)
    {
        for (int i = 0; i <9; i++)
        {
            sprite[i] = (uint16_t)user_projectile[i];
        }
    } else 
    {
        for (int i = 0; i < 9; i++)
        {
            sprite[i] = (uint16_t)enemy_projectile[i];
        }
    }
    b1 = bounding_box(start_x - 1, start_y + 3, 4, 4);
}

projectile::~projectile()
{
    // nothing allocated on the heap. 
}

