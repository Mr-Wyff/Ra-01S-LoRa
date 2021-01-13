//和硬件无关的
#include "HAL_gpio_API.h"

//和硬件相关的

const static uint16_t HAL_GpioPingIndex[]={GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_6,GPIO_Pin_7,GPIO_Pin_8,GPIO_Pin_9,GPIO_Pin_10,GPIO_Pin_11,GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15,GPIO_Pin_All};//这个是方便计算pinIndex和pinName关系的数组，因为不用改变直接用const放到代码段中去，这个需要根据MCU适配 __WAIT_TODO__
const static uint32_t HAL_GpioRccPeriph[]={RCC_APB2Periph_GPIOA,RCC_APB2Periph_GPIOB,RCC_APB2Periph_GPIOC,RCC_APB2Periph_GPIOD,RCC_APB2Periph_GPIOE,RCC_APB2Periph_GPIOF,RCC_APB2Periph_GPIOG};//这个是方便计算开启时钟和pinName关系的数组，因为不用改变直接用const放到代码段中去，这个需要根据MCU适配 __WAIT_TODO__
const static uint32_t HAL_GpioEXTI_Line[]={EXTI_Line0,EXTI_Line1,EXTI_Line2,EXTI_Line3,EXTI_Line4,EXTI_Line5,EXTI_Line6,EXTI_Line7,EXTI_Line8,EXTI_Line9,EXTI_Line10,EXTI_Line11,EXTI_Line12,EXTI_Line13,EXTI_Line14,EXTI_Line15};//这个是方便计算外部中断线和pinName关系的数组，因为不用改变直接用const放到代码段中去，这个需要根据MCU适配 __WAIT_TODO__
const static EXTITrigger_TypeDef HAL_GpioEXTITrigger_Type[]={EXTI_Trigger_Rising,EXTI_Trigger_Falling,EXTI_Trigger_Rising_Falling};//这个是方便计算HAL_IrqModes和EXTITrigger_TypeDef对应关系的数组，因为不用改变直接用const放到代码段中去，这个需要根据MCU适配 __WAIT_TODO__
const static IRQn_Type HAL_GpioIrqN[16]={EXTI0_IRQn,EXTI1_IRQn,EXTI2_IRQn,EXTI3_IRQn,EXTI4_IRQn,EXTI9_5_IRQn,EXTI9_5_IRQn,EXTI9_5_IRQn,EXTI9_5_IRQn,EXTI9_5_IRQn,EXTI15_10_IRQn,EXTI15_10_IRQn,EXTI15_10_IRQn,EXTI15_10_IRQn,EXTI15_10_IRQn,EXTI15_10_IRQn};

static GPIO_TypeDef *HAL_GpioPortIndex[]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG};//这个是方便计算portIndex和pinName关系的数组，这个需要根据MCU适配 __WAIT_TODO__
static HAL_GpioIrqCallBack HAL_GpioIrqCallBackArray[16]={0};	//用来保存每个中断线的回调函数

