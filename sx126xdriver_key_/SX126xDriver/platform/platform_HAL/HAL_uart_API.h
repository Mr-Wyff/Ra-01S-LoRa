#ifndef __HAL_UART_API_H__
#define __HAL_UART_API_H__
//平台无关头文件
#include "common.h"
//平台相关头文件
#include "stm32f10x.h"

//用来表示串口的名字
typedef enum
{
	//注意在stm32f103中没有uart0
	HAL_UART1=1,
	HAL_UART2,
	HAL_UART3,
	HAL_UART4,
	HAL_UART5,
	HAL_UART_NC=0xff
}HAL_UartName;

typedef void (*HAL_UartcallBack_t)(uint8_t data);

typedef struct
{
	HAL_UartName	uartName;	//uart的名字(eg:HAL_UART1)
	HAL_UartcallBack_t	callBack;	//接收回调函数,这里只是记录一下，方便查找，实际调用是用 HAL_UartCallBackArray 数组中调用的
}HAL_Uart_t;

common_ret HALUartInit(HAL_Uart_t *uartObj,HAL_UartName uartName,uint32_t uartBand,HAL_UartcallBack_t uartCallBackFunc);
void HALUarSendData(HAL_UartName uartName,uint8_t data);
void HALUarSendBuf(HAL_UartName uartName,uint8_t *buf,uint32_t len);

#endif //end of __HAL_UART_API_H__
