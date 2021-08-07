#include "USART_book.h"


/**************************************************************
* @brief  
* void fn_LED_Corporate(GPIO_TypeDef*  _GPIO_x , uint16_t  _GPIO_Pin_x , 
*            LED_Corporate_state_t  _LED_Corporate_state_t );
* @param  
* //����1
*    #define  _DEBUG_NVIC_USART_IRQ               USART1_IRQn
*    #define  _DRBUG_NVIC_USART_IRQHandler        USART1_IRQHandler
* @retval 
*************************************************************/ 
static void NVIC_Configuration(void){
  NVIC_InitTypeDef  NVIC_InitStructure;
  /* Ƕ�������жϿ��ƼĴ�����ѡ��*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ���� USART Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = _DEBUG_NVIC_USART_IRQ;
  /* �������ȼ�Ϊ 1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ�Ϊ 1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������ NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************
* @brief  
* void fn_LED_Corporate(GPIO_TypeDef*  _GPIO_x , uint16_t  _GPIO_Pin_x , 
*            LED_Corporate_state_t  _LED_Corporate_state_t );
* @param  
* //����1    
*    // USART  GPIO ���Ŷ���
*    #define  _DEBUG_USART_GPIO_CLK          RCC_APB2Periph_GPIOA
*    #define  _DEBUG_USART_GPIO_APBxCLKCmd   RCC_APB2PeriphClockCmd
*    
*    #define  _DEBUG_USART_TX_GPIO_PORT      GPIOA
*    #define  _DEBUG_USART_TX_GPIO_PIN       GPIO_Pin_9
*    #define  _DEBUG_USART_TX_GPIO_MODE      GPIO_Mode_AF_PP
*    #define  _DEBUG_USART_RX_GPIO_PORT      GPIOA
*    #define  _DEBUG_USART_RX_GPIO_PIN       GPIO_Pin_10
*    #define  _DEBUG_USART_RX_GPIO_MODE      GPIO_Mode_AF_FLOATING
* @retval 
*************************************************************/ 
void  fn_USART_IO_Config(void){
  GPIO_InitTypeDef    GPIO_InitStructure;
  // �򿪴��� GPIO ��ʱ��
  _DEBUG_USART_GPIO_APBxCLKCmd(_DEBUG_USART_GPIO_CLK , ENABLE);
  
//��USART TX ��GPIO����Ϊ����ģʽ
  GPIO_InitStructure.GPIO_Pin = _DEBUG_USART_TX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = _DEBUG_USART_TX_GPIO_MODE;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(_DEBUG_USART_TX_GPIO_PORT,&GPIO_InitStructure);
   //��USART RX ��GPIO����Ϊ��������
  GPIO_InitStructure.GPIO_Pin = _DEBUG_USART_RX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = _DEBUG_USART_RX_GPIO_MODE;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(_DEBUG_USART_RX_GPIO_PORT,&GPIO_InitStructure);
}

/**************************************************************
* @brief  
* void fn_LED_Corporate(GPIO_TypeDef*  _GPIO_x , uint16_t  _GPIO_Pin_x , 
*            LED_Corporate_state_t  _LED_Corporate_state_t );
* @param  
* //����1
*    #define  _DEBUG_USARTx                  USART1
*    #define  _DEBUG_USART_CLK               RCC_APB2Periph_USART1
*    #define  _DEBUG_USART_APBxClkCmd        RCC_APB2PeriphClockCmd
*    #define  _DEBUG_USART_BAUDRATE          115200
* @retval 
*************************************************************/ 
void  fn_USART_Config(void){
  USART_InitTypeDef   USART_InitStructure;
 
  // �򿪴��������ʱ��
  _DEBUG_USART_APBxClkCmd(_DEBUG_USART_CLK , ENABLE);
  
  //���ô��ڵĹ�������
  USART_InitStructure.USART_BaudRate = _DEBUG_USART_BAUDRATE;
   //���ò�����
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  // ���� �������ֳ�
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  // ����ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;
  // ����У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  // ����Ӳ��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx ;
  // ���ù���ģʽ���շ�һ��
  
  USART_Init(_DEBUG_USARTx , &USART_InitStructure);// ��ɴ��ڵĳ�ʼ������
  
  NVIC_Configuration();// �����ж����ȼ�����
  
  USART_ITConfig(_DEBUG_USARTx , USART_IT_RXNE , ENABLE);// ʹ�ܴ��ڽ����ж�
  
  USART_Cmd(_DEBUG_USARTx , ENABLE);// ʹ�ܴ���
}

/**************************************************************
* @brief  
* void fn_Usart_Send_Byte(USART_TypeDef * pUSARTx , uint8_t ch );
* @param  
* //����1
*    #define  _DEBUG_USARTx                  USART1
*    #define  _DEBUG_USART_CLK               RCC_APB2Periph_USART1
*    #define  _DEBUG_USART_APBxClkCmd        RCC_APB2PeriphClockCmd
*    #define  _DEBUG_USART_BAUDRATE          115200
* @retval 
*************************************************************/ 
void fn_Usart_Send_Byte(USART_TypeDef * pUSARTx , uint8_t ch ){
  /*����һ���ֽ����ݵ�USART*/
  USART_SendData(pUSARTx , ch);
  /*�ȴ��������ݼĴ���Ϊ��*/
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

/**************************************************************
* @brief  
* void fn_Usart_SendString(USART_TypeDef *pUSARTx , char * str);
* @param  
* //����1
*    #define  _DEBUG_USARTx                  USART1
*    #define  _DEBUG_USART_CLK               RCC_APB2Periph_USART1
*    #define  _DEBUG_USART_APBxClkCmd        RCC_APB2PeriphClockCmd
*    #define  _DEBUG_USART_BAUDRATE          115200
* @retval 
*************************************************************/ 
void fn_Usart_SendString(USART_TypeDef *pUSARTx , char * str){
  unsigned int k = 0;
  do{
    fn_Usart_Send_Byte(pUSARTx,*(str + k++));
    
  }while(*(str + k)!='\0');
  
  /*�ȴ��������*/
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC));
}

