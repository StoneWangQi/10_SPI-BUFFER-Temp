#include "I2C_soft_book.h"
#include "Systick_book.h"

static I2C_GPIO_Soft_Config(void){
  GPIO_InitTypeDef   GPIO_InitStructure;
  _Soft_I2C_GPIO_APBxClock_FUN(_Soft_I2C_GPIO_CLK , ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = _Soft_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //��©���
  GPIO_Init(_Soft_I2C_SCL_PORT,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = _Soft_I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //��©���
  GPIO_Init(_Soft_I2C_SDA_PORT,&GPIO_InitStructure);
}

/**
  * @brief  I2C_EE_Init �����ʼ��
  * @param  ��
  * @retval ��
  */
static void I2C_Start(void){
  // ��SCL�ߵ�ƽʱ��SDA ����һ���½��ر��λI2C ���������ź�
   _I2C_SCL_1();
   _I2C_SDA_1();
   fn_Systick_Delay(50,_Systick_us);
   _I2C_SDA_0();
   fn_Systick_Delay(50,_Systick_us);
   _I2C_SCL_0();
   fn_Systick_Delay(50,_Systick_us);
}


/**
  * @brief  I2C_Stop �����ʼ��
  * @param  ��
  * @retval ��
  */

static void I2C_Stop(void){
  // ��SCL�ߵ�ƽ��SDA���������ر�ʾI2C����ֹͣ�ź�
  _I2C_SDA_0();
  _I2C_SCL_1();
  fn_Systick_Delay(50,_Systick_us);
  _I2C_SDA_1(); 
}


/**
  * @brief  I2C_SendByte �����ʼ��
  * @param  ��
  * @retval ��
  */

static void I2C_SendByte(uint8_t _ucByte){
  uint8_t i;
  //�����ֽڵĸ�λ
  for( i=0; i<8;i++ ){
    if(_ucByte & 0x80){
      _I2C_SDA_1();
    }else{
      _I2C_SDA_0();
    }
    fn_Systick_Delay(50,_Systick_us);
    _I2C_SCL_1();
    fn_Systick_Delay(50,_Systick_us);
    _I2C_SCL_0();
    _ucByte <<=1;               //------ע�����ﲻ̫һ��
    fn_Systick_Delay(50,_Systick_us);
  }
  _I2C_SDA_1();// �ͷ�����
}

/**
  * @brief  I2C_ReadByte �����ʼ��
  * @param  ��
  * @retval ��
  */
static uint8_t I2C_ReadByte(void){
  uint8_t i;
  uint8_t value;
  
  //������1��bit Ϊ���ݵ�bit7
  value = 0;
  for(i=0 ;i<8 ;i++ ){
    value <<=1;
    _I2C_SCL_1();
    fn_Systick_Delay(50,_Systick_us);
    if(_I2C_SDA_READ()){
      value++;
    }
    _I2C_SCL_0();
    fn_Systick_Delay(50,_Systick_us);
  }
  return value;
}


/**
  * @brief  I2C_WaitAck  
  * @param  ��
  * @retval ��
  */

static uint8_t I2C_WaitAck(void){
  uint8_t re;
  _I2C_SDA_1();
  fn_Systick_Delay(50,_Systick_us);
  _I2C_SCL_1();
  fn_Systick_Delay(50,_Systick_us);
  if(_I2C_SDA_READ()){
    re = 1;
  }else{
    re = 0;
  }
  _I2C_SCL_0();
  fn_Systick_Delay(50,_Systick_us);
  return re;
}


/**
  * @brief  I2C_ACK  
  * @param  ��
  * @retval ��
  */

static void I2C_ACK(void){
  _I2C_SDA_0();  //CPU ����SDA = 0;
  fn_Systick_Delay(50,_Systick_us);
  _I2C_SCL_1();  //CPU ����1��ʱ��
  fn_Systick_Delay(50,_Systick_us);
  _I2C_SCL_0();
  fn_Systick_Delay(50,_Systick_us);
  _I2C_SDA_1(); //CPU �ͷ�SDA����
}



/**
  * @brief  I2C_ACK  
  * @param  ��
  * @retval ��
  */

static void I2C_NACK(void){
  _I2C_SDA_1();  //CPU ����SDA = 1;
  fn_Systick_Delay(50,_Systick_us);
  _I2C_SCL_1();  //CPU ����1��ʱ��
  fn_Systick_Delay(50,_Systick_us);
  _I2C_SCL_0();
  fn_Systick_Delay(50,_Systick_us);
}

static uint8_t I2C_CheckDevice(uint8_t _Address){
  uint8_t ucAck;
  
  I2C_GPIO_Soft_Config();
  I2C_Start();
  
  I2C_SendByte(_Address | I2C_WR);
  ucAck = I2C_WaitAck();  /* ����豸��ACKӦ�� */
  
  I2C_Stop(); /* ����ֹͣ�ź� */
  
  return ucAck;
}


//----------------------I2C ��д����--------------------
//------------------------------------------------------

/**
  * @brief  EE_Soft_Check_State  
  * @param  �жϴ���EERPOM�Ƿ�����
  * @retval ��
  */

static uint8_t EE_Soft_Check_State(void){
  if(I2C_CheckDevice(Soft_EEPROM_ADDRESS)==0){return 1;}
  else{I2C_Stop(); /* ����ֹͣ�ź� */ return 0;}
}

/**
  * @brief  uint8_t I2C_Soft_BufferRead(uint8_t* pBuffer, 
  *      uint8_t ReadAddr, uint16_t NumByteToRead)
  * @param  �жϴ���EERPOM�Ƿ�����
  * @retval ��
  */
static uint8_t I2C_Soft_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead){
  uint16_t i ;
  //����ȡ�����ɸ��ֽ�
  // ����I2C���������ź�
  I2C_Start();
  //���Ϳ����ֽڵ�ַ�Ͷ�ȡ�����ź�
  I2C_SendByte(Soft_EEPROM_ADDRESS | I2C_WR);
  //�ȴ�Ӧ��״̬
  if(I2C_WaitAck()!=0){printf("EEPROM ���� 1 !\r\n"); goto  CMD_Fail;}
  //�������ݶ�ȡλ����Ϣ�ź�
  I2C_SendByte((uint8_t)ReadAddr);
  //�ȴ�Ӧ��״̬
  if(I2C_WaitAck()!=0){printf("EEPROM ���� 2 !\r\n");goto  CMD_Fail;}
  
  //--------------
  //��������I2C���� 
  I2C_Start();
  //����������ַ
  I2C_SendByte(Soft_EEPROM_ADDRESS| I2C_RD);
  //�ȴ�Ӧ��״̬
  if(I2C_WaitAck()!=0){printf("EEPROM ����3 !\r\n"); goto  CMD_Fail;}
  for(i=0 ;i<NumByteToRead ;i++ ){
    pBuffer[i] = I2C_ReadByte();
    if(i!=NumByteToRead-1){
      I2C_ACK();
    }else{
      I2C_NACK();
    }
  }
  I2C_Stop();
  return 1;
 
 CMD_Fail:
  I2C_Stop();
  return 0;
}

/**
  * @brief  uint8_t EE_Soft_WriteBytes(uint8_t* pBuffer, 
  *                   uint8_t ReadAddr, uint16_t NumByteToRead)
  * @param  �жϴ���EERPOM�Ƿ�����
  * @retval ��
  */

static uint8_t EE_Soft_WriteBytes(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead){
  uint16_t i , m;
  uint16_t usAddr;
  /*
    д����EEPROM �������������������ȡ�ܶ��ֽ�ÿ��д����ֻ����ͬһ��page
    ����24C page size = 8 
    �򵥵Ĵ�����Ϊ ���ֽ�д����ģʽ��дһ���ֽڶ����͵�ַ
    Ϊ���������д��Ч�� ����������Page write����
  */
  usAddr = ReadAddr;
  for(i=0 ;i<NumByteToRead;i++ ){
    // �����͵�һ���ֽڻ���ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ
    if((i==0)||(usAddr)&(_I2C_Soft_PageSize-1)==0){
      // ����ֹͣ�ź�
      I2C_Stop();
      //ͨ��������ж��ڴ�д���Ƿ�ɹ�
      m = 100;
      for (m = 0; m < 100; m++){	
        //����I2C���� 
        I2C_Start();
        //����������ַ
        I2C_SendByte(Soft_EEPROM_ADDRESS| I2C_WR);
        //�ȴ�Ӧ��״̬
        if(I2C_WaitAck()==0){break;}
      }
      if(m==100){printf("EEPROM ���� 4 !\r\n"); goto CMD_FAIL_bytes ; }
      I2C_SendByte((uint8_t)usAddr);
      if(I2C_WaitAck()!=0){printf("EEPROM ���� 5 !\r\n"); goto CMD_FAIL_bytes;}
    }
    // ��ʼд������ 
    I2C_SendByte(pBuffer[i]);
    //�ȴ�Ӧ��״̬
    if(I2C_WaitAck()!=0){printf("EEPROM ���� 7 !\r\n"); goto CMD_FAIL_bytes;}
    usAddr++;
  }
  // ����ֹͣ�ź�
  I2C_Stop();
  return 1;
  
CMD_FAIL_bytes:
  // ����ֹͣ�ź�
  I2C_Stop();
  return 0;
}



/**
  * @brief  void  EE_Soft_Trase(void)  
  * @param  �жϴ���EERPOM�Ƿ�����
  * @retval ��
  */
void  EE_Soft_Trase(void){
  uint16_t i ;
  uint8_t buf[_I2C_Soft_SIZE]={0};
  
  // ��仺����
  for(i=0 ;i<_I2C_Soft_SIZE ;i++ ){
    buf[i] = 0xFF;
  }
  //дEEPROM ��ʼ��ַ= 0 ���ݳ���Ϊ256
  if(EE_Soft_WriteBytes(buf,0,_I2C_Soft_SIZE)==0){
    printf("����EEPROM����\r\n");
		return;
  }else{
    printf("����EEPROM����\r\n");
  }
}


/**
  * @brief  void I2C_Soft_Init(void)
  * @param   
  * @retval ��
  */
void I2C_Soft_Init(void){
  if(EE_Soft_Check_State()==0){
      /* û�м�⵽EEPROM */
      printf("û�м�⵽����EEPROM!\r\n");
  }
}


/**
  * @brief  void I2C_Soft_Write_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite) 
  * @param  �жϴ���EERPOM�Ƿ�����
  * @retval ��
  */
uint8_t I2C_Soft_Write_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite){
  uint16_t i;
   
  //-------------------------------
  if(EE_Soft_Check_State()==0){
    /* û�м�⵽EEPROM */
		printf("û�м�⵽����EEPROM!\r\n");
    return 1;
  }  
  //------------д��I2C-------------------
  if(EE_Soft_WriteBytes(pBuffer,WriteAddr ,NumByteToWrite)==0){
    /* û�м�⵽EEPROM */
		printf("дEEPROM����!\r\n");
    return 1;
  }else{
    /* û�м�⵽EEPROM */
		printf("дEEPROM�ɹ�!\r\n");
  }
  fn_Systick_Delay(150,_Systick_us);
  //--------------���ݼ��--------------
  printf("EEPROMд�����ݼ����\r\n");
  for(i=0 ;i<NumByteToWrite ;i++ ){
     
     printf(" %d ",pBuffer[i]);
    if((i & 15)==15){
      printf("\r\n");
    }
  }
  return 0;
  
}


uint8_t I2C_Soft_Read_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite){
  uint16_t i;
  //-------------��ȡI2C------------------
  if(I2C_Soft_BufferRead(pBuffer,WriteAddr,NumByteToWrite)==0){
    /* û�м�⵽EEPROM */
		printf("��EEPROM����!\r\n");
    return 1;
  }else{
    /* û�м�⵽EEPROM */
		printf("\n��EEPROM�ɹ�!\r\n");
  }
  //--------------���ݼ��--------------
  printf("EEPROM��ȡ�������ݼ�� \r\n");
  for(i=0 ;i<NumByteToWrite ;i++ ){
     printf(" %d ",pBuffer[i]);
    if((i & 15)==15){
      printf("\r\n");
    }
  }
  return 1;
}



/*********************END OF FILE**********************/




