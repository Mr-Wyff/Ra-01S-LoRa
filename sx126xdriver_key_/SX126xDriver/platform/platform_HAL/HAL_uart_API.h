#ifndef __HAL_UART_API_H__
#define __HAL_UART_API_H__
//ƽ̨�޹�ͷ�ļ�
#include "common.h"
//ƽ̨���ͷ�ļ�
#include "stm32f10x.h"

//������ʾ���ڵ�����
typedef enum
{
	//ע����stm32f103��û��uart0
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
	HAL_UartName	uartName;	//uart������(eg:HAL_UART1)
	HAL_UartcallBack_t	callBack;	//���ջص�����,����ֻ�Ǽ�¼һ�£�������ң�ʵ�ʵ������� HAL_UartCallBackArray �����е��õ�
}HAL_Uart_t;

common_ret HALUartInit(HAL_Uart_t *uartObj,HAL_UartName uartName,uint32_t uartBand,HAL_UartcallBack_t uartCallBackFunc);
void HALUarSendData(HAL_UartName uartName,uint8_t data);
void HALUarSendBuf(HAL_UartName uartName,uint8_t *buf,uint32_t len);

#endif //end of __HAL_UART_API_H__