/**************************************************************
* @brief  
* void Usart_SendHalf_32_Word( USART_TypeDef * pUSARTx, uint32_t ch);
* @param  
* @retval 
*************************************************************/ 
void Usart_SendHalf_32_Word( USART_TypeDef * pUSARTx, uint32_t ch){
  uint32_t temp_Half32;
  uint8_t temp_Half=0,i_Half=4; 
  temp_Half32 =ch;
  while(i_Half-->0){
     temp_Half=(temp_Half32 & 0xFF000000)>>24;
     temp_Half32<<=8;
     fn_Usart_Send_Byte(pUSARTx,temp_Half);
  }
  /*�ȴ��������*/
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC));
}

/**************************************************************
* @brief  
* void fn_USART_Init(void);
* @param  
* @retval 
*************************************************************/ 
void fn_USART_Init(void){
  fn_USART_IO_Config();
  fn_USART_Config();
}

//���� MDK �Ĺ���ѡ��ѡ�Use MicroLIB����ѡ�ϣ�MicoroLIB ��ȱʡ C ��ı�ѡ�⣬����
//��׼ C ������˸߶��Ż�ʹ������٣�ռ�ø�����Դ��
/**************************************************************
* @brief  
* int fputc (int ch , FILE *f)
* @param  ���¶���C�⺯��Printf ��USART1
* @retval 
*************************************************************/ 
int fputc (int ch , FILE *f){ 
  /*����һ���ֽ����ݵ�USART*/
  USART_SendData(_DEBUG_USARTx , (uint8_t)ch);
  /*�ȴ��������ݼĴ���Ϊ��*/
  while(USART_GetFlagStatus(_DEBUG_USARTx,USART_FLAG_TXE)==RESET);
  return (ch);
}

/**************************************************************
* @brief  
* int fgetc(FILE *f);
* @param  ���¶���C�⺯��Printf ��USART1
* @retval 
*************************************************************/  
int fgetc(FILE *f){
  //�ȴ�����1��������
  while(USART_GetITStatus(_DEBUG_USARTx,USART_IT_RXNE)==RESET);
  return (int)USART_ReceiveData(_DEBUG_USARTx);
}

/**************************************************************
* @brief  
* void USART1_IRQHandler(void);  �жϷ���
* @param  
* @retval 
*************************************************************/ 
void _DRBUG_USART_IRQHandler(void){
  uint8_t ucTemp =  0; 
  if(USART_GetITStatus(_DEBUG_USARTx,USART_IT_RXNE)!=RESET){
    ucTemp = USART_ReceiveData(_DEBUG_USARTx);
    USART_SendData(_DEBUG_USARTx ,ucTemp );
  }
}




