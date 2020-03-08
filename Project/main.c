/**************************************************************************
 * @文件  : main.c
 * @作者  : caochao
 * @日期  : 10-1-2018
 * @摘要  : 这个C文件时实现了系统的初始化和ucosII的启动
 *
 * 修改人 :
 *  日期  :
 *
 * 修改人 :
 *  日期  :
 **************************************************************************/
#include "stm32f10x.h"
#include <stdio.h>
#include "ff.h"
#include "rcc.h"
#include "usart.h"
#include "sdio_sd.h"

#define IAP_ADDR_START    0x8000000  //bootloder起始地址
#define IAP_FLASH_SIZE    0x0010000  //bootloderflash大小
#define APP_ADDR_START    (IAP_ADDR_START+IAP_FLASH_SIZE) //用户程序起始地址

/*----------------文件系统相关变量------------------------------------*/
FIL  fp;
FATFS  fs;
FRESULT Res;
/*------------------------------------------------------------------*/

/*------------------------FLASH页大小的定义 -------------------------*/
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

/*------------------数据缓存大小------------------------------------*/
uint16_t dataBuf0[FLASH_PAGE_SIZE/2];  //写入的数据缓存
uint16_t dataBuf1[FLASH_PAGE_SIZE/2];  //读取的数据缓存

/***********************************************************************
*   函数名: NVIC_DeInit
*   功  能: 恢复NVIC为复位状态.使中断不再发生.
*   输  入:   
*           无 
*   输  出:   
*           无
***********************************************************************/
void NVIC_DeInit(void) 
{ 
    uint32_t index = 0; 
    
    NVIC->ICER[0] = 0xFFFFFFFF; 
    NVIC->ICER[1] = 0x000007FF; 
    NVIC->ICPR[0] = 0xFFFFFFFF; 
    NVIC->ICPR[1] = 0x000007FF; 
    
    for(index = 0; index < 0x0B; index++) 
    { 
        NVIC->IP[index] = 0x00000000; 
    }  
}

/***********************************************************************
*   函数名: Jumpto_APP
*   功  能: IAP跳转到APP函数
*   输  入:   
*           无 
*   输  出:   
*           无
***********************************************************************/
void Jumpto_APP(void)
{
    uint32_t IapSpInitVal;
    uint32_t IapJumpAddr;
    void (*pIapFun)(void);

    RCC_DeInit();//关闭外设
    NVIC_DeInit(); //恢复NVIC为复位状态.使中断不再发生

    __set_PRIMASK(1); //IAP关中断 APP如果没用UCOS系统，APP初始化后要开中断 用UCOS后，在起动任务后会开中断
    NVIC_SetVectorTable(NVIC_VectTab_FLASH,IAP_FLASH_SIZE);
    
    IapSpInitVal = *(uint32_t *)APP_ADDR_START;
    IapJumpAddr =  *(uint32_t *)(APP_ADDR_START + 4);

    if((IapSpInitVal & 0x2FFE0000) != 0x20000000)//检查栈顶地址是否合法.
    {
        /*添加自己的bootloder启动失败的告警信息 必须是死循环
         *可以是指示灯或者是警报
         *
         *
         *
         */
    }
    
    if ((IapJumpAddr& 0xFFF8000) != 0x8000000)
    {
         /*添加自己的bootloder启动失败的告警信息 必须是死循环
         *可以是指示灯或者是警报
         *
         *
         *
         */
    }

    __set_MSP (IapSpInitVal);
    pIapFun = (void (*)(void))IapJumpAddr; //生成跳转函数.
    (*pIapFun)();  //跳转到指定位置运行程序
}

///***********************************************************************
//*   函数名: JumpToIAP
//*   功  能: APP跳转到IAP函数
//*   输  入:   
//*           无 
//*   输  出:   
//*           无
//***********************************************************************/
//void JumpToIAP(void)
//{
//    uint32_t IapSpInitVal;
//    uint32_t IapJumpAddr;
//    void (*pIapFun)(void);

////    RCC_DeInit(); //关闭外设
////    NVIC_DeInit(); //恢复NVIC为复位状态.使中断不再发生
//    
////    __set_PRIMASK(1); //关中断
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x00);

//    // APP如跳转前关中断，跳转到IAP后，IAP初始化后要打开中断
//    IapSpInitVal = *(uint32_t *)IAP_ADDR_START;
//    IapJumpAddr =  *(uint32_t *)(IAP_ADDR_START + 4);

