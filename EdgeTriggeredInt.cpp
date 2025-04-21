#include <ti/devices/msp/msp.h>
#include "../inc/EdgeTriggered.h"
#include "../inc/LaunchPad.h"

// Edge triggered interrups
void EdgeTriggered_Init(void){
  GPIOA->POLARITY31_16 = 0x00550000;     // falling
  GPIOA->CPU_INT.ICLR = (1<<24) + (1<<25) + (1<<26) + (1<<27);   // clear bit 21
  GPIOA->CPU_INT.IMASK = (1<<24) + (1<<25) + (1<<26) + (1<<27);   // arm PB21
  NVIC->IP[0] = (NVIC->IP[0]&(~0x0000FF00))|2<<14;    // set priority (bits 15,14) IRQ 1
  NVIC->ISER[0] = 1 << 1; // Group1 interrupt

}