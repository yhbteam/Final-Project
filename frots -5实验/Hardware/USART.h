#ifndef USART_H
#define USART_H
#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <string.h>

#define BUFFSIZE 200
extern char message[200];
extern uint8_t flag;

void USART_DMAConfig(void );
void USARTInit(void);
void Send_Byte(uint8_t data);
void Send_Arr(uint8_t *arr,uint16_t len);
void Send_String(char *arr);

#endif

