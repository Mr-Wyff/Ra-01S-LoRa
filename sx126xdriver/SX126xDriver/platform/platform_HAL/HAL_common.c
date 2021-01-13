//硬件无关的
#include "HAL_common.h"
#include "common.h"

//下面定义的是BoardInitMcu()中返回值的含义(一位表示一种错误)
#define BOARD_INIT_RCC_ERROR	0x00000001<1	//时钟配置错误

//初初始化板子
uint32_t CommonMcuInit(void){
	uint32_t u32_ret=0;	//用一个位表示一个状态

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//NVIC(中断优先级管理)分组配置,注意:这个分组整个程序只能有一次,配置后不要修改,否则会出现很多问题
	SysTick_Config(SystemCoreClock/1000);	//将滴答计时器设置为1ms溢出一次(除以N就表示1/N秒中断一次)(中断函数SysTick_Handler())

	//STM32F103系列会在main函数之前执行系统初始化函数void SystemInit (void)
	//SystemInit()在启动文件中自动调用，我们一般不用管，如果外部晶振需要接8M的(接其他频率需要自己配置时钟)，系统时钟会被自动倍频配置为72M
	//其他平台一般不会自动配置时钟，首先就需要我们自己配置时钟。
	//if(COMMON_RET_OK!=HAL_RCCInit()){
	//	u32_ret|=BOARD_INIT_RCC_ERROR;
	//}

	//继续其它需要初始化的内容

	return u32_ret;
}
