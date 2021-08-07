#include "I2C_book.h"
#include "Systick_book.h"

/**
  * @brief  I2C_EE_Init 程序初始化
  * @param  无
  * @retval 无
  */
static void I2C_GPIO_Config(void){
  GPIO_InitTypeDef   GPIO_InitStructure;
  //  初始化 I2C 相关时钟
  _EEPROM_I2C_APBxClock_FUN(_EEPROM_I2C_CLK,ENABLE);
  _EEPROM_I2C_GPIO_APBxClock_FUN(_EEPROM_I2C_GPIO_CLK,ENABLE);
  //  初始化I2C_SCL SDA
  
  GPIO_InitStructure.GPIO_Pin = _EEPROM_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  //开漏输出
  GPIO_Init(_EEPROM_I2C_SCL_PORT,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = _EEPROM_I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  //开漏输出
  GPIO_Init(_EEPROM_I2C_SDA_PORT,&GPIO_InitStructure);
}

/**
  * @brief  I2C_EE_Init 程序初始化
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Config(void){
  I2C_InitTypeDef I2C_InitStructure;
  /* i2C 配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  // 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  
  I2C_InitStructure.I2C_OwnAddress1 = _I2Cx_OWN_ADDRESS7;
  
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  
  //I2C 寻址模式
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  //通讯频率
  I2C_InitStructure.I2C_ClockSpeed = _I2C_Speed;
  //I2C 初始化
  I2C_Init(_EEPROM_I2Cx,&I2C_InitStructure);
  //使能I2C
  I2C_Cmd(_EEPROM_I2Cx,ENABLE);
}
  
/**************************************/

static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode){
  fn_Usart_SendString(_DEBUG_USARTx,"I2C 等待超时!errorCode =");
  printf("%d\n",errorCode);
 
  return 0;
}
/**************************************/

/*通讯等待超时时间*/
#define  I2CT_FLAG_TIMEOUT  ((uint32_t)0x6000)
#define  I2CT_LONG_TIMEOUT  ((uint32_t)(10*I2CT_FLAG_TIMEOUT))
static uint16_t I2CTimeout;
/**************************************/
/**
* @brief  写一个字节到 I2C EEPROM 中
* @param  pBuffer:缓冲区指针
* @param  WriteAddr:写地址
* @retval 正常返回 1，异常返回 0
*/
static uint32_t  I2C_EE_ByteWrite(u8* pBuffer, uint8_t WriteAddr ){
  
  I2CTimeout = I2CT_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(_EEPROM_I2Cx , ENABLE)){
    if((I2CTimeout--) == 0){return  I2C_TIMEOUT_UserCallback(4);}
  }
 
  //产生I2C起始信号
  I2C_GenerateSTART(_EEPROM_I2Cx , ENABLE);
  I2CTimeout = I2CT_LONG_TIMEOUT; //这个变量是延时异常时间
  //检测EV5 事件并清除标识位
  while(!I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT)){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(5);}
  } 
  
  //发送EEPROM 设备地址
  I2C_Send7bitAddress(_EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Transmitter);
  I2CTimeout = I2CT_LONG_TIMEOUT; //这个变量是延时异常时间
  //检测EV6 事件并清除标识位
  while(I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(6);}
  }
  
  //发送要写入的EEPROM 内部地址(即EEPROM内部存储其地址);
  I2C_SendData(_EEPROM_I2Cx,WriteAddr);
  I2CTimeout = I2CT_LONG_TIMEOUT;
  //检测EV8 事件清除标志位
  while(!I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(2);}
  }
   
  //发送要写入的EEPROM 内部的数据;
  I2C_SendData(_EEPROM_I2Cx,*pBuffer);
  I2CTimeout = I2CT_LONG_TIMEOUT;
  //检测EV8 事件清除标志位
  while(!I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(3);}
  }
  
  //发送要写入的EEPROM 内部的数据;
  I2C_SendData(_EEPROM_I2Cx,ENABLE);
  
  return 1;
}

 /**
  * @brief 将缓冲区中的数据写到 I2C EEPROM 中，采用单字节写入的方式，速度比页写入慢
  * @param pBuffer:缓冲区指针
  * @param WriteAddr:写地址
  * @param NumByteToWrite:写的字节数
  */

