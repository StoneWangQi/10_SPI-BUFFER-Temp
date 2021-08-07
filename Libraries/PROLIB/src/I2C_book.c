#include "I2C_book.h"
#include "Systick_book.h"

/**
  * @brief  I2C_EE_Init �����ʼ��
  * @param  ��
  * @retval ��
  */
static void I2C_GPIO_Config(void){
  GPIO_InitTypeDef   GPIO_InitStructure;
  //  ��ʼ�� I2C ���ʱ��
  _EEPROM_I2C_APBxClock_FUN(_EEPROM_I2C_CLK,ENABLE);
  _EEPROM_I2C_GPIO_APBxClock_FUN(_EEPROM_I2C_GPIO_CLK,ENABLE);
  //  ��ʼ��I2C_SCL SDA
  
  GPIO_InitStructure.GPIO_Pin = _EEPROM_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  //��©���
  GPIO_Init(_EEPROM_I2C_SCL_PORT,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = _EEPROM_I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  //��©���
  GPIO_Init(_EEPROM_I2C_SDA_PORT,&GPIO_InitStructure);
}

/**
  * @brief  I2C_EE_Init �����ʼ��
  * @param  ��
  * @retval ��
  */
static void I2C_Mode_Config(void){
  I2C_InitTypeDef I2C_InitStructure;
  /* i2C ���� */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  // �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ�
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  
  I2C_InitStructure.I2C_OwnAddress1 = _I2Cx_OWN_ADDRESS7;
  
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  
  //I2C Ѱַģʽ
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  //ͨѶƵ��
  I2C_InitStructure.I2C_ClockSpeed = _I2C_Speed;
  //I2C ��ʼ��
  I2C_Init(_EEPROM_I2Cx,&I2C_InitStructure);
  //ʹ��I2C
  I2C_Cmd(_EEPROM_I2Cx,ENABLE);
}
  
/**************************************/

static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode){
  fn_Usart_SendString(_DEBUG_USARTx,"I2C �ȴ���ʱ!errorCode =");
  printf("%d\n",errorCode);
 
  return 0;
}
/**************************************/

/*ͨѶ�ȴ���ʱʱ��*/
#define  I2CT_FLAG_TIMEOUT  ((uint32_t)0x6000)
#define  I2CT_LONG_TIMEOUT  ((uint32_t)(10*I2CT_FLAG_TIMEOUT))
static uint16_t I2CTimeout;
/**************************************/
/**
* @brief  дһ���ֽڵ� I2C EEPROM ��
* @param  pBuffer:������ָ��
* @param  WriteAddr:д��ַ
* @retval �������� 1���쳣���� 0
*/
static uint32_t  I2C_EE_ByteWrite(u8* pBuffer, uint8_t WriteAddr ){
  
  I2CTimeout = I2CT_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(_EEPROM_I2Cx , ENABLE)){
    if((I2CTimeout--) == 0){return  I2C_TIMEOUT_UserCallback(4);}
  }
 
  //����I2C��ʼ�ź�
  I2C_GenerateSTART(_EEPROM_I2Cx , ENABLE);
  I2CTimeout = I2CT_LONG_TIMEOUT; //�����������ʱ�쳣ʱ��
  //���EV5 �¼��������ʶλ
  while(!I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT)){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(5);}
  } 
  
  //����EEPROM �豸��ַ
  I2C_Send7bitAddress(_EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Transmitter);
  I2CTimeout = I2CT_LONG_TIMEOUT; //�����������ʱ�쳣ʱ��
  //���EV6 �¼��������ʶλ
  while(I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(6);}
  }
  
  //����Ҫд���EEPROM �ڲ���ַ(��EEPROM�ڲ��洢���ַ);
  I2C_SendData(_EEPROM_I2Cx,WriteAddr);
  I2CTimeout = I2CT_LONG_TIMEOUT;
  //���EV8 �¼������־λ
  while(!I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(2);}
  }
   
  //����Ҫд���EEPROM �ڲ�������;
  I2C_SendData(_EEPROM_I2Cx,*pBuffer);
  I2CTimeout = I2CT_LONG_TIMEOUT;
  //���EV8 �¼������־λ
  while(!I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(3);}
  }
  
  //����Ҫд���EEPROM �ڲ�������;
  I2C_SendData(_EEPROM_I2Cx,ENABLE);
  
  return 1;
}

 /**
  * @brief ���������е�����д�� I2C EEPROM �У����õ��ֽ�д��ķ�ʽ���ٶȱ�ҳд����
  * @param pBuffer:������ָ��
  * @param WriteAddr:д��ַ
  * @param NumByteToWrite:д���ֽ���
  */

