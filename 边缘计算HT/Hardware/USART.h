//#ifndef USART_H
//#define USART_H
//#include "stm32f10x.h"                  // Device header
//#include <stdio.h>
//#include <string.h>

//#define BUFFSIZE 200
//extern char message[200];
//extern uint8_t flag;

//void USART_DMAConfig(void );
//void USARTInit(void);
//void Send_Byte(uint8_t data);
//void Send_Arr(uint8_t *arr,uint16_t len);
//void Send_String(char *arr);

//#endif

#ifndef USART_H
#define USART_H
#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"                   // 新增：FreeRTOS核心头文件
#include "queue.h"                      // 新增：队列操作头文件

#define BUFFSIZE 200
#define USART_REC_LEN BUFFSIZE          // 新增：统一接收长度宏（对齐DMA和队列）

extern char message[200];
extern uint8_t flag;
extern QueueHandle_t uartRecvQueue;     // 新增：串口接收队列句柄声明

void USART_DMAConfig(void );
void USARTInit(void);
void Send_Byte(uint8_t data);
void Send_Arr(uint8_t *arr,uint16_t len);
void Send_String(char *arr);

#endif
