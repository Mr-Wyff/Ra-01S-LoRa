//和硬件无关的
#include "HAL_uart_API.h"
#include <string.h>
//和硬件相关的
#include "stm32f10x.h"

//这个是串口驱动的例子
//效果
//    uart1,PA_9(TX),PA_10(RX),波特率9600,不开启接收中断，可以发送数据，不能接收数据，初始化成功可以接收到“Uart1 init ok”
//    uart2,PA_2(TX),PA_3(RX),115200,开启接收中断,回调函数myUart2CallBack，可以收发数据，上电可以接收到一些debug信息和“Uart2 init ok”，输入可以实现echo回显(如果接收一个字符打印多个字符会导致中断时间太长导致后面的数据丢失)

//串口2的回调函数
void myUart2CallBack(uint8_t data){
	//HALUarSendBuf(HAL_UART1,(uint8_t *)"uart2 recive:",strlen("uart2 recive:"));
	HALUarSendData(HAL_UART2,data);
	//HALUarSendData(HAL_UART1,'\r');
	//HALUarSendData(HAL_UART1,'\n');
}

common_ret ExampleUartInit(void){
	common_ret ret=COMMON_RET_ERROR;
	//HAL_Uart_t uart1Obj,uart2Obj;
	HAL_Uart_t uart1Obj;

	ret=HALUartInit(&uart1Obj,HAL_UART1,115200,NULL);//驱动uart1,PA_9(TX),PA_10(RX),波特率9600,不开启接收中断
	if(COMMON_RET_OK!=ret){
		return ret;
	}
	/*HALUarSendData(HAL_UART1,'1');
	HALUarSendData(HAL_UART1,'\n');
	HALUarSendBuf(HAL_UART1,(uint8_t *)"Uart1 init ok\r\n",strlen("Uart1 init ok\r\n") );*/
	
	/*ret=HALUartInit(&uart2Obj,HAL_UART2,115200,myUart2CallBack);//驱动uart2,PA_2(TX),PA_3(RX),115200,开启接收中断,回调函数myUart2CallBack
	if(COMMON_RET_OK!=ret){
		return ret;
	}

	HALUarSendData(HAL_UART2,'2');
	HALUarSendData(HAL_UART2,'\n');
	HALUarSendBuf(HAL_UART2,(uint8_t *)"Uart2 init ok\r\n",strlen("Uart2 init ok\r\n") );
	
	HALUarSendData(HAL_UART1,'3');
	HALUarSendData(HAL_UART2,'4');*/
	//printf("\r\nprintf from uart2\r\n");
	
	return ret;
}

