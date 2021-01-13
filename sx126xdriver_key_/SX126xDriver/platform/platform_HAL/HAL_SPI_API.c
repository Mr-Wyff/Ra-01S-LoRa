//��Ӳ���޹ص�
#include "HAL_SPI_API.h"
#include "HAL_gpio_API.h"
//��Ӳ����ص�

#define HAL_MAX_SPI_NUM	4	//stm32f103ϵ�������1-3�������ڣ�0�����ˣ��������ﶨ�����4�������Ҫ����MCU���� __WAIT_TODO__

const static uint16_t HAL_CPOLArray[]={SPI_CPOL_Low,SPI_CPOL_High};//HAL_SpiCPOL���Զ������飬�����Ҫ����MCU���� __WAIT_TODO__
const static uint16_t HAL_CPHAArray[]={SPI_CPHA_1Edge,SPI_CPHA_2Edge};//HAL_SpiCPHA��λ�������飬�����Ҫ����MCU���� __WAIT_TODO__
const static uint16_t HAL_PrescalerArray[]={SPI_BaudRatePrescaler_2,SPI_BaudRatePrescaler_4,SPI_BaudRatePrescaler_8,SPI_BaudRatePrescaler_16,SPI_BaudRatePrescaler_32,SPI_BaudRatePrescaler_64,SPI_BaudRatePrescaler_128,SPI_BaudRatePrescaler_256};//HAL_SpiBaudRatePrescaler��Ƶϵ���������飬�����Ҫ����MCU���� __WAIT_TODO__
const static uint16_t HAL_FirstBitArray[]={SPI_FirstBit_MSB,SPI_FirstBit_LSB};//HAL_SpiFirstBit��λ/��λ���ȶ������飬�����Ҫ����MCU���� __WAIT_TODO__

static HAL_Spi_t	HAL_SpiLockArray[HAL_MAX_SPI_NUM]={{HAL_SPI_NC,COMMON_UNLOCKED,HAL_SPI_CPOL_LOW,HAL_SPI_CPHA_1Edge,HAL_SPI_PRESCALER_2,HAL_SPI_FirstBit_MSB}};	//���������¼spi��һЩ״̬��Ϣ,HAL_SpiLockArray[0]������HAL_SpiLockArray[1]-HAL_SpiLockArray[3]�ֱ�������¼SPI1-SPI3
static SPI_TypeDef *HAL_SpiTypeDefArray[]={NULL,SPI1,SPI2,SPI3};//SPI_TypeDef *��HAL_SpiName�������飬�����Ҫ����MCU���� __WAIT_TODO__

