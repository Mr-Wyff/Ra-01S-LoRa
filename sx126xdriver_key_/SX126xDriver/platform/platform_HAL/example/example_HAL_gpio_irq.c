//硬件无关的
#include "HAL_gpio_API.h"
//硬件相关的

void HAL_GpioIrqLine0CallBack(void){
	printf("irq func:%s()\r\n",__func__);
}

void HAL_GpioIrqLine6CallBack(void){
	printf("irq func:%s()\r\n",__func__);
}

void HAL_GpioIrqLine11CallBack(void){
	printf("irq func:%s()\r\n",__func__);
}

void ExampleHALGpioIrq(void){
	uint16_t ret=0;
	ret=HALGpioSetInterrupt(PA_0,HAL_GPIO_PULL_UP,HAL_IRQ_FALLING_EDGE,IRQ_MEDIUM_PRIORITY,HAL_GpioIrqLine0CallBack);//PA_0 设置上拉，下降沿触发，优先级中，回调函数HAL_GpioIrqLine0CallBack
	ret+=HALGpioSetInterrupt(PB_6,HAL_GPIO_PULL_DOWN,HAL_IRQ_RISING_EDGE,IRQ_MEDIUM_PRIORITY,HAL_GpioIrqLine6CallBack);//PB_6 设置下拉，上升沿触发，优先级中，回调函数HAL_GpioIrqLine6CallBack
	ret+=HALGpioSetInterrupt(PB_11,HAL_GPIO_PULL_UP,HAL_IRQ_RISING_FALLING_EDGE,IRQ_MEDIUM_PRIORITY,HAL_GpioIrqLine11CallBack);//PB_11 设置上拉，双边沿沿触发，优先级中，回调函数HAL_GpioIrqLine11CallBack
}
