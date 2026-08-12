#include "candriver.h"
#include "lcdheader.h"
#include "motor_driver.h"

/* ---------------------------------------------------------
   Body Control Module (BCM) - LPC2148 + CAN Bus
   -----------------------------------------------------------
   This is the "master" node in the project. It watches three
   push button switches:
     SW1 -> left indicator
     SW2 -> right indicator
     SW3 -> wiper motor

   For the indicators, it just sends a CAN message out on the
   bus - the left.c and right.c nodes are the ones that actually
   flash the LEDs when they receive it.

   For the wiper, this board drives the motor directly using
   PWM (see motor_driver.h).

   The switches are wired active-low, so "pressed" reads as 0,
   which is why every check below is "== 0".
   --------------------------------------------------------- */

#define SW1 14   // Left indicator switch  -> P0.14
#define SW2 15   // Right indicator switch -> P0.15
#define SW3 16   // Wiper motor switch     -> P0.16

int main(void)
{
    CAN_MSG indicatorMsg;   // CAN message we send out for the indicators

    init();     // set up CAN controller
    INIT();     // set up LCD (pin directions + init sequence)

    // Splash screen on power-up
    STR("BODY CONTROL MODULE");
    CMD(0XC0);                 // move to line 2
    STR("   IN AUTOMOTIVES");

    pwm_init();                // set up PWM for the wiper motor

    // These fields don't change between messages, so set them once
    indicatorMsg.id  = 0X100;
    indicatorMsg.rtr = 0;
    indicatorMsg.dlc = 1;

    while (1)
    {
        /* ---------------- LEFT indicator ---------------- */
        if (((IOPIN0 >> SW1) & 1) == 0)
        {
            CMD(0x80);
            STR("=> LEFT Indicator ON");
            CMD(0xC0);
            STR("<=RIGHT IndicatorOFF");
            CMD(0x94);
            STR("WIPER OFF");

            indicatorMsg.byteA = 0X01;   // 0x01 = "left ON" code
            indicatorMsg.byteB = 0;
            can_tx(indicatorMsg);

            delay_ms(500);   // basic debounce
        }

        /* ---------------- RIGHT indicator ---------------- */
        if (((IOPIN0 >> SW2) & 1) == 0)
        {
            CMD(0x80);
            STR("=>LEFT Indicator OFF");
            CMD(0xC0);
            STR("<=RIGHT Indicator ON");
            CMD(0x94);
            STR("WIPER OFF");

            indicatorMsg.byteA = 0X02;   // 0x02 = "right ON" code
            indicatorMsg.byteB = 0;
            can_tx(indicatorMsg);

            delay_ms(500);
        }

        /* ---------------- WIPER motor ---------------- */
        if (((IOPIN0 >> SW3) & 1) == 0)
        {
            CMD(0x80);
            STR("=>LEFT Indicator OFF");
            CMD(0xC0);
            STR("<=RIGHT IndicatorOFF");
            CMD(0x94);
            STR("WIPER  ON");

            driver();   // runs the wiper motor until SW3 tells it to stop
        }
    }
}
