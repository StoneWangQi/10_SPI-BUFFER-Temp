#include "Exit_book.h"
#include "Led_book.h"

/**************************************************************
* @brief  
* void  fn_EXTI_GPIO_Config(void)
* @param  
*    
*   #define  _KEY_EXTI_IN_GPIO_Port      GPIOA
*   #define  _KEY_EXTI_IN_GPIO_Pin       GPIO_Pin_0
*   #define  _EXTI_IN_GPIO_PortSource     GPIO_PortSourceGPIOA
*   #define  _EXTI_IN_GPIO_PinSource      GPIO_PinSource0
*   #define  _EXTI_IN_EXTI_Line           EXTI_Line0 
*   #define  _EXTI_IN_EXTI_Trigger        EXTI_Trigger_Rising
*   #define  _EXTI_IN_GPIO_Clock          RCC_APB2Periph_AFIO
*   #define  _EXTI_IN_EXTI_Mode           EXTI_Mode_Interrupt
*   #define  _EXTI_IN_EXTI_LineCmd        ENABLE
*   
*   #define  _KEY2_EXTI_IN_GPIO_Port     GPIOC
*   #define  _KEY2_EXTI_IN_GPIO_Pin      GPIO_Pin_13
*   #define  _EXTI_IN2_GPIO_PortSource    GPIO_PortSourceGPIOC
*   #define  _EXTI_IN2_GPIO_PinSource     GPIO_PinSource13
*   #define  _EXTI_IN2_EXTI_Line          EXTI_Line13
*   #define  _EXTI_IN2_EXTI_Trigger       EXTI_Trigger_Falling   
*   #define  _EXTI_IN2_GPIO_Clock         RCC_APB2Periph_AFIO
*   #define  _EXTI_IN2_EXTI_Mode          EXTI_Mode_Interrupt
*   #define  _EXTI_IN2_EXTI_LineCmd       ENABLE
* @retval 
*************************************************************/ 
void  fn_EXTI_GPIO_Config(void){
  EXTI_InitTypeDef   EXIT_InitStruck;
  RCC_APB2PeriphClockCmd(_EXTI_IN_GPIO_Clock , ENABLE);  
  //注意：我们除了开 GPIO 的端口时钟外，我们还打开了 AFIO 的时钟
  GPIO_EXTILineConfig(_EXTI_IN_GPIO_PortSource | _EXTI_IN2_GPIO_PortSource , _EXTI_IN_GPIO_PinSource | _EXTI_IN2_GPIO_PinSource);
  /* 选择 EXTI 的信号源 */
  // GPIO_EXTILineConfig 函数用来指定中断/事件线的输入源，它实际是设定外部中断配
  // 置寄存器的 AFIO_EXTICRx 值，该函数接收两个参数，第一个参数指定 GPIO 端口源，第
  // 二个参数为选择对应 GPIO 引脚源编号。
  
  
  EXIT_InitStruck.EXTI_Line = _EXTI_IN_EXTI_Line ; /* 选择 EXTI 的信号源 */
  EXIT_InitStruck.EXTI_Mode = _EXTI_IN_EXTI_Mode;   /* EXTI 为中断模式 */
  EXIT_InitStruck.EXTI_Trigger = _EXTI_IN_EXTI_Trigger ; /* 上升沿中断 */
  EXIT_InitStruck.EXTI_LineCmd = _EXTI_IN_EXTI_LineCmd; /* 使能中断 */
  EXTI_Init(&EXIT_InitStruck);
  //  EXTI初始化配置的变量
  //  fn_NVIC_Config();
  //  调用 NVIC_Configuration函数完成对按键 1、按键 2 优先级配置并使能中断通道
  
  EXIT_InitStruck.EXTI_Line = _EXTI_IN2_EXTI_Line; /* 选择 EXTI 的信号源 */
  EXIT_InitStruck.EXTI_Mode = _EXTI_IN2_EXTI_Mode;   /* EXTI 为中断模式 */
  EXIT_InitStruck.EXTI_Trigger = _EXTI_IN2_EXTI_Trigger; /* 下降沿中断 */
  EXIT_InitStruck.EXTI_LineCmd = _EXTI_IN_EXTI_LineCmd;/* 使能中断 */
  EXTI_Init(&EXIT_InitStruck);
  
  fn_NVIC_Config();
}

/**************************************************************
* @brief  
* void  fn_NVIC_Config(void)
* @param  
*   #define  _NVIC_IN_EXTI_IRQChannel     EXTI0_IRQn
*   #define  _NVIC_IN_EXTI_IRQChannelCmd  ENABLE
*   #define  _NVIC_IN2_EXTI_IRQChannel    EXTI15_10_IRQn
*   #define  _NVIC_IN2_EXTI_IRQChannelCmd  ENABLE
* @retval 
*************************************************************/ 
void  fn_NVIC_Config(void){
  NVIC_InitTypeDef NVIC_InitStruct;
  /* 配置 NVIC 为优先级组 1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源：  */
  NVIC_InitStruct.NVIC_IRQChannel = _NVIC_IN_EXTI_IRQChannel; //EXTI0_IRQn;
  /* 配置抢占优先级：1 */
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  /* 配置子优先级：1 */
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
  NVIC_InitStruct.NVIC_IRQChannelCmd = _NVIC_IN_EXTI_IRQChannelCmd; //ENABLE
  NVIC_Init(&NVIC_InitStruct);
  
  /* 配置中断源：  */
  NVIC_InitStruct.NVIC_IRQChannel = _NVIC_IN2_EXTI_IRQChannel; //EXTI0_IRQn;
  NVIC_Init(&NVIC_InitStruct);
}

/**************************************************************
* @brief  
* void  fn_NVIC_Config(void)
* @param  
*   #define   _KEY_EXTI_IN_GPIO_Port      GPIOA
*   #define   _KEY_EXTI_IN_GPIO_Pin       GPIO_Pin_0
* @retval 
*************************************************************/ 
void EXTI0_IRQHandler(void){
//  EXTI_GetITStatus 函数用来获取 EXTI 的中断标志位状态，如果 EXTI 线有中断发生函
//数返回“SET”否则返回“RESET”。实际上，EXTI_GetITStatus 函数是通过读取
//EXTI_PR寄存器值来判断 EXTI线状态的。
  if(EXTI_GetITStatus(_EXTI_IN_EXTI_Line)!= RESET){
    if(GPIO_ReadInputDataBit(_KEY_EXTI_IN_GPIO_Port, _KEY_EXTI_IN_GPIO_Pin)==1){
      __LED_Change__;
    }
  }
  EXTI_ClearITPendingBit(_EXTI_IN_EXTI_Line);  // 重要的清除中断标志位 
}


void EXTI15_10_IRQHandler(void){
 if(EXTI_GetITStatus(_EXTI_IN2_EXTI_Line)!= RESET){
    if(GPIO_ReadInputDataBit(_KEY2_EXTI_IN_GPIO_Port, _KEY2_EXTI_IN_GPIO_Pin)==0){
      __LED_Change__;
    }
  }
  EXTI_ClearITPendingBit(_EXTI_IN2_EXTI_Line);  // 重要的清除中断标志位 
}
 

