#include "stm32f4xx_hal.h"
#include "wizchip_conf.h"
#include "socket.h"
#include "wiznet.h"


extern SPI_HandleTypeDef WIZ_SPI_STRUCT;
uint8_t socket_status;

uint8_t wizSpiReadWrite(uint8_t data)
{
	HAL_SPI_TransmitReceive(&WIZ_SPI_STRUCT, &data, &data, 1, 100);
	return data;
}

void wizSelect(void)
{
	HAL_GPIO_WritePin(WIZ_CHIP_SELECT_PORT, WIZ_CHIP_SELECT_PIN, GPIO_PIN_RESET);
}

void wizDeSelect(void)
{
	HAL_GPIO_WritePin(WIZ_CHIP_SELECT_PORT, WIZ_CHIP_SELECT_PIN, GPIO_PIN_SET);
}

uint8_t wizSpiRead()
{
	uint8_t rb;
	rb = wizSpiReadWrite(0x00);
	return rb;
}

void wizSpiWrite(uint8_t wb)
{
	wizSpiReadWrite(wb);
}

void wizSpiReadBurst(uint8_t* pBuf, uint16_t len)
{
	for(uint16_t i=0; i<len; i++)
	{
		*pBuf = wizSpiReadWrite(0x00);
		pBuf++;
	}
}

void wizSpiWriteBurst(uint8_t* pBuf, uint16_t len)
{
	for(uint16_t i=0; i<len; i++)
	{
		*pBuf = wizSpiReadWrite(*pBuf);
		pBuf++;
	}
}

void wizNetSend(uint8_t* data, uint8_t size)
{
	socket_status = getSn_SR(WIZ_SOCK_NUM);
	
	switch(socket_status)
	{
		case SOCK_ESTABLISHED:
			send(WIZ_SOCK_NUM, data, size);
		break;
		case SOCK_CLOSED:
			wizOpenListen();
		case SOCK_CLOSE_WAIT:
			wizOpenListen();
		break;
	}
}


void wizNetRecv(uint8_t* data, uint8_t size)
{
	socket_status = getSn_SR(WIZ_SOCK_NUM);

	if(getSn_RX_RSR(WIZ_SOCK_NUM) >= size)  // Check the received data size
		recv(WIZ_SOCK_NUM, data, size);
	
	if( (socket_status == SOCK_CLOSED) || (socket_status == SOCK_CLOSE_WAIT) )
		wizOpenListen();
}


uint8_t wizInit(void)
{
			
	wizDeSelect();
	
	// Send a pulse on reset pin
	wizReset();
	
	// Register callback functions
	reg_wizchip_cs_cbfunc(wizSelect, wizDeSelect);
	reg_wizchip_spi_cbfunc(wizSpiRead, wizSpiWrite);
	reg_wizchip_spiburst_cbfunc(wizSpiReadBurst, wizSpiWriteBurst);
	
	// Set Network Information
	wiz_NetInfo net_info = {
		.mac = WIZ_NET_INFO_MAC,
		.ip = WIZ_NET_INFO_IP,
		.sn = WIZ_NET_INFO_SN,
		.gw = WIZ_NET_INFO_GW,
		.dns = WIZ_NET_INFO_DNS,
		.dhcp = WIZ_NET_INFO_DHCP };

	wiz_NetTimeout	net_timeout = {
		
		.retry_cnt = WIZ_NET_RETRY_CNT,     	///< retry count 
		.time_100us = WIZ_NET_TIMEOUT_100US    	///< time unit 100us
	};		
	
	wizchip_setnetinfo(&net_info);
	wizchip_settimeout(&net_timeout);
	
	setSn_TXBUF_SIZE(WIZ_SOCK_NUM, WIZ_TX_BUF_SIZE);
	setSn_RXBUF_SIZE(WIZ_SOCK_NUM, WIZ_RX_BUF_SIZE);

	if(socket(WIZ_SOCK_NUM, Sn_MR_TCP, WIZ_SOCK_PORT, 0) == 0)
	{
		socket(WIZ_SOCK_NUM, Sn_MR_TCP, WIZ_SOCK_PORT, 0);
		listen(WIZ_SOCK_NUM);
	}
	else
	{
		// Wizchip not correctly connect to microcontroller.
		// Handle Error at this place
		return 1;
	}

 return 0;
}

uint8_t wizGetVersion(void)
{	
	wizSelect();
	wizSpiWrite(0x00);
	wizSpiWrite(0x39);
	uint8_t w5500_version = wizSpiRead();
	wizDeSelect();
	
	return w5500_version;
}

void wizReset(void)
{
	HAL_GPIO_WritePin(WIZ_RESET_PORT, WIZ_RESET_PIN, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(WIZ_RESET_PORT, WIZ_RESET_PIN, GPIO_PIN_SET);
	HAL_Delay(10);
}

void wizOpenListen()
{
	socket(WIZ_SOCK_NUM, Sn_MR_TCP, WIZ_SOCK_PORT, 0);
	listen(WIZ_SOCK_NUM);
}