//GPIO初始化函数
//参数
//    gpioObj	:gpio对象结构体，初始化的时候不需要赋值，初始化函数内会对gpioObj进行初始化和赋值
//    pinName:gpio引脚名字eg: PA_0
//    mode	:模式选择
//    pullType:上下拉模式（stm32f103中只有mode==HAL_GPIO_INPUT时pullType生效，其它模式pullType可以任意填写，这个和mcu相关）
//    value	:
//           当mode为外设模式的时候vale表示PSEL寄存器的值，用来确定使用哪种外设功能(需要查数据手册)
//           当mode为开漏/推挽输出的时候0表示初始化为低电平，非零表示初始化为高电平
//           当mode为模拟输出的时候value表示输出的电压值(不是实际的电压值，是根据DAC精度输出的电压值，例如参考电压3.3V，精度100，设置value为10输出的电压是10/100*3.3=0.33V)
//           当mode为输入或者片上外设的时候该值无效，可以任意写
common_ret HALGpioInit(HAL_Gpio_t *gpioObj,HAL_GpioPinNames pinName,HAL_GpioPinModes mode,HAL_GpioPinPullTypes pullType,uint32_t value ){
	GPIO_InitTypeDef  GPIO_InitStructure;	//这个需要根据MCU适配 __WAIT_TODO__
	
	//参数检查
	if(NULL==gpioObj){
		return COMMON_RET_PARAM_ERROR;
	}
	
	if(pinName>PG_15){//这个PG_15只是最大为PG_15不同芯片的GPIO组数不同，例如stm32f103c8没有PE_7端口，所以实际情况可能要根据不同芯片来确定，这里因为同一系列芯片代码通用，所以直接写软件支持的最大值PG_15，这个需要根据MCU适配 __WAIT_TODO__
		return COMMON_RET_PARAM_ERROR;
	}
	
	//初始化结构体
	//这个初始化是基于一组IO是16个脚写的，一组不是16个脚需要根据实际情况修改，这个需要根据MCU适配 __WAIT_TODO__
	gpioObj->pinName=pinName;
	gpioObj->pinIndex=HAL_GpioPingIndex[pinName%16];
	gpioObj->portIndex=HAL_GpioPortIndex[pinName/16];
	gpioObj->mode=mode;
	
	//下面是GPIO初始化过程，这个需要根据MCU适配 __WAIT_TODO__
	RCC_APB2PeriphClockCmd(HAL_GpioRccPeriph[pinName/16], ENABLE);	    //使能指定端口时钟
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz，这里不用传参，直接写死用最大速度50MHZ
	GPIO_InitStructure.GPIO_Pin = gpioObj->pinIndex;
	
	switch(mode){
		case HAL_GPIO_INPUT:	//输入
			if(HAL_GPIO_PULL_UP==pullType){
				GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
			}else if(HAL_GPIO_PULL_DOWN==pullType){
				GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//下拉输入
			}else{
				GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
			}
			break;
		case HAL_GPIO_PUSH_PULL_OUTPUT:	//推挽输出
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
			break;
		case HAL_GPIO_OPEN_DRAIN_OUTPUT:	//开漏输出
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
			break;
		case HAL_GPIO_ALTERNATE_FCT_PP:	//片上外设，推挽模式(需要配合PSEL设置才能确定是复用哪个功能)
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
			break;
		case HAL_GPIO_ALTERNATE_FCT_OD:	//片上外设，开漏模式(需要配合PSEL设置才能确定是复用哪个功能)
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
			return COMMON_RET_NOT_SUPPORT;
			//break;
		case HAL_GPIO_ANALOG_IN:		//模拟输入
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
			return COMMON_RET_NOT_SUPPORT;
			//break;
		case HAL_GPIO_ANALOG_OUT:	//模拟输出
			//GPIO_InitStructure.GPIO_Mode=;//暂时还没确定
			return COMMON_RET_NOT_SUPPORT;
			//break;
		default:
			return COMMON_RET_ERROR;
			//break;
	}

	GPIO_Init(gpioObj->portIndex, &GPIO_InitStructure);	//初始化GPIO
	if(HAL_GPIO_PUSH_PULL_OUTPUT==mode || HAL_GPIO_OPEN_DRAIN_OUTPUT==mode){//输出模式才会设置高低电平
		if(0==value){
			GPIO_ResetBits(gpioObj->portIndex,gpioObj->pinIndex);	//输出低电平
		}else{
			GPIO_SetBits(gpioObj->portIndex,gpioObj->pinIndex);	//输出高电平
		}
	}

	return COMMON_RET_OK;
}

