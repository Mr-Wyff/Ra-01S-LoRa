#ifndef __HAL_COMMON_H__
#define __HAL_COMMON_H__
//Ӳ���޹ص�
//Ӳ����ص�
#include "stm32f10x.h"

//ע������汾����platform_HAL�İ汾�ţ������������̵İ汾�ţ�����ʵ��ģ�黯����
#define HAL_PLATFORM_MAJOR_VERSION		0 //��Ҫ�汾��(0~255)������ɾ�����޸�֮ǰ�Ķ���ӿ�ʱ(��ɾ������ֵ��ATָ���ʽ��д����ܵ���ɾ��)��������汾�ţ�����汾�Ÿı��ʾ�߰汾��һ�����ݵͰ汾����ʱ��Ҫ�汾������Ҳ������汾�ţ�
#define HAL_PLATFORM_MINOR_SUBVERSION	0 //��Ҫ�汾��(0~255)��������������Ľӿ�ʱ(����ӷ���ֵ)��������汾�ţ�����汾�Ÿ߰汾���ݵͰ汾
#define HAL_PLATFORM_ITERATIVE_REVISION	0 //�����汾��(0~255)������汾��û�иı����ӿڣ�ֻ���޸��˲���bug�����Ż��˲��ִ����ʱ���޸ĵİ汾��

uint32_t CommonMcuInit(void);

#endif //end of __HAL_COMMON_H__
