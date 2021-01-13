#include "HAL_gpio_API.h"

//�����������ʾGPIO������������ӣ�PC_13��f103c8��Сϵͳ�Ͻ���һ��LED����PB_0����
//ִ�к��Ч����
//    PB_0��ȡ���ߵ�ƽ��ʱ��PC_13����͵�ƽ��LED��
//    PB_0��ȡ���͵�ƽ��ʱ��PC_13����ߵ�ƽ��LED��
//    ��ΪPB_0���õ����������룬���Ը���״̬PB_0�Ǹߵ�ƽ��Ҳ���Ǹ���ʱLED�ǳ���ģʽ



void ExampleHALGpioInOut(void){
	/*common_ret ret=COMMON_RET_ERROR;
	HAL_Gpio_t gpioLed,gpioKey;
	uint32_t u32_keyValue;
	
	ret=HALGpioInit(&gpioLed,PC_13,HAL_GPIO_PUSH_PULL_OUTPUT,HAL_GPIO_NO_PULL,1);//��ʼ��GPIO PC_13Ϊ��������ߵ�ƽ
	if(COMMON_RET_OK!=ret){
		//��ʼ��ʧ����
		return;
	}

	ret=HALGpioInit(&gpioKey,PB_0,HAL_GPIO_INPUT,HAL_GPIO_PULL_UP,0);//��ʼ��GPIO PB_0Ϊ��������ģʽ
	if(COMMON_RET_OK!=ret){
		//��ʼ��ʧ����
		return;
	}

	while(1)
	{
		u32_keyValue=HALGpioRead(gpioKey);
		if(1==u32_keyValue){
			HALGpioWrite(gpioLed,0);//����LED
		}else if(0==u32_keyValue){
			HALGpioWrite(gpioLed,1);//���LED
		}else{
			//дGPIOʧ��
			return;
		}
	}*/
	common_ret ret=COMMON_RET_ERROR;
	HAL_Gpio_t tmpGpio;
	
	//RF_RST RADIO_nRESET_PIN RADIO_nRESET_PORT
	ret=HALGpioInit(&tmpGpio,PB_1,HAL_GPIO_PUSH_PULL_OUTPUT,HAL_GPIO_PULL_UP,1);//��ʼ��GPIO Ϊ��������ߵ�ƽ
	if(COMMON_RET_OK!=ret){
		//��ʼ��ʧ����
		printf("[%s()-%d]gpio init error\r\n",__func__,__LINE__);
		return;
	}
	
	//RF_RST RADIO_BUSY_PIN RADIO_BUSY_PORT
	ret=HALGpioInit(&tmpGpio,PA_0,HAL_GPIO_INPUT,HAL_GPIO_PULL_UP,1);//��ʼ��GPIO Ϊ��������
	if(COMMON_RET_OK!=ret){
		//��ʼ��ʧ����
		printf("[%s()-%d]gpio init error\r\n",__func__,__LINE__);
		return;
	}
	
	//RF_RST RADIO_DIO1_PIN RADIO_DIO1_PORT
	ret=HALGpioInit(&tmpGpio,PA_1,HAL_GPIO_INPUT,HAL_GPIO_PULL_UP,1);//��ʼ��GPIO Ϊ��������
	if(COMMON_RET_OK!=ret){
		//��ʼ��ʧ����
		printf("[%s()-%d]gpio init error\r\n",__func__,__LINE__);
		return;
	}
	
	//RF_RST RADIO_DIO3_PIN RADIO_DIO3_PORT
	ret=HALGpioInit(&tmpGpio,PA_11,HAL_GPIO_INPUT,HAL_GPIO_PULL_UP,1);//��ʼ��GPIO Ϊ��������
	if(COMMON_RET_OK!=ret){
		//��ʼ��ʧ����
		printf("[%s()-%d]gpio init error\r\n",__func__,__LINE__);
		return;
	}
}
