#include "infrared.h"

// 红外引脚PA2（上拉输入）
void InfraredInit(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

uint8_t InfraredRead(void) {
    // 低电平=检测到物体，返回1；否则返回0
      return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0) ? 1 : 0;
}
