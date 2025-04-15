#ifndef STARS_H
#define STARS_H

#include <stdlib.h>
#include <stdint.h>
#include "ST7735.h"  // Display library

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160
#define STAR_COUNT 10

// Function declarations
void initializeStars();
void drawStars();
void updateStars();

#endif // STARS_H