//    if((IapSpInitVal & 0x2FFE0000) != 0x20000000)//检查栈顶地址是否合法.
//    {
//        /*添加自己的bootloder启动失败的告警信息  必须是死循环
//         *可以是指示灯或者是警报
//         *
//         *
//         *
//         */
//    }
//    
//    if ((IapJumpAddr& 0xFFF8000) != 0x8000000)
//    {
//         /*添加自己的bootloder启动失败的告警信息 必须是死循环
//         *可以是指示灯或者是警报 
//         *
//         *
//         *
//         */
//    }
// 
//    __set_CONTROL(0);

//    //进入用户级线程模式 进入软中断后才可以回到特权级线程模式
//    //APP如使用系统如ucos必须要有此过程否则跳到IAP后，无法再次跳到APP
//    __set_MSP (IapSpInitVal);

//    pIapFun = (void (*)(void))IapJumpAddr;
//    (*pIapFun) ();
//}

/***********************************************************************
*   函数名: System_LoadUpdateFile
*   功  能: 程序固件升级
*   输  入:   
*           addr：SD卡中的路径 
*   输  出:   
*           无
***********************************************************************/
void System_LoadUpdateFile(const char* addr)
{
    uint16_t i = 0;
    uint16_t page = 0;
    uint32_t readNum;   
    uint32_t tempAddr;
    FLASH_Status  status;  
    
    Res = f_mount (&fs,"0:",1);
	if (Res != FR_OK)  
	{
        printf("文件系统不存在,更新失败！");
		Jumpto_APP(); //跳转到原来的程序执行APP
	}
    else
    {
        printf("SD卡 挂在成功！ \r\n");
    }

    Res = f_open (&fp,addr, FA_READ);	
    if (Res != FR_OK)
    {
        printf("文件固件不存在,更新失败！");
        Jumpto_APP(); //跳转到原来的程序执行APP
    }
    else
    {
        printf("打开文件成功！ \r\n");
    }
    
    while (1)
    {
        tempAddr = APP_ADDR_START+page*FLASH_PAGE_SIZE; //计算要更新的flash页数
        
        Res = f_read(&fp,(uint8_t *)dataBuf0,FLASH_PAGE_SIZE,&readNum);	
        if (Res == FR_OK)
        {
            FLASH_Unlock();  //解除STM32的FALSH写保护
            
            status = FLASH_ErasePage(tempAddr);
            if ((status != FLASH_COMPLETE) && (page == 0))
            {
                FLASH_Lock(); //使能STM32的FALSH写保护
                
                printf("flash擦除失败,更新失败！");
                Jumpto_APP(); //跳转到原来的程序执行APP  
                
                break;  //如果正确跳转到原来的程序执 不会返回了
            }
            else if ((status != FLASH_COMPLETE) && (page != 0))
            {
                while(1)
                {
                   /*添加自己的bootloder启动失败的告警信息 必须是死循环
                    *可以是指示灯或者是警报 
                    *此时可能已经发生严重错误
                    */
                    
                    printf("固件升级发生严重错误！\r\n");
                }
            }
            
            for (i = 0; i < readNum/2; i++)
            {
                status = FLASH_ProgramHalfWord(tempAddr+2*i,dataBuf0[i]);  //解除STM32的FALSH写保护
                if (status != FLASH_COMPLETE)
                {
                    while(1)
                    {
                        /*添加自己的bootloder启动失败的告警信息 必须是死循环
                        *可以是指示灯或者是警报 
                        *此时可能已经发生严重错误
                        */
                        
                        printf("固件升级发生严重错误！\r\n");
                    }
                } 
            }

            FLASH_Lock(); //使能STM32的FALSH写保护
        }
        else
        { 
            if (page == 0)
            {
                printf("文件读取失败,更新失败！");
                Jumpto_APP(); //跳转到原来的程序执行APP
                
                break; //如果正确跳转到原来的程序执 不会返回了
            }
            else
            { 
                while(1)
                {
                   /*添加自己的bootloder启动失败的告警信息 必须是死循环
                    *可以是指示灯或者是警报 
                    *此时可能已经发生严重错误
                    */
                    
                    printf("固件升级发生严重错误！\r\n");
                }
            }
        }
        
        page++;  //更新flash页数
    }
    
    printf("固件升级成功！\r\n");
    Jumpto_APP(); //跳转到原来的程序执行APP
}

/**************************************************************************
* 函数名:  main
* 功  能:  程序的主函数
* 输  入:   
*         无 
* 输  出:   
*         有
**************************************************************************/
int main(void)
{
    RCC_Configuration();         //初始化系统时钟
    USART1_Configuration();      //初始化usart1 为了打印更新状态

    System_LoadUpdateFile("0:/System/Template.bin");     //加载固件	

	while(1)
	{
         ;
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    
    /* Infinite loop */
    while (1)
    {
		printf("Wrong parameters value: file %s on line %d\r\n", file, line);
    }
}
#endif

/**
  * @}
  */

/***********************************文件结束***************************************/


