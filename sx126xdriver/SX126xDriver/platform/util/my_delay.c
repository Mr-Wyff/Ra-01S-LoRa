//硬件无关的
#include "my_delay.h"
//硬件相关的

__IO uint32_t g_u32DelayCount=0;

//设置延时
//注意：使用之前要先初始化systick,并在systick中断中执行g_u32DelayCount--直到g_u32DelayCount为0停止
void DelayMs(uint32_t delayMS){
	g_u32DelayCount=delayMS;
	while(g_u32DelayCount);
}
