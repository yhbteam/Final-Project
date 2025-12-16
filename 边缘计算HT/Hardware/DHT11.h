#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"
#include "Delay.h"

// 状态定义
#define SUCCESS 1
#define ERROR   0

// 函数声明
void DHTInit(void);
uint8_t DHT_ReadData(uint8_t data[5]);
float DHT_GetTemperature(uint8_t data[5]);
float DHT_GetHumidity(uint8_t data[5]);

#endif
