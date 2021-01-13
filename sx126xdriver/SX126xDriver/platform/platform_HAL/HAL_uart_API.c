//ƽ̨�޹�
#include "HAL_uart_API.h"
#include "HAL_gpio_API.h"
//ƽ̨���


#define ENALBE_PRINTF	//Ĭ�Ͽ���printf������֧�֣�ͨ��UART2��ӡ
#define HAL_MAX_UART_NUM	6	//stm32f103ϵ�������1-5������ڣ�0�����ˣ��������ﶨ�����6�������Ҫ����MCU���� __WAIT_TODO__

const static IRQn_Type HAL_UartIrqArray[HAL_MAX_UART_NUM]={(IRQn_Type)0xff,USART1_IRQn,USART2_IRQn,USART3_IRQn,(IRQn_Type)0xff,(IRQn_Type)0xff};//���� HAL_UartName ��ÿ�����ڶ�Ӧ���жϣ�����������Ʒ����UART4_IRQn,UART5_IRQn����������޸�ֱ�ӷ��ڴ�����ˣ������Ҫ����MCU���� __WAIT_TODO__

static HAL_UartcallBack_t HAL_UartCallBackArray[HAL_MAX_UART_NUM]={0};	//�����洢���ڻص����������˳��� HAL_UartName ˳���Ӧ
static USART_TypeDef *HAL_UartTypeDefArray[HAL_MAX_UART_NUM]={0,USART1,USART2,USART3,UART4,UART5};		//�����Ҫ����MCU���� __WAIT_TODO__

#ifdef ENALBE_PRINTF
/*��δ���������֧��printf��ӡ�����ڵĴ���*/
#pragma import(__use_no_semihosting)             
//��׼����Ҫ֧�ֵĺ���
struct __FILE 
{
	int handle;
};

FILE __stdout;       
//����_sys_exit()�Ա��⹤���ڰ�����״̬
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc����
//�����Ҫ����MCU������ϣ��printf���ĸ����������ȷ�� __WAIT_TODO__
int fputc(int ch, FILE *f)
{
	//ע�⣺USART_FLAG_TXE�Ǽ�鷢�ͻ������Ƿ�Ϊ�գ����Ҫ�ڷ���ǰ��飬������������߷���Ч�ʣ����������ߵ�ʱ����ܵ������һ���ַ���ʧ
	//USART_FLAG_TC�Ǽ�鷢����ɱ�־������ڷ��ͺ��飬����������˯�߶�ʧ�ַ����⣬����Ч�ʵͣ����͹����з��ͻ������Ѿ�Ϊ���ˣ����Խ�����һ�������ˣ�������ΪҪ�ȴ�������ɣ�����Ч�ʵͣ�
	//��Ҫ����һ���ã�һ����Ч�����
	
	//ѭ���ȴ�ֱ�����ͻ�����Ϊ��(TX Empty)��ʱ���Է������ݵ�������
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {}
	USART_SendData(USART1, (uint8_t) ch);

  /* ѭ���ȴ�ֱ�����ͽ���*/
  //while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  //{}

	return ch;
}
#endif /*end of ENALBE_PRINTF*/

