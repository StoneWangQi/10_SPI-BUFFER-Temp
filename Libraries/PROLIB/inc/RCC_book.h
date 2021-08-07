#ifndef  __RCC_BOOK_H_
#define  __RCC_BOOK_H_

#include "stm32f10x.h"
 
#define   RCC_OUT_GPIO_Port     GPIOA                 //GPIO Point
#define   RCC_OUT_GPIO_Clock    RCC_APB2Periph_GPIOA  //GPIO clock
#define   RCC_OUT_GPIO_Pin      GPIO_Pin_8             
#define   RCC_OUT_GPIO_Pin_Bit  8
#define   RCC_OUT_GPIO_Modle    GPIO_Mode_AF_PP
#define   RCC_OUT_GPIO_Speed    GPIO_Speed_10MHz

void  fn_HRCC_SetSystic( uint32_t  _RCC_PLLMul_x );
void  fn_IRCC_SetSystic( uint32_t  _RCC_PLLMul_x );
void  fn_MCO_GPIO_Config(void);
void  fn_RCC_Init(void);
#endif

