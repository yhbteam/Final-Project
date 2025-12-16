#include "stm32f10x.h"                  // Device header

void pwmInit(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	TIM_TimeBaseInitTypeDef time_base;
	time_base.TIM_ClockDivision=TIM_CKD_DIV1;
	time_base.TIM_CounterMode=TIM_CounterMode_Up;
	time_base.TIM_Period=10000-1;
	time_base.TIM_Prescaler=144-1;
	time_base.TIM_RepetitionCounter=0x00;
	
	TIM_TimeBaseInit(TIM3,&time_base);

	
	//TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	TIM_OCInitTypeDef oc_struct;
	TIM_OCStructInit(&oc_struct);
	oc_struct.TIM_OCMode=TIM_OCMode_PWM1;
	oc_struct.TIM_OCPolarity=TIM_OCPolarity_High;
	oc_struct.TIM_Pulse=3000;
	oc_struct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM3, &oc_struct);
	
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	
	gpio_struct.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init(GPIOA,&gpio_struct);
	TIM_Cmd(TIM3,ENABLE);
}

void motorInit(void){
	pwmInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_struct.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;
	GPIO_Init(GPIOB,&gpio_struct);
}
void forward(void){
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}
void back(void){
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
}
void stop(void){
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
}
void setSpeed(uint8_t num){
	TIM_SetCompare1(TIM3,num*100);
}
