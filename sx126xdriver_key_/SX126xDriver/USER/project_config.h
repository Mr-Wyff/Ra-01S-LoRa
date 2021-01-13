#ifndef __PROJECT_CONFIG_H__
#define __PROJECT_CONFIG_H__

/*-------------common ����������----------------------------*/
#define ENABLE_DEBUG	1	//ʹ�� DEBUG �ӿ�
#define ENABLE_INFO_LOG	1	//ʹ�� ERROR_LOG �ӿ�
#define AT_SERVICE_BUFLEN	512	//tmp_print_buf��ӡbuf��С
#define ENABLE_INFO_LOG	1	//ʹ�� AT_PRINT �ӿ�

#define RADIO_NSS_PIN       GPIO_Pin_4
#define RADIO_NSS_PORT      GPIOA
#define RADIO_MOSI_PIN      GPIO_Pin_7
#define RADIO_MOSI_PORT     GPIOA
#define RADIO_MISO_PIN      GPIO_Pin_6
#define RADIO_MISO_PORT     GPIOA
#define RADIO_SCK_PIN       GPIO_Pin_5
#define RADIO_SCK_PORT      GPIOA
#define RADIO_nRESET_PIN    GPIO_Pin_1
#define RADIO_nRESET_PORT   GPIOB
#define RADIO_BUSY_PIN      GPIO_Pin_0
#define RADIO_BUSY_PORT     GPIOA
#define RADIO_DIO1_PIN      GPIO_Pin_1
#define RADIO_DIO1_PORT     GPIOA 
#define RADIO_DIO3_PIN      GPIO_Pin_11
#define RADIO_DIO3_PORT     GPIOA

#endif	/*end of __PROJECT_CONFIG_H__*/