static void I2C_EE_WaitEepromStandbyState(void){
  vu16 SR1_Tmp = 0;
  do{
    //产生I2C起始信号
    I2C_GenerateSTART(_EEPROM_I2Cx,ENABLE);
    //读取I2C1 SR1 寄存器
    SR1_Tmp = I2C_ReadRegister(_EEPROM_I2Cx, I2C_Register_SR1);
    //发送EEPROM 地址+ 方向
    I2C_Send7bitAddress(_EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(_EEPROM_I2Cx, I2C_Register_SR1) & 0x0002));
  /* 清除 AF 位 */
  I2C_ClearFlag(_EEPROM_I2Cx, I2C_FLAG_AF);
  //发送停止位信号
  I2C_GenerateSTOP(_EEPROM_I2Cx , ENABLE);
}

//zuozuo04-30 

/**
* @brief 在 EEPROM 的一个写循环中可以写多个字节，但一次写入的字节数
* 不能超过 EEPROM 页的大小，AT24C02 每页有 8 个字节
* @param
* @param pBuffer:缓冲区指针
* @param WriteAddr:写地址
* @param NumByteToWrite:要写的字节数要求 NumByToWrite 小于页大小
* @retval 正常返回 1，异常返回 0
*/
static uint8_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite){
  I2CTimeout = I2CT_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(_EEPROM_I2Cx , ENABLE)){
    if((I2CTimeout--) == 0){return  I2C_TIMEOUT_UserCallback(4);}
  }
  //产生I2C起始信号
  I2C_GenerateSTART(_EEPROM_I2Cx , ENABLE);
  I2CTimeout = I2CT_LONG_TIMEOUT; //这个变量是延时异常时间
  //检测EV5 事件并清除标识位
  while(!I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT)){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(5);}
  }
  
  //发送EEPROM 设备地址
  I2C_Send7bitAddress(_EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Transmitter);
  I2CTimeout = I2CT_LONG_TIMEOUT; //这个变量是延时异常时间
  //检测EV6 事件并清除标识位
  while(I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(6);}
  }
  
  //发送要写入的EEPROM内部地址（EEPROM内部存储器地址）
  I2C_SendData(_EEPROM_I2Cx,WriteAddr);
  I2CTimeout = I2CT_LONG_TIMEOUT; //这个变量是延时异常时间
  //检测EV7 事件并清除标识位
  while(!I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING)){
    if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(7);}
  }

  //循环发送 NumByteToWrite个数据
  while(NumByteToWrite--){
    //发送缓冲区的数据
    I2C_SendData(_EEPROM_I2Cx,*pBuffer++);
    I2CTimeout = I2CT_FLAG_TIMEOUT; //这个变量是延时异常时间
    //检测EV8 事件并清除标识位
    while(!I2C_CheckEvent(_EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
      if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(8);}
    }       
  }
  //发送停止信号
  I2C_GenerateSTOP(_EEPROM_I2Cx,ENABLE);
  return 1;
}



