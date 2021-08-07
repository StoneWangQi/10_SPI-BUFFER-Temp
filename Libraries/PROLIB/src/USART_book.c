#include "USART_book.h"


/**************************************************************
* @brief  
* void fn_LED_Corporate(GPIO_TypeDef*  _GPIO_x , uint16_t  _GPIO_Pin_x , 
*            LED_Corporate_state_t  _LED_Corporate_state_t );
* @param  
* //串口1
*    #define  _DEBUG_NVIC_USART_IRQ               USART1_IRQn
*    #define  _DRBUG_NVIC_USART_IRQHandler        USART1_IRQHandler
* @retval 
*************************************************************/ 
static void NVIC_Configuration(void){
  NVIC_InitTypeDef  NVIC_InitStructure;
  /* 嵌套向量中断控制寄存器组选择*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置 USART 为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = _DEBUG_NVIC_USART_IRQ;
  /* 抢断优先级为 1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级为 1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置 NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************
* @brief  
* void fn_LED_Corporate(GPIO_TypeDef*  _GPIO_x , uint16_t  _GPIO_Pin_x , 
*            LED_Corporate_state_t  _LED_Corporate_state_t );
* @param  
* //串口1    
*    // USART  GPIO 引脚定义
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
  // 打开串口 GPIO 的时钟
  _DEBUG_USART_GPIO_APBxCLKCmd(_DEBUG_USART_GPIO_CLK , ENABLE);
  
//将USART TX 的GPIO配置为推挽模式
  GPIO_InitStructure.GPIO_Pin = _DEBUG_USART_TX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = _DEBUG_USART_TX_GPIO_MODE;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(_DEBUG_USART_TX_GPIO_PORT,&GPIO_InitStructure);
   //将USART RX 的GPIO配置为浮空输入
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
* //串口1
*    #define  _DEBUG_USARTx                  USART1
*    #define  _DEBUG_USART_CLK               RCC_APB2Periph_USART1
*    #define  _DEBUG_USART_APBxClkCmd        RCC_APB2PeriphClockCmd
*    #define  _DEBUG_USART_BAUDRATE          115200
* @retval 
*************************************************************/ 
void  fn_USART_Config(void){
  USART_InitTypeDef   USART_InitStructure;
 
  // 打开串口外设的时钟
  _DEBUG_USART_APBxClkCmd(_DEBUG_USART_CLK , ENABLE);
  
  //配置串口的工作参数
  USART_InitStructure.USART_BaudRate = _DEBUG_USART_BAUDRATE;
   //配置波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  // 配置 针数据字长
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  // 配置停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;
  // 配置校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  // 配置硬件流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx ;
  // 配置工作模式，收发一起
  
  USART_Init(_DEBUG_USARTx , &USART_InitStructure);// 完成串口的初始化配置
  
  NVIC_Configuration();// 串口中断优先级配置
  
  USART_ITConfig(_DEBUG_USARTx , USART_IT_RXNE , ENABLE);// 使能串口接收中断
  
  USART_Cmd(_DEBUG_USARTx , ENABLE);// 使能串口
}

/**************************************************************
* @brief  
* void fn_Usart_Send_Byte(USART_TypeDef * pUSARTx , uint8_t ch );
* @param  
* //串口1
*    #define  _DEBUG_USARTx                  USART1
*    #define  _DEBUG_USART_CLK               RCC_APB2Periph_USART1
*    #define  _DEBUG_USART_APBxClkCmd        RCC_APB2PeriphClockCmd
*    #define  _DEBUG_USART_BAUDRATE          115200
* @retval 
*************************************************************/ 
void fn_Usart_Send_Byte(USART_TypeDef * pUSARTx , uint8_t ch ){
  /*发送一个字节数据到USART*/
  USART_SendData(pUSARTx , ch);
  /*等待发送数据寄存器为空*/
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

/**************************************************************
* @brief  
* void fn_Usart_SendString(USART_TypeDef *pUSARTx , char * str);
* @param  
* //串口1
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
  
  /*等待发送完成*/
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
  /*等待发送完成*/
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

//须在 MDK 的工程选项把“Use MicroLIB”勾选上，MicoroLIB 是缺省 C 库的备选库，它对
//标准 C 库进行了高度优化使代码更少，占用更少资源。
/**************************************************************
* @brief  
* int fputc (int ch , FILE *f)
* @param  重新定向C库函数Printf 到USART1
* @retval 
*************************************************************/ 
int fputc (int ch , FILE *f){ 
  /*发送一个字节数据到USART*/
  USART_SendData(_DEBUG_USARTx , (uint8_t)ch);
  /*等待发送数据寄存器为空*/
  while(USART_GetFlagStatus(_DEBUG_USARTx,USART_FLAG_TXE)==RESET);
  return (ch);
}

/**************************************************************
* @brief  
* int fgetc(FILE *f);
* @param  重新定向C库函数Printf 到USART1
* @retval 
*************************************************************/  
int fgetc(FILE *f){
  //等待串口1输入数据
  while(USART_GetITStatus(_DEBUG_USARTx,USART_IT_RXNE)==RESET);
  return (int)USART_ReceiveData(_DEBUG_USARTx);
}

/**************************************************************
* @brief  
* void USART1_IRQHandler(void);  中断服务
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




