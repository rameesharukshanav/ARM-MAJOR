#include "candriver.h"

/* ---------------------------------------------------------
   Right Indicator Node
   -----------------------------------------------------------
   Same idea as left.c, but for the right indicator, and the
   LED chase runs in the opposite direction (right to left)
   so it visually mirrors the left side.
   --------------------------------------------------------- */

int main(void)
{
    int i;
    CAN_MSG rxMsg;

    init();            // set up the CAN controller
    IODIR0 |= 0XFF;    // P0.0 - P0.7 as outputs (8 LEDs)

    IOSET0 = 0XFF;     // all LEDs off at power-up (active-low)

    while (1)
    {
        can_rx(&rxMsg);             // block here until a CAN message arrives

        if (rxMsg.byteA == 0X02)    // 0x02 = "right indicator ON"
        {
            // Chase the LEDs from the last one back to the first
            for (i = 7; i >= 0; i--)
            {
                IOCLR0 = 1 << i;    // LED i ON  (active-low)
                delay_ms(400);
                IOSET0 = 1 << i;    // LED i OFF
            }
        }
    }
}