//��ʼ������
//ע������һ�����ڹ̶�һ��λ��(ͨ��һ�����ڿ����ж��λ����ʹ�ã����ﲻ����ֱ��ָ����ĳ��λ�ã���ͨ�������޸�)
//������
//     uartObj����Ҫ��ʼ�Ĵ��ڽṹ��
//     uartName��Ҫ��ʼ���ĸ�����,��ʱʵ�������¼�������
//               HAL_UART1ָ����PA_9(TX)��PA_10(RX)
//               HAL_UART2ָ����PA_2(TX)��PA_3(RX)
//               ����û��ָ��
//     uartBand	����Ҫ��ʼ���Ĳ�����(��������һ�㲻�����ã�ֱ��ʹ��д����)
//     uartCallBackFunc�����յ�������Ϣ�жϵĻص�����(������ý�����Ϊ�գ�����Ϊ�պ�ʹ�ܽ����жϣ���ʱ���Է�������)
//ע�⣺����ʹ���жϱ���Ҫ�ڴ��ڳ�ʼ������֮ǰʹ��NVIC_PriorityGroupConfig()����NVIC(�ж����ȼ�����)(���������������ֻ����һ��,��Ҫ�޸�,�������ֺܶ�����)
//�����Ҫ����MCU���� __WAIT_TODO__
common_ret HALUartInit(HAL_Uart_t *uartObj,HAL_UartName uartName,uint32_t uartBand,HAL_UartcallBack_t uartCallBackFunc){
	common_ret ret=COMMON_RET_ERROR;
	HAL_Gpio_t uartTX,uartRX;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	if(NULL==uartObj || uartName > (HAL_MAX_UART_NUM-1) ){
		return COMMON_RET_PARAM_ERROR;
	}

	HAL_UartCallBackArray[uartName]=uartCallBackFunc;
	uartObj->callBack=uartCallBackFunc;

	USART_DeInit(HAL_UartTypeDefArray[uartName]);  //��λ����
	
	switch(uartName){
		case HAL_UART1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//ע��APB1��APB2ʱ��ʹ�ܺ�����һ��
			ret=HALGpioInit(&uartTX,PA_9,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			ret+=HALGpioInit(&uartRX,PA_10,HAL_GPIO_INPUT,HAL_GPIO_NO_PULL,0);
			if(COMMON_RET_OK!=ret){
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);//�ر�ʱ��
				ret=COMMON_RET_MULTIPLE_ERROR;
				goto OUT1;
			}
			break;
		case HAL_UART2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//ע��APB1��APB2ʱ��ʹ�ܺ�����һ��
			ret=HALGpioInit(&uartTX,PA_2,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			ret+=HALGpioInit(&uartRX,PA_3,HAL_GPIO_INPUT,HAL_GPIO_NO_PULL,0);
			if(COMMON_RET_OK!=ret){
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);//�ر�ʱ��
				ret=COMMON_RET_MULTIPLE_ERROR;
				goto OUT1;
			}
			break;
		case HAL_UART3://�����//break;
		case HAL_UART4://�����//break;
		case HAL_UART5:
			//RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);//ע��APB1��APB2ʱ��ʹ�ܺ�����һ��
			ret=COMMON_RET_NOT_SUPPORT;
			goto OUT1;
			//break;
		default:
			ret=COMMON_RET_PARAM_ERROR;
			goto OUT1;
			//break;
	}

	
	if(NULL!=uartCallBackFunc){//û�лص�������ʱ�����������жϣ���ʱ�Ĵ����ܷ�������
		//�ж����ȼ�������ǰ�ж����ȼ�����Ϊ2(NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2))�Ѿ����壬��Ҫ�޸�
		NVIC_InitStructure.NVIC_IRQChannel = HAL_UartIrqArray[uartName];	//ָ�������ĸ��ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�Ϊ3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//���� NVIC
	}

	//USART����
	USART_InitStructure.USART_BaudRate = uartBand;	//���ô��ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;	//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(HAL_UartTypeDefArray[uartName], &USART_InitStructure); //����USART����
	if(NULL!=uartCallBackFunc){
		USART_ITConfig(HAL_UartTypeDefArray[uartName], USART_IT_RXNE, ENABLE);	//���������ж�(�������޷���������,û�н��ջص������Ͳ������ˣ���Ϊû�лص���û�д�����յĵ����ݣ�������Ҳû������)
	}
	USART_Cmd(HAL_UartTypeDefArray[uartName], ENABLE);                    //ʹ��USART
	ret=COMMON_RET_OK;
	goto OUT1;

OUT1:
	return ret;
}

