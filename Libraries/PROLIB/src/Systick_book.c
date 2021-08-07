#include "Systick_book.h"

/************************************************************
* @brief  
* void fn_Systick_Delay(uint32_t  _Delay_time , \
Systick_time_state_t  _Systick_time_state_t){
* @param  
* @retval 
*************************************************************/ 
void fn_Systick_Delay(uint32_t  _Delay_time , Systick_time_state_t  _Systick_time_state_t){
   uint32_t  i;
   if(_Systick_time_state_t == _Systick_us){SysTick_Config(SystemCoreClock/1000000);}
   if(_Systick_time_state_t == _Systick_ms){
    SysTick_Config(SystemCoreClock/1000);
   }  
   else{SysTick_Config(SystemCoreClock);}      
   for( i=0;i<_Delay_time ; i++){
    while(!((SysTick->CTRL)&(1<<16)));
   }
   SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

}

/************************************************************
* @brief  
* void fn_Systick_Delay_Handler_set(uint32_t  _Delay_ms , \
*       Systick_time_state_t  _Systick_time_state_t){
* @param  
* @retval 
*************************************************************/ 
static uint32_t _SysTick_delay  = 0 ;
void fn_Systick_Delay_Handler_set(uint32_t  _Delay_ms , Systick_time_state_t  _Systick_time_state_t){
  if(_Systick_time_state_t == _Systick_us){SysTick_Config(SystemCoreClock/1000000);}
  if(_Systick_time_state_t == _Systick_ms){
      SysTick_Config(SystemCoreClock/1000);
  }  
  else{SysTick_Config(SystemCoreClock);}      
  _SysTick_delay = _Delay_ms ;
}

/************************************************************
* @brief  
* void fn_SysTick_delay_decrement(void)
* @param  
* @retval 
*************************************************************/ 
static uint32_t SysTick_delay = 0 ;
void fn_SysTick_delay_decrement(void){
  if(SysTick_delay++ > _SysTick_delay){
    SysTick_delay = 0;
    bkey_10ms = 1;
  }
}

/************************************************************
* @brief  
* void SysTick_Handler(void)
* @param  
* @retval 
*************************************************************/ 
void SysTick_Handler(void){
  fn_SysTick_delay_decrement();
}
