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

//gpio占用表
//PA_2	UART2_TX(作为log口)
//PA_3	UART2_RX(作为log口)
//PA_4	SPI1_CS(使用gpio软件控制)
//PA_5	SPI1_SCK
//PA_6	SPI1_MISO
//PA_7	SPI1_MOSI
//PA_9	UART1_TX
//PA_10	UART1_RX
//PB_3	注意：PB_3默认是JTAG接口，使用PB_3的功能需要进行端口重映射
//PB_6	I2C1_SCL
//PB_7	I2C1_SDA
//PB_10	UART3_TX
//PB_11	UART3_RX
//PB_13	SPI2_SCK
//PB_14	SPI2_MISO
//PB_15	SPI2_MOSI


//bool EnableMaster=false;//主从选择
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

const uint8_t PingMsg[] = "Embeded-Party";
const uint8_t PongMsg[] = "Hello-Everyone";

uint16_t BufferSize = BUFFER_SIZE;
uint8_t TX_Buffer[BUFFER_SIZE];
uint8_t RX_Buffer[BUFFER_SIZE];

int8_t RssiValue = 0;
int8_t SnrValue = 0;


void OnTxDone( void )
{
	printf("[%s()-%d]TxDone\r\n",__func__,__LINE__);
	Radio.Standby();
	Radio.Rx( RX_TIMEOUT_VALUE ); //进入接收
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
	BufferSize = size;
	memcpy( RX_Buffer, payload, BufferSize );
	RssiValue = rssi;
	SnrValue = snr;
    
	Radio.Standby();
	printf("[%s()-%d]RxDone\r\nrecive:%s\r\nsize:%d\r\nrssi:%d\r\nsnr:%d\r\n",__func__,__LINE__,payload,size,rssi,snr);
	if(EnableMaster){
		if(memcmp(RX_Buffer,PongMsg,14)==0){
        printf("[%s()-%d]recive ok\r\n",__func__,__LINE__);
		}
     
//		TX_Buffer[0] = 'P';
//		TX_Buffer[1] = 'I';
//		TX_Buffer[2] = 'N';
//		TX_Buffer[3] = 'G'; 
		
		TX_Buffer[0] = 'E';
		TX_Buffer[1] = 'm';
		TX_Buffer[2] = 'b';
		TX_Buffer[3] = 'e'; 
		TX_Buffer[4] = 'd';
		TX_Buffer[5] = 'e';
		TX_Buffer[6] = 'd';
		TX_Buffer[7] = '-'; 
		TX_Buffer[8] = 'P';
		TX_Buffer[9] = 'a';
		TX_Buffer[10] = 'r';
		TX_Buffer[11] = 't'; 
		TX_Buffer[12] = 'y'; 
		
        
		crc_value=RadioComputeCRC(TX_Buffer,13,CRC_TYPE_IBM);//计算得出要发送数据包CRC值
		TX_Buffer[13]=crc_value>>8;
		TX_Buffer[14]=crc_value;
		Radio.Send( TX_Buffer, 15);
	}else{
		if(memcmp(RX_Buffer,PingMsg,13)==0){
			printf("[%s()-%d]recive ok\r\n",__func__,__LINE__);
//			TX_Buffer[0] = 'P';
//			TX_Buffer[1] = 'O';
//			TX_Buffer[2] = 'N';
//			TX_Buffer[3] = 'G'; 
		TX_Buffer[0] = 'H';
		TX_Buffer[1] = 'e';
		TX_Buffer[2] = 'l';
		TX_Buffer[3] = 'l'; 
		TX_Buffer[4] = 'o';
		TX_Buffer[5] = '-';
		TX_Buffer[6] = 'E';
		TX_Buffer[7] = 'v'; 
		TX_Buffer[8] = 'e';
		TX_Buffer[9] = 'r';
		TX_Buffer[10] = 'y';
		TX_Buffer[11] = 'o'; 
		TX_Buffer[12] = 'n'; 
		TX_Buffer[13] = 'e'; 
			crc_value=RadioComputeCRC(TX_Buffer,14,CRC_TYPE_IBM);//计算得出要发送数据包CRC值
			TX_Buffer[14]=crc_value>>8;
			TX_Buffer[15]=crc_value;
			Radio.Send( TX_Buffer, 16);
		}else{
			Radio.Rx( RX_TIMEOUT_VALUE ); 
		}   
	}
}

void OnTxTimeout( void )
{
	printf("[%s()-%d]TxTimeout\r\n",__func__,__LINE__);
}

void OnRxTimeout( void )
{
	printf("[%s()-%d]RxTimeout\r\n",__func__,__LINE__);
	Radio.Standby();
	if(EnableMaster){
//		TX_Buffer[0] = 'P';
//		TX_Buffer[1] = 'I';
//		TX_Buffer[2] = 'N';
//		TX_Buffer[3] = 'G'; 
//        
//		crc_value=RadioComputeCRC(TX_Buffer,4,CRC_TYPE_IBM);//计算得出要发送数据包CRC值
//		TX_Buffer[4]=crc_value>>8;
//		TX_Buffer[5]=crc_value;
//		Radio.Send( TX_Buffer, 6);
				TX_Buffer[0] = 'E';
		TX_Buffer[1] = 'm';
		TX_Buffer[2] = 'b';
		TX_Buffer[3] = 'e'; 
		TX_Buffer[4] = 'd';
		TX_Buffer[5] = 'e';
		TX_Buffer[6] = 'd';
		TX_Buffer[7] = '-'; 
		TX_Buffer[8] = 'P';
		TX_Buffer[9] = 'a';
		TX_Buffer[10] = 'r';
		TX_Buffer[11] = 't'; 
		TX_Buffer[12] = 'y'; 
		
        
		crc_value=RadioComputeCRC(TX_Buffer,13,CRC_TYPE_IBM);//计算得出要发送数据包CRC值
		TX_Buffer[13]=crc_value>>8;
		TX_Buffer[14]=crc_value;
		Radio.Send( TX_Buffer, 15);
	}else{
		Radio.Rx( RX_TIMEOUT_VALUE ); 
	}
}

void OnRxError( void )
{
	printf("[%s()-%d]RxError\r\n",__func__,__LINE__);
}

int main(void)
{
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
		printf("[%s()-%d]master send PING\r\n",__func__,__LINE__);
		TX_Buffer[0] = 'P';
		TX_Buffer[1] = 'I';
		TX_Buffer[2] = 'N';
		TX_Buffer[3] = 'G'; 
          
		crc_value=RadioComputeCRC(TX_Buffer,4,CRC_TYPE_IBM);//计算得出要发送数据包CRC值
		TX_Buffer[4]=crc_value>>8;
		TX_Buffer[5]=crc_value;
		Radio.Send( TX_Buffer, 6);
	}else{
		printf("[%s()-%d]slave enter rx mode\r\n",__func__,__LINE__);
		Radio.Rx( RX_TIMEOUT_VALUE ); 
	}
    
	while( 1 ){
		Radio.IrqProcess( ); // Process Radio IRQ
	}
}
