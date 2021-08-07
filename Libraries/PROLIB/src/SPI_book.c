#include "SPI_book.h"
#include "Systick_book.h"


static __IO  uint32_t SPITimeout = FLASH_SPI_LONG_TIMEOUT;


/**
  * @brief  SPII/O����
  * @param  ��
  * @retval ��
  */
static void SPI_GPIO_Config(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
  //ʹ����SPI �йص�ʱ��
  _FLASH_SPI_APBxClock_FUN(_FLASH_SPI_CLK , ENABLE);
  _FLASH_SPI_GPIO_APBxClock_FUN(_FLASH_SPI_GPIO_CLK , ENABLE);
  
  //MISO MOSI SCK
  GPIO_InitStructure.GPIO_Pin = _FLASH_SPI_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_Init(_FLASH_SPI_SCL_PORT,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = _FLASH_SPI_MISO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_Init(_FLASH_SPI_SCL_PORT,&GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = _FLASH_SPI_MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_Init(_FLASH_SPI_SCL_PORT,&GPIO_InitStructure);
  
  //��ʼ��CSS���ţ�ʹ��������ƣ�����ֱ������Ϊ�������
  GPIO_InitStructure.GPIO_Pin = _FLASH_SPI_CSS_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(_FLASH_SPI_SCL_PORT,&GPIO_InitStructure);
  
  _FLASH_CSS_HIGH();
}



/**
  * @brief  static void SPI_Mode_Config(void) ����
  * @param  ��
  * @retval ��
  */
static void SPI_Mode_Config(void){
  SPI_InitTypeDef  SPI_InitStructure;
  
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2 ; //������Ԥ��ƵֵΪ 2
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge ;        //���ݲ����ڵڶ���ʱ���� 
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;          //ʱ�����ո�
  SPI_InitStructure.SPI_CRCPolynomial = 0;             //��ʹ��CRC���ܣ���ֵ���д
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;    //SPI ���ͽ��� 8 λ֡�ṹ
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex ; //˫��ȫ˫��
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;   //���ݴ���� MSB λ��ʼ
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master ;       //����Ϊ�� SPI
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;            //�ڲ� NSS �ź��� SSI λ����
  
  SPI_Init(_FLASH_SPIx , &SPI_InitStructure ); //д�����õ��Ĵ���
  SPI_Cmd(_FLASH_SPIx , ENABLE);   //ʹ��SPI
}

/**
  * @brief  void SPI_FLASH_Init(void) ��ʼ��
  * @param  ��
  * @retval ��
  */
void SPI_FLASH_Init(void){
  SPI_GPIO_Config();
  SPI_Mode_Config();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                         ͨѶ�������� 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode��������룬����������λ���ĸ����ڳ���.
  * @retval ����0����ʾSPI��ȡʧ��.
  */
static  uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  
  return 0;
}
/**
  * @brief  uint8_t SPI_FLASH_Send_Byte(uint8_t data) ��ʼ��
  * @param  ���Ͳ��ҽ���һ���ֽ�
  * @retval ��
  */
static uint8_t SPI_FLASH_Send_Byte(uint8_t data){
  SPITimeout = FLASH_SPI_TIMEOUT;
  //��鲢�ȴ���TX������
  while(SPI_I2S_GetFlagStatus(_FLASH_SPIx,SPI_I2S_FLAG_TXE) == RESET){//���ͻ���ձ�־λ
    if(SPITimeout--==0) {return SPI_TIMEOUT_UserCallback(0);}
  }
  //�жϳ����Ѿ�Ϊ��
  SPI_I2S_SendData(_FLASH_SPIx , data);
  //�жϽ��ܻ���ǿ� 
  SPITimeout = FLASH_SPI_TIMEOUT;
  while(SPI_I2S_GetFlagStatus(_FLASH_SPIx,SPI_I2S_FLAG_RXNE) == RESET){//���ܻ���ǿձ�־λ��־λ
    if(SPITimeout--==0) {return SPI_TIMEOUT_UserCallback(1);}
  }
  //���������.������Ҫ����һ���ֽ�
  return SPI_I2S_ReceiveData(_FLASH_SPIx);
}

/**
  * @brief  uint8_t SPI_FLASH_Send_Byte(uint8_t data) ��ʼ��
  * @param  ���Ͳ��ҽ���һ���ֽ�
  * @retval ��
  */
static uint8_t SPI_FLASH_SendHalf_Byte(uint16_t Halfdata){
  SPITimeout = FLASH_SPI_TIMEOUT;
  //��鲢�ȴ���TX������
  while(SPI_I2S_GetFlagStatus(_FLASH_SPIx,SPI_I2S_FLAG_TXE) == RESET){//���ͻ���ձ�־λ
    if(SPITimeout--==0) {return SPI_TIMEOUT_UserCallback(2);}
  }
  //�жϳ����Ѿ�Ϊ��
  SPI_I2S_SendData(_FLASH_SPIx , Halfdata);
  //�жϽ��ܻ���ǿ� 
  SPITimeout = FLASH_SPI_TIMEOUT;
  while(SPI_I2S_GetFlagStatus(_FLASH_SPIx,SPI_I2S_FLAG_RXNE) == RESET){//���ܻ���ǿձ�־λ��־λ
    if(SPITimeout--==0) {return SPI_TIMEOUT_UserCallback(3);}
  }
  //���������.������Ҫ����һ���ֽ�
  return SPI_I2S_ReceiveData(_FLASH_SPIx);
}

/**
  * @brief  uint32_t SPI_Read_ID(void)
  * @param  ��ȡID��
  * @retval  
  */
uint32_t SPI_Read_ID(void){
  uint32_t flash_id;
  //Ƭѡʹ��
  _FLASH_CSS_LOW();
  SPI_FLASH_Send_Byte(FLASH_SPI_READ_JEDEC_ID);
  flash_id = SPI_FLASH_Send_Byte(FLASH_SPI_DUMMY);//Memory typeID
  flash_id<<=8;
  flash_id|=SPI_FLASH_Send_Byte(FLASH_SPI_DUMMY);//Capcity typeID
  flash_id<<=8;
  flash_id|=SPI_FLASH_Send_Byte(FLASH_SPI_DUMMY);//Capcity typeID
  _FLASH_CSS_HIGH();
  return flash_id;
}

/**
  * @brief  uint32_t SPI_Read_ID(void)
  * @param  ��ȡID��
  * @retval  
  */
//uint32_t SPI_Read_DeviceID(void){
//  uint32_t flash_id;
//  //Ƭѡʹ��
//  _FLASH_CSS_LOW();
//  SPI_FLASH_Send_Byte(FLASH_SPI_READ_JEDEC_ID);
//  SPI_FLASH_Send_Byte(FLASH_SPI_DUMMY);//Memory typeID
//  SPI_FLASH_Send_Byte(FLASH_SPI_DUMMY);//Capcity typeID
//  SPI_FLASH_Send_Byte(FLASH_SPI_DUMMY);//Capcity typeID
//  flash_id = SPI_FLASH_Send_Byte(FLASH_SPI_DUMMY);//Capcity typeID
//  _FLASH_CSS_HIGH();
//  return flash_id;
//}

/**
  * @brief  void SPI_Write_Enable(void)
  * @param  д��ʹ��
  * @retval  
  */
static void SPI_Write_Enable(void){
  //Ƭѡʹ��
  _FLASH_CSS_LOW();
  SPI_FLASH_Send_Byte(FLASH_SPI_WRITE_ENABLE);
  _FLASH_CSS_HIGH();  
}

/**
  * @brief  static void SPI_WaitForWriteEnd(void);
  * @param  //�ȴ�FLASH�ڲ�ʱ��������
  * @retval  
  */
static SPI_WaitForWriteEnd(void){
  uint8_t status_reg = 0;  //�ж����λS0 erse or write in progress
  // Ƭѡָ��
  _FLASH_CSS_LOW();
  SPI_FLASH_Send_Byte(FLASH_SPI_READ_STATUS);
  do{
    status_reg = SPI_FLASH_Send_Byte(FLASH_SPI_DUMMY); //��Ҫ��ȡ������Ҫ��������
  }while((status_reg & 0x01)==1);  //У�����λ����У��
  _FLASH_CSS_HIGH(); 
 
}
/** 
  * @brief  svoid SPI_Erase_Sector(uint32_t addr)
  * @param  ����FLASHָ������
  * @retval  
  */
void SPI_Erase_Sector(uint32_t addr){
  SPI_Write_Enable();
  /* �������� */
  /* ѡ��FLASH: CS�͵�ƽ */
  _FLASH_CSS_LOW();
  /* ������������ָ��*/
  SPI_FLASH_Send_Byte(FLASH_SPI_REASE_SECTOR);
  /*���Ͳ���������ַ�ĸ�λ*/
  SPI_FLASH_Send_Byte((addr & 0xFF0000) >> 16);
  /* ���Ͳ���������ַ����λ */
  SPI_FLASH_Send_Byte((addr & 0xFF00) >> 8);
  /* ���Ͳ���������ַ�ĵ�λ */
  SPI_FLASH_Send_Byte(addr & 0xFF);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  _FLASH_CSS_HIGH(); 
  /* �ȴ��������*/
  SPI_WaitForWriteEnd();
}
  
 /**
  * @brief  ����FLASH��������Ƭ����
  * @param  ��
  * @retval ��
  */
void SPI_FLASH_BulkErase(void){
  //����FLASH дʹ������
  SPI_Write_Enable();
  //����Erase
  //ѡ��FLASH :CS �͵�ƽ
  _FLASH_CSS_LOW();
  SPI_FLASH_Send_Byte(FLASH_SPI_ChipErase);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  _FLASH_CSS_HIGH(); 
  /* �ȴ��������*/
  SPI_WaitForWriteEnd();
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                         ��д���� 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
/** 
  * @brief  void SPI_Read_Data(uint8_t *readBuffer , uint32_t addr ,uint32_t  numByteToRead ));
  * @param  ��ȡFLASH������
  * @retval  
  */
void SPI_Read_Data(uint8_t *readBuffer , uint32_t addr ,uint16_t  numByteToRead ){
  //Ƭѡʹ��
  _FLASH_CSS_LOW();
  //���͵�ַ
  /* ���� �� ָ�� */
  SPI_FLASH_Send_Byte(FLASH_SPI_READ_DATA);
  /* ���� �� ��ַ��λ */
  SPI_FLASH_Send_Byte((addr>>16)&0xff);
  /* ���� �� ��ַ��λ */
  SPI_FLASH_Send_Byte((addr>>8)&0xff);
  /* ���� �� ��ַ��λ */
  SPI_FLASH_Send_Byte(addr&0xff);
  
//  if(numByteToRead > SPI_PAGE_SIZE){
//    numByteToRead = SPI_PAGE_SIZE;
//    printf("SPI_FLASH_PageWrite too large!\n");
//  }
  /* ��ȡ���� */
  while(numByteToRead--){
    /* ��ȡһ���ֽ�*/
    *readBuffer = SPI_FLASH_Send_Byte(FLASH_SPI_DUMMY);
    /* ָ����һ���ֽڻ����� */
    readBuffer++;
  }
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  _FLASH_CSS_HIGH();
}

 


/** 
  * @brief  void SPI_Write_Data(uint8_t *readBuffer , uint32_t addr ,uint32_t  numByteToRead ));
  * @param  ��ȡFLASH������
  * @retval  
  */
void SPI_Write_Data(uint8_t *writeBuffer , uint32_t addr ,uint16_t  numByteToRead ){
  SPI_Write_Enable();
  //Ƭѡʹ��
  _FLASH_CSS_LOW();
  /* дҳдָ��*/
  SPI_FLASH_Send_Byte(FLASH_SPI_WRITE_DATA);
  /*����д��ַ�ĸ�λ*/
  SPI_FLASH_Send_Byte((addr&0xff0000)>>16);
  /*����д��ַ����λ*/
  SPI_FLASH_Send_Byte((addr&0xff00)>>8);
  /*����д��ַ�ĵ�λ*/
  SPI_FLASH_Send_Byte(addr&0xff);
  
  if(numByteToRead > SPI_PAGE_SIZE){
    numByteToRead = SPI_PAGE_SIZE;
    printf("SPI_FLASH_PageWrite too large!\n");
  }
  //д������
  while(numByteToRead--){
    //���͵�ǰҪд����ֽ�����
    SPI_FLASH_Send_Byte(*writeBuffer);
    //ָ�������ֽ�����
    writeBuffer++;
  }
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  _FLASH_CSS_HIGH();
  /* �ȴ�д�����*/
  SPI_WaitForWriteEnd();
}

/** 
  * @brief  SPI_Write_Data(uint8_t *writeBuffer , uint32_t addr ,uint32_t  numByteToRead ){
  * @param  ��ȡFLASH������
  * @retval  
  */
void SPI_BufferWrite_Data(uint8_t *writeBuffer , uint32_t WriteAddr ,uint16_t  numByteToWrite ){
  uint32_t NumOfPage , NumOfSingle , BufferAddr ,count , temp;
  
  if(numByteToWrite == 0){printf("SPI_FLASH_PageWrite too small!\n"); return;}
  BufferAddr = WriteAddr % SPI_PAGE_Write_SIZE;
  /*��ַ��Ӧҳ��ǰ����������*/
  count = SPI_PAGE_Write_SIZE - BufferAddr;
  /*��ǰҳʣ�µ�ȫ����ַ����*/
  if(count >= numByteToWrite){
    //ʣ�µ����ݿ���һ��д��
     SPI_Write_Data(writeBuffer ,WriteAddr ,numByteToWrite );
     return;
  }
  SPI_Write_Data(writeBuffer ,WriteAddr ,(uint16_t)count );//�ָ�д�뵥��ҳ��
  temp = numByteToWrite -  count ;  //�ų����ಿ��
  WriteAddr += count;
  writeBuffer+=count;
  NumOfPage = temp / SPI_PAGE_Write_SIZE ; //�Դ�����ָ�����
  NumOfSingle = temp % SPI_PAGE_Write_SIZE ;
  
  if(NumOfPage == 0){
    SPI_Write_Data(writeBuffer ,WriteAddr ,(uint16_t)NumOfSingle );
    return;
  }else{
    while(NumOfPage--){
      SPI_Write_Data(writeBuffer ,WriteAddr ,SPI_PAGE_Write_SIZE );
      WriteAddr += SPI_PAGE_Write_SIZE;
      writeBuffer += SPI_PAGE_Write_SIZE;
    }
    SPI_Write_Data(writeBuffer ,WriteAddr ,(uint16_t)NumOfSingle );
    return;
  }
  
}

/** 
  * @brief  void SPI_Show_Data(uint8_t *readBuffer);
  * @param  ��ȡFLASH������
  * @retval  
  */
void SPI_Show_Data(uint8_t *readBuffer , uint16_t  numByteToRead){
  uint32_t i;
  for(i=0 ;i<numByteToRead ;i++ ){
    if(i%SPI_PAGE_Write_SIZE == 0){ //ÿ��256�ֽڻ���
      printf("\r\n ");
    }
     printf("0x%x ",readBuffer[i]);
  }
}

 