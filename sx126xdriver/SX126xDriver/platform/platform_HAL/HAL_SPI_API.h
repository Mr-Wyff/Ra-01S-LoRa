#ifndef __HAL_SPI_API_H__
#define __HAL_SPI_API_H__
//Ӳ���޹ص�
#include "common.h"
//Ӳ����ص�
#include "stm32f10x.h"

//������ʾSPI������
typedef enum
{
	//ע����stm32f103��û��spi0
	HAL_SPI1=1,
	HAL_SPI2,
	HAL_SPI3,
	HAL_SPI_NC=0xff
}HAL_SpiName;

//������ʾSPI�ļ���(���е�ƽΪ�߻��ǵ�)
typedef enum
{
	HAL_SPI_CPOL_LOW=0,
	HAL_SPI_CPOL_HIGH
}HAL_SpiCPOL;

//������ʾSPI����λ(�����ڵڼ��������زɼ�)
typedef enum
{
	HAL_SPI_CPHA_1Edge=0,
	HAL_SPI_CPHA_2Edge
}HAL_SpiCPHA;

//������ʾSPI�ķ�Ƶϵ�������Կ���spi���ٶȣ�
typedef enum
{
	HAL_SPI_PRESCALER_2=0,
	HAL_SPI_PRESCALER_4,	//ע��SPI�ڲ�ͬ��APB�����ϵ�ʱ��ʱ��Ƶ�ʲ�ͬ�����Է�Ƶ���Ƶ��Ҳ��ͬ(spi1�ķ�Ƶ���²��ξͲ�̫����(�ٶ�̫��))
	HAL_SPI_PRESCALER_8,
	HAL_SPI_PRESCALER_16,
	HAL_SPI_PRESCALER_32,
	HAL_SPI_PRESCALER_64,
	HAL_SPI_PRESCALER_128,
	HAL_SPI_PRESCALER_256
}HAL_SpiBaudRatePrescaler;

//������ʾSPI���ݴ�LSB����MSB��ʼ
typedef enum
{
	HAL_SPI_FirstBit_MSB=0,	//�Ӹ�λ��ʼ����
	HAL_SPI_FirstBit_LSB
}HAL_SpiFirstBit;

typedef struct
{
	HAL_SpiName	spiName;	//spi������(eg:HAL_SPI1)
	common_LockTypeDef	lock;	//���������¼spi�����Ƿ�������ע�����֮ǰ��Ҫ����������������������������豸����spi���ߵ�ʱ����ܵ��³�ͻ
	//������Щ�������õ�ʱ���Լ���¼�ģ���������л���ʱ��ʹ�ã���ͬ�Ĵ��豸spi���ÿ��ܲ�һ����
	HAL_SpiCPOL	cpol;	//��¼spi����
	HAL_SpiCPHA	cpha;	//��¼spi��λ
	HAL_SpiBaudRatePrescaler prescaler;	//��¼��Ƶϵ��
	HAL_SpiFirstBit	firstBit;	//��¼���ݴ�LSB����MSB��ʼ
}HAL_Spi_t;

common_ret HALSpiInit(HAL_SpiName spiName,HAL_SpiCPOL cpol,HAL_SpiCPHA cpha,HAL_SpiBaudRatePrescaler prescaler,HAL_SpiFirstBit firstBit);
uint8_t HALSpiInOut(HAL_SpiName spiName,uint8_t data);

#endif //end of __HAL_SPI_API_H__
