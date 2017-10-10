#include <msp430.h> 
#include "Tanner's MSP Lib/setup/setup.h"
#include "Tanner's MSP Lib/led/led.h"
#include "Tanner's MSP Lib/button/button.h"
#include "Tanner's MSP Lib/timer/timer.h"


/**
 * main.c
 */

unsigned int pull_up = 1;
unsigned int neg_edge_trigger = 1;

unsigned volatile int i = 0;

int main(void){

    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

    Setup_Init();                   // Turn off high impedance for GPIO

    Led1_Init();                    // Initialize Led1
    Led1_On();                     // Turn on Led1
    Led2_Init();                    // Initialize Led2
    Led2_Off();                     // Turn off Led2

    Button1_Init(pull_up);                          // Initialize Button1 and add a pull up resistor
    Button1_EnableInterrupt(neg_edge_trigger);      // Enable Button1 to interrupt on pos edge

    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/interrupt
}



void Button1_ISR(){
    Led1_Toggle();              // Toggle Led1
    Led2_Toggle();              // Toggle Led2


    for(i = 0; i < 50000; i++);         // Delay for debouncing

    Button1_ClearFlag();                // Clear flag so interrupt can fire again
}


/** ALL BUTTON1 VECTORS FOR THE 5 PROCESSORS */

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    Button1_ISR();
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {
    Button1_ISR();
}

#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void) {
    Button1_ISR();
}
