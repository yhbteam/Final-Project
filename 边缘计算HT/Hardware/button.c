#include "stm32f10x.h"                  // Device header
#include "FreeRTos.h"
#include "task.h"

uint8_t num=0;
// 两个按钮分别是 
void initButton(){
	// 1：
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// 2:
	GPIO_InitTypeDef gpioStruct;
	gpioStruct.GPIO_Mode = GPIO_Mode_IPU;		// 上拉输入模式
	gpioStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioStruct);
}

// 按下去是0， 没有按是1
uint8_t getButtonNum(void){
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0){
		vTaskDelay(20);	// 按下去的抖动
		
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0){}  // 模拟一直按着
		
		vTaskDelay(20); // 松开的抖动
			
		num = 1;
	}
	return num;
	
}
