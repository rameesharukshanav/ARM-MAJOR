/* ---------------------------------------------------------
   16x2 LCD driver (8-bit mode) for LPC2148
   -----------------------------------------------------------
   Port0 wiring used in this project:
     P0.0 - P0.7 -> LCD data lines D0-D7
     P0.8        -> RS  (0 = command, 1 = data)
     P0.9        -> E   (enable / latch pulse)
   --------------------------------------------------------- */

#define DATA_PIN 0xFF        // P0.0 - P0.7 = LCD data bus
#define RS       (1 << 8)    // Register Select pin
#define E        (1 << 9)    // Enable pin

void CMD(unsigned char cmd);   // forward declaration (used inside INIT)

// Configure the LCD pins and run the standard HD44780
// startup sequence. Call this once at the start of main().
void INIT(void)
{
    IODIR0 = DATA_PIN | RS | E;   // data bus + RS + E as outputs

    CMD(0X01);   // clear display
    CMD(0X02);   // cursor to home position
    CMD(0X0C);   // display ON, cursor OFF
    CMD(0X38);   // 8-bit interface, 2 lines, 5x7 font
}

// Send a command byte to the LCD (RS = 0 -> "this is an instruction")
void CMD(unsigned char cd)
{
    IOCLR0 = DATA_PIN;
    IOSET0 = cd;
    IOCLR0 = RS;
    IOSET0 = E;
    delay_ms(2);      // LCD needs a short pulse width on E
    IOCLR0 = E;
}

// Send one character to the LCD (RS = 1 -> "this is data / a character")
void DATA(unsigned char d)
{
    IOCLR0 = DATA_PIN;
    IOSET0 = d;
    IOSET0 = RS;
    IOSET0 = E;
    delay_ms(2);
    IOCLR0 = E;
}

// Print a null-terminated string on the LCD, character by character
void STR(unsigned char *s)
{
    while (*s)
    {
        DATA(*s++);
    }
}

// Same job as STR() - kept as a separate function in case we want
// to add line-wrapping logic here later without touching STR()
void LCD_STR(unsigned char *s)
{
    while (*s)
    {
        DATA(*s++);
    }
}

// Print an integer on the LCD, digit by digit (handles negatives too)
void INT(int n)
{
    unsigned char digits[5];
    signed char i = 0;

    if (n == 0)
    {
        DATA('0');
    }
    else
    {
        if (n < 0)
        {
            DATA('-');
            n = -n;
        }

        // Peel off digits from the end (units, tens, hundreds...)
        while (n > 0)
        {
            digits[i++] = n % 10;
            n = n / 10;
        }

        // Print them back in the correct order
        for (--i; i >= 0; i--)
        {
            DATA(digits[i] + 48);   // '0' is ASCII 48
        }
    }
}
