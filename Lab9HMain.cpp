// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Your name
// Last Modified: 12/26/2024

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/SlidePot.h"
#include "../inc/DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"
#include "../inc/I2C.h"
#include "Stars.h"
#include "bounding_box.h"
#include "Player.h"
#include "../inc/EdgeTriggered.h"
#include "enemy_spawner.h"
#include "asteroids.h"

extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
extern "C" void GROUP1_IRQHandler(void);
extern "C" void SysTick_Handler(void);

// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}


SlidePot Sensor(1500,0); // copy calibration from Lab 7
//Player p1; // default player in starting position


// button requests
bool weapon1 = false;
bool weapon2 = false;
bool turbo1 = false;
bool turbo2 = false;

extern asteroids* spawn1;
extern asteroids* spawn2;
extern asteroids* spawn3;

extern bool explosion;
extern int32_t explosion_sequence;
extern int32_t explosion_x;
extern int32_t explosion_y;

uint32_t index = 0;
uint32_t index2 = 0;
uint32_t groovy_index = 0;
int32_t global_timer = 3600; // seconds * 30 to get this global timer. 5400 is 3 minutes of time

bool ENGLISH_OR_SPANISH = true; // cuz english be cooler like that 

uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

void soundPlayShoot();
void soundPlayRadial();

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};

// testing why no work bruhhhhhhhhhhhhhhh. Working edge triggered interrupts and switches. 

uint32_t Count1, Count2, Count3, Count4;
int main_edge(void){
  __disable_irq();
  LaunchPad_Init();
  PLL_Init();
  Switch_Init();
  LED_Init();
  EdgeTriggered_Init();
  Count1=0; Count2 =0;Count3=0;Count4=0;
  __enable_irq();
  while(1){
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
  }
}

void GROUP1_IRQHandler(void){
  uint32_t stat;
  while((stat = GPIOA->CPU_INT.IIDX)!=0){
    if(stat == 25){ // PB21
      weapon1 = !weapon1; // number of touches
      GPIOB->DOUTTGL31_0 = RED; // toggle PB26, // set flags for the game. Delays based on. Set toggles
    } else if (stat == 26)
    {
      turbo2=!turbo2;
      GPIOB->DOUTTGL31_0 = RED;
    } else if (stat == 27)
    {
      turbo1 = !turbo1;
      GPIOB->DOUTTGL31_0 = RED;
    } else if (stat == 28)
    {
      weapon2 = !weapon2;
      GPIOB->DOUTTGL31_0 = RED;
    }
  }
}



// games  engine runs at 30Hz
// sensor input at a specific frequency
uint32_t slide_pot_data;
int16_t gyro_x, gyro_y, gyro_z; // global ram data variables 
uint32_t Time = 0;
void TIMG12_IRQHandler(void){
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    // sample
    slide_pot_data = Sensor.In();
    Sensor.Save(slide_pot_data);
    Sensor.Convert(slide_pot_data);
    adxl345_read_accel(&gyro_x, &gyro_y, &gyro_z); // using the slidepot semaphor for timer interrupts at slower frequency
    // store data into mailbox
    // set the semaphore
    global_timer--;
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}

void Sound_Init(uint32_t period, uint32_t priority){
  // write this
  SysTick->CTRL = 0x00;      // disable SysTick during setup
  SysTick->LOAD = period-1;  // reload value
  SCB->SHP[1] = (SCB->SHP[1]&(~0xC0000000));//|(priority<<30); // priority 2
  SysTick->VAL = 0;          // any write to VAL clears COUNT and sets VAL equal to LOAD
  SysTick->CTRL = 0x07;      // enable SysTick with 80 MHz bus clock and interrupts
}



void soundPlayShoot(void) {
    index = 0;  // Reset index for new sound playback
    SysTick->LOAD = 7256; // Set SysTick timer for playback rate
    SysTick->VAL = 0;  // Enable SysTick with interrupts
}

void soundPlayRadial(void) {
    index2 = 0;  // Reset index for new sound playback
    SysTick->LOAD = 7256; // Set SysTick timer for playback rate
    SysTick->VAL = 0;  // Enable SysTick with interrupts
}

void soundPlayGroovy(void)
{
  groovy_index = 0;
  SysTick->LOAD = 7256;
  SysTick->VAL = 0;
}

void soundStop(void){
  SysTick->LOAD = 0;
  groovy_index = 0;

}
//SysTick Sound Functions:
void SysTick_Handler(void) {
  if (weapon1 && !weapon2) { // Weapon 1: Shooting sound
      DAC5_Out(shoot[index]);  
      index++;
      if (index > 4800) {
          soundStop();
      }
  } 
  else if (!weapon1 && weapon2) { // Weapon 2: Radial sound
      DAC5_Out(iceball[index2]);  
      index2++;
      if (index2 > 16986) {
          soundStop();
      }
  } else {
    DAC5_Out(GalaxyGroove[groovy_index]);
    groovy_index++;
    if (groovy_index > 23195)
    {
      groovy_index = 0;
    }
  } 
}

