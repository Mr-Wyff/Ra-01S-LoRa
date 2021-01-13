//Ӳ���޹ص�
#include "HAL_SPI_API.h"
#include "HAL_gpio_API.h"
#include "common.h"
//Ӳ����ص�

#define LORA_ADDR 0x01

common_ret ExampleSpiInit(void){
	common_ret ret=COMMON_RET_ERROR;
	//uint8_t u8_recive=0;
	HAL_Gpio_t spi1CS;
	//uint32_t i;

	//ע�ⲻͬ��ŵ�spi���ܲ���ͬһ��apb�����ϣ�������ͬ�ķ�Ƶϵ���ڲ�ͬ��spi�����ڿ��ܲ�ͬ
	ret=HALSpiInit(HAL_SPI1,HAL_SPI_CPOL_LOW,HAL_SPI_CPHA_1Edge,HAL_SPI_PRESCALER_32,HAL_SPI_FirstBit_MSB);
	if(COMMON_RET_OK!=ret){
		return ret;
	}
	//��ʼ��һ��gpio��������spi1��cs PA_4 RADIO_NSS_PIN RADIO_NSS_PORT
	ret=HALGpioInit(&spi1CS,PA_4,HAL_GPIO_PUSH_PULL_OUTPUT,HAL_GPIO_NO_PULL,1);//��ʼ��GPIO PC_13Ϊ��������ߵ�ƽ
	if(COMMON_RET_OK!=ret){
		//��ʼ��ʧ����
		return COMMON_RET_ERROR;
	}

	/*ret=HALSpiInit(HAL_SPI2,HAL_SPI_CPOL_HIGH,HAL_SPI_CPHA_2Edge,HAL_SPI_PRESCALER_256,HAL_SPI_FirstBit_MSB);
	if(COMMON_RET_OK!=ret){
		return ret;
	}
	//��ʼ��һ��gpio��������spi2��cs

	HALGpioWrite(spi1CS,0);	//Ƭѡspi1
	HALSpiInOut(HAL_SPI1, LORA_ADDR | 0x80 );
	u8_recive=HALSpiInOut(HAL_SPI1,0x88);//д������
	HALGpioWrite(spi1CS,1);
	
	HALGpioWrite(spi1CS,0);	//Ƭѡspi1
	HALSpiInOut(HAL_SPI1, LORA_ADDR & 0x7F );
	u8_recive=HALSpiInOut(HAL_SPI1,0x00);//��ȡ����
	HALGpioWrite(spi1CS,1);//ȡ��spi1Ƭѡ
	printf("read from reg(0x%02x) value=0x%02x\r\n",LORA_ADDR,u8_recive);
	
	while(1){
		for(i=0;i<0x8fffff;i++);
		//ʡ����Ƭѡ��ʵ��������Ҫ��spi2Ƭѡ
		u8_recive=HALSpiInOut(HAL_SPI2,0x55);
		//ʡ����ȡ��Ƭѡ��ʵ��������Ҫ��ȡ��spi2Ƭѡ
	}*/
	return ret;
}
