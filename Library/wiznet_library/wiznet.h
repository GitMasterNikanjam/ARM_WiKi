#ifndef SRC_W5500_W5500_SPI_H_
#define SRC_W5500_W5500_SPI_H_

#include <stdint.h>

// --------------------------------------------------
// User Functions:

// High level functions
uint8_t wizInit(void);															 // Wiznet module initialize according to user parameters
uint8_t wizGetVersion(void);												 // Get Wiznet module version number.(just for test connection)
void wizNetSend(uint8_t* data, uint8_t size);				 // Send network data as server to client
void wizNetRecv(uint8_t* data, uint8_t size);				 // Recieve network data as server from client
void wizOpenListen(void);														 // Open and Listen Socket

// Low level functions
void wizSelect(void);																	// SPI Chip Select Actived for wiznet module 
void wizDeSelect(void);																// SPI Chip Select Disabled for wiznet module 
void wizSpiWrite(uint8_t wb);													// SPI write one byte to wiznet module 
uint8_t wizSpiRead(void);															// SPI read one byte from wiznet module
uint8_t wizSpiReadWrite(uint8_t data);								// SPI read and write at same time from wiznet module
void wizSpiReadBurst(uint8_t* pBuf, uint16_t len);		// SPI read some bytes from wiznet module
void wizSpiWriteBurst(uint8_t* pBuf, uint16_t len);		// SPI write some bytes to wiznet module
void wizReset(void);																	// Hardware Reset on Reset pin of wiznet module


// ---------------------------------------------------
// User Define Parameters:

// Reset Port & Pin
#define WIZ_RESET_PORT						GPIOB
#define WIZ_RESET_PIN							GPIO_PIN_5

// SPI Chip Select Port & Pin
#define WIZ_CHIP_SELECT_PORT			GPIOA
#define WIZ_CHIP_SELECT_PIN			  GPIO_PIN_11

// HAL SPI Struct
#define WIZ_SPI_STRUCT						hspi1

// Net Information
#define WIZ_NET_INFO_MAC					{ 0x00, 0x08, 0xdc, 0x01, 0x02, 0x03 }
#define WIZ_NET_INFO_IP						{ 192, 168, 1, 100 }
#define WIZ_NET_INFO_SN						{ 255, 255, 255, 0 }
#define WIZ_NET_INFO_GW						{ 192, 168, 1, 1 }
#define WIZ_NET_INFO_DNS					{ 8, 8, 8, 8 }
#define WIZ_NET_INFO_DHCP					NETINFO_STATIC

// Buffer TX/RX Size (max = 16)
#define WIZ_TX_BUF_SIZE						2				// K bytes (max = 16)
#define WIZ_RX_BUF_SIZE						2				// K bytes (max = 16)

// Socket Number & Port
#define WIZ_SOCK_NUM							0				// must be in range [0 ~ 7]
#define WIZ_SOCK_PORT							5000

// Net Timeout Parameters	
#define WIZ_NET_RETRY_CNT					3				// Set retry count
#define WIZ_NET_TIMEOUT_100US			10000		// Set timeout. time unit 100us

#endif