//SPI��ʼ��
//�����ʼ���Ǹ���SPI�ų�ʼ����
//���¼���û�д���ֱ��д����
//    �����ж��λ�õ����ǹ̶�ĳ������
//    ʹ��˫��ģʽ(MISO,MOSI����)
//    ����ģʽ
//    ����λΪ8λ
//    CRCУ��
//    SPI1:SCK(PA_5)MISO(PA_6)MOSI(PA_7) CS���������(���ѡһ����ͨ��gpio���Լ���ʼ��������û�г�ʼ��cs)
//    SPI2:SCK(PB_13)MISO(PB_14)MOSI(PB_15) CS���������(���ѡһ����ͨ��gpio���Լ���ʼ��������û�г�ʼ��cs)
//    SPI3:��ʱû��֧��
//������
//     spiName :SPI����
//     cpol	:����
//     cpha	:��λ
//     prescaler :Ԥ��Ƶϵ��
//     firstBit  :��λ���Ȼ��ǵ�λ����
//ע�⣺spi��ʼ��ֻ��ʼ��CLK,MISO,MOSI�������ţ�csȫ�������������(��cs����ͨ��gpio)
//      ��ͬ����оƬ��ЩSPI�������ܲ�ͬ����Ҫ�Լ����ֲ����ҵ���Ӧ����
//�����Ҫ����MCU���� __WAIT_TODO__
common_ret HALSpiInit(HAL_SpiName spiName,HAL_SpiCPOL cpol,HAL_SpiCPHA cpha,HAL_SpiBaudRatePrescaler prescaler,HAL_SpiFirstBit firstBit){
	common_ret ret=COMMON_RET_ERROR;
	HAL_Gpio_t t_tmpGpio;
	SPI_InitTypeDef  SPI_InitStructure;

	if(COMMON_LOCKED==HAL_SpiLockArray[spiName].lock){
		//���������ˣ���ʾ�����豸����ʹ������
		ret=COMMON_RET_DEV_LOCKED;
		goto OUT1;
	}
	
	HAL_SpiLockArray[spiName].lock=COMMON_LOCKED;	//��ʼ����spi������

	//��ʼ���ṹ��
	HAL_SpiLockArray[spiName].spiName=spiName;
	HAL_SpiLockArray[spiName].cpol=cpol;
	HAL_SpiLockArray[spiName].cpha=cpha;
	HAL_SpiLockArray[spiName].prescaler=prescaler;
	HAL_SpiLockArray[spiName].firstBit=firstBit;

	switch(spiName){
		case HAL_SPI1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//SPIʱ��ʹ��
			ret=HALGpioInit(&t_tmpGpio,PA_5,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			ret+=HALGpioInit(&t_tmpGpio,PA_6,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			ret+=HALGpioInit(&t_tmpGpio,PA_7,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			if(COMMON_RET_OK!=ret){
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE);//�ر�ʱ��
				ret=COMMON_RET_MULTIPLE_ERROR;
				goto OUT2;
			}
			//GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  //PA5/6/7����
			break;
		case HAL_SPI2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);//SPIʱ��ʹ��
			ret=HALGpioInit(&t_tmpGpio,PB_13,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			ret+=HALGpioInit(&t_tmpGpio,PB_14,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			ret+=HALGpioInit(&t_tmpGpio,PB_15,HAL_GPIO_ALTERNATE_FCT_PP,HAL_GPIO_NO_PULL,0);
			if(COMMON_RET_OK!=ret){
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE);//�ر�ʱ��
				ret=COMMON_RET_MULTIPLE_ERROR;
				goto OUT2;
			}
			//GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15����
			break;
		case HAL_SPI3://��ʱû������SPI3
			//RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);//SPIʱ��ʹ��
			ret=COMMON_RET_NOT_SUPPORT;
			goto OUT2;
			//break;
		default:
			ret=COMMON_RET_PARAM_ERROR;
			goto OUT2;
			//break;
	}

	//����SPI
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = HAL_CPOLArray[cpol];		//����ʱ�Ӽ���(����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ���ǵ͵�ƽ)
	SPI_InitStructure.SPI_CPHA = HAL_CPHAArray[cpha];	//������λ(����ͬ��ʱ�ӵĵڼ��������أ��������½������ݱ�����)
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:����Ϊ�������(SSI����)
	SPI_InitStructure.SPI_BaudRatePrescaler = HAL_PrescalerArray[prescaler];		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256(256�����,�����������֮������ٶ�,����ٶȹ��쵼��ͨ��ʧ���ٵ�С)
	SPI_InitStructure.SPI_FirstBit = HAL_FirstBitArray[firstBit];	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ(CRCУ�����)
	SPI_Init(HAL_SpiTypeDefArray[spiName],&SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	SPI_Cmd(HAL_SpiTypeDefArray[spiName],ENABLE); //ʹ��SPI����

OUT2:
	HAL_SpiLockArray[spiName].lock=COMMON_UNLOCKED;	//����
OUT1:
	return ret;
}

//spi��������(����ֵ�ǽ��յ�������)
//������
//     spiName	:��Ҫͨ���ĸ�spi��������
//     data		:Ҫ���͵�����
//����ֵ��
//       ���յ�������
//ע�⣺����û�п���CS�źţ���Ҫ�Լ����ƣ�ͨ��һ����ͨ��GPIO���ƾͿ��ԣ�
//�����Ҫ����MCU���� __WAIT_TODO__
uint8_t HALSpiInOut(HAL_SpiName spiName,uint8_t data){
	uint32_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(HAL_SpiTypeDefArray[spiName], SPI_I2S_FLAG_TXE) == RESET){//���ָ����SPI��־λ�������:���ͻ���ձ�־λ(���������˾Ϳ��Կ���������)
		if((retry++)>2000){
			//��ʱ��
			return 0xff;
		}
	}
	SPI_I2S_SendData(HAL_SpiTypeDefArray[spiName], data); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(HAL_SpiTypeDefArray[spiName], SPI_I2S_FLAG_RXNE) == RESET){ //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ���ǿ��˾ͱ�ʾ������������ˣ�
		if((retry++)>2000){
			//��ʱ��
			return 0xff;
		}
	}
	return SPI_I2S_ReceiveData(HAL_SpiTypeDefArray[spiName]); //����ͨ��SPIx������յ�����	
}
