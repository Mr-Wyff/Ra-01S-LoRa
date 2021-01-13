//��Ӳ���޹ص�
#include "HAL_gpio_API.h"

//��Ӳ����ص�

const static uint16_t HAL_GpioPingIndex[]={GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_6,GPIO_Pin_7,GPIO_Pin_8,GPIO_Pin_9,GPIO_Pin_10,GPIO_Pin_11,GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15,GPIO_Pin_All};//����Ƿ������pinIndex��pinName��ϵ�����飬��Ϊ���øı�ֱ����const�ŵ��������ȥ�������Ҫ����MCU���� __WAIT_TODO__
const static uint32_t HAL_GpioRccPeriph[]={RCC_APB2Periph_GPIOA,RCC_APB2Periph_GPIOB,RCC_APB2Periph_GPIOC,RCC_APB2Periph_GPIOD,RCC_APB2Periph_GPIOE,RCC_APB2Periph_GPIOF,RCC_APB2Periph_GPIOG};//����Ƿ�����㿪��ʱ�Ӻ�pinName��ϵ�����飬��Ϊ���øı�ֱ����const�ŵ��������ȥ�������Ҫ����MCU���� __WAIT_TODO__
const static uint32_t HAL_GpioEXTI_Line[]={EXTI_Line0,EXTI_Line1,EXTI_Line2,EXTI_Line3,EXTI_Line4,EXTI_Line5,EXTI_Line6,EXTI_Line7,EXTI_Line8,EXTI_Line9,EXTI_Line10,EXTI_Line11,EXTI_Line12,EXTI_Line13,EXTI_Line14,EXTI_Line15};//����Ƿ�������ⲿ�ж��ߺ�pinName��ϵ�����飬��Ϊ���øı�ֱ����const�ŵ��������ȥ�������Ҫ����MCU���� __WAIT_TODO__
const static EXTITrigger_TypeDef HAL_GpioEXTITrigger_Type[]={EXTI_Trigger_Rising,EXTI_Trigger_Falling,EXTI_Trigger_Rising_Falling};//����Ƿ������HAL_IrqModes��EXTITrigger_TypeDef��Ӧ��ϵ�����飬��Ϊ���øı�ֱ����const�ŵ��������ȥ�������Ҫ����MCU���� __WAIT_TODO__
const static IRQn_Type HAL_GpioIrqN[16]={EXTI0_IRQn,EXTI1_IRQn,EXTI2_IRQn,EXTI3_IRQn,EXTI4_IRQn,EXTI9_5_IRQn,EXTI9_5_IRQn,EXTI9_5_IRQn,EXTI9_5_IRQn,EXTI9_5_IRQn,EXTI15_10_IRQn,EXTI15_10_IRQn,EXTI15_10_IRQn,EXTI15_10_IRQn,EXTI15_10_IRQn,EXTI15_10_IRQn};

static GPIO_TypeDef *HAL_GpioPortIndex[]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG};//����Ƿ������portIndex��pinName��ϵ�����飬�����Ҫ����MCU���� __WAIT_TODO__
static HAL_GpioIrqCallBack HAL_GpioIrqCallBackArray[16]={0};	//��������ÿ���ж��ߵĻص�����

