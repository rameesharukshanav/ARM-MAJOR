/* ---------------------------------------------------------
   Wiper Motor Driver - PWM based speed ramp
   -----------------------------------------------------------
   Drives the wiper motor using PWM5 on P0.21. Instead of
   jumping straight to full speed, the duty cycle is ramped
   up gradually in small steps for a smoother start, then
   reset once it hits the max value.

   Timer0 is used for a simple blocking delay function -
   nothing fancy, just busy-waits the CPU for the given
   number of milliseconds.
   --------------------------------------------------------- */

// Blocking delay in milliseconds, using Timer0.
// Not efficient (CPU just spins here), but simple and good
// enough for this project since we don't need multitasking.
void delay_mills(unsigned int ms)
{
    T0PR  = 60000 - 1;   // prescaler -> Timer0 now counts in ms (60 MHz PCLK)
    T0TCR = 0x01;        // start the timer
    while (T0TC < ms);   // wait here until 'ms' milliseconds have passed
    T0TCR = 0x03;        // reset + hold the timer
    T0TCR = 0x00;        // stop the timer
}

int pulseWidth = (1000 * 60) - 1;   // current PWM match value = motor speed
int flag = 0;                        // not used yet, kept for future use

// Set up PWM5 on P0.21 to control the wiper motor.
// Call this once from main() before the motor is ever used.
void pwm_init(void)
{
    PINSEL1 |= 1 << 10;          // route P0.21 to its PWM5 function
    VPBDIV = 1;                  // peripheral clock = CPU clock (60 MHz)

    PWMMR0 = (20000 * 60) - 1;   // PWM period = 20 ms
    PWMMR5 = pulseWidth;         // starting duty cycle

    PWMMCR = 2;                  // reset PWM counter every time it hits MR0
    PWMPCR = 1 << 13;            // enable PWM5 output on the pin
    PWMTCR = 0x09;               // enable PWM mode + start counting
    PWMLER = (1 << 0) | (1 << 5); // latch the MR0 and MR5 values in
}

// Runs the wiper motor. Ramps the PWM duty cycle up step by
// step, and keeps looping until the wiper switch tells it to
// stop. Called from main() whenever SW3 is active.
void driver(void)
{
    delay_mills(300);   // short delay before the motor starts (debounce)

    while (1)
    {
        PWMMR5 = pulseWidth;   // load the new duty cycle
        PWMLER = 1 << 5;       // tell the PWM hardware to use it

        delay_mills(100);      // hold this speed for a bit before ramping more

        pulseWidth = pulseWidth + (56 * 60);   // step the speed up slightly

        // Check the wiper switch (P0.16). If it's no longer
        // pressed, stop the motor loop and go back to main().
        if (((IOPIN0 >> 16) & 1) == 0)
        {
            delay_mills(300);
            return;
        }

        // Once we hit the top speed, drop back down to the
        // starting speed and pause before ramping up again
        if (pulseWidth > ((2100 * 60) - 1))
        {
            pulseWidth = (1000 * 60) - 1;
            delay_mills(1000);
        }
    }
}
