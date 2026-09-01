#include "candriver.h"

int main(){
	int i;
	CAN_MSG m1;
	init();
	IODIR0 |=0XFF;
	for(i=0;i<8;i++){
	IOSET0=1<<i;
	}
	while(1){
	can_rx(&m1);
	if(m1.byteA==0x01){
			for(i=0;i<8;i++){
				IOCLR0=1<<i;
				delay_ms(400);
				IOSET0=1<<i;
			}
		}
	}
}
