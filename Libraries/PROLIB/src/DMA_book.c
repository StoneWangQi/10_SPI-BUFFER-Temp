#include "DMA_book.h"
#include "USART_book.h"
#include "Systick_book.h"

const uint32_t  aSRC_Cont_Buffer  [Map_BUFFER_SIZE]={
   'W','E','L','L',
   'C','O','M','E',
   ' ','S','T','M',
   '3','2',' ','S',
   'T','U','D','Y',
   
};
uint32_t    aDST_Buffer[Map_BUFFER_SIZE] ;
uint32_t*   USART_DMA_Buffer ;
uint32_t  USART_BUFFER_SIZE ;

void _DMA_Config(DMA_Channel_TypeDef* _DMAy_Channelx , uint32_t _BUFFER_SIZE , uint32_t  _DMA_Source_ADDR,  uint32_t  _DMA_AIM_ADDR , uint32_t  _DMA_DIR){
  DMA_InitTypeDef   DMA_InitStructure ;
  //开启DMA时钟
  RCC_AHBPeriphClockCmd(DMA_CLOCK,ENABLE);
  //源数据缓存地址(外设地址)
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)_DMA_Source_ADDR ; 
  //转换缓存地址地址(内存地址)
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)_DMA_AIM_ADDR;
  
  //方向：外设到存储器(这里的外设是内部的FLASH)DMA_DIR_PeripheralSRC:为方向外设到内存 DMA_DIR_PeripheralDST:为方向外设到内存
  DMA_InitStructure.DMA_DIR = _DMA_DIR ;
  //传输大小
  DMA_InitStructure.DMA_BufferSize = _BUFFER_SIZE;
  //外设(内部的FLASH)地址递增
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  //内存地址递增
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  //外设数据单位
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  //内存数据单位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  //DMA模式，一次或者循环模式
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  //优先级：高
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  //使能内存到内存的传输
  DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
  //配置DMA通道
  DMA_Init(_DMAy_Channelx , &DMA_InitStructure);
  //使能DMA
  DMA_Cmd(_DMAy_Channelx , ENABLE);
}

void _USART_DMA_Config(DMA_Channel_TypeDef* _DMAy_Channelx , uint32_t _BUFFER_SIZE , uint32_t  _DMA_Source_ADDR,  uint32_t  _DMA_AIM_ADDR , uint32_t  _DMA_DIR){
  DMA_InitTypeDef   DMA_InitStructure ;
  //开启DMA时钟
  RCC_AHBPeriphClockCmd(DMA_CLOCK,ENABLE);  
  //源数据缓存地址(外设地址)
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)_DMA_Source_ADDR ; 
  //转换缓存地址地址(内存地址)
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)_DMA_AIM_ADDR;
  
  //方向：外设到存储器(这里的外设是内部的FLASH)DMA_DIR_PeripheralSRC:为方向外设到内存 DMA_DIR_PeripheralDST:为方向外设到内存
  DMA_InitStructure.DMA_DIR = _DMA_DIR ;
  //传输大小
  DMA_InitStructure.DMA_BufferSize = _BUFFER_SIZE;
  //外设(内部的FLASH)地址递增
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  //内存地址递增
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  //外设数据单位
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//DMA_PeripheralDataSize_Byte; //注意这里需要根据数据类型经行修改
  //内存数据单位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//DMA_MemoryDataSize_Byte; //注意这里需要根据数据类型经行修改
  //DMA模式，一次或者循环模式
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  //优先级：高
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  //使能内存到内存的传输
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  //配置DMA通道
  DMA_Init(_DMAy_Channelx , &DMA_InitStructure);
  //使能DMA
  DMA_Cmd(_DMAy_Channelx , ENABLE);
}
 
///源数据与目标地址数据对比
uint8_t  _Buffercmp(const uint32_t *pBuffer, uint32_t * pBuffer1 , uint16_t BufferLength){
  /*数据长度递减*/
  while(BufferLength--){
//  Usart_SendHalf_32_Word(_DEBUG_USARTx,*pBuffer);  
//  Usart_SendHalf_32_Word(_DEBUG_USARTx,*pBuffer1);
    /*判断两个数据源是否相等*/
    if(*pBuffer != *pBuffer1){
      /* 对应数据源不相等马上退出函数，并返回 0 */
      return 0;
    }
    /* 递增两个数据源的地址指针 */
    pBuffer++;
    pBuffer1++;
  }
  /* 完成判断并且对应数据相对 */
  return 1;
}


//对RAM数据进行展示
void  _Buffer_Show(uint32_t * pBuffer , uint16_t BufferLength){
  /*数据长度递减*/
  while(BufferLength--){
    Usart_SendHalf_32_Word(_DEBUG_USARTx,*pBuffer++);  
  }
}

//DMA对内存ROM数据的取出
void _DMA_ROM_TO_RAM(uint32_t _BUFFER_SIZE , uint32_t  _DMA_Source_ADDR,  uint32_t  _DMA_AIM_ADDR ){
  //----------------------------------------------------------------
  printf("开始 ROM内存到RAM内存的DMA操作 \n");
  //内存到内存DMA初始化
  _DMA_Config(Map_DMA_CHANNEL ,_BUFFER_SIZE ,_DMA_Source_ADDR , _DMA_AIM_ADDR , DMA_DIR_PeripheralSRC);       
  while(DMA_GetFlagStatus(DMA_FLAG_TC) == RESET);  //判断DMA传输结果是否正确          
  if(_DMA_InnerChange_== 0 ){
      printf("ROM内存到DMA操作异常 \n");  
  }else{
      printf("ROM内存到DMA操作正常 \n");      
  }  
  _RMA_InnerShow_;
}

//DMA对RAM到USART数据的取出
void _DMA_RAM_TO_USART(uint32_t _BUFFER_SIZE , uint32_t  _DMA_Source_ADDR,  uint32_t  _DMA_AIM_ADDR ){
  // 开始 USART内存到RAM内存的DMA操作 
  printf("\n开始 ROM到USART的传送初始化\n"); 
  USART_BUFFER_SIZE = _BUFFER_SIZE;
  USART_DMA_Buffer = _DMA_AIM_ADDR;
  //内存到USART DMA初始化
  _USART_DMA_Config(USART_DMA_CHANNEL ,USART_BUFFER_SIZE ,_DMA_Source_ADDR , USART_DMA_Buffer , DMA_DIR_PeripheralDST);
  USART_DMACmd(_DEBUG_USARTx , USART_DMAReq_Tx , ENABLE);  //串口DMA使能
  /*USART_DMACmd 函数用于控制 USART 的 DMA 请求的启动和关闭。它接收三个参
  数，第一个参数用于设置串口外设，可以是 USART1/2/3 和 UART4/5 这 5 个参数可选，第
  二个参数设置串口的具体 DMA 请求，有串口发送请求 USART_DMAReq_Tx 和接收请求
  USART_DMAReq_Rx 可选，第三个参数用于设置启动请求 ENABLE 或者关闭请求*/
  
  fn_Systick_Delay(250,_Systick_ms);   //DMA 传输进程中进行LED输出闪烁  
  while(USART_GetFlagStatus(_DEBUG_USARTx,USART_FLAG_TXE)==RESET);
  printf("\rROM内存到USART外设的DMA操作完毕\n");//这个函数需要Delay 一段时间才可以用
}
 
//----------------------------------------------------------------

//----------------------------------------------------------------   