//�Ӵ��ڷ���1byte����
//�����Ҫ����MCU���� __WAIT_TODO__
void HALUarSendData(HAL_UartName uartName,uint8_t data){
	uint16_t u16_tmp=data;
	
	//ѭ���ȴ�ֱ�����ͻ�����Ϊ��(TX Empty)��ʱ���Է������ݵ��������������ַ����Ͳ���ⷢ����� HALUarSendBuf() �з������һ���ַ�֮���鷢�����״̬
  while (USART_GetFlagStatus(HAL_UartTypeDefArray[uartName], USART_FLAG_TXE) == RESET)
  {}

	USART_SendData(HAL_UartTypeDefArray[uartName],u16_tmp);
}

//����һ������
//������
//     uartName��Ҫ���͵��ĸ�����
//     buf	����������ĵ�ַ
//     len	�����Ͷ����ֽ�����
void HALUarSendBuf(HAL_UartName uartName,uint8_t *buf,uint32_t len){
	uint32_t i=0;
	for(i=0;i<len;i++){
		HALUarSendData(uartName,buf[i]);
	}
	//ѭ���ȴ�ֱ�����ͽ���(���ﲻ�ȴ��Ļ�������ڴ�ӡ�����д����ᵼ�����һ���ַ�û�д�ӡ�����͹ر���)
  while (USART_GetFlagStatus(HAL_UartTypeDefArray[uartName], USART_FLAG_TC) == RESET)
  {}
}

//USART1�жϺ���(����)
//�����Ҫ����MCU���� __WAIT_TODO__
void USART1_IRQHandler(void){
	if(USART_GetFlagStatus(HAL_UartTypeDefArray[1], USART_FLAG_RXNE) == SET)  
	{
		if(NULL!=HAL_UartCallBackArray[1]){
			(*HAL_UartCallBackArray[1])(USART_ReceiveData(HAL_UartTypeDefArray[1]));	//ִ�лص�����
		}
	}
}

//USART2�жϺ���(����)
//�����Ҫ����MCU���� __WAIT_TODO__
void USART2_IRQHandler(void){
	if(USART_GetFlagStatus(HAL_UartTypeDefArray[2], USART_FLAG_RXNE) == SET)  
	{
		if(NULL!=HAL_UartCallBackArray[2]){
			(*HAL_UartCallBackArray[2])(USART_ReceiveData(HAL_UartTypeDefArray[2]));	//ִ�лص�����
		}
	}
}

//USART3�жϺ���(����)
//�����Ҫ����MCU���� __WAIT_TODO__
void USART3_IRQHandler(void){
	if(USART_GetFlagStatus(HAL_UartTypeDefArray[3], USART_FLAG_RXNE) == SET)  
	{
		if(NULL!=HAL_UartCallBackArray[3]){
			(*HAL_UartCallBackArray[3])(USART_ReceiveData(HAL_UartTypeDefArray[3]));	//ִ�лص�����
		}
	}
}
/*
//UART4�жϺ���(����)
//�����Ҫ����MCU���� __WAIT_TODO__
void UART4_IRQHandl(void){
	if(USART_GetFlagStatus(HAL_UartTypeDefArray[4], USART_FLAG_RXNE) == SET)  
	{
		if(NULL!=HAL_UartCallBackArray[4]){
			(*HAL_UartCallBackArray[4])(USART_ReceiveData(HAL_UartTypeDefArray[4]));	//ִ�лص�����
		}
	}
}

//UART5�жϺ���(����)
//�����Ҫ����MCU���� __WAIT_TODO__
void UART5_IRQHandl(void){
	if(USART_GetFlagStatus(HAL_UartTypeDefArray[5], USART_FLAG_RXNE) == SET)  
	{
		if(NULL!=HAL_UartCallBackArray[5]){
			(*HAL_UartCallBackArray[5])(USART_ReceiveData(HAL_UartTypeDefArray[5]));	//ִ�лص�����
		}
	}
}*/

