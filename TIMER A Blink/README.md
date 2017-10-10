# TIMER A Blink
Now that we have a little introduction to embedded systems, it is time to dive deeper into how efficient a processor can run. Timers and interrupts are two fundamental parts of all decent processors. This excercise aims to recreate Multiple Blink from Lab 2 but by using timer interrupts. 

![alt text](https://github.com/RU09342/lab-3-interrupts-and-timers-tcsmith20/blob/master/TIMER%20A%20Blink/Timer%20A%20Blink.gif)

## How Does The Code Work
Using the MSP430 library I created, it was easy to implement all five required processors into one main.c file. The main method disables the watchdog timer, initializes the processors setup, LEDs and Timer A (This is all done through the Library). Timer A is initialized by providing it a clock source, clock divider, clock mode and capture/compare value. I used the 1 MHz SMCLK, divider 8, continuous mode and the led1 period to initialize the clock. The timer is run at 125 kHz (1 MHz divided by 8). I set it to continuous mode so it can use multiple capture/compare values for the different periods. The second capture/compare value was set to the led2 period and both capture/compare interrupts were enabled. Once the interrupts fired, the CCR0 interrupt would toggle LED1 and increment its register by the period. The CCR1 interrupt toggled LED 2 and incremented itself as well. Using two capture/compare registers allows the LEDs to blink at their own independent rates. 


## Important Things to Note
* A switch case statement MUST be used with TA0IV whenever using an interrupt vector that isn't from CCR0.
* Interrupt vector naming follows the following examples:
CCR0 -> #pragma vector=TIMER0_A0_VECTOR
CCR1 -> #pragma vector=TIMER0_A1_VECTOR