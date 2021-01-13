#ifndef __HAL_SPI_API_H__
#define __HAL_SPI_API_H__
//硬件无关的
#include "common.h"
//硬件相关的
#include "stm32f10x.h"

//用来表示SPI的名字
typedef enum
{
	//注意在stm32f103中没有spi0
	HAL_SPI1=1,
	HAL_SPI2,
	HAL_SPI3,
	HAL_SPI_NC=0xff
}HAL_SpiName;

//用来表示SPI的极性(空闲电平为高还是低)
typedef enum
{
	HAL_SPI_CPOL_LOW=0,
	HAL_SPI_CPOL_HIGH
}HAL_SpiCPOL;

//用来表示SPI的相位(数据在第几个跳变沿采集)
typedef enum
{
	HAL_SPI_CPHA_1Edge=0,
	HAL_SPI_CPHA_2Edge
}HAL_SpiCPHA;

//用来表示SPI的分频系数（可以控制spi的速度）
typedef enum
{
	HAL_SPI_PRESCALER_2=0,
	HAL_SPI_PRESCALER_4,	//注意SPI在不同的APB总线上的时候时钟频率不同，所以分频后的频率也不同(spi1四分频以下波形就不太好了(速度太快))
	HAL_SPI_PRESCALER_8,
	HAL_SPI_PRESCALER_16,
	HAL_SPI_PRESCALER_32,
	HAL_SPI_PRESCALER_64,
	HAL_SPI_PRESCALER_128,
	HAL_SPI_PRESCALER_256
}HAL_SpiBaudRatePrescaler;

//用来表示SPI数据从LSB还是MSB开始
typedef enum
{
	HAL_SPI_FirstBit_MSB=0,	//从高位开始传输
	HAL_SPI_FirstBit_LSB
}HAL_SpiFirstBit;

typedef struct
{
	HAL_SpiName	spiName;	//spi的名字(eg:HAL_SPI1)
	common_LockTypeDef	lock;	//这个用来记录spi总线是否上锁，注意操作之前都要检查这个，并操作这个锁，否则多个设备共用spi总线的时候可能导致冲突
	//下面这些都是设置的时候自己记录的，方便后面切换的时候使用（不同的从设备spi配置可能不一样）
	HAL_SpiCPOL	cpol;	//记录spi极性
	HAL_SpiCPHA	cpha;	//记录spi相位
	HAL_SpiBaudRatePrescaler prescaler;	//记录分频系数
	HAL_SpiFirstBit	firstBit;	//记录数据从LSB还是MSB开始
}HAL_Spi_t;

common_ret HALSpiInit(HAL_SpiName spiName,HAL_SpiCPOL cpol,HAL_SpiCPHA cpha,HAL_SpiBaudRatePrescaler prescaler,HAL_SpiFirstBit firstBit);
uint8_t HALSpiInOut(HAL_SpiName spiName,uint8_t data);

#endif //end of __HAL_SPI_API_H__
