#ifndef __MY_DELAY_H__
#define __MY_DELAY_H__
//硬件无关的
//硬件相关的
#include "stm32f10x.h"

extern __IO uint32_t g_u32DelayCount;

void DelayMs(uint32_t delayMS);

#endif //end of __MY_DELAY_H__
