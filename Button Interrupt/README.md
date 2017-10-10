# Button Interrupt
Now that we have a little introduction to embedded systems, it is time to dive deeper into how efficient a processor can run. Timers and interrupts are two fundamental parts of all decent processors. This excercise aims to recreate Button Blink from Lab 2 but by using GPIO interrupts. 

![alt text](https://github.com/RU09342/lab-3-interrupts-and-timers-tcsmith20/blob/master/Button%20Interrupt/Button%20Interrupt.gif)

## How Does The Code Work
Using the MSP430 library I created, it was easy to implement all five required processors into one main.c file. The main method disables the watchdog timer, initializes the processors setup, LEDs and button1 (This is all done through the Library). The GPIO interrupt enable register (IE) is set to fire on the negative edge of the button (IES register). The interrupt header: #pragma vector=PORT1_VECTOR
__interrupt void Port_1(void), is then used to create a function which toggles the LEDs everytime it interrupts. A delay is used to help debouncing and the button interrupt flag is cleared so the interrupt can fire again.


## Important Things to Note
* The button interrupt flag MUST be cleared before the interrupt will fire again.
* Using an interrupt is more efficient than polling because the processor is free to do whatever it wants until an interrupt fires.