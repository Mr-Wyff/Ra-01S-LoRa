#ifndef __COMMON_H__
#define __COMMON_H__
/*���������һЩͨ�÷���ֵ��debug������Ϣ����Ϣ*/

//�ⲿ���õ�ͷ�ļ�
#include "stdio.h"
#include "common_API.h"
#include "project_config.h"	// to_do_something ����Ҫ�����Լ����������̵������ļ�����������ܶ����ö���ʹ��Ĭ������

//ע������汾����common.h�İ汾�ţ������������̵İ汾�ţ�����ʵ��ģ�黯����
#define COMMON_H_PLATFORM_MAJOR_VERSION		0 //��Ҫ�汾��(0~255)������ɾ�����޸�֮ǰ�Ķ���ӿ�ʱ(��ɾ������ֵ��ATָ���ʽ��д����ܵ���ɾ��)��������汾�ţ�����汾�Ÿı��ʾ�߰汾��һ�����ݵͰ汾����ʱ��Ҫ�汾������Ҳ������汾�ţ�
#define COMMON_H_PLATFORM_MINOR_SUBVERSION	0 //��Ҫ�汾��(0~255)��������������Ľӿ�ʱ(����ӷ���ֵ)��������汾�ţ�����汾�Ÿ߰汾���ݵͰ汾
#define COMMON_H_PLATFORM_ITERATIVE_REVISION	2 //�����汾��(0~255)������汾��û�иı����ӿڣ�ֻ���޸��˲���bug�����Ż��˲��ִ����ʱ���޸ĵİ汾��

#ifndef ENABLE_DEBUG
	#define ENABLE_DEBUG	1	//�Ƿ��ӡdebug��Ϣ��1��ӡ0����ӡ
#endif /*end of ENABLE_DEBUG */

#ifndef ENABLE_ERROR_LOG
	#define ENABLE_ERROR_LOG	1	//�Ƿ��ӡ������Ϣ��1��ӡ0����ӡ
#endif /*end of ENABLE_ERROR_LOG */

#ifndef AT_SERVICE_BUFLEN
	#define AT_SERVICE_BUFLEN	512	//tmp_print_buf��ӡbuf��С
#endif /*end of AT_SERVICE_BUFLEN */

#ifndef ENABLE_INFO_LOG
	#define ENABLE_INFO_LOG	1	//�Ƿ��ӡ��ʾ��Ϣ(AT_PRINT()����)��1��ӡ0����ӡ
#endif /*end of ENABLE_INFO_LOG */

typedef enum __common_ret{
	COMMON_RET_OK=0,			//û�д���
	COMMON_RET_ERROR=1,			//��������
	COMMON_RET_PARAM_FORM_ERROR=2,//������ʽ����
	COMMON_RET_PARAM_ERROR=3,	//��������(ָ�������ݴ���)
	COMMON_RET_DEV_LOCKED=4,	//��ʾ�豸�Ѿ��������ˣ����ܲ���
	COMMON_RET_BUS_BUSY=5,		//��ʾ����æ
	COMMON_RET_MULTIPLE_ERROR=253,	//��ʾ�ж������(����������ֵ��Ӳ�Ϊ���ʾ������һ��������COMMON_RET_OK��������һ������)
	COMMON_RET_NOT_SUPPORT=254,	//��ʱ��֧�ֵĲ���
	COMMON_RET_TIME_OUT=255	//��ʱ
}common_ret;

//������ʾ����״̬
typedef enum
{
  COMMON_UNLOCKED = 0x00,
  COMMON_LOCKED   = 0x01  
}common_LockTypeDef;

#if ENABLE_DEBUG	//�Ƿ��ӡ������Ϣ
	#define DEBUG( format , ... )	printf( format , ##__VA_ARGS__ )
#else
	#define DEBUG( format , ... )
#endif /*end of ENABLE_DEBUG*/

#if ENABLE_ERROR_LOG	//�Ƿ��ӡ������Ϣ
	#define ERROR_LOG( format , ... )	printf( format , ##__VA_ARGS__ )
#else
	#define ERROR_LOG( format , ... )
#endif /*end of ENABLE_ERROR_LOG*/

#if ENABLE_INFO_LOG	//�Ƿ��ӡ��ʾ��Ϣ(�������ָ������������治ͬ��λ�ã�������һ������)
	extern char tmp_print_buf[AT_SERVICE_BUFLEN];	//�����ӡ���ȳ��� AT_SERVICE_BUFLEN �ᱻ��ȡ��������ͷ�ļ��������static
	#define at_printf(fmt, args...)  do{\
				snprintf(tmp_print_buf, AT_SERVICE_BUFLEN, fmt, ##args); \
				uart_at_send_string(tmp_print_buf);\
		}while(0)
	#define AT_PRINT( format , ... )	at_printf(format, ##__VA_ARGS__ );	//������ڵ�ǰ��ATָ������
#else
	#define AT_PRINT( format , ... )
#endif /*end of ENABLE_AT_PRINT*/

//����궨���������ݽṹ���ĳ����Ա������ַ��ȡ�ṹ��ĵ�ַ
//ptr:�������֪��ĳ���ṹ���Ա�ĵ�ַ
//type:���������Ҫ��ȡ�Ľṹ��ĵ�ַ
//meber:�����ʾptr��type�ṹ����ĸ���Ա���������ǽṹ�嶨��ĵط��ĳ�Ա�������֣�
#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#endif /*end of __COMMON_H__ */
