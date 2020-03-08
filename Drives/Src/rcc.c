/**************************************************************************
* @文件  : rcc.c
* @作者  : caochao
* @日期  : 10-1-2018
* @摘要  : 这个C文件包含了系统时钟的设置的实现
* 修改人 :
*  日期  :
*
* 修改人 :
*  日期  :
************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"
#include "rcc.h"

/*************************************************************************
*   函数名:   RCC_Configuration
*   功  能:   设置STM32系统时钟
*   输  入:  
*             无
*   输  出:  
*             有
*
************************************************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	
	RCC_DeInit();                           //RCC寄存器初始化                  
	RCC_HSEConfig(RCC_HSE_ON);             //打开外部高速晶振     
	HSEStartUpStatus = RCC_WaitForHSEStartUp();   //等待外部高速晶振起振  
    
	if (HSEStartUpStatus == SUCCESS)        //判断外部高速晶振是否起振成功
	{
		RCC_HCLKConfig( RCC_SYSCLK_Div1);      //设置AHB时钟（HCLK）   
		RCC_PCLK1Config(RCC_HCLK_Div2);        //设置低速AHB时钟（PCLK1）   
		RCC_PCLK2Config(RCC_HCLK_Div1);        //设置高速AHB时钟（PCLK2）    
		
		FLASH_SetLatency(FLASH_Latency_2);                         
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);      
		
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);   //设置PLL时钟源及倍频系数
		RCC_PLLCmd(ENABLE);                                   //使能PLL时钟
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
			;                       //检查指定的RCC标志位设置与否,PLL就绪
		} 
		
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    //设置系统时钟（SYSCLK）
		while (RCC_GetSYSCLKSource() != 0x08)
		{
			;                       //返回用作系统时钟的时钟源,0x08：PLL作为系统时钟 
		}			
	}
}

/********************************文件结束*************************************/