//GPIO��ʼ������
//����
//    gpioObj	:gpio����ṹ�壬��ʼ����ʱ����Ҫ��ֵ����ʼ�������ڻ��gpioObj���г�ʼ���͸�ֵ
//    pinName:gpio��������eg: PA_0
//    mode	:ģʽѡ��
//    pullType:������ģʽ��stm32f103��ֻ��mode==HAL_GPIO_INPUTʱpullType��Ч������ģʽpullType����������д�������mcu��أ�
//    value	:
//           ��modeΪ����ģʽ��ʱ��vale��ʾPSEL�Ĵ�����ֵ������ȷ��ʹ���������蹦��(��Ҫ�������ֲ�)
//           ��modeΪ��©/���������ʱ��0��ʾ��ʼ��Ϊ�͵�ƽ�������ʾ��ʼ��Ϊ�ߵ�ƽ
//           ��modeΪģ�������ʱ��value��ʾ����ĵ�ѹֵ(����ʵ�ʵĵ�ѹֵ���Ǹ���DAC��������ĵ�ѹֵ������ο���ѹ3.3V������100������valueΪ10����ĵ�ѹ��10/100*3.3=0.33V)
//           ��modeΪ�������Ƭ�������ʱ���ֵ��Ч����������д
common_ret HALGpioInit(HAL_Gpio_t *gpioObj,HAL_GpioPinNames pinName,HAL_GpioPinModes mode,HAL_GpioPinPullTypes pullType,uint32_t value ){
	GPIO_InitTypeDef  GPIO_InitStructure;	//�����Ҫ����MCU���� __WAIT_TODO__
	
	//�������
	if(NULL==gpioObj){
		return COMMON_RET_PARAM_ERROR;
	}
	
	if(pinName>PG_15){//���PG_15ֻ�����ΪPG_15��ͬоƬ��GPIO������ͬ������stm32f103c8û��PE_7�˿ڣ�����ʵ���������Ҫ���ݲ�ͬоƬ��ȷ����������Ϊͬһϵ��оƬ����ͨ�ã�����ֱ��д���֧�ֵ����ֵPG_15�������Ҫ����MCU���� __WAIT_TODO__
		return COMMON_RET_PARAM_ERROR;
	}
	
	//��ʼ���ṹ��
	//�����ʼ���ǻ���һ��IO��16����д�ģ�һ�鲻��16������Ҫ����ʵ������޸ģ������Ҫ����MCU���� __WAIT_TODO__
	gpioObj->pinName=pinName;
	gpioObj->pinIndex=HAL_GpioPingIndex[pinName%16];
	gpioObj->portIndex=HAL_GpioPortIndex[pinName/16];
	gpioObj->mode=mode;
	
	//������GPIO��ʼ�����̣������Ҫ����MCU���� __WAIT_TODO__
	RCC_APB2PeriphClockCmd(HAL_GpioRccPeriph[pinName/16], ENABLE);	    //ʹ��ָ���˿�ʱ��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz�����ﲻ�ô��Σ�ֱ��д��������ٶ�50MHZ
	GPIO_InitStructure.GPIO_Pin = gpioObj->pinIndex;
	
	switch(mode){
		case HAL_GPIO_INPUT:	//����
			if(HAL_GPIO_PULL_UP==pullType){
				GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������
			}else if(HAL_GPIO_PULL_DOWN==pullType){
				GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//��������
			}else{
				GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
			}
			break;
		case HAL_GPIO_PUSH_PULL_OUTPUT:	//�������
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
			break;
		case HAL_GPIO_OPEN_DRAIN_OUTPUT:	//��©���
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
			break;
		case HAL_GPIO_ALTERNATE_FCT_PP:	//Ƭ�����裬����ģʽ(��Ҫ���PSEL���ò���ȷ���Ǹ����ĸ�����)
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
			break;
		case HAL_GPIO_ALTERNATE_FCT_OD:	//Ƭ�����裬��©ģʽ(��Ҫ���PSEL���ò���ȷ���Ǹ����ĸ�����)
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
			return COMMON_RET_NOT_SUPPORT;
			//break;
		case HAL_GPIO_ANALOG_IN:		//ģ������
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
			return COMMON_RET_NOT_SUPPORT;
			//break;
		case HAL_GPIO_ANALOG_OUT:	//ģ�����
			//GPIO_InitStructure.GPIO_Mode=;//��ʱ��ûȷ��
			return COMMON_RET_NOT_SUPPORT;
			//break;
		default:
			return COMMON_RET_ERROR;
			//break;
	}

	GPIO_Init(gpioObj->portIndex, &GPIO_InitStructure);	//��ʼ��GPIO
	if(HAL_GPIO_PUSH_PULL_OUTPUT==mode || HAL_GPIO_OPEN_DRAIN_OUTPUT==mode){//���ģʽ�Ż����øߵ͵�ƽ
		if(0==value){
			GPIO_ResetBits(gpioObj->portIndex,gpioObj->pinIndex);	//����͵�ƽ
		}else{
			GPIO_SetBits(gpioObj->portIndex,gpioObj->pinIndex);	//����ߵ�ƽ
		}
	}

	return COMMON_RET_OK;
}

