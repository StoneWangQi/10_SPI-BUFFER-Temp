#ifndef  __SYSTIC_BOOK_H_
#define  __SYSTIC_BOOK_H_

#include "stm32f10x.h"
#include  "Key_book.h"
 
typedef enum {
		_Systick_us = 1,
		_Systick_ms = 2,
		_Systick_s = 3, 
} Systick_time_state_t;

void fn_Systick_Delay(uint32_t  _Delay_time , Systick_time_state_t  _Systick_time_state_t);
void fn_Systick_Delay_Handler_set(uint32_t  _Delay_ms , Systick_time_state_t  _Systick_time_state_t);
void fn_SysTick_delay_decrement(void);
void SysTick_Handler(void);


#define  __Systick_Delay_Handler_set__      fn_Systick_Delay_Handler_set(10,_Systick_ms)
#endif

