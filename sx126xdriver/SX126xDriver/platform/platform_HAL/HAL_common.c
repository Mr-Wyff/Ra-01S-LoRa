//Ӳ���޹ص�
#include "HAL_common.h"
#include "common.h"

//���涨�����BoardInitMcu()�з���ֵ�ĺ���(һλ��ʾһ�ִ���)
#define BOARD_INIT_RCC_ERROR	0x00000001<1	//ʱ�����ô���

//����ʼ������
uint32_t CommonMcuInit(void){
	uint32_t u32_ret=0;	//��һ��λ��ʾһ��״̬

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//NVIC(�ж����ȼ�����)��������,ע��:���������������ֻ����һ��,���ú�Ҫ�޸�,�������ֺܶ�����
	SysTick_Config(SystemCoreClock/1000);	//���δ��ʱ������Ϊ1ms���һ��(����N�ͱ�ʾ1/N���ж�һ��)(�жϺ���SysTick_Handler())

	//STM32F103ϵ�л���main����֮ǰִ��ϵͳ��ʼ������void SystemInit (void)
	//SystemInit()�������ļ����Զ����ã�����һ�㲻�ùܣ�����ⲿ������Ҫ��8M��(������Ƶ����Ҫ�Լ�����ʱ��)��ϵͳʱ�ӻᱻ�Զ���Ƶ����Ϊ72M
	//����ƽ̨һ�㲻���Զ�����ʱ�ӣ����Ⱦ���Ҫ�����Լ�����ʱ�ӡ�
	//if(COMMON_RET_OK!=HAL_RCCInit()){
	//	u32_ret|=BOARD_INIT_RCC_ERROR;
	//}

	//����������Ҫ��ʼ��������

	return u32_ret;
}