//д��gpio��ֵ
//������
//     gpioObj������HALGpioInit()��ʼ�������GPIO�豸�ṹ��(ע������ȳ�ʼ��)
//     value��
//           ��gpioObj->mode== HAL_GPIO_PUSH_PULL_OUTPUT ���� HAL_GPIO_OPEN_DRAIN_OUTPUT ʱ��0��ʾ����GPIO����Ϊ�͵�ƽ�������ʾ����Ϊ�ߵ�ƽ
//           ��gpioObj->mode== HAL_GPIO_ANALOG_OUT ʱvalue��ʾ����ĵ�ѹֵ(����ʵ�ʵĵ�ѹֵ���Ǹ���DAC��������ĵ�ѹֵ������ο���ѹ3.3V������100������valueΪ10����ĵ�ѹ��10/100*3.3=0.33V)
//           ����ģʽ��Ч
//�����Ҫ����MCU���� __WAIT_TODO__
common_ret HALGpioWrite(HAL_Gpio_t gpioObj, uint32_t value ){
	switch(gpioObj.mode){
		case HAL_GPIO_PUSH_PULL_OUTPUT:	//�������
		case HAL_GPIO_OPEN_DRAIN_OUTPUT:	//��©���
			if(0==value){
				GPIO_ResetBits(gpioObj.portIndex,gpioObj.pinIndex);	//����͵�ƽ
			}else{
				GPIO_SetBits(gpioObj.portIndex,gpioObj.pinIndex);	//����ߵ�ƽ
			}
			return COMMON_RET_OK;
			//break;
		case HAL_GPIO_ANALOG_OUT:	//ģ�����
			//GPIO_InitStructure.GPIO_Mode=;//��ʱ��ûȷ��
			return COMMON_RET_NOT_SUPPORT;
			//break;
		case HAL_GPIO_INPUT:	//����
		case HAL_GPIO_ALTERNATE_FCT_PP:	//Ƭ�����裬����ģʽ
		case HAL_GPIO_ALTERNATE_FCT_OD:	//Ƭ�����裬��©ģʽ
		case HAL_GPIO_ANALOG_IN:		//ģ������
			//gpioObj->mode ģʽ���󣬲�֧��д����
			return COMMON_RET_PARAM_ERROR;
			//break;
		default:
			return COMMON_RET_ERROR;
			//break;
	}
}

//��ȡgpio��ֵ
//������
//     gpioObj������HALGpioInit()��ʼ�������GPIO�豸�ṹ��(ע������ȳ�ʼ��)
//����ֵ��
//       ��gpioObj->mode== HAL_GPIO_INPUT ʱ��0��ʾGPIO��ȡ���͵�ƽ��1��ʾGPIO��ȡ���ߵ�ƽ
//       ��gpioObj->mode== HAL_GPIO_ANALOG_IN ʱvalue��ʾ����ĵ�ѹֵ(����ʵ�ʵĵ�ѹֵ���Ǹ���ADC��������ĵ�ѹֵ������ο���ѹ3.3V������100������valueΪ10����ĵ�ѹ��10/100*3.3=0.33V)
//       ���ִ��󷵻�0xffffffff
//�����Ҫ����MCU���� __WAIT_TODO__
uint32_t HALGpioRead(HAL_Gpio_t gpioObj){
	switch(gpioObj.mode){
		case HAL_GPIO_INPUT:	//����
			return GPIO_ReadInputDataBit(gpioObj.portIndex,gpioObj.pinIndex);
			//break;
		case HAL_GPIO_ANALOG_IN:		//ģ������
			//��ʱ��֧�֣��ȴ�����
			return 0xffffffff;
			//break;
		case HAL_GPIO_PUSH_PULL_OUTPUT:	//�������
		case HAL_GPIO_OPEN_DRAIN_OUTPUT:	//��©���
		case HAL_GPIO_ANALOG_OUT:	//ģ�����
		case HAL_GPIO_ALTERNATE_FCT_PP:	//Ƭ�����裬����ģʽ
		case HAL_GPIO_ALTERNATE_FCT_OD:	//Ƭ�����裬��©ģʽ
		default:
			return 0xffffffff;
			//break;
	}
}

