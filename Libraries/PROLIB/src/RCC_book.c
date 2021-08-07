#include  "RCC_book.h"

/************************************************************
* @brief  
* void  fn_HRCC_SetSystic( uint32_t  _RCC_PLLMul_x );
* @param  
* @retval 
*************************************************************/ 
void  fn_HRCC_SetSystic( uint32_t  _RCC_PLLMul_x ){
  __IO  uint32_t  HSEStatus = 0;
  /* SYSCLK , HCLK , PCLK2 and PCLK1 cnfiguration */
  RCC_DeInit();
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  /* wait till HSE is ready and if Time Out is reached exit*/
  HSEStatus = RCC_WaitForHSEStartUp();
  if(HSEStatus == SUCCESS){
    // Enable Prefetch Buffer  缓冲区可开启
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    // Flash 2 wait state 
    FLASH_SetLatency(FLASH_Latency_2);
    // 这些位表示SYSCLK(系统时钟)周期与闪存访问的比例
    // SYSCLK 周期与闪存访问时间的比例设置，这里统一设置成 2
    // 设置成 2 的时候，SYSCLK 低于 48M 也可以工作，如果设置成 0 或者 1 的时候，
    // 如果配置的 SYSCLK 超出了范围的话，则会进入硬件错误，程序就死了
    // 0：0 < SYSCLK <= 24M
    // 1：24< SYSCLK <= 48M
    // 2：48< SYSCLK <= 72M */
    
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    /* HCLK = SYSCLK */     //AHB预分频 (AHB Prescaler) 
    RCC_PCLK1Config(RCC_HCLK_Div2);
    /* PCLK1 = HCLK */     //低速APB预分频(APB1) (APB low-speed prescaler (APB1))   
    RCC_PCLK2Config(RCC_HCLK_Div1);
    /* PCLK1 = HCLK */     //低速APB预分频(APB1) (APB low-speed prescaler (APB1))   
  
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1 ,_RCC_PLLMul_x);
    RCC_PLLCmd(ENABLE);
    /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz 
    //  PLLSRC： PLL输入时钟源 (PLL entry clock source)
    //  RCC_PLLSource_HSE_Div1 RCC_PLLSource_HSE_Div2 RCC_PLLSource_HSI_Div1
    //  _RCC_PLLMul_x  为倍频因子*/
    while((RCC_GetFlagStatus(RCC_FLAG_PLLRDY))==RESET);
     /* Wait till PLL is ready */
    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
     /* Select PLL as system clock source */
    //		SW[1:0]：系统时钟切换 (System clock switch)  位1:0
    //		由软件置’1’或清’0’来选择系统时钟源。
    //		在从停止或待机模式中返回时或直接或间接作为系统时钟的HSE出现故障时，由硬件强制选择
    //		HSI作为系统时钟(如果时钟安全系统已经启动)
    //		00：HSI作为系统时钟；
    //		01：HSE作为系统时钟；
    //		10：PLL输出作为系统时钟；
    //		11：不可用  */
    
    while (RCC_GetSYSCLKSource()!=0x08);  
  }else{
     
  }
}

