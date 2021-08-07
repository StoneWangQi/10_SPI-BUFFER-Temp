#ifndef  __EXIT_BOOK_H_
#define  __EXIT_BOOK_H_

#include "stm32f10x.h"

#define  _KEY_EXTI_IN_GPIO_Port      GPIOA
#define  _KEY_EXTI_IN_GPIO_Pin       GPIO_Pin_0
#define  _EXTI_IN_GPIO_PortSource     GPIO_PortSourceGPIOA
#define  _EXTI_IN_GPIO_PinSource      GPIO_PinSource0
#define  _EXTI_IN_EXTI_Line           EXTI_Line0 
#define  _EXTI_IN_EXTI_Trigger        EXTI_Trigger_Rising
#define  _EXTI_IN_GPIO_Clock          RCC_APB2Periph_AFIO
#define  _EXTI_IN_EXTI_Mode           EXTI_Mode_Interrupt
#define  _EXTI_IN_EXTI_LineCmd        ENABLE

#define  _NVIC_IN_EXTI_IRQChannel     EXTI0_IRQn
#define  _NVIC_IN_EXTI_IRQChannelCmd  ENABLE


#define  _KEY2_EXTI_IN_GPIO_Port     GPIOC
#define  _KEY2_EXTI_IN_GPIO_Pin      GPIO_Pin_13
#define  _EXTI_IN2_GPIO_PortSource    GPIO_PortSourceGPIOC
#define  _EXTI_IN2_GPIO_PinSource     GPIO_PinSource13
#define  _EXTI_IN2_EXTI_Line          EXTI_Line13
#define  _EXTI_IN2_EXTI_Trigger       EXTI_Trigger_Falling
#define  _EXTI_IN2_GPIO_Clock         RCC_APB2Periph_AFIO
#define  _EXTI_IN2_EXTI_Mode          EXTI_Mode_Interrupt
#define  _EXTI_IN2_EXTI_LineCmd       ENABLE

#define  _NVIC_IN2_EXTI_IRQChannel    EXTI15_10_IRQn
#define  _NVIC_IN2_EXTI_IRQChannelCmd  ENABLE



void  fn_EXTI_GPIO_Config(void);
void  fn_NVIC_Config(void);
void  EXTI0_IRQHandler(void);

#endif
