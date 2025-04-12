/*
 * Switch.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    // write this
  IOMUX->SECCFG.PINCM[PA24INDEX] = 0x00040081; // input, no pull
  IOMUX->SECCFG.PINCM[PA25INDEX] = 0x00040081; // input, no pull
  IOMUX->SECCFG.PINCM[PA26INDEX] = 0x00040081; // input, no pull
  IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00040081; // input, no pull
}
// return current state of switches
uint32_t Switch_In(void){
    // write this
  uint32_t data = GPIOA->DIN31_0;
  data &= (1<<24) | (1<<25) | (1<<26) | (1<<27); // right, down, left, up
  data = data >> 24;
  return data; // return 0; //replace this your code
}