#ifndef __COMMON_H__
#define __COMMON_H__
/*这个定义了一些通用返回值，debug调试信息等信息*/

//外部引用的头文件
#include "stdio.h"
#include "common_API.h"
#include "project_config.h"	// to_do_something 这里要引用自己的整个工程的配置文件，否则下面很多配置都会使用默认配置

//注意这个版本号是common.h的版本号，不是整个工程的版本号，这里实行模块化管理
#define COMMON_H_PLATFORM_MAJOR_VERSION		0 //主要版本号(0~255)，当有删除或修改之前的对外接口时(如删除返回值、AT指令格式重写、框架的增删改)增加这个版本号，这个版本号改变表示高版本不一定兼容低版本（有时次要版本号满了也会改主版本号）
#define COMMON_H_PLATFORM_MINOR_SUBVERSION	0 //次要版本号(0~255)，当有新增对外的接口时(如添加返回值)增加这个版本号，这个版本号高版本兼容低版本
#define COMMON_H_PLATFORM_ITERATIVE_REVISION	2 //迭代版本号(0~255)，这个版本是没有改变对外接口，只是修复了部分bug或者优化了部分代码的时候修改的版本号

#ifndef ENABLE_DEBUG
	#define ENABLE_DEBUG	1	//是否打印debug信息，1打印0不打印
#endif /*end of ENABLE_DEBUG */

#ifndef ENABLE_ERROR_LOG
	#define ENABLE_ERROR_LOG	1	//是否打印错误信息，1打印0不打印
#endif /*end of ENABLE_ERROR_LOG */

#ifndef AT_SERVICE_BUFLEN
	#define AT_SERVICE_BUFLEN	512	//tmp_print_buf打印buf大小
#endif /*end of AT_SERVICE_BUFLEN */

#ifndef ENABLE_INFO_LOG
	#define ENABLE_INFO_LOG	1	//是否打印提示信息(AT_PRINT()函数)，1打印0不打印
#endif /*end of ENABLE_INFO_LOG */

typedef enum __common_ret{
	COMMON_RET_OK=0,			//没有错误
	COMMON_RET_ERROR=1,			//其他错误
	COMMON_RET_PARAM_FORM_ERROR=2,//参数格式错误
	COMMON_RET_PARAM_ERROR=3,	//参数错误(指参数内容错误)
	COMMON_RET_DEV_LOCKED=4,	//表示设备已经被加锁了，不能操作
	COMMON_RET_BUS_BUSY=5,		//表示总线忙
	COMMON_RET_MULTIPLE_ERROR=253,	//表示有多个错误(例如多个返回值相加不为零表示至少有一个不等于COMMON_RET_OK，至少有一个错误)
	COMMON_RET_NOT_SUPPORT=254,	//暂时不支持的操作
	COMMON_RET_TIME_OUT=255	//超时
}common_ret;

//用来表示加锁状态
typedef enum
{
  COMMON_UNLOCKED = 0x00,
  COMMON_LOCKED   = 0x01  
}common_LockTypeDef;

#if ENABLE_DEBUG	//是否打印调试信息
	#define DEBUG( format , ... )	printf( format , ##__VA_ARGS__ )
#else
	#define DEBUG( format , ... )
#endif /*end of ENABLE_DEBUG*/

#if ENABLE_ERROR_LOG	//是否打印报错信息
	#define ERROR_LOG( format , ... )	printf( format , ##__VA_ARGS__ )
#else
	#define ERROR_LOG( format , ... )
#endif /*end of ENABLE_ERROR_LOG*/

#if ENABLE_INFO_LOG	//是否打印提示信息(这个可以指定输出到和上面不同的位置，例如另一个串口)
	extern char tmp_print_buf[AT_SERVICE_BUFLEN];	//这个打印长度超过 AT_SERVICE_BUFLEN 会被截取，这里是头文件，必须加static
	#define at_printf(fmt, args...)  do{\
				snprintf(tmp_print_buf, AT_SERVICE_BUFLEN, fmt, ##args); \
				uart_at_send_string(tmp_print_buf);\
		}while(0)
	#define AT_PRINT( format , ... )	at_printf(format, ##__VA_ARGS__ );	//这个会在当前的AT指令串口输出
#else
	#define AT_PRINT( format , ... )
#endif /*end of ENABLE_AT_PRINT*/

//这个宏定义用来根据结构体的某个成员变量地址获取结构体的地址
//ptr:这个是已知的某个结构体成员的地址
//type:这个是我们要获取的结构体的地址
//meber:这个表示ptr是type结构体的哪个成员变量（就是结构体定义的地方的成员变量名字）
#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#endif /*end of __COMMON_H__ */
