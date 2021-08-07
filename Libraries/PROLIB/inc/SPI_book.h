#ifndef  __SPI_BOOK_H_
#define  __SPI_BOOK_H_

#include "stm32f10x.h"

//#define  _SPI_FLASH_ID              0xEF3015   //W25X16
//#define  _SPI_FLASH_ID              0xEF4015	 //W25Q16
//#define  _SPI_FLASH_ID              0XEF4018   //W25Q128
#define  _SPI_FLASH_ID              0XEF4017    //W25Q64
//---------------- 这里封装了 I2C 通讯配置信息 -------------------
#define   _FLASH_SPIx                     SPI1
#define   _FLASH_SPI_APBxClock_FUN        RCC_APB2PeriphClockCmd    
#define   _FLASH_SPI_CLK                  RCC_APB2Periph_SPI1
#define   _FLASH_SPI_GPIO_APBxClock_FUN   RCC_APB2PeriphClockCmd

#define   _FLASH_SPI_GPIO_CLK             RCC_APB2Periph_GPIOA
#define   _FLASH_SPI_SCL_PORT             GPIOA
#define   _FLASH_SPI_SCL_PIN              GPIO_Pin_5
#define   _FLASH_SPI_MISO_PORT            GPIOA
#define   _FLASH_SPI_MISO_PIN             GPIO_Pin_6
#define   _FLASH_SPI_MOSI_PORT            GPIOA
#define   _FLASH_SPI_MOSI_PIN             GPIO_Pin_7
#define   _FLASH_SPI_CSS_PORT             GPIOA
#define   _FLASH_SPI_CSS_PIN              GPIO_Pin_4

//FLASH_SPI 引脚配置
#define   _FLASH_CSS_HIGH()      _FLASH_SPI_CSS_PORT->BSRR = _FLASH_SPI_CSS_PIN
#define   _FLASH_CSS_LOW()       _FLASH_SPI_CSS_PORT->BRR =  _FLASH_SPI_CSS_PIN
 
/*通讯等待超时时间*/
#define  FLASH_SPI_TIMEOUT        ((uint32_t)0x6000)
#define  FLASH_SPI_LONG_TIMEOUT   ((uint32_t)(10*FLASH_SPI_TIMEOUT))

//信息输出
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)

//
#define  SPI_PAGE_SIZE   4096
#define  SPI_PAGE_Write_SIZE   256
 

//FLASH 指令
#define  FLASH_SPI_DUMMY            0x00
#define  FLASH_SPI_READ_JEDEC_ID    0x9f
#define  FLASH_SPI_REASE_SECTOR     0x20
#define  FLASH_SPI_READ_STATUS      0x05
#define  FLASH_SPI_READ_DATA        0x03
#define  FLASH_SPI_WRITE_ENABLE     0x06
#define  FLASH_SPI_WRITE_DATA       0x02
#define  FLASH_SPI_ChipErase        0xC7


void SPI_FLASH_Init(void);
uint32_t SPI_Read_ID(void);
uint32_t SPI_Read_DeviceID(void);
 
void SPI_Erase_Sector(uint32_t addr);
void SPI_FLASH_BulkErase(void);
void SPI_Read_Data(uint8_t *readBuffer , uint32_t addr ,uint16_t  numByteToRead );
void SPI_BufferRead_Data(uint8_t *writeBuffer , uint32_t WriteAddr ,uint16_t  numByteToWrite );
void SPI_Write_Data(uint8_t *writeBuffer , uint32_t addr ,uint16_t  numByteToRead );
void SPI_BufferWrite_Data(uint8_t *writeBuffer , uint32_t WriteAddr ,uint16_t  numByteToWrite );
void SPI_Show_Data(uint8_t *readBuffer , uint16_t  numByteToRead);
#endif 
