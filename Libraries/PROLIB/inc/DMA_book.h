#ifndef  __DMA_BOOK_H_
#define  __DMA_BOOK_H_

#include "stm32f10x.h"

#define   DMA_CLOCK     RCC_AHBPeriph_DMA1    //DMA  ʱ��

/******  A   ****************** ROM �� RAM ��DMA��� *******************************/
#define   Map_DMA_CHANNEL     DMA1_Channel6    // ��ʹ�ô洢�����洢��ģʽʱ��ͨ���������ѡ��û��Ӳ�ԵĹ涨
#define   Map_BUFFER_SIZE     20             // Ҫ���͵����ݴ�С
#define   DMA_FLAG_TC   DMA1_FLAG_TC6         // ������ɱ�־
/* ���� aSRC_Const_Buffer ������Ϊ DMA ��������Դ
* const �ؼ��ֽ� aSRC_Const_Buffer �����������Ϊ��������
* ��ʾ���ݴ洢���ڲ��� FLASH ��*/
extern  const uint32_t  aSRC_Cont_Buffer[Map_BUFFER_SIZE] ;
 /* ���� DMA ����Ŀ��洢���洢���ڲ��� SRAM ��*/
extern    uint32_t aDST_Buffer[Map_BUFFER_SIZE];
/*************************************************************************************/

/******** B   **************** USART �� RAM ��DMA��� *******************************/
#define   USART_DMA_CHANNEL     DMA1_Channel4         //���ڶ�Ӧ�� DMA ����ͨ�� 
#define   USART_Source_ADDR     (USART1_BASE+0x04)    //�������ݵĵ�ַ
extern    uint32_t              USART_BUFFER_SIZE ;   // Ҫ���͵����ݴ�С
extern    uint32_t*             USART_DMA_Buffer ;
/************************************************************************************/

void      _DMA_Config(DMA_Channel_TypeDef* _DMAy_Channelx , uint32_t _BUFFER_SIZE , uint32_t  _DMA_OutSource_ADDR,  uint32_t  _DMA_InSource_ADDR , uint32_t  _DMA_DIR);
void      _USART_DMA_Config(DMA_Channel_TypeDef* _DMAy_Channelx , uint32_t _BUFFER_SIZE , uint32_t  _DMA_OutSource_ADDR,  uint32_t  _DMA_InSource_ADDR , uint32_t  _DMA_DIR);
uint8_t   _Buffercmp(const uint32_t *pBuffer, uint32_t * pBuffer1 , uint16_t BufferLength);
void      _Buffer_Show(uint32_t * pBuffer , uint16_t BufferLength);
//DMA���ڴ�ROM���ݵ�ȡ��
void _DMA_ROM_TO_RAM(uint32_t _BUFFER_SIZE , uint32_t  _DMA_Source_ADDR,  uint32_t  _DMA_AIM_ADDR );
//DMA��RAM��USART���ݵ�ȡ��
void _DMA_RAM_TO_USART(uint32_t _BUFFER_SIZE , uint32_t  _DMA_Source_ADDR,  uint32_t  _DMA_AIM_ADDR );

  
#define   _Map_DMA_Config_    _DMA_Config(Map_DMA_CHANNEL ,Map_BUFFER_SIZE ,aSRC_Cont_Buffer , aDST_Buffer , DMA_DIR_PeripheralSRC)
//  ROM �� RAM ��DMA���  �ĳ����ʼ��   DMA_DIR_PeripheralSRC:Ϊ�������赽�ڴ�
#define   _USART_DMA_Config_   _USART_DMA_Config(USART_DMA_CHANNEL ,USART_BUFFER_SIZE ,USART_Source_ADDR , USART_DMA_Buffer , DMA_DIR_PeripheralDST)
//  ROM �� RAM ��DMA���  �ĳ����ʼ��  DMA_DIR_PeripheralDST:Ϊ�������赽�ڴ�
#define   _DMA_InnerChange_    _Buffercmp(aSRC_Cont_Buffer , aDST_Buffer, Map_BUFFER_SIZE)
//  ROM �� RAM ��DMA���  ��������֤
#define   _RMA_InnerShow_      _Buffer_Show(aDST_Buffer, Map_BUFFER_SIZE)

#endif

