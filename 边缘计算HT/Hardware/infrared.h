#ifndef INFRARED_H
#define INFRARED_H
#include "stm32f10x.h"

void InfraredInit(void);
uint8_t InfraredRead(void);

#endif
