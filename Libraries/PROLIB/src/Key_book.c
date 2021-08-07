#include  "Key_book.h"
 

volatile  Per_key_type key_flag;

/**************************************************************
* @brief  
* void  fn_Key_GPIO_Config( GPIO_TypeDef* _GPIO_x , uint32_t _GPIO_Clock , 
*                  uint16_t _GPIO_Pin_x , GPIOMode_TypeDef  _GPIOMode_TypeDef );
* @param  
*     #define   KEY_IN_GPIO_Port      GPIOA
*     #define   KEY_IN_GPIO_Clock     RCC_APB2Periph_GPIOA
*     #define   KEY_IN_GPIO_Pin       GPIO_Pin_0
*     #define   KEY_IN_GPIO_Pin_Bit   0
*     #define   Key_IN_GPIO_Modle     GPIO_Mode_IN_FLOATING   //∏°ø’ ‰»Î
*     
*     #define   KEY2_IN_GPIO_Port      GPIOC
*     #define   KEY2_IN_GPIO_Clock     RCC_APB2Periph_GPIOC
*     #define   KEY2_IN_GPIO_Pin       GPIO_Pin_13
*     #define   KEY2_IN_GPIO_Pin_Bit   13
*     #define   Key2_IN_GPIO_Modle     GPIO_Mode_IN_FLOATING   //∏°ø’ ‰»Î
* @retval 
*************************************************************/ 
void  fn_Key_GPIO_Config( GPIO_TypeDef* _GPIO_x , uint32_t _GPIO_Clock , uint16_t _GPIO_Pin_x , GPIOMode_TypeDef  _GPIOMode_TypeDef ){
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = _GPIOMode_TypeDef;
    GPIO_InitStruct.GPIO_Pin = _GPIO_Pin_x;
    RCC_APB2PeriphClockCmd(_GPIO_Clock,ENABLE);
    GPIO_Init(_GPIO_x , &GPIO_InitStruct);  
}

/**************************************************************
* @brief  
* void fn_Key_Init(void);
* @param  
* @retval 
*************************************************************/ 
void  fn_Key_Init(void){
    fn_Key_GPIO_Config(KEY_IN_GPIO_Port,KEY_IN_GPIO_Clock,KEY_IN_GPIO_Pin,Key_IN_GPIO_Modle);
    fn_Key_GPIO_Config(KEY2_IN_GPIO_Port,KEY2_IN_GPIO_Clock,KEY2_IN_GPIO_Pin,Key2_IN_GPIO_Modle);
}

/************************************************************
* @brief  
* void  fn_key_judge(void);
* @param  
* @retval 
**************************************************************/ 
#define  _LONG_key  30
static uint16_t count_key ;
void  fn_key_judge(void){
   
   if(!bkey_10ms){return;}
   bkey_10ms = 0;
   if(GPIO_ReadInputDataBit(KEY_IN_GPIO_Port,KEY_IN_GPIO_Pin)){
     if(count_key++<3){return;}
     if(!bkey_judge){
       bkey_judge = 1;
       bkey_Effect = 1; 
     }else{
       if(count_key>_LONG_key){
          bkey_judge_long = 1;
          bkey_LongEffect = 1;
       }
     }
   }
   else{
     count_key = 0;
     if(bkey_judge){
        bkey_judge = 0;
        if(bkey_judge_long){
            bkey_judge_long = 0;
            bkey_Effect_LLose = 1;
        }else{
            bkey_judge_long = 0;
            bkey_Effect_Lose = 1;
        }
     }else{
        bkey_judge = 0;         
     }
  }
}

/************************************************************
* @brief  
* void fn_key_Effect(void);
* @param  
* @retval 
*************************************************************/ 
void  fn_key_Effect(void){
  if(bkey_Effect){
    bkey_Effect = 0;
    fn_LED_Corporate(LED_OUT_GPIO_Port,LED_OUT_GPIO_Pin,LED_Corporate_Toggle);
  }
}

/**************************************************************
* @brief  
* void fn_key_Check(void);
* @param  
* @retval 
*************************************************************/ 
void fn_key_Check(void){
  fn_key_judge();
  fn_key_Effect();
}

