#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160
#define ENEMY_COUNT 3 // 3 spawn zones. max of three obstacles being spawned at once. 
#define EASY_COUNTER 45
#define MED_COUNTER 30
#define HARD_COUNTER 20


void initialize_enemies();
void draw_enemies();
void update_enemies(int, bool, bool);


#endif