#include "Stars.h"

// Global arrays for star positions
int stillStars[28] = {
    34, 120,  72, 45,  112, 90,  50, 130,  15, 25,
    98, 60,  3, 140,  120, 30,  65, 100,  10, 55,
    120, 154, 100, 132, 17, 80,  93, 93
};

int slowstars[20] = {
    28, 50,  120, 135,  99, 30,  5, 120,  63, 75,
    110, 20,  80, 155,  45, 90,  10, 50,  77, 40
};

int fastStars[20] = {
    12, 145,  104, 60,  56, 100,  31, 30,  68, 55,
    90, 25,  23, 140,  110, 80,  80, 160,  50, 20
};

int oldslowstars[20];
int oldfastStars[20];

// Function to initialize star positions
void initializeStars() {
    for (int i = 0; i < STAR_COUNT * 2; i++) {
        oldslowstars[i] = slowstars[i];
        oldfastStars[i] = fastStars[i];
    }
}

// Function to draw stars
void drawStars() {
    for (int i = 0; i < STAR_COUNT; i++) {
        ST7735_DrawPixel(oldslowstars[i*2], oldslowstars[i*2 + 1], 0x0000);
        ST7735_DrawPixel(oldfastStars[i*2], oldfastStars[i*2 + 1], 0x0000);
    }
    
    for (int i = 0; i < 14; i++) {
        ST7735_DrawPixel(stillStars[i*2], stillStars[i*2 + 1], 0xFFFF);
    }

    for (int i = 0; i < STAR_COUNT; i++) {
        ST7735_DrawPixel(slowstars[i*2], slowstars[i*2 + 1], 0xFFFF);
        ST7735_DrawPixel(fastStars[i*2], fastStars[i*2 + 1], 0xFFFF);
    }
}

// Function to update star positions
void updateStars() {
    for (int i = 0; i < STAR_COUNT; i++) {
        oldslowstars[i*2] = slowstars[i*2];
        oldslowstars[i*2 + 1] = slowstars[i*2 + 1];
        oldfastStars[i*2] = fastStars[i*2];
        oldfastStars[i*2 + 1] = fastStars[i*2 + 1];

        fastStars[i*2 + 1] += 3;
        slowstars[i*2 + 1] += 1;

        if (fastStars[i*2 + 1] > SCREEN_HEIGHT) {
            fastStars[i*2 + 1] = 0;
            fastStars[i*2] = rand() % SCREEN_WIDTH;
        }
        if (slowstars[i*2 + 1] > SCREEN_HEIGHT) {
            slowstars[i*2 + 1] = 0;
            slowstars[i*2] = rand() % SCREEN_WIDTH;
        }
    }
}