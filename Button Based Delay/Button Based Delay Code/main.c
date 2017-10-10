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
unsigned int clk_freq = (32768>>3); // ACLK and divide by 8

unsigned volatile int i = 0;

int main(void){

    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

    Setup_Init();                   // Turn off high impedance for GPIO

    Led1_Init();                    // Initialize Led1
    Led1_Off();                     // Turn off Led1
    Led2_Init();                    // Initialize Led2
    Led2_Off();                     // Turn off Led2

    Button1_Init(pull_up);                          // Initialize Button1 and add a pull up resistor
    Button1_EnableInterrupt(neg_edge_trigger);      // Enable Button1 to interrupt on pos edge

    TimerA_Init(ACLK, UP, DIV8, Convert_HzToVal(10, clk_freq));  // Initialize Timer A to:
                                                                         //ACLK = 32768Hz, UP Mode, divide by 8, start at 10Hz
    TimerA_EnableInterrupt0();  //Enable CCR0 interrupt

    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/interrupt
}

unsigned int period = 0x199;                  // Initialize period to a number other than 0

void Button1_ISR(){
    Led1_Toggle();              // Toggle Led1 to indicate button is pressed

    if(neg_edge_trigger){
        TimerB_Init(ACLK, CONTINUOUS, DIV8, Convert_HzToVal(10, clk_freq));  // Initialize Timer A to:
                                                                     //ACLK = 32768Hz, Continuous Mode, divide by 8, start at 10Hz

        neg_edge_trigger = 0;                           // Change the edge so interrupt is triggered on depress
        Button1_EnableInterrupt(neg_edge_trigger);      // Enable the interrupt on newly changed edge
    }
    else{
        period = TimerB_TimeSince(0);       // Capture time since Button1 was initially pressed
        TimerA_SetCompareVal0(period);      // Set Timer A interrupt to fire at this interval
        TimerB_Stop();                      // Stop and reset timer for next button press
        neg_edge_trigger = 1;                           // Change the edge so interrupt is triggered on press
        Button1_EnableInterrupt(neg_edge_trigger);      // Enable the interrupt on newly changed edge
    }


    for(i = 0; i < 50000; i++);         // Delay for debouncing

    Button1_ClearFlag();                // Clear flag so interrupt can fire again
}

void TimerA_ISR(){
    Led2_Toggle();       // Toggle Led at a specific interval
}

/** ALL BUTTON1 INTERRUPT ROUTINES FOR 5 PROCESSORS */

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

/** ALL TIMER A INTERRUPT ROUTINES FOR 5 PROCESSORS */

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void){
    TimerA_ISR();
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B (void){
    TimerA_ISR();
}
