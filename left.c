#include "candriver.h"

/* ---------------------------------------------------------
   Left Indicator Node
   -----------------------------------------------------------
   This board just sits and listens on the CAN bus. Whenever
   the BCM (main.c) sends a message with byteA = 0x01, it means
   "left indicator switch was pressed", so this node runs a
   simple LED chase pattern to look like a turn signal blinking.
   --------------------------------------------------------- */

int main(void)
{
    int i;
    CAN_MSG rxMsg;

    init();            // set up the CAN controller
    IODIR0 |= 0XFF;    // P0.0 - P0.7 as outputs (8 LEDs)

    // LEDs are active-low here, so setting the pins HIGH
    // means "all LEDs off" at power-up
    for (i = 0; i < 8; i++)
    {
        IOSET0 = 1 << i;
    }

    while (1)
    {
        can_rx(&rxMsg);             // block here until a CAN message arrives

        if (rxMsg.byteA == 0x01)    // 0x01 = "left indicator ON"
        {
            // Turn LEDs on one at a time, left to right, then
            // back off, to give a chasing/blinking effect
            for (i = 0; i < 8; i++)
            {
                IOCLR0 = 1 << i;    // LED i ON  (active-low)
                delay_ms(400);
                IOSET0 = 1 << i;    // LED i OFF
            }
        }
    }
}
