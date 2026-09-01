#include "candriver.h"
int main(){
	int i;
	CAN_MSG m1;
	init();
	IODIR0 |=0XFF;
	
	IOSET0=0XFF;
	
	while(1){
	can_rx(&m1);
	if(m1.byteA==0X02){
			for(i=7;i>=0;i--){
				IOCLR0=1<<i;
				delay_ms(400);
				IOSET0=1<<i;
			}
		}
	}
}