//gpio�ж�����
//ע�⣺ʹ���������֮ǰҪʹ��NVIC_PriorityGroupConfig()�����ж����ȼ����飨�����õķ���2��(���������������ֻ����һ��,��Ҫ�޸�,�������ֺܶ�����)
//      stm32��ֻ��EXTI0-EXTI15һ��16���ж��ߣ��������ͬʱ����16���ⲿ�жϣ����ҽǱ겻���ظ�(����������PA_0�Ͳ�������PB_0��PC_0�ˣ����ǿ���ͬʱ����PA_0��PB_1��PA_2������ͬ�Ǳ���ж�)
//      ����stm32��16���ж���ֻ��7���жϺ���(�ж��������ж����)��0-4ÿ���ж��߶���һ���жϺ�����5-9����һ���жϺ�����10-15����һ���жϺ�����ע���������������Ϊÿһ���ж϶�������һ���ص����������õ�ͨ���ж��жϱ�־�ж��ĸ������ж��ˣ��Ӷ����ö�Ӧ�Ļص�������
//�����Ҫ����MCU���� __WAIT_TODO__
common_ret HALGpioSetInterrupt(HAL_GpioPinNames pinName,HAL_GpioPinPullTypes pullType,HAL_IrqModes irqMode,HAL_IrqPriorities irqPriority,HAL_GpioIrqCallBack irqCallBack )
{
	common_ret ret=COMMON_RET_ERROR;
	uint8_t u8_pinNum=0;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	HAL_Gpio_t gpioObj;

	//�������
	if(NULL==irqCallBack){
		return COMMON_RET_PARAM_ERROR;
	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ��RCC_APB2Periph_AFIO��ʱ��
	ret=HALGpioInit(&gpioObj,pinName,HAL_GPIO_INPUT,pullType,0);
	if(COMMON_RET_OK!=ret){
		return ret;
	}

	u8_pinNum=(pinName%16);
	HAL_GpioIrqCallBackArray[u8_pinNum]=irqCallBack;
	
	//�����ж�
	GPIO_EXTILineConfig(pinName/16,u8_pinNum);//����ӳ���߹�ϵ(����� GPIO_PortSourceGPIOX �պ���0-5������ֱ����pinName/16��ʾ��GPIO_PinSourceXX�պ���0-15��������pinName%16��ʾ��)
	EXTI_InitStructure.EXTI_Line=HAL_GpioEXTI_Line[u8_pinNum];	//�ж���u8_pinNum
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ⲿ�ж�
	EXTI_InitStructure.EXTI_Trigger = HAL_GpioEXTITrigger_Type[irqMode];//�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ���ж�
	EXTI_Init(&EXTI_InitStructure);//�����ж�

	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = HAL_GpioIrqN[u8_pinNum];	//�ⲿ�ж�XX
	switch(irqPriority){
		case IRQ_VERY_LOW_PRIORITY:	//����0��ʾ���ȼ��ͣ�ʵ����stm32��0��ʾ���ȼ���ߣ�stm32f103c8������������ȼ���Ӧ��ռ���ȼ�3�����ȼ�3
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03;	//��ռ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;   //�����ȼ�
			break;
		case IRQ_LOW_PRIORITY:		//stm32f103c8������������ȼ���Ӧ��ռ���ȼ�3�����ȼ�0
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03;	//��ռ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;   //�����ȼ�
			break;
		case IRQ_MEDIUM_PRIORITY:	//stm32f103c8������������ȼ���Ӧ��ռ���ȼ�2�����ȼ�3
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;	//��ռ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;   //�����ȼ�
			break;
		case IRQ_HIGH_PRIORITY:		//stm32f103c8������������ȼ���Ӧ��ռ���ȼ�1�����ȼ�3
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;	//��ռ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;   //�����ȼ�
			break;
		case IRQ_VERY_HIGH_PRIORITY://stm32f103c8������������ȼ���Ӧ��ռ���ȼ�0�����ȼ�0(�����)
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;	//��ռ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;   //�����ȼ�
			break;
		default:
			return COMMON_RET_PARAM_ERROR;
			//break;
	}
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);   //�������ȼ�
	
	EXTI_ClearITPendingBit(HAL_GpioEXTI_Line[u8_pinNum]);//����жϱ�־λ

	return COMMON_RET_OK;
}

//�ⲿ�ж���0�жϴ�����
void EXTI0_IRQHandler(void){
	if(NULL!=HAL_GpioIrqCallBackArray[0]){
		(*HAL_GpioIrqCallBackArray[0])();
	}
	EXTI_ClearITPendingBit(EXTI_Line0);//����жϱ�־λ
}

//�ⲿ�ж���1�жϴ�����
void EXTI1_IRQHandler(void){
	if(NULL!=HAL_GpioIrqCallBackArray[1]){
		(*HAL_GpioIrqCallBackArray[1])();
	}
	EXTI_ClearITPendingBit(EXTI_Line1);//����жϱ�־λ
}

//�ⲿ�ж���2�жϴ�����
void EXTI2_IRQHandler(void){
	if(NULL!=HAL_GpioIrqCallBackArray[2]){
		(*HAL_GpioIrqCallBackArray[2])();
	}
	EXTI_ClearITPendingBit(EXTI_Line2);//����жϱ�־λ
}

