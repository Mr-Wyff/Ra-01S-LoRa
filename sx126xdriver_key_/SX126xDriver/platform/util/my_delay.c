//Ӳ���޹ص�
#include "my_delay.h"
//Ӳ����ص�

__IO uint32_t g_u32DelayCount=0;

//������ʱ
//ע�⣺ʹ��֮ǰҪ�ȳ�ʼ��systick,����systick�ж���ִ��g_u32DelayCount--ֱ��g_u32DelayCountΪ0ֹͣ
void DelayMs(uint32_t delayMS){
	g_u32DelayCount=delayMS;
	while(g_u32DelayCount);
}