static void I2C_EE_WaitEepromStandbyState(void){
  vu16 SR1_Tmp = 0;
  do{
    //����I2C��ʼ�ź�
    I2C_GenerateSTART(_EEPROM_I2Cx,ENABLE);
    //��ȡI2C1 SR1 �Ĵ���
    SR1_Tmp = I2C_ReadRegister(_EEPROM_I2Cx, I2C_Register_SR1);
    //����EEPROM ��ַ+ ����
    I2C_Send7bitAddress(_EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(_EEPROM_I2Cx, I2C_Register_SR1) & 0x0002));
  /* ��� AF λ */
  I2C_ClearFlag(_EEPROM_I2Cx, I2C_FLAG_AF);
  //����ֹͣλ�ź�
  I2C_GenerateSTOP(_EEPROM_I2Cx , ENABLE);
}

//zuozuo04-30 

/**
* @brief �� EEPROM ��һ��дѭ���п���д����ֽڣ���һ��д����ֽ���
* ���ܳ��� EEPROM ҳ�Ĵ�С��AT24C02 ÿҳ�� 8 ���ֽ�
* @param
* @param pBuffer:������ָ��
* @param WriteAddr:д��ַ
* @param NumByteToWrite:Ҫд���ֽ���Ҫ�� NumByToWrite С��ҳ��С
* @retval �������� 1���쳣���� 0
*/
static uint8_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite){
  I2CTimeout = I2CT_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(_EEPROM_I2Cx , ENABLE)){
    if((I2CTimeout--) == 0){return  I2C_TIMEOUT_UserCallback(4);}
  }
  //����I2C��ʼ�ź�
  I2C_GenerateSTART(_EEPROM_I2Cx , ENABLE);
  I2CTimeout = I2CT_LONG_TIMEOUT; //�����������ʱ�쳣ʱ��
  //���EV5 �¼��������ʶλ
  while(!I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT)){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(5);}
  }
  
  //����EEPROM �豸��ַ
  I2C_Send7bitAddress(_EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Transmitter);
  I2CTimeout = I2CT_LONG_TIMEOUT; //�����������ʱ�쳣ʱ��
  //���EV6 �¼��������ʶλ
  while(I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(6);}
  }
  
  //����Ҫд���EEPROM�ڲ���ַ��EEPROM�ڲ��洢����ַ��
  I2C_SendData(_EEPROM_I2Cx,WriteAddr);
  I2CTimeout = I2CT_LONG_TIMEOUT; //�����������ʱ�쳣ʱ��
  //���EV7 �¼��������ʶλ
  while(!I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING)){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(7);}
  }

  //ѭ������ NumByteToWrite������
  while(NumByteToWrite--){
    //���ͻ�����������
    I2C_SendData(_EEPROM_I2Cx,*pBuffer++);
    I2CTimeout = I2CT_FLAG_TIMEOUT; //�����������ʱ�쳣ʱ��
    //���EV8 �¼��������ʶλ
    while(!I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
      if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(8);}
    }       
  }
  //����ֹͣ�ź�
  I2C_GenerateSTOP(_EEPROM_I2Cx,ENABLE);
  return 1;
}



