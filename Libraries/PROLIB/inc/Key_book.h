#ifndef  __KEY_BOOK_H_
#define  __KEY_BOOK_H_


#include "stm32f10x.h"
#include "Led_book.h"

#define   KEY_IN_GPIO_Port      GPIOA
#define   KEY_IN_GPIO_Clock     RCC_APB2Periph_GPIOA
#define   KEY_IN_GPIO_Pin       GPIO_Pin_0
#define   KEY_IN_GPIO_Pin_Bit   0
#define   Key_IN_GPIO_Modle     GPIO_Mode_IN_FLOATING   //∏°ø’ ‰»Î


#define   KEY2_IN_GPIO_Port      GPIOC
#define   KEY2_IN_GPIO_Clock     RCC_APB2Periph_GPIOC
#define   KEY2_IN_GPIO_Pin       GPIO_Pin_13
#define   KEY2_IN_GPIO_Pin_Bit   13
#define   Key2_IN_GPIO_Modle     GPIO_Mode_IN_FLOATING   //∏°ø’ ‰»Î


typedef union {
  struct{
    unsigned char BIT0:1;unsigned char BIT1:1;unsigned char BIT2:1;unsigned char BIT3:1;
    unsigned char BIT4:1;unsigned char BIT5:1;unsigned char BIT6:1;unsigned char BIT7:1;
    //unsigned char BIT8:1;unsigned char BIT9:1;unsigned char BIT10:1;unsigned char BIT11:1;
    //unsigned char BIT12:1;unsigned char BIT13:1;unsigned char BIT14:1;unsigned char BIT15:1;
  }DATA_BIT;
  uint8_t DATA_BYTE;
}Per_key_type;

extern volatile  Per_key_type key_flag;
  #define bkey_10ms         key_flag.DATA_BIT.BIT0
  #define bkey_judge        key_flag.DATA_BIT.BIT1
  #define bkey_judge_long   key_flag.DATA_BIT.BIT2
  #define bkey_Effect       key_flag.DATA_BIT.BIT3
  #define bkey_LongEffect   key_flag.DATA_BIT.BIT4
  #define bkey_Effect_Lose  key_flag.DATA_BIT.BIT5
  #define bkey_Effect_LLose key_flag.DATA_BIT.BIT6
void  fn_Key_GPIO_Config( GPIO_TypeDef* _GPIO_x , uint32_t _GPIO_Clock , uint16_t _GPIO_Pin_x , GPIOMode_TypeDef  _GPIOMode_TypeDef );
void  fn_Key_Init(void);
void  fn_key_judge(void);
void  fn_key_Effect(void);  
void  fn_key_Check(void);
#endif


