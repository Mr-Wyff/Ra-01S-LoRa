#ifndef __MY_DELAY_H__
#define __MY_DELAY_H__
//Ӳ���޹ص�
//Ӳ����ص�
#include "stm32f10x.h"

extern __IO uint32_t g_u32DelayCount;

void DelayMs(uint32_t delayMS);

#endif //end of __MY_DELAY_H__
