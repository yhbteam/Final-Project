//#include "buzzer.h"

//// 蜂鸣器引脚PA1（低电平触发：拉低=响，置高=停）
//void BuzzerInit(void) {
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//    GPIO_InitTypeDef GPIO_InitStruct;
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStruct);
//    
//    BuzzerOff(); // 初始化后默认关闭（置高电平）
//}

//// 蜂鸣器开：拉低PA1（低电平触发）
//void BuzzerOn(void) { 
//    GPIO_ResetBits(GPIOA, GPIO_Pin_1); 
//}

//// 蜂鸣器关：置高PA1
//void BuzzerOff(void) { 
//    GPIO_SetBits(GPIOA, GPIO_Pin_1); 
//}
// buzzer.c 最终版
#include "buzzer.h"

void BuzzerInit(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // 初始化后强制置高（关闭蜂鸣器）
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

// 开：拉低（低电平触发）
void BuzzerOn(void) { 
    GPIO_ResetBits(GPIOA, GPIO_Pin_1); 
}

// 关：置高
void BuzzerOff(void) { 
    GPIO_SetBits(GPIOA, GPIO_Pin_1); 
}
