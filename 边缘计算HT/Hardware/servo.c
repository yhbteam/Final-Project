#include "stm32f10x.h"                  // Device header
void servoInit(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	TIM_TimeBaseInitTypeDef time_base;
	time_base.TIM_ClockDivision=TIM_CKD_DIV1;
	time_base.TIM_CounterMode=TIM_CounterMode_Up;
	time_base.TIM_Period=10000-1;
	time_base.TIM_Prescaler=144-1;
	time_base.TIM_RepetitionCounter=0x00;
	
	TIM_TimeBaseInit(TIM2,&time_base);
	
	//TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	TIM_OCInitTypeDef oc_struct;
	TIM_OCStructInit(&oc_struct);
	oc_struct.TIM_OCMode=TIM_OCMode_PWM1;
	oc_struct.TIM_OCPolarity=TIM_OCPolarity_High;
	oc_struct.TIM_Pulse=250;
	oc_struct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM2, &oc_struct);
	
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_struct.GPIO_Pin=GPIO_Pin_15;
	GPIO_Init(GPIOA,&gpio_struct);
	TIM_Cmd(TIM2,ENABLE);
}
void setAngle(uint8_t num){
	TIM_SetCompare1(TIM2,250+1000/180*num);
}