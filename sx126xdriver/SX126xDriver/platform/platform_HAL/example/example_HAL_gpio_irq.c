//Ӳ���޹ص�
#include "HAL_gpio_API.h"
//Ӳ����ص�

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
	ret=HALGpioSetInterrupt(PA_0,HAL_GPIO_PULL_UP,HAL_IRQ_FALLING_EDGE,IRQ_MEDIUM_PRIORITY,HAL_GpioIrqLine0CallBack);//PA_0 �����������½��ش��������ȼ��У��ص�����HAL_GpioIrqLine0CallBack
	ret+=HALGpioSetInterrupt(PB_6,HAL_GPIO_PULL_DOWN,HAL_IRQ_RISING_EDGE,IRQ_MEDIUM_PRIORITY,HAL_GpioIrqLine6CallBack);//PB_6 ���������������ش��������ȼ��У��ص�����HAL_GpioIrqLine6CallBack
	ret+=HALGpioSetInterrupt(PB_11,HAL_GPIO_PULL_UP,HAL_IRQ_RISING_FALLING_EDGE,IRQ_MEDIUM_PRIORITY,HAL_GpioIrqLine11CallBack);//PB_11 ����������˫�����ش��������ȼ��У��ص�����HAL_GpioIrqLine11CallBack
}
