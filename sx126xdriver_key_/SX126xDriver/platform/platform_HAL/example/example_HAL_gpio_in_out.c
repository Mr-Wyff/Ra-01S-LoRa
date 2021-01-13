#include "HAL_gpio_API.h"

//这个例子是演示GPIO输入输出的例子，PC_13在f103c8最小系统上接了一个LED，受PB_0控制
//执行后的效果是
//    PB_0读取到高电平的时候PC_13输出低电平，LED亮
//    PB_0读取到低电平的时候PC_13输出高电平，LED灭
//    因为PB_0设置的是上拉输入，所以浮空状态PB_0是高电平，也就是浮空时LED是常亮模式



void ExampleHALGpioInOut(void){
	/*common_ret ret=COMMON_RET_ERROR;
	HAL_Gpio_t gpioLed,gpioKey;
	uint32_t u32_keyValue;
	
	ret=HALGpioInit(&gpioLed,PC_13,HAL_GPIO_PUSH_PULL_OUTPUT,HAL_GPIO_NO_PULL,1);//初始化GPIO PC_13为推挽输出高电平
	if(COMMON_RET_OK!=ret){
		//初始化失败了
		return;
	}

	ret=HALGpioInit(&gpioKey,PB_0,HAL_GPIO_INPUT,HAL_GPIO_PULL_UP,0);//初始化GPIO PB_0为上拉输入模式
	if(COMMON_RET_OK!=ret){
		//初始化失败了
		return;
	}

	while(1)
	{
		u32_keyValue=HALGpioRead(gpioKey);
		if(1==u32_keyValue){
			HALGpioWrite(gpioLed,0);//点亮LED
		}else if(0==u32_keyValue){
			HALGpioWrite(gpioLed,1);//灭掉LED
		}else{
			//写GPIO失败
			return;
		}
	}*/
	common_ret ret=COMMON_RET_ERROR;
	HAL_Gpio_t tmpGpio;
	
	//RF_RST RADIO_nRESET_PIN RADIO_nRESET_PORT
	ret=HALGpioInit(&tmpGpio,PB_1,HAL_GPIO_PUSH_PULL_OUTPUT,HAL_GPIO_PULL_UP,1);//初始化GPIO 为推挽输出高电平
	if(COMMON_RET_OK!=ret){
		//初始化失败了
		printf("[%s()-%d]gpio init error\r\n",__func__,__LINE__);
		return;
	}
	
	//RF_RST RADIO_BUSY_PIN RADIO_BUSY_PORT
	ret=HALGpioInit(&tmpGpio,PA_0,HAL_GPIO_INPUT,HAL_GPIO_PULL_UP,1);//初始化GPIO 为上拉输入
	if(COMMON_RET_OK!=ret){
		//初始化失败了
		printf("[%s()-%d]gpio init error\r\n",__func__,__LINE__);
		return;
	}
	
	//RF_RST RADIO_DIO1_PIN RADIO_DIO1_PORT
	ret=HALGpioInit(&tmpGpio,PA_1,HAL_GPIO_INPUT,HAL_GPIO_PULL_UP,1);//初始化GPIO 为上拉输入
	if(COMMON_RET_OK!=ret){
		//初始化失败了
		printf("[%s()-%d]gpio init error\r\n",__func__,__LINE__);
		return;
	}
	
	//RF_RST RADIO_DIO3_PIN RADIO_DIO3_PORT
	ret=HALGpioInit(&tmpGpio,PA_11,HAL_GPIO_INPUT,HAL_GPIO_PULL_UP,1);//初始化GPIO 为上拉输入
	if(COMMON_RET_OK!=ret){
		//初始化失败了
		printf("[%s()-%d]gpio init error\r\n",__func__,__LINE__);
		return;
	}
	
	//初始化LED
	ret=HALGpioInit(&tmpGpio,PB_12,HAL_GPIO_PUSH_PULL_OUTPUT,HAL_GPIO_PULL_UP,1);//初始化GPIO 为推挽输出高电平
	if(COMMON_RET_OK!=ret){
		//初始化失败了
		printf("[%s()-%d]gpio init error\r\n",__func__,__LINE__);
		return;
	}
	
	ret=HALGpioInit(&tmpGpio,PB_13,HAL_GPIO_INPUT,HAL_GPIO_PULL_UP,1);//初始化GPIO 为上拉输入
	if(COMMON_RET_OK!=ret){
		//初始化失败了
		printf("[%s()-%d]gpio init error\r\n",__func__,__LINE__);
		return;
	}
}
