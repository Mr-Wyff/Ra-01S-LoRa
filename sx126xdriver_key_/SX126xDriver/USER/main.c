//硬件无关的
//硬件相关的
#include "stm32f10x.h"
//测试demo
#include "example_HAL_gpio_in_out.h"
#include "example_HAL_uart_init.h"
#include "example_HAL_spi_init.h"
#include "example_HAL_gpio_irq.h"
#include "HAL_common.h"
#include "sx126x-board.h"
#include "radio.h"
#include "sx126x.h"
#include "sx126x-board.h"
#include "crc.h"
#include "string.h"
#include "my_delay.h"

bool EnableMaster=true;//主从选择

#define RF_FREQUENCY                                434000000 // Hz
#define TX_OUTPUT_POWER                             22        // 22 dBm
#define LORA_BANDWIDTH                              1         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       11         // [SF7..SF12]
#define LORA_CODINGRATE                             2         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

uint16_t  crc_value;
/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 64 // Define the payload size here

const uint8_t MSG[] = "hello";

uint16_t BufferSize = BUFFER_SIZE;
uint8_t TX_Buffer[BUFFER_SIZE];
uint8_t RX_Buffer[BUFFER_SIZE];

int8_t RssiValue = 0;
int8_t SnrValue = 0;


void OnTxDone( void )
{
	printf("[%s()-%d]TxDone\r\n",__func__,__LINE__);
	Radio.Standby();
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
	static bool ledStatus=false;
	BufferSize = size;
	
	memcpy( RX_Buffer, payload, BufferSize );
	RssiValue = rssi;
	SnrValue = snr;
    
	Radio.Standby();
	printf("[%s()-%d]RxDone\r\nrecive:%s\r\nsize:%d\r\nrssi:%d\r\nsnr:%d\r\n",__func__,__LINE__,payload,size,rssi,snr);
	
	if( strncmp( ( const char* )payload, ( const char* )MSG, strlen((char *)MSG) ) == 0 ){
		if(ledStatus){
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);	//输出低电平
		}else{
			GPIO_SetBits(GPIOB,GPIO_Pin_12);	//输出高电平
		}
		ledStatus=!ledStatus;
	}
	
	Radio.Rx( RX_TIMEOUT_VALUE );
}

void OnTxTimeout( void )
{
	printf("[%s()-%d]TxTimeout\r\n",__func__,__LINE__);
}

void OnRxTimeout( void )
{
	Radio.Standby();
	printf("[%s()-%d]RxTimeout retry recive\r\n",__func__,__LINE__);
	Radio.Rx( RX_TIMEOUT_VALUE ); 
}

void OnRxError( void )
{
	Radio.Standby();
	printf("[%s()-%d]RxError retry recive\r\n",__func__,__LINE__);
	Radio.Rx( RX_TIMEOUT_VALUE ); 
}

int main(void)
{
	uint32_t count=0;
	
	CommonMcuInit();
	 
	ExampleHALGpioInOut();	//gpio输入输出测试
	ExampleUartInit();	//串口初始化测试
	ExampleSpiInit();
	//ExampleHALGpioIrq();
	printf("[%s()-%d]init ok\r\n",__func__,__LINE__);
	// Radio initialization
	RadioEvents.TxDone = OnTxDone;
	RadioEvents.RxDone = OnRxDone;
	RadioEvents.TxTimeout = OnTxTimeout;
	RadioEvents.RxTimeout = OnRxTimeout;
	RadioEvents.RxError = OnRxError;
    
    
	Radio.Init( &RadioEvents );
	Radio.SetChannel( RF_FREQUENCY );
	Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                     LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                     LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                     true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

	Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                     LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                     LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                     0, true, 0, 0, LORA_IQ_INVERSION_ON, false );
	if(EnableMaster){
		printf("[%s()-%d]This is master\r\n",__func__,__LINE__);
	}else{
		printf("[%s()-%d]slave enter rx mode\r\n",__func__,__LINE__);
		Radio.Rx( RX_TIMEOUT_VALUE ); 
	}
    
	while( 1 ){
		Radio.IrqProcess( ); // Process Radio IRQ
		if(EnableMaster){
			//检测按键并发送
			count=0;
			while(0==GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)){
				DelayMs(2);
				if(count++ > 10){//按键消抖
					//按下了
					printf("[%s()-%d]sen msg\r\n",__func__,__LINE__);
					Radio.Send( (uint8_t *)MSG, strlen((char *)MSG) );
					DelayMs( 500 );
					break;
				}
			}
		}
	}
}