//写入gpio数值
//参数：
//     gpioObj：经过HALGpioInit()初始化过后的GPIO设备结构体(注意必须先初始化)
//     value：
//           当gpioObj->mode== HAL_GPIO_PUSH_PULL_OUTPUT 或者 HAL_GPIO_OPEN_DRAIN_OUTPUT 时，0表示将改GPIO设置为低电平，非零表示设置为高电平
//           当gpioObj->mode== HAL_GPIO_ANALOG_OUT 时value表示输出的电压值(不是实际的电压值，是根据DAC精度输出的电压值，例如参考电压3.3V，精度100，设置value为10输出的电压是10/100*3.3=0.33V)
//           其他模式无效
//这个需要根据MCU适配 __WAIT_TODO__
common_ret HALGpioWrite(HAL_Gpio_t gpioObj, uint32_t value ){
	switch(gpioObj.mode){
		case HAL_GPIO_PUSH_PULL_OUTPUT:	//推挽输出
		case HAL_GPIO_OPEN_DRAIN_OUTPUT:	//开漏输出
			if(0==value){
				GPIO_ResetBits(gpioObj.portIndex,gpioObj.pinIndex);	//输出低电平
			}else{
				GPIO_SetBits(gpioObj.portIndex,gpioObj.pinIndex);	//输出高电平
			}
			return COMMON_RET_OK;
			//break;
		case HAL_GPIO_ANALOG_OUT:	//模拟输出
			//GPIO_InitStructure.GPIO_Mode=;//暂时还没确定
			return COMMON_RET_NOT_SUPPORT;
			//break;
		case HAL_GPIO_INPUT:	//输入
		case HAL_GPIO_ALTERNATE_FCT_PP:	//片上外设，推挽模式
		case HAL_GPIO_ALTERNATE_FCT_OD:	//片上外设，开漏模式
		case HAL_GPIO_ANALOG_IN:		//模拟输入
			//gpioObj->mode 模式错误，不支持写操作
			return COMMON_RET_PARAM_ERROR;
			//break;
		default:
			return COMMON_RET_ERROR;
			//break;
	}
}

//读取gpio数值
//参数：
//     gpioObj：经过HALGpioInit()初始化过后的GPIO设备结构体(注意必须先初始化)
//返回值：
//       当gpioObj->mode== HAL_GPIO_INPUT 时，0表示GPIO读取到低电平，1表示GPIO读取到高电平
//       当gpioObj->mode== HAL_GPIO_ANALOG_IN 时value表示输出的电压值(不是实际的电压值，是根据ADC精度输出的电压值，例如参考电压3.3V，精度100，设置value为10输出的电压是10/100*3.3=0.33V)
//       出现错误返回0xffffffff
//这个需要根据MCU适配 __WAIT_TODO__
uint32_t HALGpioRead(HAL_Gpio_t gpioObj){
	switch(gpioObj.mode){
		case HAL_GPIO_INPUT:	//输入
			return GPIO_ReadInputDataBit(gpioObj.portIndex,gpioObj.pinIndex);
			//break;
		case HAL_GPIO_ANALOG_IN:		//模拟输入
			//暂时不支持，等待适配
			return 0xffffffff;
			//break;
		case HAL_GPIO_PUSH_PULL_OUTPUT:	//推挽输出
		case HAL_GPIO_OPEN_DRAIN_OUTPUT:	//开漏输出
		case HAL_GPIO_ANALOG_OUT:	//模拟输出
		case HAL_GPIO_ALTERNATE_FCT_PP:	//片上外设，推挽模式
		case HAL_GPIO_ALTERNATE_FCT_OD:	//片上外设，开漏模式
		default:
			return 0xffffffff;
			//break;
	}
}

