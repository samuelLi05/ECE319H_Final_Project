// Player method implementation

#include "Player.h"
#include "bounding_box.h"
#include "images/images.h"
#include <cstdint>


void Player::gyro_controls(int gyro_x, int gyro_y)
{
    // x controls
    if (gyro_x < 30 && gyro_x > -30) vx = 0;
    else if (gyro_x >= 30 && gyro_x < 40) vx = 1;
    else if (gyro_x <= -30 && gyro_x > -40) vx = -1;
    else if (gyro_x >= 40 && gyro_x < 70) vx = 2;
    else if (gyro_x <= -40 && gyro_x > -70) vx = -2;
    else if (gyro_x >= 70 && gyro_x < 100) vx = 3;
    else if (gyro_x <= -70 && gyro_x > -100) vx = -3;
    else if (gyro_x >= 100) vx = 4;
    else if (gyro_x <= -100) vx = -4;
    else vx = 0;

    x += vx;
    if (x < 0) x = 0;
    if (x > 103) x = 103;

    if (gyro_y > 240) vy = 1;
    else if (gyro_y <= 240 && gyro_y > 180) vy = 0;
    else if (gyro_y <= 180) vy= -1;
    else gyro_y = 0;

    // adjust y movement frequency
    freq_count++;
    if (freq_count % 1 == 0)
    {
        y += vy;
    if (y > 160) y = 160;
    if (y < 50) y = 50;
    }
}
void Player::weapon_switch(int)
{

}

void Player::draw_player()
{
    ST7735_DrawBitmap(x, y, sprite, w, h);
}
// gloabls to be used for slide_pot data tansformation
const int32_t  in_min = 142;
const int32_t in_max = 1800;
const uint8_t out_min = 0;
const uint8_t out_max = 123;
const uint8_t cursor_y = 10;
void Player::update_cursor(int value)
{
    // convert slide_pot distance to horizontal pixel readings. 0.142 to 1.8 cm
    // range 0 - 123. Translate linearly
    // convert slide_pot distance to horizontal pixel readings using integer math
    if (value <= in_min) {
        cursor_position = out_min;
    }
    else if (value >= in_max) {
        cursor_position = out_max;
    }
    else {
        // Scale factor multiplied by 1000 to simulate floating point
        const int32_t scale = (out_max - out_min) * 1000 / (in_max - in_min); // 123 * 1000 / (1800 - 142)
        int32_t scaled = (value - in_min) * scale;   // scaled is now in 1/1000ths
        cursor_position = (scaled + 500) / 1000;     
    }

    aim_x = cursor_position + 2;
    aim_y = cursor_y + 2;
}

void Player::draw_cursor()
{
    ST7735_DrawBitmap(prev_cursor_position, cursor_y,aim_black, 5, 5);
    ST7735_DrawBitmap(cursor_position, cursor_y, aim_cursor, 5, 5);
    prev_cursor_position = cursor_position;
}
Player::Player()
{
    x = 0;
    y= 80;
    vx = 0;
    vy = 0;
    h = 27;
    for (int i = 0; i < 810; i++) {
        sprite[i] = (uint16_t)userBlueSpaceship[i];  // Cast for clarity
    }
    for (int i = 0; i < 25; i++)
    {
        aim_cursor[i] = (uint16_t)RedCross5x5[i];
        aim_black[i] = (uint16_t)Black5x5[i];
    }
    w = 30;
    weapon_select =0;
    freq_count = 0;
    freq_mod = 2;
    b1[0] = bounding_box(x + 2, y + 3, 23, 5); // back
    b1[1] = bounding_box(x + 10, y + 8, 7, 12); // center
    b1[2] = bounding_box(x + 14, y + 20, 3, 5); // nose
    prev_cursor_position = 0;
}
