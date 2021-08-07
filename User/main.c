/**
  ******************************************************************************
  * @file    GPIO/JTAG_Remap/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *    
  *    
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "PROJ_book.h" 

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

void fn_LED_Flash_Init(void);
void fn_usart_show_Init(void);
void fn_DMA_show_Init(void);
void fn_I2C_EE_Init(void);
void fn_I2C_EE_Soft_Init(void);
void fn_SPI_FLASH_Soft_Init(void);

#define countof(a)      (sizeof(a) / sizeof(*(a)))
  
#define  _I2C_BufferSize (countof(writeData)-1)
static uint8_t writeData[_I2C_PageSize]={4,5,6,7,8,9,10,11};
static uint8_t writeData2[_I2C_PageSize]={24,25,26,27,28,29,30,31};
static uint8_t ReadData[_I2C_BufferSize]={0};

#define  _SPI_BufferSize  SPI_PAGE_SIZE   //(countof(write_SPI_Data)-1)
static uint8_t write_SPI_Data[_SPI_BufferSize]={0};
static uint8_t Read_SPI_Data[_SPI_BufferSize]={0};

int main(void)
{ 
      
      fn_RCC_Init();          //CPU ��Ƶ
      fn_Led_Init();          //LED �����ʼ��
      fn_Key_Init();          //���� �����ʼ��
      fn_USART_Init();        //���������ʼ��
      fn_LED_Flash_Init();    //RGB �������
      fn_usart_show_Init();   //�����������
      fn_EXTI_GPIO_Config();  //�ⲿ�ж����
      fn_DMA_show_Init();     //��ʼ��DMA������·
      fn_I2C_EE_Init();       //��ʼ��Ӳ��I2C������·
      fn_I2C_EE_Soft_Init();  //��ʼ�����I2C������·
 
      fn_SPI_FLASH_Soft_Init();  //SPI����ͨѶ
      
      while(1){
        fn_LED_ALL_OFF();
         fn_Systick_Delay(500,_Systick_ms);
        __G_OUT__;
        fn_Systick_Delay(500,_Systick_ms);
      }
}


void fn_LED_Flash_Init(void){
  uint16_t  count_Init = 2;
  printf("\n ---> LED��ʼ���� \n");
  while(count_Init-->0){
    fn_LED_ALL_OFF();
    __R_OUT__;
    fn_Systick_Delay(500,_Systick_ms);
    fn_LED_ALL_OFF();
    __G_OUT__;
    fn_Systick_Delay(500,_Systick_ms);
    fn_LED_ALL_OFF();
    __B_OUT__;
    fn_Systick_Delay(500,_Systick_ms);
    fn_LED_ALL_OFF();
    __R_OUT__;
  } 
}

void fn_usart_show_Init(void){ 
  fn_Usart_Send_Byte(_DEBUG_USARTx,'\r');
  printf("-->����ͨ��ָ������� \n");
  fn_Usart_SendString(_DEBUG_USARTx," : wangqi \n");
}

void fn_DMA_show_Init(void){
  printf("\n ---> DMA��ʼ���� \n");
  _DMA_ROM_TO_RAM(Map_BUFFER_SIZE ,aSRC_Cont_Buffer , aDST_Buffer);
  _DMA_RAM_TO_USART(Map_BUFFER_SIZE ,USART_Source_ADDR , aDST_Buffer); 
  printf("---> DMA������� \n");
}
void fn_I2C_EE_Init(void){
  printf("\n-->I2C_����д�뿪ʼ \n");
  _I2C_EE_Init();
  I2C_Write_fun(writeData ,EEP_Firstpage ,_I2C_BufferSize);
  I2C_Read_fun(ReadData ,EEP_Firstpage ,_I2C_BufferSize);
  printf("--->I2C_����д�����\n\r");
}

void fn_I2C_EE_Soft_Init(void){
  printf("\n-->I2C_�������д�뿪ʼ \n");
  I2C_Soft_Init();
  I2C_Soft_Write_fun(writeData2 ,EEP_Firstpage ,_I2C_BufferSize);
  I2C_Soft_Read_fun(ReadData ,EEP_Firstpage ,_I2C_BufferSize);
  printf("\n--->I2C_�������д�����\n\r");
}

void fn_SPI_FLASH_Soft_Init(void){
  uint16_t i,FlashID;
  printf("-->SPIͨ��ָ���Կ�ʼ \n");
  SPI_FLASH_Init(); 
  FlashID = SPI_Read_ID() ;
  if(FlashID == _SPI_FLASH_ID){
    printf("-->SPI  0x%x \n",FlashID);
  }
  
  SPI_Erase_Sector(0); //���һ��ҳ�Ŀռ� 
  printf("\n\n-->SPI��տ�ʼ \n");
  SPI_Read_Data(Read_SPI_Data , 0, SPI_PAGE_SIZE);
  SPI_Show_Data(Read_SPI_Data , SPI_PAGE_SIZE);
  printf("\n\n-->SPI������ \n");
   
  for(i=0 ; i < _SPI_BufferSize ; i++){
    write_SPI_Data[i] = 0xA7;
  }
  SPI_Show_Data(write_SPI_Data , SPI_PAGE_SIZE);
  
  SPI_BufferWrite_Data(write_SPI_Data ,0x000000,_SPI_BufferSize);
  
  
  printf("\n\n-->SPI������� \n");
  SPI_Read_Data(Read_SPI_Data , 0x000000, _SPI_BufferSize);
  SPI_Show_Data(Read_SPI_Data , _SPI_BufferSize);
  
  printf("-->SPIͨ��ָ������� \n");
}

void delay(int x){
	int y = 0xFFFFF;
	while((x--)>0){
		while((y--)>0){
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
		}
	}
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
