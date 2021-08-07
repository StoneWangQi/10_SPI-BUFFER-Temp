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
  //ע�⣺���ǳ��˿� GPIO �Ķ˿�ʱ���⣬���ǻ����� AFIO ��ʱ��
  GPIO_EXTILineConfig(_EXTI_IN_GPIO_PortSource | _EXTI_IN2_GPIO_PortSource , _EXTI_IN_GPIO_PinSource | _EXTI_IN2_GPIO_PinSource);
  /* ѡ�� EXTI ���ź�Դ */
  // GPIO_EXTILineConfig ��������ָ���ж�/�¼��ߵ�����Դ����ʵ�����趨�ⲿ�ж���
  // �üĴ����� AFIO_EXTICRx ֵ���ú�������������������һ������ָ�� GPIO �˿�Դ����
  // ��������Ϊѡ���Ӧ GPIO ����Դ��š�
  
  
  EXIT_InitStruck.EXTI_Line = _EXTI_IN_EXTI_Line ; /* ѡ�� EXTI ���ź�Դ */
  EXIT_InitStruck.EXTI_Mode = _EXTI_IN_EXTI_Mode;   /* EXTI Ϊ�ж�ģʽ */
  EXIT_InitStruck.EXTI_Trigger = _EXTI_IN_EXTI_Trigger ; /* �������ж� */
  EXIT_InitStruck.EXTI_LineCmd = _EXTI_IN_EXTI_LineCmd; /* ʹ���ж� */
  EXTI_Init(&EXIT_InitStruck);
  //  EXTI��ʼ�����õı���
  //  fn_NVIC_Config();
  //  ���� NVIC_Configuration������ɶ԰��� 1������ 2 ���ȼ����ò�ʹ���ж�ͨ��
  
  EXIT_InitStruck.EXTI_Line = _EXTI_IN2_EXTI_Line; /* ѡ�� EXTI ���ź�Դ */
  EXIT_InitStruck.EXTI_Mode = _EXTI_IN2_EXTI_Mode;   /* EXTI Ϊ�ж�ģʽ */
  EXIT_InitStruck.EXTI_Trigger = _EXTI_IN2_EXTI_Trigger; /* �½����ж� */
  EXIT_InitStruck.EXTI_LineCmd = _EXTI_IN_EXTI_LineCmd;/* ʹ���ж� */
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
  /* ���� NVIC Ϊ���ȼ��� 1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ��  */
  NVIC_InitStruct.NVIC_IRQChannel = _NVIC_IN_EXTI_IRQChannel; //EXTI0_IRQn;
  /* ������ռ���ȼ���1 */
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  /* ���������ȼ���1 */
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStruct.NVIC_IRQChannelCmd = _NVIC_IN_EXTI_IRQChannelCmd; //ENABLE
  NVIC_Init(&NVIC_InitStruct);
  
  /* �����ж�Դ��  */
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
//  EXTI_GetITStatus ����������ȡ EXTI ���жϱ�־λ״̬����� EXTI �����жϷ�����
//�����ء�SET�����򷵻ء�RESET����ʵ���ϣ�EXTI_GetITStatus ������ͨ����ȡ
//EXTI_PR�Ĵ���ֵ���ж� EXTI��״̬�ġ�
  if(EXTI_GetITStatus(_EXTI_IN_EXTI_Line)!= RESET){
    if(GPIO_ReadInputDataBit(_KEY_EXTI_IN_GPIO_Port, _KEY_EXTI_IN_GPIO_Pin)==1){
      __LED_Change__;
    }
  }
  EXTI_ClearITPendingBit(_EXTI_IN_EXTI_Line);  // ��Ҫ������жϱ�־λ 
}


void EXTI15_10_IRQHandler(void){
 if(EXTI_GetITStatus(_EXTI_IN2_EXTI_Line)!= RESET){
    if(GPIO_ReadInputDataBit(_KEY2_EXTI_IN_GPIO_Port, _KEY2_EXTI_IN_GPIO_Pin)==0){
      __LED_Change__;
    }
  }
  EXTI_ClearITPendingBit(_EXTI_IN2_EXTI_Line);  // ��Ҫ������жϱ�־λ 
}
 

