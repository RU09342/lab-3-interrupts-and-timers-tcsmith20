#include <msp430.h> 
#include "Tanner's MSP Lib/setup/setup.h"
#include "Tanner's MSP Lib/led/led.h"
#include "Tanner's MSP Lib/timer/timer.h"


/**
 * main.c
 */

unsigned int period1 = 5000;                //period of led1
unsigned int period2 = 22500;               //period of led2
unsigned int period1_increment = 5000;
unsigned int period2_increment = 22500;

unsigned volatile int i = 0;

int main(void){

    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

    Setup_Init();                   // Turn off high impedance for GPIO

    Led1_Init();                    // Initialize Led1
    Led1_Off();                     // Turn off Led1
    Led2_Init();                    // Initialize Led2
    Led2_Off();                     // Turn off Led2

    TimerA_Init(SMCLK, CONTINUOUS, DIV8, period1);  // Initialize Timer A to:
                                                                         //SMCLK = 1MHz, CONTINUOUS Mode, divide by 8, start at 10Hz
    TimerA_SetCompareVal1(period2);
    TimerA_EnableInterrupt0();  //Enable CCR0 interrupt
    TimerA_EnableInterrupt1();  //Enable CCR0 interrupt

    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/interrupt
}

void TimerA_CCR0_ISR(){
    Led1_Toggle();                       // Toggle Led at a specific interval
    period1 += period1_increment;        // Increment period since clock is in continuous mode
    TimerA_SetCompareVal0(period1);      // Set CCR0 to new value
}

void TimerA_CCR1_ISR(){
    Led2_Toggle();                       // Toggle Led at a specific interval
    period2 += period2_increment;        // Increment period since clock is in continuous mode
    TimerA_SetCompareVal1(period2);      // Set CCR0 to new value
}

/** ALL TIMER A INTERRUPTS FOR 5 PROCESSORS */

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0 (void){
    TimerA_CCR0_ISR();
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A_CCR1 (void){
    switch(TA0IV){
    case 2:
        TimerA_CCR1_ISR();
        break;
    default:
        break;
    }
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B_CCR0 (void){
    TimerA_CCR0_ISR();
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer_B_CCR1 (void){
    switch(TA0IV){
    case 2:
        TimerA_CCR1_ISR();
        break;
    default:
        break;
    }
}
