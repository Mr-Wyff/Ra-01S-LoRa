//和硬件无关的
#include "HAL_SPI_API.h"
#include "HAL_gpio_API.h"
//和硬件相关的

#define HAL_MAX_SPI_NUM	4	//stm32f103系列最多有1-3三个串口，0跳过了，所以这里定义的是4，这个需要根据MCU适配 __WAIT_TODO__

const static uint16_t HAL_CPOLArray[]={SPI_CPOL_Low,SPI_CPOL_High};//HAL_SpiCPOL极性对照数组，这个需要根据MCU适配 __WAIT_TODO__
const static uint16_t HAL_CPHAArray[]={SPI_CPHA_1Edge,SPI_CPHA_2Edge};//HAL_SpiCPHA相位对照数组，这个需要根据MCU适配 __WAIT_TODO__
const static uint16_t HAL_PrescalerArray[]={SPI_BaudRatePrescaler_2,SPI_BaudRatePrescaler_4,SPI_BaudRatePrescaler_8,SPI_BaudRatePrescaler_16,SPI_BaudRatePrescaler_32,SPI_BaudRatePrescaler_64,SPI_BaudRatePrescaler_128,SPI_BaudRatePrescaler_256};//HAL_SpiBaudRatePrescaler分频系数对照数组，这个需要根据MCU适配 __WAIT_TODO__
const static uint16_t HAL_FirstBitArray[]={SPI_FirstBit_MSB,SPI_FirstBit_LSB};//HAL_SpiFirstBit高位/低位优先对照数组，这个需要根据MCU适配 __WAIT_TODO__

static HAL_Spi_t	HAL_SpiLockArray[HAL_MAX_SPI_NUM]={{HAL_SPI_NC,COMMON_UNLOCKED,HAL_SPI_CPOL_LOW,HAL_SPI_CPHA_1Edge,HAL_SPI_PRESCALER_2,HAL_SPI_FirstBit_MSB}};	//这个用来记录spi的一些状态信息,HAL_SpiLockArray[0]跳过，HAL_SpiLockArray[1]-HAL_SpiLockArray[3]分别用来记录SPI1-SPI3
static SPI_TypeDef *HAL_SpiTypeDefArray[]={NULL,SPI1,SPI2,SPI3};//SPI_TypeDef *和HAL_SpiName对照数组，这个需要根据MCU适配 __WAIT_TODO__

