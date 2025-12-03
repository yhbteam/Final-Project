#include "stm32f10x.h"  
#include "Delay.h"
void delayUs(uint32_t us){
	SysTick->LOAD=72*us -1;
	SysTick ->VAL =0;
	SysTick->CTRL=0x00000005;
															
	while(!(SysTick->CTRL &0x00010000)){}
	SysTick->CTRL =0;
}



void  delayMs(uint32_t ms){
	if(ms==0){
		return ;
	}
	while(ms--){
		delayUs (1000);
	}
}
void delays(uint32_t s){
	if(s==0){
		return;
	}
	while(s--){
		delayMs (1000);
	}
}