int main(void)
{
  // player movement
  uint32_t global_score_goal = 25000;
  bool play_restart = true;
  // set up a timer. 
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  DAC5_Init();
  // SysTick->LOAD = 0xFFFFFF;    // max
  // SysTick->VAL = 0;            // any write to current clears it
  // SysTick->CTRL = 0x00000005;  // enable SysTick with core clock
  Sound_Init(1, 0);
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  Sensor.Init(); //PB18 = ADC1 channel 5, slidepot
  ST7735_FillScreen(ST7735_BLACK);
  //ST7735_PlotClear(0,2000);
  initializeStars();
  I2C_ADXL345_Init(0x53);
    // initialize interrupts on TimerG12 at 30 Hz
    TimerG12_Init();
    TimerG12_IntArm(2666666, 4);

     Switch_Init();
    LED_Init();
    EdgeTriggered_Init();

  Time = 0;
    int32_t switch_start = 0;
    while (switch_start != 1 && switch_start != 4)
    {
      ST7735_DrawBitmap(0, 159, LoadingEnglishSpanish, 128, 160);
      switch_start = Switch_In();
      if (switch_start == 1) ENGLISH_OR_SPANISH = true;
      else if (switch_start == 4) ENGLISH_OR_SPANISH = false;
    }
    ST7735_FillScreen(ST7735_BLACK);
  __enable_irq();
  while (play_restart)
  {
    Player p1;
    global_timer = 3600;
    index = 0;
    index2 = 0;
    groovy_index = 0;
    
    ST7735_FillScreen(ST7735_BLACK);
    soundPlayGroovy();
    int32_t percent = 0;
    // start screen until button press ocurrrs
    while(global_timer >= 0 && percent < 10000){
        // complete this
      // wait for semaphore using a call to a method in Sensor

      // display as a percentage

      Sensor.Sync();
      ST7735_SetCursor(0, 0); 
      percent = (10000 * p1.score) / global_score_goal;
      int32_t time_remaining= global_timer / 30;
      printf("%2d.%02d%%: %03d", percent/100, percent % 100, time_remaining);
      //ST7735_SetCursor(uint32_t newX, uint32_t newY)
    
      GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
      GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
      // toggle red LED2 on Port B, PB26

      // convert Data to Position
      //uint32_t Position = Sensor.Convert(slide_pot_data);

      // move cursor to top
      // display distance in top row OutFix
      drawStars();
      updateStars();
      p1.recieveButton(turbo1, turbo2, weapon1, weapon2);
      p1.gyro_controls(gyro_x, gyro_y);
      p1.draw_player();
      uint32_t Position = Sensor.Convert(slide_pot_data);
      p1.update_cursor(Position);
      p1.draw_cursor();
      initialize_enemies();
      draw_enemies();
      update_enemies(gyro_y, turbo1, turbo2);
      p1.shoot();
      //p1.boom();

      p1.radial();
      // probably need to pass in a stop flag for the player and the obstacle when they are in contact with each other. 
      // Collision checking framework. if collsion is detected set flags. Can probably user global setter flags. Update the flags
      // if a colliosn does indeed happen, possilbiy subtract points and stop furhter gain of points to the score. 
      // also check for horizontal collision flags so you don't phase into a asteroid. Horizontal collision need to know what direction 
      // coming from. COLLISION DETECTION AND SETTING FLAGS
      if (spawn1 != NULL) {
        if (p1.b1[2].collision(spawn1->b1))
        {
          //check if radial is happening here as the force field effect to be used. Set stop flag to be true
          p1.collided = true;
          spawn1->user_collided = true;
        } else
        {
            // set the stop flag to be false which should allow free movement and score updates
            p1.collided = false;
            spawn1->user_collided = false;
        }
        if (p1.b1[0].collision_left(spawn1->b1) || p1.b1[1].collision_left(spawn1->b1))
        {
          p1.left_collided = true;
          p1.right_collided = false;

        } else if (p1.b1[0].collision_right(spawn1->b1) || p1.b1[1].collision_right(spawn1->b1))
        {
          p1.left_collided = false;
          p1.right_collided = true;
        } else{
          p1.right_collided = false;
          p1.left_collided = false;
        }
        if (p1.w1->b1.collision(spawn1->b1))
        {
          p1.w1->collided = true;
          spawn1->proj_collided = true;
        }
      }
      if (spawn2 != NULL) {
        if (p1.b1[2].collision(spawn2->b1))
        {
          p1.collided = true;
          spawn2->user_collided = true;
        }
        else{
          if (!p1.collided) p1.collided = false;
          spawn2->user_collided = false;
        }
        if (p1.b1[0].collision_left(spawn2->b1) || p1.b1[1].collision_left(spawn2->b1))
        {
          p1.left_collided = true;
          if (!p1.right_collided) p1.right_collided = false;

        } else if (p1.b1[0].collision_right(spawn2->b1) || p1.b1[1].collision_right(spawn2->b1))
        {
          if (!p1.left_collided) p1.left_collided = false;
          p1.right_collided = true;
        } else{
          if (!p1.left_collided) p1.left_collided = false;
          if (!p1.right_collided) p1.right_collided = false;
        }
        if (p1.w1->b1.collision(spawn2->b1))
        {
          p1.w1->collided = true;
          spawn2->proj_collided = true;
        }
      }
      if (spawn3 != NULL) {
        if (p1.w1->b1.collision(spawn3->b1))
        {
          p1.w1->collided = true;
          spawn3->proj_collided = true;
        }
        if (p1.b1[2].collision(spawn3->b1))
        {
          p1.collided = true;
          spawn3->user_collided = true;
        } else {
        {
          if (!p1.collided) p1.collided = false;
          spawn3->user_collided = false;
        }
        if (p1.b1[0].collision_left(spawn3->b1) || p1.b1[1].collision_left(spawn3->b1))
        {
          p1.left_collided = true;
          if (!p1.right_collided) p1.right_collided = false;
        } else if (p1.b1[0].collision_right(spawn3->b1) || p1.b1[1].collision_right(spawn3->b1))
        {
          if (!p1.left_collided) p1.left_collided = false;
          p1.right_collided = true;
        } else{
          if (!p1.left_collided) p1.left_collided = false;
          if (!p1.right_collided) p1.right_collided = false;
        }
      }
      // delettions based on the flags. Check destructor and bounding box deletion. Use radial happening with these flags in Player. 
      // using as a force field which gives immunity rather than as a weapon. Erorors edit collison and bounding box logic. DESTRUCTOR DELETES
      

      // slide_pot cursor controls


      // obstacles enemies


      // Green means go lolll. FIRE????
      if (turbo1 && !turbo2) LED_Toggle(0x02);
      else if (!turbo1 && turbo2) LED_On(0x02);
      else LED_Off(0x02);
      // Radial would mean full on yellow. Can't do both so you have to turn on and off
      if (weapon1 && !weapon2)
      {
        LED_Toggle(0x01);
      } 
      else if (!weapon1 && weapon2) 
      {
        LED_On(0x01);
      }
      else LED_Off(0x01);

      if (groovy_index == 0 && !weapon1 && !weapon2)
      {
        soundPlayGroovy();
      }

      // set the score output here with printf
      Time++;
      GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
      
      }
    }
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_SetCursor(0, 0);
    soundStop();
    // you won 
    if (percent >= 10000)
    {
      if (ENGLISH_OR_SPANISH)
      {
        printf("LEVEL COMPLETE:\n 100%% COMPLETION\nPRESS LEFT TO\nPLAY AGAIN\nPRESS RIGHT TO QUIT");
      }else
      {
        printf("NIVEL COMPLETADO:\n 100%% COMPLETADO\nPRESIONA IZQUIERDA\nPARA JUGAR\nDE NUEVO\nPRESIONA DERECHA\nPARA SALIR");
      }
    } else { // lol too bad so sad
    
      if (ENGLISH_OR_SPANISH)
      {
        printf("LEVEL INCOMPLETE:\n %2d.%02d%% COMPLETION\nPRESS LEFT TO\nPLAY AGAIN\nPRESS RIGHT TO QUIT", percent/100, percent % 100);
      } else
      {
        printf("NIVEL INCOMPLETO:\n %2d.%02d%% COMPLETADO\nPRESIONA IZQUIERDA\nPARA JUGAR\nDE NUEVO\nPRESIONA DERECHA\nPARA SALIR", percent/100, percent % 100);
      }
    
    }
    int8_t play_again = Switch_In();
    while (play_again != 1 && play_again != 4)
    {
      play_again = Switch_In();
      if (play_again == 1)
      {
        play_restart = false;
      } else if (play_again == 4)
      {
        play_restart = true;
      }
    }
  }
  ST7735_FillScreen(ST7735_BLACK);
}



// use to sample adc and gyro readings at specified frequency
int main_sensor(void)
{
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  SysTick->LOAD = 0xFFFFFF;    // max
  SysTick->VAL = 0;            // any write to current clears it
  SysTick->CTRL = 0x00000005;  // enable SysTick with core clock
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  Sensor.Init(); //PB18 = ADC1 channel 5, slidepot
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_PlotClear(0,2000);
  I2C_ADXL345_Init(0x53);
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_Init();
  TimerG12_IntArm(2666666, 2);

  Time = 0;
  __enable_irq();
  while(1){
       // complete this
    // wait for semaphore using a call to a method in Sensor

    Sensor.Sync();
   
    GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
    // toggle red LED2 on Port B, PB26

    // convert Data to Position
    uint32_t Position = Sensor.Convert(slide_pot_data);

    // move cursor to top
    ST7735_SetCursor(0,0);
    // display distance in top row OutFix
    printf("%d.%d cm, %d, %d, %d xyz", Position / 1000, Position % 1000, gyro_x, gyro_y, gyro_z);

    Time++;
    GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
  }
}


