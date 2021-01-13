/*
  ______                              _
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: SX126x driver specific target board functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#include "stm32f10x.h"
#include "my_delay.h"
//#include "gpio.h"
#include "HAL_SPI_API.h"
#include "radio.h"
#include "sx126x.h"
#include "sx126x-board.h"
#include "common.h"

void SX126xReset( void )
{
    DelayMs( 10 );
    GPIO_WriteBit( RADIO_nRESET_PORT, RADIO_nRESET_PIN,Bit_RESET);
    DelayMs( 20 );
    GPIO_WriteBit( RADIO_nRESET_PORT, RADIO_nRESET_PIN,Bit_SET);
    DelayMs( 10 );
}

void SX126xWaitOnBusy( void )
{
   while(GPIO_ReadInputDataBit(RADIO_BUSY_PORT,RADIO_BUSY_PIN)==Bit_SET);
}


void SX126xWakeup( void )
{
    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_RESET);
   
    HALSpiInOut(HAL_SPI1,RADIO_GET_STATUS);
    HALSpiInOut(HAL_SPI1,0);
    
    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_SET);

    // Wait for chip to be ready.
    SX126xWaitOnBusy( );
}

void SX126xWriteCommand( RadioCommands_t command, uint8_t *buffer, uint16_t size )
{
	uint16_t i = 0;
    SX126xCheckDeviceReady( );

    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_RESET);

    HALSpiInOut(HAL_SPI1,( uint8_t )command );

    for( i = 0; i < size; i++ )
    {
        HALSpiInOut(HAL_SPI1,buffer[i] );
    }

    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_SET);
    
    if( command != RADIO_SET_SLEEP )
    {
        SX126xWaitOnBusy( );
    }
}

void SX126xReadCommand( RadioCommands_t command, uint8_t *buffer, uint16_t size )
{
	uint16_t i = 0;
    SX126xCheckDeviceReady( );

    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_RESET);

    HALSpiInOut(HAL_SPI1,( uint8_t )command );
    HALSpiInOut(HAL_SPI1,0x00 );
    for( i = 0; i < size; i++ )
    {
        buffer[i] = HALSpiInOut(HAL_SPI1,0 );
    }

    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_SET);

    SX126xWaitOnBusy( );
}

void SX126xWriteRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
{
	uint16_t i = 0;
	
    SX126xCheckDeviceReady( );

    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_RESET);
    
    HALSpiInOut(HAL_SPI1,RADIO_WRITE_REGISTER );
    HALSpiInOut(HAL_SPI1,( address & 0xFF00 ) >> 8 );
    HALSpiInOut(HAL_SPI1, address & 0x00FF );
    
    for( i = 0; i < size; i++ )
    {
        HALSpiInOut(HAL_SPI1,buffer[i] );
    }


    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_SET);

    SX126xWaitOnBusy( );
}

void SX126xWriteRegister( uint16_t address, uint8_t value )
{
    SX126xWriteRegisters( address, &value, 1 );
}

void SX126xReadRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
{
	uint16_t i = 0;
    SX126xCheckDeviceReady( );

    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_RESET);

    HALSpiInOut(HAL_SPI1,RADIO_READ_REGISTER );
    HALSpiInOut(HAL_SPI1,( address & 0xFF00 ) >> 8 );
    HALSpiInOut(HAL_SPI1, address & 0x00FF );
    HALSpiInOut(HAL_SPI1, 0 );
    for( i = 0; i < size; i++ )
    {
        buffer[i] = HALSpiInOut(HAL_SPI1,0 );
    }

    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_SET);

    SX126xWaitOnBusy( );
}

uint8_t SX126xReadRegister( uint16_t address )
{
    uint8_t data;
    SX126xReadRegisters( address, &data, 1 );
    return data;
}

void SX126xWriteBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
{
	uint16_t i = 0;
    SX126xCheckDeviceReady( );

    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_RESET);
    
    HALSpiInOut(HAL_SPI1, RADIO_WRITE_BUFFER );
    HALSpiInOut(HAL_SPI1, offset );
    for( i = 0; i < size; i++ )
    {
        HALSpiInOut(HAL_SPI1, buffer[i] );
    }

    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_SET);

    SX126xWaitOnBusy( );
}

void SX126xReadBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
{
	uint16_t i = 0;
	
    SX126xCheckDeviceReady( );

    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_RESET);

    HALSpiInOut(HAL_SPI1,  RADIO_READ_BUFFER );
    HALSpiInOut(HAL_SPI1,  offset );
    HALSpiInOut(HAL_SPI1,  0 );
    for( i = 0; i < size; i++ )
    {
        buffer[i] = HALSpiInOut(HAL_SPI1, 0 );
    }

    GPIO_WriteBit( RADIO_NSS_PORT, RADIO_NSS_PIN,Bit_SET);
    
    SX126xWaitOnBusy( );
}

void SX126xSetRfTxPower( int8_t power )
{
    SX126xSetTxParams( power, RADIO_RAMP_40_US );
}

uint8_t SX126xGetPaSelect( uint32_t channel )
{
//    if( GpioRead( &DeviceSel ) == 1 )
//    {
//        return SX1261;
//    }
//    else
//    {
//        return SX1262;
//    }
  
  return SX1262;
}

void SX126xAntSwOn( void )
{
    //GpioInit( &AntPow, ANT_SWITCH_POWER, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1 );
}

void SX126xAntSwOff( void )
{
   // GpioInit( &AntPow, ANT_SWITCH_POWER, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
}

bool SX126xCheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}