//�ⲿ�ж���3�жϴ�����
void EXTI3_IRQHandler(void){
	if(NULL!=HAL_GpioIrqCallBackArray[3]){
		(*HAL_GpioIrqCallBackArray[3])();
	}
	EXTI_ClearITPendingBit(EXTI_Line3);//����жϱ�־λ
}

//�ⲿ�ж���4�жϴ�����
void EXTI4_IRQHandler(void){
	if(NULL!=HAL_GpioIrqCallBackArray[4]){
		(*HAL_GpioIrqCallBackArray[4])();
	}
	EXTI_ClearITPendingBit(EXTI_Line4);//����жϱ�־λ
}

//�ⲿ�ж���5-9�жϴ�����
void EXTI9_5_IRQHandler(void){
	if( EXTI_GetITStatus(EXTI_Line5)!= RESET ){//����жϱ�־λ
		if(NULL!=HAL_GpioIrqCallBackArray[5]){
			(*HAL_GpioIrqCallBackArray[5])();
		}
		EXTI_ClearITPendingBit(EXTI_Line5);//����жϱ�־λ
	}

	if( EXTI_GetITStatus(EXTI_Line6)!= RESET ){//����жϱ�־λ
		if(NULL!=HAL_GpioIrqCallBackArray[6]){
			(*HAL_GpioIrqCallBackArray[6])();
		}
		EXTI_ClearITPendingBit(EXTI_Line6);//����жϱ�־λ
	}

	if( EXTI_GetITStatus(EXTI_Line7)!= RESET ){//����жϱ�־λ
		if(NULL!=HAL_GpioIrqCallBackArray[7]){
			(*HAL_GpioIrqCallBackArray[7])();
		}
		EXTI_ClearITPendingBit(EXTI_Line7);//����жϱ�־λ
	}

	if( EXTI_GetITStatus(EXTI_Line8)!= RESET ){//����жϱ�־λ
		if(NULL!=HAL_GpioIrqCallBackArray[8]){
			(*HAL_GpioIrqCallBackArray[8])();
		}
		EXTI_ClearITPendingBit(EXTI_Line8);//����жϱ�־λ
	}

	if( EXTI_GetITStatus(EXTI_Line9)!= RESET ){//����жϱ�־λ
		if(NULL!=HAL_GpioIrqCallBackArray[9]){
			(*HAL_GpioIrqCallBackArray[9])();
		}
		EXTI_ClearITPendingBit(EXTI_Line9);//����жϱ�־λ
	}
}

//�ⲿ�ж���10-15�жϴ�����
void EXTI15_10_IRQHandler(void){
	if( EXTI_GetITStatus(EXTI_Line10)!= RESET ){//����жϱ�־λ
		if(NULL!=HAL_GpioIrqCallBackArray[10]){
			(*HAL_GpioIrqCallBackArray[10])();
		}
		EXTI_ClearITPendingBit(EXTI_Line10);//����жϱ�־λ
	}

	if( EXTI_GetITStatus(EXTI_Line11)!= RESET ){//����жϱ�־λ
		if(NULL!=HAL_GpioIrqCallBackArray[11]){
			(*HAL_GpioIrqCallBackArray[11])();
		}
		EXTI_ClearITPendingBit(EXTI_Line11);//����жϱ�־λ
	}

	if( EXTI_GetITStatus(EXTI_Line12)!= RESET ){//����жϱ�־λ
		if(NULL!=HAL_GpioIrqCallBackArray[12]){
			(*HAL_GpioIrqCallBackArray[12])();
		}
		EXTI_ClearITPendingBit(EXTI_Line12);//����жϱ�־λ
	}

	if( EXTI_GetITStatus(EXTI_Line13)!= RESET ){//����жϱ�־λ
		if(NULL!=HAL_GpioIrqCallBackArray[13]){
			(*HAL_GpioIrqCallBackArray[13])();
		}
		EXTI_ClearITPendingBit(EXTI_Line13);//����жϱ�־λ
	}

	if( EXTI_GetITStatus(EXTI_Line14)!= RESET ){//����жϱ�־λ
		if(NULL!=HAL_GpioIrqCallBackArray[14]){
			(*HAL_GpioIrqCallBackArray[14])();
		}
		EXTI_ClearITPendingBit(EXTI_Line14);//����жϱ�־λ
	}

	if( EXTI_GetITStatus(EXTI_Line15)!= RESET ){//����жϱ�־λ
		if(NULL!=HAL_GpioIrqCallBackArray[15]){
			(*HAL_GpioIrqCallBackArray[15])();
		}
		EXTI_ClearITPendingBit(EXTI_Line15);//����жϱ�־λ
	}
}
