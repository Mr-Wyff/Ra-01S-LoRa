//平台无关
#include "HAL_uart_API.h"
#include "HAL_gpio_API.h"
//平台相关


#define ENALBE_PRINTF	//默认开启printf函数的支持，通过UART2打印
#define HAL_MAX_UART_NUM	6	//stm32f103系列最多有1-5五个串口，0跳过了，所以这里定义的是6，这个需要根据MCU适配 __WAIT_TODO__

const static IRQn_Type HAL_UartIrqArray[HAL_MAX_UART_NUM]={(IRQn_Type)0xff,USART1_IRQn,USART2_IRQn,USART3_IRQn,(IRQn_Type)0xff,(IRQn_Type)0xff};//定义 HAL_UartName 中每个串口对应的中断，（大容量产品还有UART4_IRQn,UART5_IRQn）这个不用修改直接放在代码段了，这个需要根据MCU适配 __WAIT_TODO__

static HAL_UartcallBack_t HAL_UartCallBackArray[HAL_MAX_UART_NUM]={0};	//用来存储串口回调函数，这个顺序和 HAL_UartName 顺序对应
static USART_TypeDef *HAL_UartTypeDefArray[HAL_MAX_UART_NUM]={0,USART1,USART2,USART3,UART4,UART5};		//这个需要根据MCU适配 __WAIT_TODO__

#ifdef ENALBE_PRINTF
/*这段代码是用来支持printf打印到串口的代码*/
#pragma import(__use_no_semihosting)             
//标准库需要支持的函数
struct __FILE 
{
	int handle;
};

FILE __stdout;       
//定义_sys_exit()以避免工作在半主机状态
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数
//这个需要根据MCU和我们希望printf从哪个串口输出来确认 __WAIT_TODO__
int fputc(int ch, FILE *f)
{
	//注意：USART_FLAG_TXE是检查发送缓冲区是否为空，这个要在发送前检查，检查这个提议提高发送效率，但是在休眠的时候可能导致最后一个字符丢失
	//USART_FLAG_TC是检查发送完成标志，这个在发送后检查，这个不会出现睡眠丢失字符问题，但是效率低（发送过程中发送缓冲区已经为空了，可以接收下一个数据了，但是因为要等待发送完成，所以效率低）
	//不要两个一起用，一起用效率最低
	
	//循环等待直到发送缓冲区为空(TX Empty)此时可以发送数据到缓冲区
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {}
	USART_SendData(USART1, (uint8_t) ch);

  /* 循环等待直到发送结束*/
  //while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  //{}

	return ch;
}
#endif /*end of ENALBE_PRINTF*/

//初始化串口
//注意这里一个串口固定一个位置(通常一个串口可以有多个位置来使用，这里不区分直接指定在某个位置，不通过传参修改)
//参数：
//     uartObj：需要初始的串口结构体
//     uartName：要初始化哪个串口,暂时实现了以下几个串口
//               HAL_UART1指定到PA_9(TX)，PA_10(RX)
//               HAL_UART2指定到PA_2(TX)，PA_3(RX)
//               其他没有指定
//     uartBand	：需要初始化的波特率(其它参数一般不用设置，直接使用写死了)
//     uartCallBackFunc：接收到串口消息中断的回调函数(如果不用接设置为空，设置为空后不使能接收中断，此时可以发不能收)
//注意：串口使用中断必须要在串口初始化函数之前使用NVIC_PriorityGroupConfig()配置NVIC(中断优先级管理)(这个在整个代码中只配置一次,不要修改,否则会出现很多问题)
//这个需要根据MCU适配 __WAIT_TODO__
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

	USART_DeInit(HAL_UartTypeDefArray[uartName]);  //复位串口
	
	switch(uartName){
		case HAL_UART1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//注意APB1和APB2时钟使能函数不一样
			ret=HALGpioInit(&uartTX,PA_9,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			ret+=HALGpioInit(&uartRX,PA_10,HAL_GPIO_INPUT,HAL_GPIO_NO_PULL,0);
			if(COMMON_RET_OK!=ret){
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);//关闭时钟
				ret=COMMON_RET_MULTIPLE_ERROR;
				goto OUT1;
			}
			break;
		case HAL_UART2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//注意APB1和APB2时钟使能函数不一样
			ret=HALGpioInit(&uartTX,PA_2,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			ret+=HALGpioInit(&uartRX,PA_3,HAL_GPIO_INPUT,HAL_GPIO_NO_PULL,0);
			if(COMMON_RET_OK!=ret){
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);//关闭时钟
				ret=COMMON_RET_MULTIPLE_ERROR;
				goto OUT1;
			}
			break;
		case HAL_UART3://待完成//break;
		case HAL_UART4://待完成//break;
		case HAL_UART5:
			//RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);//注意APB1和APB2时钟使能函数不一样
			ret=COMMON_RET_NOT_SUPPORT;
			goto OUT1;
			//break;
		default:
			ret=COMMON_RET_PARAM_ERROR;
			goto OUT1;
			//break;
	}

	
	if(NULL!=uartCallBackFunc){//没有回调函数的时候不启动接收中断，此时的串口能发不能收
		//中断优先级管理，当前中断优先级分组为2(NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2))已经定义，不要修改
		NVIC_InitStructure.NVIC_IRQChannel = HAL_UartIrqArray[uartName];	//指定配置哪个中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级为3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//配置 NVIC
	}

	//USART配置
	USART_InitStructure.USART_BaudRate = uartBand;	//设置串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;	//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(HAL_UartTypeDefArray[uartName], &USART_InitStructure); //配置USART参数
	if(NULL!=uartCallBackFunc){
		USART_ITConfig(HAL_UartTypeDefArray[uartName], USART_IT_RXNE, ENABLE);	//开启接收中断(不开启无法接收数据,没有接收回调函数就不开启了，因为没有回调就没有处理接收的到数据，开启了也没有意义)
	}
	USART_Cmd(HAL_UartTypeDefArray[uartName], ENABLE);                    //使能USART
	ret=COMMON_RET_OK;
	goto OUT1;

