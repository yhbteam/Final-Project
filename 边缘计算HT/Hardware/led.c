#include "stm32f10x.h"                  // Device header
void ledInit(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	TIM_TimeBaseInitTypeDef time_base;
	time_base.TIM_ClockDivision=TIM_CKD_DIV1;
	time_base.TIM_CounterMode=TIM_CounterMode_Up;
	time_base.TIM_Period=10000-1;
	time_base.TIM_Prescaler=144-1;
	time_base.TIM_RepetitionCounter=0x00;
	
	TIM_TimeBaseInit(TIM1,&time_base);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_OCInitTypeDef oc_struct;
	TIM_OCStructInit(&oc_struct);
	oc_struct.TIM_OCMode=TIM_OCMode_PWM1;
	oc_struct.TIM_OCPolarity=TIM_OCPolarity_High;
	oc_struct.TIM_Pulse=0;
	oc_struct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM1, &oc_struct);
	
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_struct.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init(GPIOA,&gpio_struct);
	
	TIM_Cmd(TIM1,ENABLE);
}
void setBrightness(uint8_t num){
	TIM_SetCompare1(TIM1,num*100);
}
