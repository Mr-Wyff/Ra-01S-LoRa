//硬件无关的
#include "HAL_SPI_API.h"
#include "HAL_gpio_API.h"
#include "common.h"
//硬件相关的

#define LORA_ADDR 0x01

common_ret ExampleSpiInit(void){
	common_ret ret=COMMON_RET_ERROR;
	//uint8_t u8_recive=0;
	HAL_Gpio_t spi1CS;
	//uint32_t i;

	//注意不同编号的spi可能不在同一个apb总线上，所以相同的分频系数在不同的spi上周期可能不同
	ret=HALSpiInit(HAL_SPI1,HAL_SPI_CPOL_LOW,HAL_SPI_CPHA_1Edge,HAL_SPI_PRESCALER_32,HAL_SPI_FirstBit_MSB);
	if(COMMON_RET_OK!=ret){
		return ret;
	}
	//初始化一个gpio用来控制spi1的cs PA_4 RADIO_NSS_PIN RADIO_NSS_PORT
	ret=HALGpioInit(&spi1CS,PA_4,HAL_GPIO_PUSH_PULL_OUTPUT,HAL_GPIO_NO_PULL,1);//初始化GPIO PC_13为推挽输出高电平
	if(COMMON_RET_OK!=ret){
		//初始化失败了
		return COMMON_RET_ERROR;
	}

	/*ret=HALSpiInit(HAL_SPI2,HAL_SPI_CPOL_HIGH,HAL_SPI_CPHA_2Edge,HAL_SPI_PRESCALER_256,HAL_SPI_FirstBit_MSB);
	if(COMMON_RET_OK!=ret){
		return ret;
	}
	//初始化一个gpio用来控制spi2的cs

	HALGpioWrite(spi1CS,0);	//片选spi1
	HALSpiInOut(HAL_SPI1, LORA_ADDR | 0x80 );
	u8_recive=HALSpiInOut(HAL_SPI1,0x88);//写入数据
	HALGpioWrite(spi1CS,1);
	
	HALGpioWrite(spi1CS,0);	//片选spi1
	HALSpiInOut(HAL_SPI1, LORA_ADDR & 0x7F );
	u8_recive=HALSpiInOut(HAL_SPI1,0x00);//读取数据
	HALGpioWrite(spi1CS,1);//取消spi1片选
	printf("read from reg(0x%02x) value=0x%02x\r\n",LORA_ADDR,u8_recive);
	
	while(1){
		for(i=0;i<0x8fffff;i++);
		//省略了片选，实际这里需要加spi2片选
		u8_recive=HALSpiInOut(HAL_SPI2,0x55);
		//省略了取消片选，实际这里需要加取消spi2片选
	}*/
	return ret;
}
