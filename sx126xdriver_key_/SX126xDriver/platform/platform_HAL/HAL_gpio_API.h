#ifndef __HAL_GPIO_API_H__
#define __HAL_GPIO_API_H__
//��Ӳ���޹ص�
#include "common.h"

//��Ӳ����ص�
#include "stm32f10x.h"

//��Ϊͨ��һ��GPIO�պ�16������ʮ�����Ʊ�ʾ�պ����һλ��ʾ�ڼ���gpio������λ��ʾ��
//����PA_0=0X00��PB_0=OX10��PC_0=0X20 ...
#define HAL_MCU_PINS \
	PA_0 = 0, PA_1, PA_2, PA_3, PA_4, PA_5, PA_6, PA_7, PA_8, PA_9, PA_10, PA_11, PA_12, PA_13, PA_14, PA_15, \
	PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7, PB_8, PB_9, PB_10, PB_11, PB_12, PB_13, PB_14, PB_15,     \
	PC_0, PC_1, PC_2, PC_3, PC_4, PC_5, PC_6, PC_7, PC_8, PC_9, PC_10, PC_11, PC_12, PC_13, PC_14, PC_15,     \
	PD_0, PD_1, PD_2, PD_3, PD_4, PD_5, PD_6, PD_7, PD_8, PD_9, PD_10, PD_11, PD_12, PD_13, PD_14, PD_15,     \
	PE_0, PE_1, PE_2, PE_3, PE_4, PE_5, PE_6, PE_7, PE_8, PE_9, PE_10, PE_11, PE_12, PE_13, PE_14, PE_15,     \
	PF_0, PF_1, PF_2, PF_3, PF_4, PF_5, PF_6, PF_7, PF_8, PF_9, PF_10, PF_11, PF_12, PF_13, PF_14, PF_15,     \
	PG_0, PG_1, PG_2, PG_3, PG_4, PG_5, PG_6, PG_7, PG_8, PG_9, PG_10, PG_11, PG_12, PG_13, PG_14, PG_15

//����ͨ�õ�GPIO����
typedef enum
{
	HAL_MCU_PINS,
	//���ﻹ�������������һЩ�ⲿGPIO�����ֶ��壬�����������Ŀ��ƣ�ʹ�õ�ʱ����Ҫ����Ӧ�ĳ�ʼ���Ͷ�д����������Դ���

	// Not connected
	HAL_GPIO_NC = 0xFFFF
}HAL_GpioPinNames;

typedef enum
{
	HAL_GPIO_INPUT = 0,	//����
	HAL_GPIO_PUSH_PULL_OUTPUT,	//�������
	HAL_GPIO_OPEN_DRAIN_OUTPUT,	//��©���
	HAL_GPIO_ALTERNATE_FCT_PP,	//Ƭ�����裬����ģʽ
	HAL_GPIO_ALTERNATE_FCT_OD,	//Ƭ�����裬��©ģʽ
	HAL_GPIO_ANALOG_IN,		//ģ������
	HAL_GPIO_ANALOG_OUT,	//ģ�����
}HAL_GpioPinModes;

typedef enum
{
	HAL_GPIO_NO_PULL = 0,//û��������
	HAL_GPIO_PULL_UP,	//����
	HAL_GPIO_PULL_DOWN	//����
}HAL_GpioPinPullTypes;

typedef struct
{
	HAL_GpioPinNames	pinName;	//pin�ŵ�����(eg:PA_11)
	uint16_t	pinIndex;	//��ʾ�ǵڼ�����,eg:PB_5 �����index���� GPIO_Pin_5���������������Ҫ���ݲ�ͬƽ̨���� __WAIT_TODO__
	GPIO_TypeDef	*portIndex;		//�����ʾ����ʹ������GPIO��eg:GPIOB���������������Ҫ���ݲ�ͬƽ̨���� __WAIT_TODO__
	HAL_GpioPinModes mode;
}HAL_Gpio_t;

//������ʽ
typedef enum
{
    HAL_IRQ_RISING_EDGE=0,	//�����ش���
    HAL_IRQ_FALLING_EDGE,	//�½��ش���
    HAL_IRQ_RISING_FALLING_EDGE	//˫���ش���
}HAL_IrqModes;

//�ж����ȼ�����
//�����stm32f103c8�������ж����ȼ�����Ϊ2��2λ��ռ���ȼ���2λ�����ȼ���
typedef enum
{
    IRQ_VERY_LOW_PRIORITY = 0,	//����0��ʾ���ȼ��ͣ�ʵ����stm32��0��ʾ���ȼ���ߣ�stm32f103c8������������ȼ���Ӧ��ռ���ȼ�3�����ȼ�3
    IRQ_LOW_PRIORITY,			//stm32f103c8������������ȼ���Ӧ��ռ���ȼ�3�����ȼ�0
    IRQ_MEDIUM_PRIORITY,		//stm32f103c8������������ȼ���Ӧ��ռ���ȼ�2�����ȼ�3
    IRQ_HIGH_PRIORITY,			//stm32f103c8������������ȼ���Ӧ��ռ���ȼ�1�����ȼ�3
    IRQ_VERY_HIGH_PRIORITY		//stm32f103c8������������ȼ���Ӧ��ռ���ȼ�0�����ȼ�0(�����)
}HAL_IrqPriorities;

typedef void (*HAL_GpioIrqCallBack)(void);

common_ret HALGpioInit(HAL_Gpio_t *gpioObj,HAL_GpioPinNames pinName,HAL_GpioPinModes mode,HAL_GpioPinPullTypes pullType,uint32_t value );
common_ret HALGpioWrite(HAL_Gpio_t gpioObj, uint32_t value );
uint32_t HALGpioRead(HAL_Gpio_t gpioObj);

common_ret HALGpioSetInterrupt(HAL_GpioPinNames pinName,HAL_GpioPinPullTypes pullType,HAL_IrqModes irqMode,HAL_IrqPriorities irqPriority,HAL_GpioIrqCallBack irqCallBack );

#endif	//end of __HAL_GPIO_API_H__
