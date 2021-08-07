#ifndef  __USART_BOOK_H_
#define  __USART_BOOK_H_

#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
 //串口的宏定义  不同的串口挂在的总线和IO不一样
 
 //串口1
#define  _DEBUG_USARTx                  USART1
#define  _DEBUG_USART_CLK               RCC_APB2Periph_USART1
#define  _DEBUG_USART_APBxClkCmd        RCC_APB2PeriphClockCmd
#define  _DEBUG_USART_BAUDRATE          115200
 
// USART  GPIO 引脚定义
#define  _DEBUG_USART_GPIO_CLK          RCC_APB2Periph_GPIOA
#define  _DEBUG_USART_GPIO_APBxCLKCmd   RCC_APB2PeriphClockCmd

#define  _DEBUG_USART_TX_GPIO_PORT      GPIOA
#define  _DEBUG_USART_TX_GPIO_PIN       GPIO_Pin_9
#define  _DEBUG_USART_TX_GPIO_MODE      GPIO_Mode_AF_PP
#define  _DEBUG_USART_RX_GPIO_PORT      GPIOA
#define  _DEBUG_USART_RX_GPIO_PIN       GPIO_Pin_10
#define  _DEBUG_USART_RX_GPIO_MODE      GPIO_Mode_IN_FLOATING

#define  _DEBUG_NVIC_USART_IRQ          USART1_IRQn
#define  _DRBUG_USART_IRQHandler        USART1_IRQHandler
 
void  fn_USART_IO_Config(void); 
void  fn_USART_Config(void); 
void  fn_USART_Init(void);

void  fn_Usart_Send_Byte(USART_TypeDef * pUSARTx , uint8_t ch );
void  fn_Usart_SendString(USART_TypeDef *pUSARTx , char * str);
void  Usart_SendHalf_32_Word( USART_TypeDef * pUSARTx, uint32_t ch);


int fputc (int ch , FILE *f);
int fgetc(FILE *f);
void  _DRBUG_USART_IRQHandler(void);

#endif

