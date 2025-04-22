// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Your name
// Last Modified: 12/26/2024

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

extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
extern "C" void GROUP1_IRQHandler(void);

// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

uint32_t M=1;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}

SlidePot Sensor(1500,0); // copy calibration from Lab 7
Player p1; // default player in starting position

// button requests
bool weapon1 = false;
bool weapon2 = false;
bool turbo1 = false;
bool turbo2 = false;

uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

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
// use main1 to observe special characters
int main1(void){ // main1
    char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(0x0000);            // set screen to black
  for(int myPhrase=0; myPhrase<= 2; myPhrase++){
    for(int myL=0; myL<= 3; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Clock_Delay1ms(2000);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

// use main2 to observe graphics
int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom
  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);

  for(uint32_t t=500;t>0;t=t-5){
    SmallFont_OutVertical(t,104,6); // top left
    Clock_Delay1ms(50);              // delay 50 msec
  }
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString((char *)"GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString((char *)"Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString((char *)"Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  while(1){
  }
}
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
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}

int main(void)
{
  DAC5_Init();
  Clock_Init80MHz();
  SysTick_IntArm(7256,1,0);
  static uint32_t index = 0;

  // player movement
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
  __enable_irq();

  while(1){
       // complete this
    // wait for semaphore using a call to a method in Sensor

    Sensor.Sync();
   
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
    p1.shoot();
    p1.radial();

    // slide_pot cursor controls


    // obstacles enemies


    // Green means go lolll
    if (turbo1 && !turbo2){
      LED_Toggle(0x02);
      soundPlayShoot(); // Play shooting sound
    }
    else if (!turbo1 && turbo2){
      LED_On(0x02);
      soundPlayRadial(); // Play radial sound
    }
    else LED_Off(0x02);
    // Radial would mean full on yellow. Can't do both so you have to turn on and off
    if (weapon1 && !weapon2) LED_Toggle(0x01);
    else if (!weapon1 && weapon2) LED_On(0x01);
    else LED_Off(0x01);
    Time++;
    GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
  }
}

//SysTick Sound Functions:
void SysTick_Handler(void) {
if (weapon1 && !weapon2) { // Weapon 1: Shooting sound
        DAC5_OUT(shoot[index]);  
        index++;
        if (index > 4800) {
            soundStop();
        }
    } 
    else if (!weapon1 && weapon2) { // Weapon 2: Radial sound
        DAC5_OUT(iceball[index]);  
        index++;
        if (index > 16987) {
            soundStop();
        }
    }
}

void soundPlayShoot(void) {
    index = 0;  // Reset index for new sound playback
    SysTick->LOAD = 7256; // Set SysTick timer for playback rate
    SysTick->CTRL = 7;  // Enable SysTick with interrupts
}

void soundPlayRadial(void) {
    index = 0;  // Reset index for new sound playback
    SysTick->LOAD = 7256; // Set SysTick timer for playback rate
    SysTick->CTRL = 7;  // Enable SysTick with interrupts
}

void soundStop(void){
  SysTick->LOAD = 0;
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
// use main4 to test sound outputs
int main4(void){ uint32_t last=0,now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while(1){
    now = Switch_In(); // one of your buttons
    if((last == 0)&&(now == 1)){
      Sound_Shoot(); // call one of your sounds
    }
    if((last == 0)&&(now == 2)){
      Sound_Killed(); // call one of your sounds
    }
    if((last == 0)&&(now == 4)){
      Sound_Explosion(); // call one of your sounds
    }
    if((last == 0)&&(now == 8)){
      Sound_Fastinvader1(); // call one of your sounds
    }
    // modify this to test all your sounds
  }
}
// ALL ST7735 OUTPUT MUST OCCUR IN MAIN. Game Logic. Setup systick and Timer interrupt handlers. Edge detection handler as well
int main5(void){ // final main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  Sensor.Init(); // PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  
  // initialize all data structures
  __enable_irq();

  while(1){
    // wait for semaphore
       // clear semaphore
       // update ST7735R+
    // check for end game or level switch
  }
}
