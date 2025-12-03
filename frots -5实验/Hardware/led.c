#include "stm32f10x.h"                  // Device header

void LedInit(void){
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE );
	GPIO_InitTypeDef GPIOStruct;
	GPIOStruct.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIOStruct.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_3;
	GPIOStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init (GPIOA ,&GPIOStruct );
}
void LedTurn01(void){
	GPIO_SetBits (GPIOA,GPIO_Pin_0 );
}
void LedOff01(void ){
	GPIO_ResetBits (GPIOA,GPIO_Pin_0);
}
void LedTurn02(void){
	GPIO_SetBits (GPIOA,GPIO_Pin_3 );
}
void LedOff02(void ){
	GPIO_ResetBits (GPIOA,GPIO_Pin_3);
}
