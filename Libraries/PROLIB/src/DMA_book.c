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
  //����DMAʱ��
  RCC_AHBPeriphClockCmd(DMA_CLOCK,ENABLE);
  //Դ���ݻ����ַ(�����ַ)
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)_DMA_Source_ADDR ; 
  //ת�������ַ��ַ(�ڴ��ַ)
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)_DMA_AIM_ADDR;
  
  //�������赽�洢��(������������ڲ���FLASH)DMA_DIR_PeripheralSRC:Ϊ�������赽�ڴ� DMA_DIR_PeripheralDST:Ϊ�������赽�ڴ�
  DMA_InitStructure.DMA_DIR = _DMA_DIR ;
  //�����С
  DMA_InitStructure.DMA_BufferSize = _BUFFER_SIZE;
  //����(�ڲ���FLASH)��ַ����
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  //�ڴ��ַ����
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  //�������ݵ�λ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  //�ڴ����ݵ�λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  //DMAģʽ��һ�λ���ѭ��ģʽ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  //���ȼ�����
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  //ʹ���ڴ浽�ڴ�Ĵ���
  DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
  //����DMAͨ��
  DMA_Init(_DMAy_Channelx , &DMA_InitStructure);
  //ʹ��DMA
  DMA_Cmd(_DMAy_Channelx , ENABLE);
}

void _USART_DMA_Config(DMA_Channel_TypeDef* _DMAy_Channelx , uint32_t _BUFFER_SIZE , uint32_t  _DMA_Source_ADDR,  uint32_t  _DMA_AIM_ADDR , uint32_t  _DMA_DIR){
  DMA_InitTypeDef   DMA_InitStructure ;
  //����DMAʱ��
  RCC_AHBPeriphClockCmd(DMA_CLOCK,ENABLE);  
  //Դ���ݻ����ַ(�����ַ)
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)_DMA_Source_ADDR ; 
  //ת�������ַ��ַ(�ڴ��ַ)
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)_DMA_AIM_ADDR;
  
  //�������赽�洢��(������������ڲ���FLASH)DMA_DIR_PeripheralSRC:Ϊ�������赽�ڴ� DMA_DIR_PeripheralDST:Ϊ�������赽�ڴ�
  DMA_InitStructure.DMA_DIR = _DMA_DIR ;
  //�����С
  DMA_InitStructure.DMA_BufferSize = _BUFFER_SIZE;
  //����(�ڲ���FLASH)��ַ����
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  //�ڴ��ַ����
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  //�������ݵ�λ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//DMA_PeripheralDataSize_Byte; //ע��������Ҫ�����������;����޸�
  //�ڴ����ݵ�λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//DMA_MemoryDataSize_Byte; //ע��������Ҫ�����������;����޸�
  //DMAģʽ��һ�λ���ѭ��ģʽ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  //���ȼ�����
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  //ʹ���ڴ浽�ڴ�Ĵ���
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  //����DMAͨ��
  DMA_Init(_DMAy_Channelx , &DMA_InitStructure);
  //ʹ��DMA
  DMA_Cmd(_DMAy_Channelx , ENABLE);
}
 
///Դ������Ŀ���ַ���ݶԱ�
uint8_t  _Buffercmp(const uint32_t *pBuffer, uint32_t * pBuffer1 , uint16_t BufferLength){
  /*���ݳ��ȵݼ�*/
  while(BufferLength--){
//  Usart_SendHalf_32_Word(_DEBUG_USARTx,*pBuffer);  
//  Usart_SendHalf_32_Word(_DEBUG_USARTx,*pBuffer1);
    /*�ж���������Դ�Ƿ����*/
    if(*pBuffer != *pBuffer1){
      /* ��Ӧ����Դ����������˳������������� 0 */
      return 0;
    }
    /* ������������Դ�ĵ�ַָ�� */
    pBuffer++;
    pBuffer1++;
  }
  /* ����жϲ��Ҷ�Ӧ������� */
  return 1;
}


//��RAM���ݽ���չʾ
void  _Buffer_Show(uint32_t * pBuffer , uint16_t BufferLength){
  /*���ݳ��ȵݼ�*/
  while(BufferLength--){
    Usart_SendHalf_32_Word(_DEBUG_USARTx,*pBuffer++);  
  }
}

//DMA���ڴ�ROM���ݵ�ȡ��
void _DMA_ROM_TO_RAM(uint32_t _BUFFER_SIZE , uint32_t  _DMA_Source_ADDR,  uint32_t  _DMA_AIM_ADDR ){
  //----------------------------------------------------------------
  printf("��ʼ ROM�ڴ浽RAM�ڴ��DMA���� \n");
  //�ڴ浽�ڴ�DMA��ʼ��
  _DMA_Config(Map_DMA_CHANNEL ,_BUFFER_SIZE ,_DMA_Source_ADDR , _DMA_AIM_ADDR , DMA_DIR_PeripheralSRC);       
  while(DMA_GetFlagStatus(DMA_FLAG_TC) == RESET);  //�ж�DMA�������Ƿ���ȷ          
  if(_DMA_InnerChange_== 0 ){
      printf("ROM�ڴ浽DMA�����쳣 \n");  
  }else{
      printf("ROM�ڴ浽DMA�������� \n");      
  }  
  _RMA_InnerShow_;
}

//DMA��RAM��USART���ݵ�ȡ��
void _DMA_RAM_TO_USART(uint32_t _BUFFER_SIZE , uint32_t  _DMA_Source_ADDR,  uint32_t  _DMA_AIM_ADDR ){
  // ��ʼ USART�ڴ浽RAM�ڴ��DMA���� 
  printf("\n��ʼ ROM��USART�Ĵ��ͳ�ʼ��\n"); 
  USART_BUFFER_SIZE = _BUFFER_SIZE;
  USART_DMA_Buffer = _DMA_AIM_ADDR;
  //�ڴ浽USART DMA��ʼ��
  _USART_DMA_Config(USART_DMA_CHANNEL ,USART_BUFFER_SIZE ,_DMA_Source_ADDR , USART_DMA_Buffer , DMA_DIR_PeripheralDST);
  USART_DMACmd(_DEBUG_USARTx , USART_DMAReq_Tx , ENABLE);  //����DMAʹ��
  /*USART_DMACmd �������ڿ��� USART �� DMA ����������͹رա�������������
  ������һ�������������ô������裬������ USART1/2/3 �� UART4/5 �� 5 ��������ѡ����
  �����������ô��ڵľ��� DMA �����д��ڷ������� USART_DMAReq_Tx �ͽ�������
  USART_DMAReq_Rx ��ѡ���������������������������� ENABLE ���߹ر�����*/
  
  fn_Systick_Delay(250,_Systick_ms);   //DMA ��������н���LED�����˸  
  while(USART_GetFlagStatus(_DEBUG_USARTx,USART_FLAG_TXE)==RESET);
  printf("\rROM�ڴ浽USART�����DMA�������\n");//���������ҪDelay һ��ʱ��ſ�����
}
 
//----------------------------------------------------------------

//----------------------------------------------------------------   