/**  快速写入一页
* @brief 将缓冲区中的数据写到 I2C EEPROM 中
* @param
* @arg pBuffer:缓冲区指针
* @arg WriteAddr:写地址
* @arg NumByteToWrite:写的字节数
* @retval 无
*/
#define I2C_PageSize 8  //AT24C01 02 每页有8个字节
static void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite){
  u8 NumOfPage = 0, NumOfSingle = 0 , Addr = 0 , count = 0,temp = 0;
  //Mod 求余运算，如果writeAddr 是 I2C_PageSize 整书倍，运算结果位Addr为0
  Addr = WriteAddr % I2C_PageSize;
  //差count个数值，刚好可以对齐到页面地址
  count = I2C_PageSize - Addr; 
  //计算出要写多少整书页
  NumOfPage = NumByteToWrite / I2C_PageSize;
  //mod运算求余计算出不满一页的字节数
  NumOfSingle = NumByteToWrite % I2C_PageSize;
  // Addr = 0,则WriteAddr 刚好按页对齐aligned
  // 这样就很简单了，直接写就可以写完整页后
  // 把剩下的不满一页的写完即可
  if(Addr == 0){
    //如果 NumByteToWrite < I2C_PageSize
    if(NumOfPage==0){
      I2C_EE_PageWrite(pBuffer , WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }//如果NumByteToWrite > I2C_PageSize
    else{
      //先把整数页写了
      while(NumOfPage--){
        I2C_EE_PageWrite(pBuffer , WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += I2C_PageSize ;
        pBuffer += I2C_PageSize ;
      }
      //若有多余的不满一页的数据，把它写完
      if(NumOfSingle != 0){
        I2C_EE_PageWrite(pBuffer , WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  //如果 WriteAddr 不是按 I2C_PageSize 对齐
  //那就算出对齐到页地址还需要多少数据，然后先把这几个数据写完，剩下开始的地址就已经对齐
  //到页地址了，代码重复上面的即可
  else{
    //如果NumByteToWrite < I2C_PageSize
    if(NumOfPage == 0){
      //若NumOfSingle > count,当前面写不完，要写下一页
      if(NumOfSingle > count){
        temp = NumOfSingle - count;
        I2C_EE_PageWrite(pBuffer , WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count ;
        pBuffer += count ;
        
        I2C_EE_PageWrite(pBuffer , WriteAddr, temp);
        I2C_EE_WaitEepromStandbyState();
      }else{//若count 比 NumOfSingle大
        I2C_EE_PageWrite(pBuffer , WriteAddr, NumByteToWrite);
        I2C_EE_WaitEepromStandbyState(); 
      }
    }
    //如果 NumByteToWrite > I2C_PageSize
    else{
      //地址不对齐多出的Count 分开处理，不加入这个运算
      NumByteToWrite -= count;
      NumOfPage = NumByteToWrite / I2C_PageSize ;
      NumOfSingle = NumByteToWrite % I2C_PageSize;
      //先把 WriteAddr 所在页的剩余字节写了
      if(count!=0){
        I2C_EE_PageWrite(pBuffer , WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        //加上 count 后，地址就对齐到页了
        WriteAddr += count ;
        pBuffer += count ;
      }
      //把整页都写了
      while(NumOfPage--){
        I2C_EE_PageWrite(pBuffer , WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += I2C_PageSize ;
        pBuffer += I2C_PageSize ;
      }
      //若多余的不满足一页，就把它写完
      if(NumOfSingle !=0){
        I2C_EE_PageWrite(pBuffer , WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState(); 
      }
    }
    
  }
}



/*  EEPROM 读取
* @brief 从 EEPROM 里面读取一块数据
* @param pBuffer:存放从 EEPROM 读取的数据的缓冲区指针
* @param ReadAddr:接收数据的 EEPROM 的地址
* @param NumByteToRead:要从 EEPROM 读取的字节数
* @retval 正常返回 1，异常返回 0
*/

static uint8_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead){
    I2CTimeout = I2CT_LONG_TIMEOUT ;
    while(I2C_GetFlagStatus(_EEPROM_I2Cx , I2C_FLAG_BUSY)){
      if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(9);}
    }
    
    //产生I2C起始信号
    I2C_GenerateSTART(_EEPROM_I2Cx , ENABLE);
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    //检测EV10 事件并清除标注
    while(! I2C_CheckEvent(_EEPROM_I2Cx , I2C_EVENT_MASTER_MODE_SELECT)){
      if((I2CTimeout--)==0){ return I2C_TIMEOUT_UserCallback(10);}
    }
    
    //发送EEPROM 设备地址
    I2C_Send7bitAddress(_EEPROM_I2Cx , EEPROM_ADDRESS , I2C_Direction_Transmitter);
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    //检测EV11 事件并清除
    while(! I2C_CheckEvent(_EEPROM_I2Cx , I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
      if((I2CTimeout--)==0){ return I2C_TIMEOUT_UserCallback(11);}
    }
    
    //通过重新设置PE位清除EV12事件
    I2C_Cmd(_EEPROM_I2Cx ,ENABLE );
    //发送要读取的EEPROM内部地址（即EEPROM内部存储器地址）
    I2C_SendData(_EEPROM_I2Cx, ReadAddr);
    I2CTimeout = I2CT_FLAG_TIMEOUT ;
    //检测EV12 事件并清除
    while(! I2C_CheckEvent(_EEPROM_I2Cx , I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
      if((I2CTimeout--)==0){ return I2C_TIMEOUT_UserCallback(12);}
    }
    
    //产生第二次I2C起始信号
    I2C_GenerateSTART(_EEPROM_I2Cx , ENABLE);
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    //检测EV13 事件并清除
    while(! I2C_CheckEvent(_EEPROM_I2Cx ,  I2C_EVENT_MASTER_MODE_SELECT)){
      if((I2CTimeout--)==0){ return I2C_TIMEOUT_UserCallback(13);}
    }
    
    //发送EEPROM 设备地址
    I2C_Send7bitAddress(_EEPROM_I2Cx , EEPROM_ADDRESS , I2C_Direction_Receiver);
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    //检测EV14 事件并清除
    while(! I2C_CheckEvent(_EEPROM_I2Cx , I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
      if((I2CTimeout--)==0){ return I2C_TIMEOUT_UserCallback(14);}
    }
    
    //读取NumByteToRead个数据
    while(NumByteToRead){
      //如果 NumByteToRead=1，表示已经收到最后一个数据了
      //发送应答信号结束输出
      if(NumByteToRead == 1){
        //发送非应答信号
        I2C_AcknowledgeConfig(_EEPROM_I2Cx , DISABLE);
        
      }
      I2CTimeout = I2CT_LONG_TIMEOUT;
      while(I2C_CheckEvent(_EEPROM_I2Cx , I2C_EVENT_MASTER_BYTE_RECEIVED)==0){
        if((I2CTimeout--)==0){return I2C_TIMEOUT_UserCallback(3);}
      }
      
      //通过I2C，从设备中读取一个字节的数据
      *pBuffer = I2C_ReceiveData(_EEPROM_I2Cx);
      //存储数据的指针指以下地址
      pBuffer++;
      //接受数据自减
      NumByteToRead--;
    }
    //发送停止信号
        I2C_GenerateSTOP(_EEPROM_I2Cx , ENABLE);
    //使能大应，方便一下I2C输出
    I2C_AcknowledgeConfig(_EEPROM_I2Cx , ENABLE);
    return 1;
}

//--------------------------------------------------------

/**
* @brief void _I2C_EE_Init(void) 
* @param 无
* @retval 正常返回 1 ，不正常返回 0
*/
void _I2C_EE_Init(void){
  I2C_GPIO_Config(); 
  I2C_Mode_Config();
}

/**
* @brief I2C_Write_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite) 
* @param 无
* @retval 正常返回 1 ，不正常返回 0
*/
void I2C_Write_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite){
  u16 i;
  
  printf("I2C_写入数据 \n");
  I2C_EE_WaitEepromStandbyState();
  I2C_EE_PageWrite(pBuffer,WriteAddr, NumByteToWrite);
  for(i=0 ;i<NumByteToWrite ;i++ ){
    printf("%d ", *pBuffer++);
    if(i%16 == 15)    
        printf("\n\r");
  }
  printf("\nI2C_写入数据完成 \n");
  I2C_EE_WaitEepromStandbyState();
   
  for(i=0 ;i<NumByteToWrite ;i++ ){
    printf("%d ", pBuffer[i]);
    if(i%16 == 15)    
        printf("\n\r");
  }
}



/**
* @brief I2C(void I2C_Read_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite))读写测试
* @param 无
* @retval EEP_SIZE
*/
void I2C_Read_fun(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite){
  u16 i;
  
  printf("I2C_数据检测 \n");
  I2C_EE_BufferRead(pBuffer,WriteAddr,NumByteToWrite);
  printf("\nI2C_数据读取完毕 \n");
  for(i=0 ;i<NumByteToWrite ;i++ ){
    printf("%d ", pBuffer[i]);
    if(i%16 == 15)    
        printf("\n\r");
  }
   printf("\n--->I2C_数据检测完成\n");
}
   
