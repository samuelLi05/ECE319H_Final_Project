// Player method implementation

#include "Player.h"
#include "bounding_box.h"
#include "images/images.h"
#include "projectile.h"
#include <cstddef>
#include <cstdint>


extern void soundPlayShoot();
extern void soundPlayRadial();
extern uint32_t index;

extern bool explosion;
extern int32_t explosion_sequence;
extern int32_t explosion_x;
extern int32_t explosion_y;

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

    // selecting if we can add in a direction if a collision has happened, -x means left collision and +x means right collision.
    // use to stop movement
    if (left_collided && !right_collided)
    {
        if (vx < 0) vx = 0;
    } else if (!left_collided && right_collided)
    {
        if (vx > 0) vx = 0;
    }
    x += vx;
    if (x < 0) x = 0;
    if (x > 103) x = 103;

    // enable y movement with turbo button, y controls should mainly change obstacle speed. Player controls. Use gyro for opponent speeds.
    // set score adders here x. 
    if (gyro_y > 240){
        if (y > 150) score_adder +=0;
        else score_adder += 1;
        vy = 1;
    }
    else if (gyro_y <= 240 && gyro_y > 180)
    {
        vy = 0;
        score_adder += 2;
    } 
    else if (gyro_y <= 180){
        vy= -1;
        score_adder += 5;
    }
    else vy = 0;

    //measure score here TODO LOLLLL. THIS IS IMPORTANT. Dependent on y velocity which controls enemy speeds and turbo. Depndent on y gyro and turbo
    // use to acess distance that is travled by the user.

    // adjust y movement frequency
    if (!collided)
    {
        uint8_t mod_amt = 0;
        if (turbo1 ^ turbo2)
        {
            if (turbo1){
                mod_amt =2;
                score_adder += 3;
            } 
            else if (turbo2){
                mod_amt = 1;
                score_adder += 8;
            } 
            freq_count++;
            if (freq_count % mod_amt == 0)
            {
                if (!radial_happening) y += vy;
                if (y > 160) y = 160;
                if (y < 50) y = 50;
            }
        }
        if (update_score_counter == 15)
        {
            score += score_adder;
            score_adder = 0;
            update_score_counter = 0;
        }
        update_score_counter++;
    } else if (collided && !radial_happening) // if radial is happening, we grant some immunity for score decreases
    {
        // subtract the score here.
        if (collided != prev_collided)
        {
            score -= 500; 
            if (score < 0) score = 0;
        }
    }
    prev_collided = collided; // width 30, height 27
    b1[0] = bounding_box(x + 2, y -8 , 26, 5); // back
    b1[1] = bounding_box(x + 10, y -19, 10, 12); // center
    b1[2] = bounding_box(x + 14, y -23 , 3, 5); // nose
}


void Player::draw_player()
{
    if (!radial_happening)
    {
        ST7735_DrawBitmap(x, y, sprite, w, h);
    }
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
    aim_y = cursor_y - 2;
}

void Player::shoot()
{ 
    // only allowing a single projectile at a time. This or could use game clock timer
    if (weapon1 && !weapon2)
    {
        if (w1 == NULL && shoot_counter >= 0) // set explicit cooldown here
        {
            w1 = new projectile(x+13, y - 27, aim_x, aim_y, 4, true);
            soundPlayShoot();
            shoot_counter = -40;
        }
    }
    shoot_counter++;
    if (w1 != NULL)
    {
        w1->update_position();
        if (w1->collided)
        {
            delete w1;
            w1 = NULL;
        }
        if (w1->end)
        {
            delete w1;
            w1 = NULL;
        }
    }
}

// array of possible positions. 2 asteroids and two aliens.  can spawn in these locations. randomize location and number of insertions made

void Player::radial()
{
    // draw the radial animation. will need frequency counter 
    if (!weapon1 && weapon2)
    {
        if (radial_counter == 0) soundPlayRadial();
        if (radial_counter < 15 && radial_counter >= 0)
        {
            // display the first sprite
            radial_happening = true;
            ST7735_DrawBitmap(x-7, y + 7, smallRadialWeapon, 43, 37);
        } else if (radial_counter >= 15 && radial_counter < 30)
        {
            // display the second sprite
            radial_happening = true;
            ST7735_DrawBitmap(x-7, y+7, medRadialWeapon, 43, 37);
        }
        else if (radial_counter >= 30 and radial_counter < 45)
        {
            // display the third sprite
            radial_happening = true;
            ST7735_DrawBitmap(x-7, y +7, largeRadialWeapon, 43, 37);
        } else if (radial_counter >= 45) {
            radial_counter = -60; // 2 second delay to not abuse this weapon. Collision checking here with temporary sprite???
            ST7735_DrawBitmap(x-7, y+ 7, radial_clear, 43, 37);
            radial_happening = false;
        }
        radial_counter++;
        // if a weapon start has not specified, start here 
    } else if (radial_counter > 0)
    {
        ST7735_DrawBitmap(x-7, y+10, radial_clear, 43, 37); //  clear the spot
        radial_counter = -120;
        radial_happening = false;
    } else
    {
        radial_happening = false;
    }
    

}

void Player::draw_cursor()
{
    ST7735_DrawBitmap(prev_cursor_position, cursor_y,aim_black, 5, 5);
    ST7735_DrawBitmap(cursor_position, cursor_y, aim_cursor, 5, 5);
    prev_cursor_position = cursor_position;
}

void Player::boom()
{
    if (explosion)
      {
        if (explosion_sequence < 15) ST7735_DrawBitmap(explosion_x, explosion_y, explosionStage3, 32, 32);
        if (explosion_sequence >= 15 &&  explosion_sequence < 30) ST7735_DrawBitmap(explosion_x, explosion_y, explosionStage5, 32, 32);
        explosion_sequence++;
        if (explosion_sequence >= 30)
        {
          explosion = false;
          explosion_sequence = 0;
        }
      }
}

void Player::recieveButton(bool t1, bool t2, bool w1, bool w2)
{
    turbo1 = t1;
    turbo2 = t2;
    weapon1 = w1;
    weapon2 = w2;
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
    // b1[0] = bounding_box(x + 2, y + 3, 23, 5); // back
    // b1[1] = bounding_box(x + 10, y + 8, 7, 12); // center
    // b1[2] = bounding_box(x + 14, y + 20, 3, 5); // nose
    b1[0] = bounding_box(x + 2, y -8 , 26, 5); // back
    b1[1] = bounding_box(x + 10, y -19, 10, 12); // center
    b1[2] = bounding_box(x + 14, y -23 , 3, 5); // nose
    prev_cursor_position = 0;
    score = 0; // update the score basedon the time of the game. Have time counter based on number on velocity and frequency. 
    // Eclipse this thershold before a certain number of sensor readings at 30 HZ. 

    turbo1 = false;
    turbo2= false;
    weapon1 = false;
    weapon2 = false;
    w1 = NULL;
    radial_counter = 0;
    radial_happening = false;
    collided = false;
    shoot_counter = 0;
    prev_collided = false;
    update_score_counter = 0;
    score_adder = 0;
    left_collided = false;
    right_collided = false;
}