//SPI初始化
//这个初始化是根据SPI号初始化的
//以下几点没有传参直接写死的
//    对于有多个位置的我们固定某个引脚
//    使用双向模式(MISO,MOSI都接)
//    主机模式
//    数据位为8位
//    CRC校验
//    SPI1:SCK(PA_5)MISO(PA_6)MOSI(PA_7) CS由软件控制(随便选一个普通的gpio，自己初始化，这里没有初始化cs)
//    SPI2:SCK(PB_13)MISO(PB_14)MOSI(PB_15) CS由软件控制(随便选一个普通的gpio，自己初始化，这里没有初始化cs)
//    SPI3:暂时没有支持
//参数：
//     spiName :SPI名字
//     cpol	:极性
//     cpha	:相位
//     prescaler :预分频系数
//     firstBit  :高位优先还是低位优先
//注意：spi初始化只初始化CLK,MISO,MOSI三个引脚，cs全部都用软件控制(即cs是普通的gpio)
//      不同外设芯片这些SPI参数可能不同，需要自己从手册里找到对应配置
//这个需要根据MCU适配 __WAIT_TODO__
common_ret HALSpiInit(HAL_SpiName spiName,HAL_SpiCPOL cpol,HAL_SpiCPHA cpha,HAL_SpiBaudRatePrescaler prescaler,HAL_SpiFirstBit firstBit){
	common_ret ret=COMMON_RET_ERROR;
	HAL_Gpio_t t_tmpGpio;
	SPI_InitTypeDef  SPI_InitStructure;

	if(COMMON_LOCKED==HAL_SpiLockArray[spiName].lock){
		//总线上锁了，表示其他设备正在使用总线
		ret=COMMON_RET_DEV_LOCKED;
		goto OUT1;
	}
	
	HAL_SpiLockArray[spiName].lock=COMMON_LOCKED;	//开始操作spi，加锁

	//初始化结构体
	HAL_SpiLockArray[spiName].spiName=spiName;
	HAL_SpiLockArray[spiName].cpol=cpol;
	HAL_SpiLockArray[spiName].cpha=cpha;
	HAL_SpiLockArray[spiName].prescaler=prescaler;
	HAL_SpiLockArray[spiName].firstBit=firstBit;

	switch(spiName){
		case HAL_SPI1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//SPI时钟使能
			ret=HALGpioInit(&t_tmpGpio,PA_5,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			ret+=HALGpioInit(&t_tmpGpio,PA_6,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			ret+=HALGpioInit(&t_tmpGpio,PA_7,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			if(COMMON_RET_OK!=ret){
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE);//关闭时钟
				ret=COMMON_RET_MULTIPLE_ERROR;
				goto OUT2;
			}
			//GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  //PA5/6/7上拉
			break;
		case HAL_SPI2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);//SPI时钟使能
			ret=HALGpioInit(&t_tmpGpio,PB_13,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			ret+=HALGpioInit(&t_tmpGpio,PB_14,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			ret+=HALGpioInit(&t_tmpGpio,PB_15,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			if(COMMON_RET_OK!=ret){
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE);//关闭时钟
				ret=COMMON_RET_MULTIPLE_ERROR;
				goto OUT2;
			}
			//GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉
			break;
		case HAL_SPI3://暂时没有驱动SPI3
			//RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);//SPI时钟使能
			ret=COMMON_RET_NOT_SUPPORT;
			goto OUT2;
			//break;
		default:
			ret=COMMON_RET_PARAM_ERROR;
			goto OUT2;
			//break;
	}

	//配置SPI
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = HAL_CPOLArray[cpol];		//设置时钟极性(串行同步时钟的空闲状态为高电平还是低电平)
	SPI_InitStructure.SPI_CPHA = HAL_CPHAArray[cpha];	//设置相位(串行同步时钟的第几个跳变沿（上升或下降）数据被采样)
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:设置为软件控制(SSI控制)
	SPI_InitStructure.SPI_BaudRatePrescaler = HAL_PrescalerArray[prescaler];		//定义波特率预分频的值:波特率预分频值为256(256是最低,可以设置完成之后调整速度,如果速度过快导致通信失败再调小)
	SPI_InitStructure.SPI_FirstBit = HAL_FirstBitArray[firstBit];	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式(CRC校验相关)
	SPI_Init(HAL_SpiTypeDefArray[spiName],&SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(HAL_SpiTypeDefArray[spiName],ENABLE); //使能SPI外设

OUT2:
	HAL_SpiLockArray[spiName].lock=COMMON_UNLOCKED;	//解锁
OUT1:
	return ret;
}

//spi传输数据(返回值是接收到的数据)
//参数：
//     spiName	:需要通过哪个spi传输数据
//     data		:要发送的数据
//返回值：
//       接收到的数据
//注意：这里没有控制CS信号，需要自己控制（通过一个普通的GPIO控制就可以）
//这个需要根据MCU适配 __WAIT_TODO__
uint8_t HALSpiInOut(HAL_SpiName spiName,uint8_t data){
	uint32_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(HAL_SpiTypeDefArray[spiName], SPI_I2S_FLAG_TXE) == RESET){//检查指定的SPI标志位设置与否:发送缓存空标志位(缓冲区空了就可以拷贝数据了)
		if((retry++)>2000){
			//超时了
			return 0xff;
		}
	}
	SPI_I2S_SendData(HAL_SpiTypeDefArray[spiName], data); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(HAL_SpiTypeDefArray[spiName], SPI_I2S_FLAG_RXNE) == RESET){ //检查指定的SPI标志位设置与否:接受缓存非空标志位（非空了就表示接收数据完成了）
		if((retry++)>2000){
			//超时了
			return 0xff;
		}
	}
	return SPI_I2S_ReceiveData(HAL_SpiTypeDefArray[spiName]); //返回通过SPIx最近接收的数据	
}