/************************************************************
* @brief  
* void  fn_IRCC_SetSystic( uint32_t  _RCC_PLLMul_x );
* @param  
* @retval 
*************************************************************/ 
void  fn_IRCC_SetSystic( uint32_t  _RCC_PLLMul_x ){
  __IO uint32_t HSIStatus = 0;
  __IO uint32_t StartUpCounter = 0;
  /*SYSCLK  HCLK PCLK2 PCLK1 con figuration---*/
  RCC_DeInit();
  
  /*Enable HSE*/
  RCC_HSICmd(ENABLE);
  
  /* Wait till HSE is ready and if Time out is reached exit */
  do{
     HSIStatus = RCC->CR & RCC_CR_HSIRDY;
     StartUpCounter++;
  }while((HSIStatus == 0)&&(StartUpCounter != HSE_STARTUP_TIMEOUT));
  
  if(HSIStatus == SUCCESS){
     /*Enable Prefetch Buffer 预计缓冲区开启 */
     FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
     /*FLASH 2 Wait state */ //这些位表示SYSCLK(系统时钟)周期与闪存访问时间的比例  
     FLASH_SetLatency(FLASH_Latency_2);
    // 这些位表示SYSCLK(系统时钟)周期与闪存访问的比例
    // SYSCLK 周期与闪存访问时间的比例设置，这里统一设置成 2
    // 设置成 2 的时候，SYSCLK 低于 48M 也可以工作，如果设置成 0 或者 1 的时候，
    // 如果配置的 SYSCLK 超出了范围的话，则会进入硬件错误，程序就死了
    // 0：0 < SYSCLK <= 24M
    // 1：24< SYSCLK <= 48M
    // 2：48< SYSCLK <= 72M */
    
      RCC_HCLKConfig(RCC_SYSCLK_Div1);
      /* HCLK = SYSCLK */     //AHB预分频 (AHB Prescaler) 
      RCC_PCLK1Config(RCC_HCLK_Div2);
      /* PCLK1 = HCLK */     //低速APB预分频(APB1) (APB low-speed prescaler (APB1))   
      RCC_PCLK2Config(RCC_HCLK_Div1);
      /* PCLK1 = HCLK */     //低速APB预分频(APB1) (APB low-speed prescaler (APB1))   
  
      RCC_PLLConfig(RCC_PLLSource_HSI_Div2,_RCC_PLLMul_x);
      RCC_PLLCmd(ENABLE);
      /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz 
      //  PLLSRC： PLL输入时钟源 (PLL entry clock source)
      //  RCC_PLLSource_HSE_Div1 RCC_PLLSource_HSE_Div2 RCC_PLLSource_HSI_Div1
      //  _RCC_PLLMul_x  为倍频因子*/
      
      while((RCC_GetFlagStatus(RCC_FLAG_PLLRDY))== RESET);
      /* Wait till PLL is ready */
      
      RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
      /* Select PLL as system clock source */
      
      //		SW[1:0]：系统时钟切换 (System clock switch)  位1:0
      //		由软件置’1’或清’0’来选择系统时钟源。
      //		在从停止或待机模式中返回时或直接或间接作为系统时钟的HSE出现故障时，由硬件强制选择
      //		HSI作为系统时钟(如果时钟安全系统已经启动)
      //		00：HSI作为系统时钟；
      //		01：HSE作为系统时钟；
      //		10：PLL输出作为系统时钟；
      //		11：不可用
          
      /** Wait till PLL is used as system clock source
            - 0x00: HSI used as system clock
            - 0x04: HSE used as system clock
            - 0x08: PLL used as system clock
      */
      while(RCC_GetSYSCLKSource()!=0x08);
  }else{
    
  }

}

/************************************************************
* @brief  
* void  fn_MCO_GPIO_Config(void);
* @param  
*    #define   RCC_OUT_GPIO_Port     GPIOA                 //GPIO Point
*    #define   RCC_OUT_GPIO_Clock    RCC_APB2Periph_GPIOA  //GPIO clock
*    #define   RCC_OUT_GPIO_Pin      GPIO_Pin_8             
*    #define   RCC_OUT_GPIO_Pin_Bit  8
*    #define   RCC_OUT_GPIO_Modle    GPIO_Mode_AF_PP
*    #define   RCC_OUT_GPIO_Speed    GPIO_Speed_10MHz
* @retval 
*************************************************************/ 
void  fn_MCO_GPIO_Config(void){
   GPIO_InitTypeDef  GPIO_InitStruct;
   GPIO_InitStruct.GPIO_Mode = RCC_OUT_GPIO_Modle;
   GPIO_InitStruct.GPIO_Pin = RCC_OUT_GPIO_Pin;
   GPIO_InitStruct.GPIO_Speed = RCC_OUT_GPIO_Speed;
   
   RCC_APB2PeriphClockCmd(RCC_OUT_GPIO_Clock,ENABLE);
   GPIO_Init(RCC_OUT_GPIO_Port,&GPIO_InitStruct);
   RCC_MCOConfig(RCC_MCO_SYSCLK);
   //RCC_MCOConfig(RCC_MCO_HSI);
   //RCC_MCOConfig(RCC_MCO_HSE);
}

/************************************************************
* @brief  
* void fn_RCC_Init(void);
* @param  
* @retval 
*************************************************************/ 
void fn_RCC_Init(void){
    fn_HRCC_SetSystic(RCC_PLLMul_9 );
    //fn_IRCC_SetSystic(RCC_PLLMul_9 );
    //fn_MCO_GPIO_Config();
}

