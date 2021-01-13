#ifndef __HAL_COMMON_H__
#define __HAL_COMMON_H__
//硬件无关的
//硬件相关的
#include "stm32f10x.h"

//注意这个版本号是platform_HAL的版本号，不是整个工程的版本号，这里实行模块化管理
#define HAL_PLATFORM_MAJOR_VERSION		0 //主要版本号(0~255)，当有删除或修改之前的对外接口时(如删除返回值、AT指令格式重写、框架的增删改)增加这个版本号，这个版本号改变表示高版本不一定兼容低版本（有时次要版本号满了也会改主版本号）
#define HAL_PLATFORM_MINOR_SUBVERSION	0 //次要版本号(0~255)，当有新增对外的接口时(如添加返回值)增加这个版本号，这个版本号高版本兼容低版本
#define HAL_PLATFORM_ITERATIVE_REVISION	0 //迭代版本号(0~255)，这个版本是没有改变对外接口，只是修复了部分bug或者优化了部分代码的时候修改的版本号

uint32_t CommonMcuInit(void);

#endif //end of __HAL_COMMON_H__
