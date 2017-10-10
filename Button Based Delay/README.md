# Button Based Delay
Now that we have a little introduction to embedded systems, it is time to dive deeper into how efficient a processor can run. Timers and interrupts are two fundamental parts of all decent processors. This excercise aims to combine our knew knowledge on GPIO interrupts and timer interrupts by creating a program which sets an LED to toggle at the rate a button is pressed.

![alt text](https://github.com/RU09342/lab-3-interrupts-and-timers-tcsmith20/blob/master/Button%20Based%20Delay/Button%20Based%20Delay.gif)

## How Does The Code Work
Using the MSP430 library I created, it was easy to implement all five required processors into one main.c file. The main method disables the watchdog timer, initializes the processors setup, LEDs, button 1 and Timer A (This is all done through the Library). Button 1's interrupt is enabled to be triggered on the negative edge while Timer A is initialized with ACLK (32 kHz), up mode, divider 8 and 10 Hz. A slower clock was used to provide more realistic button pressing times. If SMCLK was used, the user would have to press the button within a half second for the clock to not overflow. Using SMCLK allows the user to hold the button up to 16 seconds and not overflow. The Timer A Interrupt is then enabled. When the Timer A interupt fires, the LED will toggle. In order to set the proper period for Timer A, within the button interrupt, a different timer (Timer B) is started when the button is pressed and read when the button is depressed. This value is then assigned to Timer A so the LED blinks in the correct interval.  


## Important Things to Note
* Within the button interrupt, I switch the edge the interrupt fires on. This is so the interrupt fires on both edges of the button. I used if statements to determine what tasks to complete based on what edge is selected.
* Multiple Timers are used in this excercise. It can be done with one, but it is more accurate and easier to do with two. One timer is designated to blinking the LED and the other register is used to determine how long the button is pressed.