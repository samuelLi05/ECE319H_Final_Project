#include "bounding_box.h"
#include <stdint.h>

bool bounding_box::collision(bounding_box& other)
{
    int left_x = x;
    int right_x = x + w;
    int top_y = y;
    int bot_y = y + h;

    int other_left_x = other.get_x();
    int other_right_x = other_left_x + other.get_w();
    int other_top_y = other.get_y();
    int other_bot_y = other_top_y + other.get_h();

     return !(right_x < other_left_x || 
             left_x > other_right_x || 
             bot_y < other_top_y || 
             top_y > other_bot_y);

}

bool bounding_box::collision_left(bounding_box& other)
{
    int left_x = x;
    int right_x = x + w;
    int top_y = y;
    int bot_y = y + h;

    int other_left_x = other.get_x();
    int other_right_x = other_left_x + other.get_w();
    int other_top_y = other.get_y();
    int other_bot_y = other_top_y + other.get_h();

    bool collision = !(right_x < other_left_x || 
             left_x > other_right_x || 
             bot_y < other_top_y || 
             top_y > other_bot_y);

    if (collision)
    {
        if (other_right_x > left_x && other_left_x < left_x) return true;
    }

    return false;
}

bool bounding_box::collision_right(bounding_box& other)
{
    int left_x = x;
    int right_x = x + w;
    int top_y = y;
    int bot_y = y + h;

    int other_left_x = other.get_x();
    int other_right_x = other_left_x + other.get_w();
    int other_top_y = other.get_y();
    int other_bot_y = other_top_y + other.get_h();

    bool collision = !(right_x < other_left_x || 
             left_x > other_right_x || 
             bot_y < other_top_y || 
             top_y > other_bot_y);

    if (collision)
    {
        if (other_left_x < right_x && other_right_x > right_x) return true;
    }

    return false;
}

bounding_box::bounding_box()
{
    x = 0;
    y = 0;
    w = 0;
    h = 0;
}

bounding_box::bounding_box(int inc_x, int inc_y, int inc_w, int inc_h)
{
    x = inc_x;
    y = inc_y;
    w = inc_w;
    h = inc_h;
}

bounding_box::bounding_box(bounding_box& other)
{
    x = other.get_x();
    y = other.get_y();
    w = other.get_w();
    h = other.get_h();
}

uint32_t bounding_box::get_x() {return x;}
uint32_t bounding_box::get_y() {return y;}
uint32_t bounding_box::get_w() {return w;}
uint32_t bounding_box::get_h() {return h;}