#include "candriver.h"
#include "lcdheader.h"
#include "motor_driver.h"
#define SW1 14
#define SW2 15
#define SW3 16

int main(){
	CAN_MSG m1;
	init();
	INIT();
	DATA("BODY CONTROL MODULE");
	CMD(0XC0);
	STR("   IN AUTOMOTIVES");
	pwm_init();
	m1.id=0X100;
	   m1.rtr=0;
	   m1.dlc=1;
	while(1){
	if (((IOPIN0>>SW1)&1)==0){
	   
	   CMD(0x80);
	   STR("=> LEFT Indicator ON");
	   CMD(0xC0);	   
	   STR("<=RIGHT IndicatorOFF");
	   CMD(0x94);
	   STR("WIPER OFF");
	   m1.byteA=0X01;
	   m1.byteB=0;
	   can_tx(m1);
	   delay_ms(500);
	}
	if (((IOPIN0>>SW2)&1)==0){
	   
	   CMD(0x80);
	   STR("=>LEFT Indicator OFF");
	   CMD(0xC0);	   
	   STR("<=RIGHT Indicator ON");
	   CMD(0x94);
	   STR("WIPER OFF");
	   m1.byteA=0X02;
	   m1.byteB=0;
	   can_tx(m1);
	   delay_ms(500);
	}
	if (((IOPIN0>>SW3)&1)==0){
		
		CMD(0x80);
	   STR("=>LEFT Indicator OFF");
	   CMD(0xC0);	   
	   STR("<=RIGHT IndicatorOFF");
	   CMD(0x94);
	   STR("WIPER  ON");
	   driver();
	}
	}
}
