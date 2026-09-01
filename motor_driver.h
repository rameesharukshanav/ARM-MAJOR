/*void driver()
{
    int x = (100 *150) - 1; //14900

    PINSEL1 = 1 << 10; //P0.21 = PWM5

    PWMPR = 59; //1 us timer (60MHz)
    PWMMR0 = (20 * 15000) - 1; //20 ms period
    PWMMR5 = x;

    PWMMCR = 0x03; //Interrupt + Reset on MR0

    PWMPCR = 1 << 13; //Enable PWM5 output

    PWMLER = (1<<0) | (1<<5);

    PWMTCR = 0x02; //Reset counter
    PWMTCR = 0x09; //Enable PWM

    while(1)
    {
        while((PWMIR & 1) == 0);
        PWMIR = 1;

        PWMMR5 = x;

        x += 100;

        if(x >= 19999)
            x = 14999;

        PWMLER = (1<<5);
    }
}
*/
void delay_mills(unsigned int ms)
{
    T0PR = 60000 - 1;
    T0TCR =0x01;
    while(T0TC < ms);
    T0TCR =0x03;
	T0TCR=0x00;
}
int x = (1000 * 60) - 1;
int flag=0;

void pwm_init(void)
{
   	PINSEL1 |= 1 << 10;  
	VPBDIV=1;           // P0.21 -> PWM5
	PWMMR0 = (20000 * 60) - 1;     // 20 ms period
    PWMMR5 = x;
	PWMMCR = 2;
    PWMPCR = 1 << 13;
    PWMTCR = 0x09;
    PWMLER = (1<<0)|(1<<5);
}
void driver(void)
{
//uart0_tx_string("inside gate  ");
			delay_mills(300);

   			while(1)
            {
                PWMMR5 = x;
                PWMLER = 1<<5;

                delay_mills(100);

                x = x + (56 * 60);
				
				      // Step for 15 MHz

                if(((IOPIN0>>16)&1)==0)
                {
					delay_mills(300);
                    return ;
                }

                if(x > ((2100 * 60) - 1))
                {
                    x = (1000 * 60) - 1;
                    
                    delay_mills(1000);
                }
            }

}
	