//gpio中断设置
//注意：使用这个函数之前要使用NVIC_PriorityGroupConfig()进行中断优先级分组（这里用的分组2）(这个在整个代码中只配置一次,不要修改,否则会出现很多问题)
//      stm32中只有EXTI0-EXTI15一共16个中断线，所以最多同时配置16个外部中断，而且角标不能重复(例如配置了PA_0就不能配置PB_0或PC_0了，但是可以同时配置PA_0，PB_1，PA_2这样不同角标的中断)
//      而且stm32中16个中断线只有7个中断函数(中断向量表中定义的)，0-4每个中断线都有一个中断函数，5-9共用一个中断函数，10-15共用一个中断函数，注意这个框架在软件上为每一个中断都配置了一个回调函数（共用的通过判断中断标志判断哪个脚来中断了，从而调用对应的回调函数）
//这个需要根据MCU适配 __WAIT_TODO__
common_ret HALGpioSetInterrupt(HAL_GpioPinNames pinName,HAL_GpioPinPullTypes pullType,HAL_IrqModes irqMode,HAL_IrqPriorities irqPriority,HAL_GpioIrqCallBack irqCallBack )
{
	common_ret ret=COMMON_RET_ERROR;
	uint8_t u8_pinNum=0;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	HAL_Gpio_t gpioObj;

	//参数检查
	if(NULL==irqCallBack){
		return COMMON_RET_PARAM_ERROR;
	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能RCC_APB2Periph_AFIO口时钟
	ret=HALGpioInit(&gpioObj,pinName,HAL_GPIO_INPUT,pullType,0);
	if(COMMON_RET_OK!=ret){
		return ret;
	}

	u8_pinNum=(pinName%16);
	HAL_GpioIrqCallBackArray[u8_pinNum]=irqCallBack;
	
	//配置中断
	GPIO_EXTILineConfig(pinName/16,u8_pinNum);//设置映射线关系(这里的 GPIO_PortSourceGPIOX 刚好是0-5，所以直接用pinName/16表示，GPIO_PinSourceXX刚好是0-15，所以用pinName%16表示了)
	EXTI_InitStructure.EXTI_Line=HAL_GpioEXTI_Line[u8_pinNum];	//中断线u8_pinNum
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//外部中断
	EXTI_InitStructure.EXTI_Trigger = HAL_GpioEXTITrigger_Type[irqMode];//下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能中断
	EXTI_Init(&EXTI_InitStructure);//配置中断

	//配置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = HAL_GpioIrqN[u8_pinNum];	//外部中断XX
	switch(irqPriority){
		case IRQ_VERY_LOW_PRIORITY:	//这里0表示优先级低，实际在stm32中0表示优先级最高，stm32f103c8例子中这个优先级对应抢占优先级3子优先级3
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03;	//抢占优先级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;   //子优先级
			break;
		case IRQ_LOW_PRIORITY:		//stm32f103c8例子中这个优先级对应抢占优先级3子优先级0
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03;	//抢占优先级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;   //子优先级
			break;
		case IRQ_MEDIUM_PRIORITY:	//stm32f103c8例子中这个优先级对应抢占优先级2子优先级3
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;	//抢占优先级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;   //子优先级
			break;
		case IRQ_HIGH_PRIORITY:		//stm32f103c8例子中这个优先级对应抢占优先级1子优先级3
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;	//抢占优先级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;   //子优先级
			break;
		case IRQ_VERY_HIGH_PRIORITY://stm32f103c8例子中这个优先级对应抢占优先级0子优先级0(最高了)
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;	//抢占优先级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;   //子优先级
			break;
		default:
			return COMMON_RET_PARAM_ERROR;
			//break;
	}
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能中断
	NVIC_Init(&NVIC_InitStructure);   //配置优先级
	
	EXTI_ClearITPendingBit(HAL_GpioEXTI_Line[u8_pinNum]);//清除中断标志位

	return COMMON_RET_OK;
}

//外部中断线0中断处理函数
void EXTI0_IRQHandler(void){
	if(NULL!=HAL_GpioIrqCallBackArray[0]){
		(*HAL_GpioIrqCallBackArray[0])();
	}
	EXTI_ClearITPendingBit(EXTI_Line0);//清除中断标志位
}

//外部中断线1中断处理函数
void EXTI1_IRQHandler(void){
	if(NULL!=HAL_GpioIrqCallBackArray[1]){
		(*HAL_GpioIrqCallBackArray[1])();
	}
	EXTI_ClearITPendingBit(EXTI_Line1);//清除中断标志位
}

//外部中断线2中断处理函数
void EXTI2_IRQHandler(void){
	if(NULL!=HAL_GpioIrqCallBackArray[2]){
		(*HAL_GpioIrqCallBackArray[2])();
	}
	EXTI_ClearITPendingBit(EXTI_Line2);//清除中断标志位
}

//外部中断线3中断处理函数
void EXTI3_IRQHandler(void){
	if(NULL!=HAL_GpioIrqCallBackArray[3]){
		(*HAL_GpioIrqCallBackArray[3])();
	}
	EXTI_ClearITPendingBit(EXTI_Line3);//清除中断标志位
}

//外部中断线4中断处理函数
void EXTI4_IRQHandler(void){
	if(NULL!=HAL_GpioIrqCallBackArray[4]){
		(*HAL_GpioIrqCallBackArray[4])();
	}
	EXTI_ClearITPendingBit(EXTI_Line4);//清除中断标志位
}

//外部中断线5-9中断处理函数
void EXTI9_5_IRQHandler(void){
	if( EXTI_GetITStatus(EXTI_Line5)!= RESET ){//检测中断标志位
		if(NULL!=HAL_GpioIrqCallBackArray[5]){
			(*HAL_GpioIrqCallBackArray[5])();
		}
		EXTI_ClearITPendingBit(EXTI_Line5);//清除中断标志位
	}

	if( EXTI_GetITStatus(EXTI_Line6)!= RESET ){//检测中断标志位
		if(NULL!=HAL_GpioIrqCallBackArray[6]){
			(*HAL_GpioIrqCallBackArray[6])();
		}
		EXTI_ClearITPendingBit(EXTI_Line6);//清除中断标志位
	}

	if( EXTI_GetITStatus(EXTI_Line7)!= RESET ){//检测中断标志位
		if(NULL!=HAL_GpioIrqCallBackArray[7]){
			(*HAL_GpioIrqCallBackArray[7])();
		}
		EXTI_ClearITPendingBit(EXTI_Line7);//清除中断标志位
	}

	if( EXTI_GetITStatus(EXTI_Line8)!= RESET ){//检测中断标志位
		if(NULL!=HAL_GpioIrqCallBackArray[8]){
			(*HAL_GpioIrqCallBackArray[8])();
		}
		EXTI_ClearITPendingBit(EXTI_Line8);//清除中断标志位
	}

	if( EXTI_GetITStatus(EXTI_Line9)!= RESET ){//检测中断标志位
		if(NULL!=HAL_GpioIrqCallBackArray[9]){
			(*HAL_GpioIrqCallBackArray[9])();
		}
		EXTI_ClearITPendingBit(EXTI_Line9);//清除中断标志位
	}
}

//外部中断线10-15中断处理函数
void EXTI15_10_IRQHandler(void){
	if( EXTI_GetITStatus(EXTI_Line10)!= RESET ){//检测中断标志位
		if(NULL!=HAL_GpioIrqCallBackArray[10]){
			(*HAL_GpioIrqCallBackArray[10])();
		}
		EXTI_ClearITPendingBit(EXTI_Line10);//清除中断标志位
	}

	if( EXTI_GetITStatus(EXTI_Line11)!= RESET ){//检测中断标志位
		if(NULL!=HAL_GpioIrqCallBackArray[11]){
			(*HAL_GpioIrqCallBackArray[11])();
		}
		EXTI_ClearITPendingBit(EXTI_Line11);//清除中断标志位
	}

	if( EXTI_GetITStatus(EXTI_Line12)!= RESET ){//检测中断标志位
		if(NULL!=HAL_GpioIrqCallBackArray[12]){
			(*HAL_GpioIrqCallBackArray[12])();
		}
		EXTI_ClearITPendingBit(EXTI_Line12);//清除中断标志位
	}

	if( EXTI_GetITStatus(EXTI_Line13)!= RESET ){//检测中断标志位
		if(NULL!=HAL_GpioIrqCallBackArray[13]){
			(*HAL_GpioIrqCallBackArray[13])();
		}
		EXTI_ClearITPendingBit(EXTI_Line13);//清除中断标志位
	}

	if( EXTI_GetITStatus(EXTI_Line14)!= RESET ){//检测中断标志位
		if(NULL!=HAL_GpioIrqCallBackArray[14]){
			(*HAL_GpioIrqCallBackArray[14])();
		}
		EXTI_ClearITPendingBit(EXTI_Line14);//清除中断标志位
	}

	if( EXTI_GetITStatus(EXTI_Line15)!= RESET ){//检测中断标志位
		if(NULL!=HAL_GpioIrqCallBackArray[15]){
			(*HAL_GpioIrqCallBackArray[15])();
		}
		EXTI_ClearITPendingBit(EXTI_Line15);//清除中断标志位
	}
}
