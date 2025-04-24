#include "enemy_spawner.h"
#include <stdlib.h>
#include <stdint.h>
#include "asteroids.h"
#include "aliens.h"


// define enemy global sections for asteroids and alines if needed.  have the bounding boxes for the enemies for collision detection
// global zones to define x positions
int which_zones[] = {1, 2, 3, 3, 1, 2, 3, 2, 1, 2, 3, 2, 1, 2, 3, 3, 2,1, 3, 1, 3}; // length 20
int zone1 [] = {3, 14, 19, 6, 8, 1, 13, 0, 18, 10, 11, 15, 9, 5, 20, 12, 4, 2, 16, 17, 7};
int zone2 [] = {53, 61, 60, 59, 47, 54, 44, 50, 46, 48, 58, 55, 52, 43, 56, 62, 49, 45, 57, 51, 42};
int zone3 [] = {101, 88, 91, 103, 100, 89, 105, 86, 99, 93, 106, 102, 87, 92, 98, 95, 104, 94, 96, 97, 90};

uint32_t freq_counter = 0;

// Globals

//global variables here. The 3 asteroids that can be spawned in. 
asteroids* spawn1 = NULL;
asteroids* spawn2 = NULL;
asteroids* spawn3 = NULL;
// spawn max of certain value, boolean flagging to determine whether we can spawn in. use frequency_counter and spawn flags


int s1 = 0;
int s2 = 0;
int s3 = 0;

int z = 0;

void initialize_enemies(){
    // select spots from each zone to spawn an enemy at that x position. 
    if (freq_counter % HARD_COUNTER == 0)
    {
        int which_zone = which_zones[z]; // dumb random functions. random number funcitons
        z++;
        int spawn_position;
        if (which_zone == 1)
        {
            spawn_position = zone1[s1];
            s1++;
        }
        else if (which_zone == 2)
        {
            spawn_position = zone2[s2];
            s2++;
        }
        else if (which_zone == 3)
        {
            spawn_position = zone3[s3];
            s3++;
        }

        if (which_zone == 1 && spawn1 == NULL)
        {
            spawn1 = new asteroids(spawn_position);
        } else if (which_zone == 2 && spawn2 == NULL)
        {
            spawn2 = new asteroids(spawn_position);
        } else if (which_zone ==3  && spawn3 == NULL)
        {
            spawn3 = new asteroids(spawn_position);
        }

        if (s1 == 21) s1= 0;
        if (s2 == 21) s2 =0;
        if (s3 == 21) s3 = 0;
        if (z == 21) z = 0;
    }
    freq_counter++;
    // check if there are pre-existing enemies in the three zones we want to spawn into 

}
void draw_enemies()
{
    // draw function for the enmeies
    if (spawn1 != NULL)
    {
        spawn1->update_position();
    }
    if (spawn2 != NULL)
    {
        spawn2->update_position();
    }
    if (spawn3 != NULL)
    {
        spawn3->update_position();
    }

}
void update_enemies(int gyro_y, bool turbo1, bool turbo2)
{
    // update the position with asteroid and alien update positions fuctions
     if (spawn1 != NULL)
    {
        spawn1->set_speed(gyro_y, turbo1, turbo2);
    }
    if (spawn2 != NULL)
    {
        spawn2->set_speed(gyro_y, turbo1, turbo2);
    }
    if (spawn3 != NULL)
    {
        spawn3->set_speed(gyro_y, turbo1, turbo2);
    }
    // if reach the end of the screen or collision, delete the pointers of these objects. deleting would set these back to NULL. Destructors?
    // probably a class end flag variale to be used here. 
    if (spawn1->end)
    {
        delete spawn1;
        spawn1 = NULL;
    }
    if (spawn2->end)
    {
        delete spawn2;
        spawn2 = NULL;
    }
    if (spawn3->end)
    {
        delete spawn3;
        spawn3 = NULL;
    }
    if (spawn1->proj_collided)
    {
        delete spawn1;
        spawn1 = NULL;
    }
    if (spawn2->proj_collided)
    {
        delete spawn2;
        spawn2 = NULL;
    }
    if (spawn3->proj_collided)
    {
        delete spawn3;
        spawn3 = NULL;
    }
} 