OUT1:
	return ret;
}

//从串口发送1byte数据
//这个需要根据MCU适配 __WAIT_TODO__
void HALUarSendData(HAL_UartName uartName,uint8_t data){
	uint16_t u16_tmp=data;
	
	//循环等待直到发送缓冲区为空(TX Empty)此时可以发送数据到缓冲区，单个字符发送不检测发送完成 HALUarSendBuf() 中发送最后一个字符之后检查发送完成状态
  while (USART_GetFlagStatus(HAL_UartTypeDefArray[uartName], USART_FLAG_TXE) == RESET)
  {}

	USART_SendData(HAL_UartTypeDefArray[uartName],u16_tmp);
}

//发送一段数据
//参数：
//     uartName：要发送到哪个串口
//     buf	：发送数组的地址
//     len	：发送多少字节数据
void HALUarSendBuf(HAL_UartName uartName,uint8_t *buf,uint32_t len){
	uint32_t i=0;
	for(i=0;i<len;i++){
		HALUarSendData(uartName,buf[i]);
	}
	//循环等待直到发送结束(这里不等待的话如果正在打印过程中待机会导致最后一个字符没有打印出来就关闭了)
  while (USART_GetFlagStatus(HAL_UartTypeDefArray[uartName], USART_FLAG_TC) == RESET)
  {}
}

//USART1中断函数(接收)
//这个需要根据MCU适配 __WAIT_TODO__
void USART1_IRQHandler(void){
	if(USART_GetFlagStatus(HAL_UartTypeDefArray[1], USART_FLAG_RXNE) == SET)  
	{
		if(NULL!=HAL_UartCallBackArray[1]){
			(*HAL_UartCallBackArray[1])(USART_ReceiveData(HAL_UartTypeDefArray[1]));	//执行回调函数
		}
	}
}

//USART2中断函数(接收)
//这个需要根据MCU适配 __WAIT_TODO__
void USART2_IRQHandler(void){
	if(USART_GetFlagStatus(HAL_UartTypeDefArray[2], USART_FLAG_RXNE) == SET)  
	{
		if(NULL!=HAL_UartCallBackArray[2]){
			(*HAL_UartCallBackArray[2])(USART_ReceiveData(HAL_UartTypeDefArray[2]));	//执行回调函数
		}
	}
}

//USART3中断函数(接收)
//这个需要根据MCU适配 __WAIT_TODO__
void USART3_IRQHandler(void){
	if(USART_GetFlagStatus(HAL_UartTypeDefArray[3], USART_FLAG_RXNE) == SET)  
	{
		if(NULL!=HAL_UartCallBackArray[3]){
			(*HAL_UartCallBackArray[3])(USART_ReceiveData(HAL_UartTypeDefArray[3]));	//执行回调函数
		}
	}
}
/*
//UART4中断函数(接收)
//这个需要根据MCU适配 __WAIT_TODO__
void UART4_IRQHandl(void){
	if(USART_GetFlagStatus(HAL_UartTypeDefArray[4], USART_FLAG_RXNE) == SET)  
	{
		if(NULL!=HAL_UartCallBackArray[4]){
			(*HAL_UartCallBackArray[4])(USART_ReceiveData(HAL_UartTypeDefArray[4]));	//执行回调函数
		}
	}
}

//UART5中断函数(接收)
//这个需要根据MCU适配 __WAIT_TODO__
void UART5_IRQHandl(void){
	if(USART_GetFlagStatus(HAL_UartTypeDefArray[5], USART_FLAG_RXNE) == SET)  
	{
		if(NULL!=HAL_UartCallBackArray[5]){
			(*HAL_UartCallBackArray[5])(USART_ReceiveData(HAL_UartTypeDefArray[5]));	//执行回调函数
		}
	}
}*/

