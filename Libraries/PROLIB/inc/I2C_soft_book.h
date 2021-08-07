#ifndef  __I2C_SOFT_BOOK_H_
#define  __I2C_SOFT_BOOK_H_

#include "stm32f10x.h"
 
//---------------- 这里封装了 I2C 通讯配置信息 -------------------
 
#define   _Soft_I2C_GPIO_APBxClock_FUN   RCC_APB2PeriphClockCmd
#define   _Soft_I2C_GPIO_CLK             RCC_APB2Periph_GPIOB
#define   _Soft_I2C_SCL_PORT             GPIOB
#define   _Soft_I2C_SCL_PIN              GPIO_Pin_6
#define   _Soft_I2C_SDA_PORT             GPIOB
#define   _Soft_I2C_SDA_PIN              GPIO_Pin_7
 

#define   _I2C_SCL_1()   _Soft_I2C_SCL_PORT->BSRR = _Soft_I2C_SCL_PIN
#define   _I2C_SCL_0()   _Soft_I2C_SCL_PORT->BRR =  _Soft_I2C_SCL_PIN
#define   _I2C_SDA_1()   _Soft_I2C_SCL_PORT->BSRR = _Soft_I2C_SDA_PIN
#define   _I2C_SDA_0()   _Soft_I2C_SCL_PORT->BRR =  _Soft_I2C_SDA_PIN
#define   _I2C_SDA_READ()  ((_Soft_I2C_SCL_PORT->IDR & _Soft_I2C_SDA_PIN)!=0)

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

//----------------器件地址--------------------
/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */
/* EEPROM Addresses defines */
#define Soft_EEPROM_ADDRESS 0xA0   /* E2 = 0 */
//#define EEPROM_ADDRESS 0xA2 /* E2 = 0 */
//#define EEPROM_ADDRESS 0xA4 /* E2 = 0 */
//#define EEPROM_ADDRESS 0xA6 /* E2 = 0 */
 
/*读取数据的格式以及字符数量*/
#define   _I2C_Soft_PageSize      8
#define   _I2C_Soft_SIZE				256			  /* 24xx02总容量 */
/*I2C 存储地址*/
#define  EEP_Soft_Firstpage      0x90

void  I2C_Soft_Init(void);
void  EE_Soft_Trase(void);
uint8_t I2C_Soft_Write_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
uint8_t I2C_Soft_Read_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);


#endif

