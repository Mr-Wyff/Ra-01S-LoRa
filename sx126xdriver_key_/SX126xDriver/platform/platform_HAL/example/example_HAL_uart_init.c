//��Ӳ���޹ص�
#include "HAL_uart_API.h"
#include <string.h>
//��Ӳ����ص�
#include "stm32f10x.h"

//����Ǵ�������������
//Ч��
//    uart1,PA_9(TX),PA_10(RX),������9600,�����������жϣ����Է������ݣ����ܽ������ݣ���ʼ���ɹ����Խ��յ���Uart1 init ok��
//    uart2,PA_2(TX),PA_3(RX),115200,���������ж�,�ص�����myUart2CallBack�������շ����ݣ��ϵ���Խ��յ�һЩdebug��Ϣ�͡�Uart2 init ok�����������ʵ��echo����(�������һ���ַ���ӡ����ַ��ᵼ���ж�ʱ��̫�����º�������ݶ�ʧ)

//����2�Ļص�����
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

	ret=HALUartInit(&uart1Obj,HAL_UART1,115200,NULL);//����uart1,PA_9(TX),PA_10(RX),������9600,�����������ж�
	if(COMMON_RET_OK!=ret){
		return ret;
	}
	/*HALUarSendData(HAL_UART1,'1');
	HALUarSendData(HAL_UART1,'\n');
	HALUarSendBuf(HAL_UART1,(uint8_t *)"Uart1 init ok\r\n",strlen("Uart1 init ok\r\n") );*/
	
	/*ret=HALUartInit(&uart2Obj,HAL_UART2,115200,myUart2CallBack);//����uart2,PA_2(TX),PA_3(RX),115200,���������ж�,�ص�����myUart2CallBack
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