/**  ����д��һҳ
* @brief ���������е�����д�� I2C EEPROM ��
* @param
* @arg pBuffer:������ָ��
* @arg WriteAddr:д��ַ
* @arg NumByteToWrite:д���ֽ���
* @retval ��
*/
#define I2C_PageSize 8  //AT24C01 02 ÿҳ��8���ֽ�
static void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite){
  u8 NumOfPage = 0, NumOfSingle = 0 , Addr = 0 , count = 0,temp = 0;
  //Mod �������㣬���writeAddr �� I2C_PageSize ���鱶��������λAddrΪ0
  Addr = WriteAddr % I2C_PageSize;
  //��count����ֵ���պÿ��Զ��뵽ҳ���ַ
  count = I2C_PageSize - Addr; 
  //�����Ҫд��������ҳ
  NumOfPage = NumByteToWrite / I2C_PageSize;
  //mod����������������һҳ���ֽ���
  NumOfSingle = NumByteToWrite % I2C_PageSize;
  // Addr = 0,��WriteAddr �պð�ҳ����aligned
  // �����ͺܼ��ˣ�ֱ��д�Ϳ���д����ҳ��
  // ��ʣ�µĲ���һҳ��д�꼴��
  if(Addr == 0){
    //��� NumByteToWrite < I2C_PageSize
    if(NumOfPage==0){
      I2C_EE_PageWrite(pBuffer , WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }//���NumByteToWrite > I2C_PageSize
    else{
      //�Ȱ�����ҳд��
      while(NumOfPage--){
        I2C_EE_PageWrite(pBuffer , WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += I2C_PageSize ;
        pBuffer += I2C_PageSize ;
      }
      //���ж���Ĳ���һҳ�����ݣ�����д��
      if(NumOfSingle != 0){
        I2C_EE_PageWrite(pBuffer , WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  //��� WriteAddr ���ǰ� I2C_PageSize ����
  //�Ǿ�������뵽ҳ��ַ����Ҫ�������ݣ�Ȼ���Ȱ��⼸������д�꣬ʣ�¿�ʼ�ĵ�ַ���Ѿ�����
  //��ҳ��ַ�ˣ������ظ�����ļ���
  else{
    //���NumByteToWrite < I2C_PageSize
    if(NumOfPage == 0){
      //��NumOfSingle > count,��ǰ��д���꣬Ҫд��һҳ
      if(NumOfSingle > count){
        temp = NumOfSingle - count;
        I2C_EE_PageWrite(pBuffer , WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count ;
        pBuffer += count ;
        
        I2C_EE_PageWrite(pBuffer , WriteAddr, temp);
        I2C_EE_WaitEepromStandbyState();
      }else{//��count �� NumOfSingle��
        I2C_EE_PageWrite(pBuffer , WriteAddr, NumByteToWrite);
        I2C_EE_WaitEepromStandbyState(); 
      }
    }
    //��� NumByteToWrite > I2C_PageSize
    else{
      //��ַ����������Count �ֿ������������������
      NumByteToWrite -= count;
      NumOfPage = NumByteToWrite / I2C_PageSize ;
      NumOfSingle = NumByteToWrite % I2C_PageSize;
      //�Ȱ� WriteAddr ����ҳ��ʣ���ֽ�д��
      if(count!=0){
        I2C_EE_PageWrite(pBuffer , WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        //���� count �󣬵�ַ�Ͷ��뵽ҳ��
        WriteAddr += count ;
        pBuffer += count ;
      }
      //����ҳ��д��
      while(NumOfPage--){
        I2C_EE_PageWrite(pBuffer , WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += I2C_PageSize ;
        pBuffer += I2C_PageSize ;
      }
      //������Ĳ�����һҳ���Ͱ���д��
      if(NumOfSingle !=0){
        I2C_EE_PageWrite(pBuffer , WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState(); 
      }
    }
    
  }
}



/*  EEPROM ��ȡ
* @brief �� EEPROM �����ȡһ������
* @param pBuffer:��Ŵ� EEPROM ��ȡ�����ݵĻ�����ָ��
* @param ReadAddr:�������ݵ� EEPROM �ĵ�ַ
* @param NumByteToRead:Ҫ�� EEPROM ��ȡ���ֽ���
* @retval �������� 1���쳣���� 0
*/

static uint8_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead){
    I2CTimeout = I2CT_LONG_TIMEOUT ;
    while(I2C_GetFlagStatus(_EEPROM_I2Cx , I2C_FLAG_BUSY)){
      if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(9);}
    }
    
    //����I2C��ʼ�ź�
    I2C_GenerateSTART(_EEPROM_I2Cx , ENABLE);
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    //���EV10 �¼��������ע
    while(! I2C_CheckEvent(_EEPROM_I2Cx , I2C_EVENT_MASTER_MODE_SELECT)){
      if((I2CTimeout--)==0){ return I2C_TIMEOUT_UserCallback(10);}
    }
    
    //����EEPROM �豸��ַ
    I2C_Send7bitAddress(_EEPROM_I2Cx , EEPROM_ADDRESS , I2C_Direction_Transmitter);
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    //���EV11 �¼������
    while(! I2C_CheckEvent(_EEPROM_I2Cx , I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
      if((I2CTimeout--)==0){ return I2C_TIMEOUT_UserCallback(11);}
    }
    
    //ͨ����������PEλ���EV12�¼�
    I2C_Cmd(_EEPROM_I2Cx ,ENABLE );
    //����Ҫ��ȡ��EEPROM�ڲ���ַ����EEPROM�ڲ��洢����ַ��
    I2C_SendData(_EEPROM_I2Cx, ReadAddr);
    I2CTimeout = I2CT_FLAG_TIMEOUT ;
    //���EV12 �¼������
    while(! I2C_CheckEvent(_EEPROM_I2Cx , I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
      if((I2CTimeout--)==0){ return I2C_TIMEOUT_UserCallback(12);}
    }
    
    //�����ڶ���I2C��ʼ�ź�
    I2C_GenerateSTART(_EEPROM_I2Cx , ENABLE);
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    //���EV13 �¼������
    while(! I2C_CheckEvent(_EEPROM_I2Cx ,  I2C_EVENT_MASTER_MODE_SELECT)){
      if((I2CTimeout--)==0){ return I2C_TIMEOUT_UserCallback(13);}
    }
    
    //����EEPROM �豸��ַ
    I2C_Send7bitAddress(_EEPROM_I2Cx , EEPROM_ADDRESS , I2C_Direction_Receiver);
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    //���EV14 �¼������
    while(! I2C_CheckEvent(_EEPROM_I2Cx , I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
      if((I2CTimeout--)==0){ return I2C_TIMEOUT_UserCallback(14);}
    }
    
    //��ȡNumByteToRead������
    while(NumByteToRead){
      //��� NumByteToRead=1����ʾ�Ѿ��յ����һ��������
      //����Ӧ���źŽ������
      if(NumByteToRead == 1){
        //���ͷ�Ӧ���ź�
        I2C_AcknowledgeConfig(_EEPROM_I2Cx , DISABLE);
        
      }
      I2CTimeout = I2CT_LONG_TIMEOUT;
      while(I2C_CheckEvent(_EEPROM_I2Cx , I2C_EVENT_MASTER_BYTE_RECEIVED)==0){
        if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(3);}
      }
      
      //ͨ��I2C�����豸�ж�ȡһ���ֽڵ�����
      *pBuffer = I2C_ReceiveData(_EEPROM_I2Cx);
      //�洢���ݵ�ָ��ָ���µ�ַ
      pBuffer++;
      //���������Լ�
      NumByteToRead--;
    }
    //����ֹͣ�ź�
        I2C_GenerateSTOP(_EEPROM_I2Cx , ENABLE);
    //ʹ�ܴ�Ӧ������һ��I2C���
    I2C_AcknowledgeConfig(_EEPROM_I2Cx , ENABLE);
    return 1;
}

//--------------------------------------------------------

/**
* @brief void _I2C_EE_Init(void) 
* @param ��
* @retval �������� 1 ������������ 0
*/
void _I2C_EE_Init(void){
  I2C_GPIO_Config(); 
  I2C_Mode_Config();
}

/**
* @brief I2C_Write_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite) 
* @param ��
* @retval �������� 1 ������������ 0
*/
void I2C_Write_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite){
  u16 i;
  
  printf("I2C_д������ \n");
  I2C_EE_WaitEepromStandbyState();
  I2C_EE_PageWrite(pBuffer,WriteAddr, NumByteToWrite);
  for(i=0 ;i<NumByteToWrite ;i++ ){
    printf("%d ", *pBuffer++);
    if(i%16 == 15)    
        printf("\n\r");
  }
  printf("\nI2C_д��������� \n");
  I2C_EE_WaitEepromStandbyState();
   
  for(i=0 ;i<NumByteToWrite ;i++ ){
    printf("%d ", pBuffer[i]);
    if(i%16 == 15)    
        printf("\n\r");
  }
}



/**
* @brief I2C(void I2C_Read_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite))��д����
* @param ��
* @retval EEP_SIZE
*/
void I2C_Read_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite){
  u16 i;
  
  printf("I2C_���ݼ�� \n");
  I2C_EE_BufferRead(pBuffer,WriteAddr,NumByteToWrite);
  printf("\nI2C_���ݶ�ȡ��� \n");
  for(i=0 ;i<NumByteToWrite ;i++ ){
    printf("%d ", pBuffer[i]);
    if(i%16 == 15)    
        printf("\n\r");
  }
   printf("\n--->I2C_���ݼ�����\n");
}
   
