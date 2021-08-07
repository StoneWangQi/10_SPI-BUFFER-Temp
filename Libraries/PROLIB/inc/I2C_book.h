#ifndef  __I2C_BOOK_H_
#define  __I2C_BOOK_H_

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
 
#include "USART_book.h"

//---------------- �����װ�� I2C ͨѶ������Ϣ -------------------
#define   _EEPROM_I2Cx                     I2C1
#define   _EEPROM_I2C_APBxClock_FUN        RCC_APB1PeriphClockCmd    
#define   _EEPROM_I2C_CLK                  RCC_APB1Periph_I2C1
#define   _EEPROM_I2C_GPIO_APBxClock_FUN   RCC_APB2PeriphClockCmd
#define   _EEPROM_I2C_GPIO_CLK             RCC_APB2Periph_GPIOB
#define   _EEPROM_I2C_SCL_PORT             GPIOB
#define   _EEPROM_I2C_SCL_PIN              GPIO_Pin_6
#define   _EEPROM_I2C_SDA_PORT             GPIOB
#define   _EEPROM_I2C_SDA_PIN              GPIO_Pin_7

/*STM32 I2C �ٶ�ģʽ  */
#define   _I2C_Speed       400000
/* I2C ������ַ */
#define   _I2Cx_OWN_ADDRESS7              0x5f
/*��ȡ���ݵĸ�ʽ�Լ��ַ�����*/
#define   _I2C_PageSize      8
/*I2C �洢��ַ*/
#define  EEP_Firstpage      0x90
#define  EEP_SIZE           0xFF
//----------------������ַ--------------------
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
#define EEPROM_ADDRESS 0xA0   /* E2 = 0 */
//#define EEPROM_ADDRESS 0xA2 /* E2 = 0 */
//#define EEPROM_ADDRESS 0xA4 /* E2 = 0 */
//#define EEPROM_ADDRESS 0xA6 /* E2 = 0 */

//----------------��������--------------------

//I2C Ӧ�ú���
void _I2C_EE_Init(void);
void I2C_Write_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void I2C_Read_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);

 
#endif

