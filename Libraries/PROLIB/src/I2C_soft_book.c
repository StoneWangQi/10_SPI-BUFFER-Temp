#include "I2C_soft_book.h"
#include "Systick_book.h"

static I2C_GPIO_Soft_Config(void){
  GPIO_InitTypeDef   GPIO_InitStructure;
  _Soft_I2C_GPIO_APBxClock_FUN(_Soft_I2C_GPIO_CLK , ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = _Soft_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //开漏输出
  GPIO_Init(_Soft_I2C_SCL_PORT,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = _Soft_I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //开漏输出
  GPIO_Init(_Soft_I2C_SDA_PORT,&GPIO_InitStructure);
}

/**
  * @brief  I2C_EE_Init 程序初始化
  * @param  无
  * @retval 无
  */
static void I2C_Start(void){
  // 当SCL高电平时候SDA 出现一个下降沿编号位I2C 总线启动信号
   _I2C_SCL_1();
   _I2C_SDA_1();
   fn_Systick_Delay(50,_Systick_us);
   _I2C_SDA_0();
   fn_Systick_Delay(50,_Systick_us);
   _I2C_SCL_0();
   fn_Systick_Delay(50,_Systick_us);
}


/**
  * @brief  I2C_Stop 程序初始化
  * @param  无
  * @retval 无
  */

static void I2C_Stop(void){
  // 当SCL高电平，SDA出现上升沿表示I2C总线停止信号
  _I2C_SDA_0();
  _I2C_SCL_1();
  fn_Systick_Delay(50,_Systick_us);
  _I2C_SDA_1(); 
}


/**
  * @brief  I2C_SendByte 程序初始化
  * @param  无
  * @retval 无
  */

static void I2C_SendByte(uint8_t _ucByte){
  uint8_t i;
  //发送字节的高位
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
    _ucByte <<=1;               //------注意这里不太一样
    fn_Systick_Delay(50,_Systick_us);
  }
  _I2C_SDA_1();// 释放总线
}

/**
  * @brief  I2C_ReadByte 程序初始化
  * @param  无
  * @retval 无
  */
static uint8_t I2C_ReadByte(void){
  uint8_t i;
  uint8_t value;
  
  //读到第1个bit 为数据的bit7
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
  * @param  无
  * @retval 无
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
  * @param  无
  * @retval 无
  */

static void I2C_ACK(void){
  _I2C_SDA_0();  //CPU 驱动SDA = 0;
  fn_Systick_Delay(50,_Systick_us);
  _I2C_SCL_1();  //CPU 产生1个时钟
  fn_Systick_Delay(50,_Systick_us);
  _I2C_SCL_0();
  fn_Systick_Delay(50,_Systick_us);
  _I2C_SDA_1(); //CPU 释放SDA总线
}



/**
  * @brief  I2C_ACK  
  * @param  无
  * @retval 无
  */

static void I2C_NACK(void){
  _I2C_SDA_1();  //CPU 驱动SDA = 1;
  fn_Systick_Delay(50,_Systick_us);
  _I2C_SCL_1();  //CPU 产生1个时钟
  fn_Systick_Delay(50,_Systick_us);
  _I2C_SCL_0();
  fn_Systick_Delay(50,_Systick_us);
}

static uint8_t I2C_CheckDevice(uint8_t _Address){
  uint8_t ucAck;
  
  I2C_GPIO_Soft_Config();
  I2C_Start();
  
  I2C_SendByte(_Address | I2C_WR);
  ucAck = I2C_WaitAck();  /* 检测设备的ACK应答 */
  
  I2C_Stop(); /* 发送停止信号 */
  
  return ucAck;
}


//----------------------I2C 独写操作--------------------
//------------------------------------------------------

/**
  * @brief  EE_Soft_Check_State  
  * @param  判断串行EERPOM是否正常
  * @retval 无
  */

static uint8_t EE_Soft_Check_State(void){
  if(I2C_CheckDevice(Soft_EEPROM_ADDRESS)==0){return 1;}
  else{I2C_Stop(); /* 发送停止信号 */ return 0;}
}

/**
  * @brief  uint8_t I2C_Soft_BufferRead(uint8_t* pBuffer, 
  *      uint8_t ReadAddr, uint16_t NumByteToRead)
  * @param  判断串行EERPOM是否正常
  * @retval 无
  */
static uint8_t I2C_Soft_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead){
  uint16_t i ;
  //连续取得若干个字节
  // 发起I2C总线启动信号
  I2C_Start();
  //发送控制字节地址和读取数据信号
  I2C_SendByte(Soft_EEPROM_ADDRESS | I2C_WR);
  //等待应答状态
  if(I2C_WaitAck()!=0){printf("EEPROM 错误 1 !\r\n"); goto  CMD_Fail;}
  //发送数据读取位置信息信号
  I2C_SendByte((uint8_t)ReadAddr);
  //等待应答状态
  if(I2C_WaitAck()!=0){printf("EEPROM 错误 2 !\r\n");goto  CMD_Fail;}
  
  //--------------
  //重新启动I2C总线 
  I2C_Start();
  //发送器件地址
  I2C_SendByte(Soft_EEPROM_ADDRESS| I2C_RD);
  //等待应答状态
  if(I2C_WaitAck()!=0){printf("EEPROM 错误3 !\r\n"); goto  CMD_Fail;}
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
  * @param  判断串行EERPOM是否正常
  * @retval 无
  */

static uint8_t EE_Soft_WriteBytes(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead){
  uint16_t i , m;
  uint16_t usAddr;
  /*
    写串行EEPROM 不像读操作可以连续读取很多字节每次写操作只能在同一个page
    对于24C page size = 8 
    简单的处理方法为 按字节写操作模式，写一个字节都发送地址
    为了提高连续写的效率 笨函数采用Page write操作
  */
  usAddr = ReadAddr;
  for(i=0 ;i<NumByteToRead;i++ ){
    // 当发送第一个字节或者页面首地址时，需要重新发起启动信号和地址
    if((i==0)||(usAddr)&(_I2C_Soft_PageSize-1)==0){
      // 发送停止信号
      I2C_Stop();
      //通过检测器判断内存写入是否成功
      m = 100;
      for (m = 0; m < 100; m++){	
        //启动I2C总线 
        I2C_Start();
        //发送器件地址
        I2C_SendByte(Soft_EEPROM_ADDRESS| I2C_WR);
        //等待应答状态
        if(I2C_WaitAck()==0){break;}
      }
      if(m==100){printf("EEPROM 错误 4 !\r\n"); goto CMD_FAIL_bytes ; }
      I2C_SendByte((uint8_t)usAddr);
      if(I2C_WaitAck()!=0){printf("EEPROM 错误 5 !\r\n"); goto CMD_FAIL_bytes;}
    }
    // 开始写入数据 
    I2C_SendByte(pBuffer[i]);
    //等待应答状态
    if(I2C_WaitAck()!=0){printf("EEPROM 错误 7 !\r\n"); goto CMD_FAIL_bytes;}
    usAddr++;
  }
  // 发送停止信号
  I2C_Stop();
  return 1;
  
CMD_FAIL_bytes:
  // 发送停止信号
  I2C_Stop();
  return 0;
}



/**
  * @brief  void  EE_Soft_Trase(void)  
  * @param  判断串行EERPOM是否正常
  * @retval 无
  */
void  EE_Soft_Trase(void){
  uint16_t i ;
  uint8_t buf[_I2C_Soft_SIZE]={0};
  
  // 填充缓冲区
  for(i=0 ;i<_I2C_Soft_SIZE ;i++ ){
    buf[i] = 0xFF;
  }
  //写EEPROM 起始地址= 0 数据长度为256
  if(EE_Soft_WriteBytes(buf,0,_I2C_Soft_SIZE)==0){
    printf("擦除EEPROM出错！\r\n");
		return;
  }else{
    printf("擦除EEPROM出错！\r\n");
  }
}


/**
  * @brief  void I2C_Soft_Init(void)
  * @param   
  * @retval 无
  */
void I2C_Soft_Init(void){
  if(EE_Soft_Check_State()==0){
      /* 没有检测到EEPROM */
      printf("没有检测到串行EEPROM!\r\n");
  }
}


/**
  * @brief  void I2C_Soft_Write_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite) 
  * @param  判断串行EERPOM是否正常
  * @retval 无
  */
uint8_t I2C_Soft_Write_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite){
  uint16_t i;
   
  //-------------------------------
  if(EE_Soft_Check_State()==0){
    /* 没有检测到EEPROM */
		printf("没有检测到串行EEPROM!\r\n");
    return 1;
  }  
  //------------写入I2C-------------------
  if(EE_Soft_WriteBytes(pBuffer,WriteAddr ,NumByteToWrite)==0){
    /* 没有检测到EEPROM */
		printf("写EEPROM错误!\r\n");
    return 1;
  }else{
    /* 没有检测到EEPROM */
		printf("写EEPROM成功!\r\n");
  }
  fn_Systick_Delay(150,_Systick_us);
  //--------------数据检查--------------
  printf("EEPROM写入数据检查检查\r\n");
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
  //-------------读取I2C------------------
  if(I2C_Soft_BufferRead(pBuffer,WriteAddr,NumByteToWrite)==0){
    /* 没有检测到EEPROM */
		printf("读EEPROM错误!\r\n");
    return 1;
  }else{
    /* 没有检测到EEPROM */
		printf("\n读EEPROM成功!\r\n");
  }
  //--------------数据检查--------------
  printf("EEPROM读取数据数据检查 \r\n");
  for(i=0 ;i<NumByteToWrite ;i++ ){
     printf(" %d ",pBuffer[i]);
    if((i & 15)==15){
      printf("\r\n");
    }
  }
  return 1;
}



/*********************END OF FILE**********************/




