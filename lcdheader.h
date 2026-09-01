#define	DATA_PIN 0Xff

#define RS 1<<8

#define E 1<<9

void CMD(unsigned char );

void INIT(void){

 IODIR0=DATA_PIN|RS|E;

 CMD(0X01);

 CMD(0X02);

 CMD(0X0C);

 CMD(0X38);

}

void CMD(unsigned char cd){

	IOCLR0=DATA_PIN;

	IOSET0=cd;

	IOCLR0=RS;

	IOSET0=E;

	delay_ms(2);

	IOCLR0=E;

	}

void DATA(unsigned char d) {

	IOCLR0=DATA_PIN;

	IOSET0=d;

	IOSET0=RS;

	IOSET0=E;

	delay_ms(2);

	IOCLR0=E;

}

void STR(unsigned char *s){
while(*s){
DATA(*s++);
}

}

void LCD_STR(unsigned char* s){
while(*s){
DATA(*s++);
}			

}

void INT(int n){
 unsigned char arr[5];
 signed char i=0;

 if(n==0)
 DATA('0');
 else{
 if (n<0){
 DATA('-');
 n=-n;
 }

 while(n>0){
 arr[i++] = n%10;
 n=n/10;
 }
 for (--i;i>=0;i--)
 DATA(arr[i]+48);

 }

}
