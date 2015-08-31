// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// December 28, 2014
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
unsigned long Switch_Input(void);
void PortE_Init(void);
void LED_On(void);
void LED_Off(void);
void Delay(void);
void LED_Toggle(void);

// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1);  // activate grader and set system clock to 80 MHz
  PortE_Init();
	LED_On();
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
    
		if(Switch_Input()==0x01){
			LED_Toggle();
			Delay();  // 5 Hz
		}else{
			LED_On();
		}
		
  }
  
}


void PortE_Init(void){ volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000010;   // 1) E clock
  delay = SYSCTL_RCGC2_R;      // 2) no need to unlock
  GPIO_PORTE_AMSEL_R &= ~0x03; // 3) disable analog function on PE1-0
  GPIO_PORTE_PCTL_R &= ~0x000000FF; // 4) enable regular GPIO on PE0 and PE1
  GPIO_PORTE_DIR_R &= ~0x01;   // 5) inputs on PE0
	GPIO_PORTE_DIR_R |= 0x02;   //     Output on PE1
  GPIO_PORTE_AFSEL_R &= ~0x03; // 6) regular function on PE1-0
  GPIO_PORTE_DEN_R |= 0x03;    // 7) enable digital on PE1-0
	
}


void LED_On(void){
	GPIO_PORTE_DATA_R |= 0x02;
}

void LED_Off(void){
	GPIO_PORTE_DATA_R &= ~0x02;
}

void LED_Toggle(void){
	GPIO_PORTE_DATA_R = GPIO_PORTE_DATA_R^0x02;
}

unsigned long Switch_Input(void){
  return (GPIO_PORTE_DATA_R&0x01); // 0x01(pressed) or 0(not pressed)
}


void Delay(void){unsigned long volatile time;
  time = 800000; 
  while(time){
   time--;
  }
}
