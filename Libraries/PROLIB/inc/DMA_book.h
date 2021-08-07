#ifndef  __DMA_BOOK_H_
#define  __DMA_BOOK_H_

#include "stm32f10x.h"

#define   DMA_CLOCK     RCC_AHBPeriph_DMA1    //DMA  时钟

/******  A   ****************** ROM 到 RAM 的DMA输出 *******************************/
#define   Map_DMA_CHANNEL     DMA1_Channel6    // 当使用存储器到存储器模式时候，通道可以随便选，没有硬性的规定
#define   Map_BUFFER_SIZE     20             // 要发送的数据大小
#define   DMA_FLAG_TC   DMA1_FLAG_TC6         // 传输完成标志
/* 定义 aSRC_Const_Buffer 数组作为 DMA 传输数据源
* const 关键字将 aSRC_Const_Buffer 数组变量定义为常量类型
* 表示数据存储在内部的 FLASH 中*/
extern  const uint32_t  aSRC_Cont_Buffer[Map_BUFFER_SIZE] ;
 /* 定义 DMA 传输目标存储器存储在内部的 SRAM 中*/
extern    uint32_t aDST_Buffer[Map_BUFFER_SIZE];
/*************************************************************************************/

/******** B   **************** USART 到 RAM 的DMA输出 *******************************/
#define   USART_DMA_CHANNEL     DMA1_Channel4         //串口对应的 DMA 请求通道 
#define   USART_Source_ADDR     (USART1_BASE+0x04)    //串口数据的地址
extern    uint32_t              USART_BUFFER_SIZE ;   // 要发送的数据大小
extern    uint32_t*             USART_DMA_Buffer ;
/************************************************************************************/

void      _DMA_Config(DMA_Channel_TypeDef* _DMAy_Channelx , uint32_t _BUFFER_SIZE , uint32_t  _DMA_OutSource_ADDR,  uint32_t  _DMA_InSource_ADDR , uint32_t  _DMA_DIR);
void      _USART_DMA_Config(DMA_Channel_TypeDef* _DMAy_Channelx , uint32_t _BUFFER_SIZE , uint32_t  _DMA_OutSource_ADDR,  uint32_t  _DMA_InSource_ADDR , uint32_t  _DMA_DIR);
uint8_t   _Buffercmp(const uint32_t *pBuffer, uint32_t * pBuffer1 , uint16_t BufferLength);
void      _Buffer_Show(uint32_t * pBuffer , uint16_t BufferLength);
//DMA对内存ROM数据的取出
void _DMA_ROM_TO_RAM(uint32_t _BUFFER_SIZE , uint32_t  _DMA_Source_ADDR,  uint32_t  _DMA_AIM_ADDR );
//DMA对RAM到USART数据的取出
void _DMA_RAM_TO_USART(uint32_t _BUFFER_SIZE , uint32_t  _DMA_Source_ADDR,  uint32_t  _DMA_AIM_ADDR );

  
#define   _Map_DMA_Config_    _DMA_Config(Map_DMA_CHANNEL ,Map_BUFFER_SIZE ,aSRC_Cont_Buffer , aDST_Buffer , DMA_DIR_PeripheralSRC)
//  ROM 到 RAM 的DMA输出  的程序初始化   DMA_DIR_PeripheralSRC:为方向外设到内存
#define   _USART_DMA_Config_   _USART_DMA_Config(USART_DMA_CHANNEL ,USART_BUFFER_SIZE ,USART_Source_ADDR , USART_DMA_Buffer , DMA_DIR_PeripheralDST)
//  ROM 到 RAM 的DMA输出  的程序初始化  DMA_DIR_PeripheralDST:为方向外设到内存
#define   _DMA_InnerChange_    _Buffercmp(aSRC_Cont_Buffer , aDST_Buffer, Map_BUFFER_SIZE)
//  ROM 到 RAM 的DMA输出  的数据验证
#define   _RMA_InnerShow_      _Buffer_Show(aDST_Buffer, Map_BUFFER_SIZE)

#endif

