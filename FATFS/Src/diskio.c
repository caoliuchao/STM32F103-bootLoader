/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "sdio_sd.h"

/* Definitions of physical drive number for each drive */
#define SD_Card	   0	/* Example: Map SD_Card harddisk to physical drive 0 */
#define MMC		   1	/* Example: Map MMC/SD card to physical drive 1 */
#define USB		   2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (BYTE pdrv)		/* Physical drive nmuber to identify the drive */
{
	DSTATUS res = RES_OK;

	switch (pdrv) 
	{
		case SD_Card :  res = RES_OK;
		                break;
		case     MMC :  res = RES_OK;
		                break;
		case     USB :  res = RES_OK;
		                break;
		default:  break;
	}
	
	return res;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DRESULT disk_initialize (BYTE pdrv)	/* Physical drive nmuber to identify the drive */
{
	DRESULT res = RES_ERROR;
	SD_Error result;

	switch (pdrv) 
	{
	   case SD_Card : result = SD_Init();
	                  if (result == SD_OK)
	                  {
					      res = RES_OK;
	                  } 
	                  break;
	    default:  break;
	}
    
	return res;
}

/*-----------------------------------------------------------------------
*     name ： disk_read (BYTE pdrv,BYTE *buff,DWORD sector,UINT count)
* function ： Read sector function of SD card
*    input ：
*             pdrv       Physical drive nmuber to identify the drive 
*             buff       DSD_Card buffer to store read dSD_Card 
*             sector     Sector address in LBA 
*             count      Number of sectors to read 
*   output ：
*             DRESULT   
*-----------------------------------------------------------------------*/
DSTATUS disk_read (BYTE pdrv,BYTE *buff,DWORD sector,UINT count)
{
	DSTATUS res = RES_ERROR;
	SD_Error result;

	switch (pdrv) 
	{
		case SD_Card :  result = SD_ReadDisk((uint8_t *)buff, sector, count);
						if (result == SD_OK)
						{
							res = RES_OK;
						} 
						break;
	    case MMC :
                   break;
		case USB :
			       break;
		  default:   
			       break;
	}

	return res;
}

/*-----------------------------------------------------------------------*
*     name ： disk_write (BYTE pdrv,const BYTE *buff,	DWORD sector,UINT count	)
* function ： write sector function of SD card
*    input ：
*             pdrv       Physical drive nmuber to identify the drive
*             buff       DSD_Card to be written
*             sector     Sector address in LBA
*             count      Number of sectors to write
*   output ：
*             DRESULT   
*-----------------------------------------------------------------------*/
DRESULT disk_write (BYTE pdrv,const BYTE *buff,	DWORD sector,UINT count	)
{
	DRESULT res = RES_ERROR;
	SD_Error result;

	switch (pdrv)
	{
		case SD_Card :  result = SD_WriteDisk((uint8_t *)buff, sector, count);
		                if (result == SD_OK)
						{
							res = RES_OK;
						} 
						else
						{
							SD_Init();	//重新初始化SD卡
							result = SD_WriteDisk((uint8_t *)buff, sector, count);
							if (result == SD_OK)
							{
								res = RES_OK;
							} 
						}
							
						break;
	    case MMC :
                   break;
		case USB :
			       break;
		  default:   
			       break;
	}
	return res;
}

/*-----------------------------------------------------------------------
*     name: disk_ioctl(BYTE pdrv,BYTE cmd,void *buff)
* function: Miscellaneous Functions      
*    input: 
*           pdrv Physical drive nmuber (0..) 
*           cmd  Control code 
*           buff Buffer to send/receive control dSD_Card 
*   output:
*           DRESULT
*-------------------------------------------------------------------------*/
DRESULT disk_ioctl(BYTE pdrv,BYTE cmd,void *buff)
{
	DRESULT res = RES_ERROR;
	
	switch (pdrv)
	{
		case SD_Card : 
			switch(cmd)
			{
				case CTRL_SYNC: res = RES_OK; //刷新磁盘
						        break;
				case GET_SECTOR_SIZE: 
					 *(WORD*)buff = 512; //得到媒体大小
                     res = RES_OK;
                     break;
				case GET_BLOCK_SIZE:
					*(WORD*)buff = SDCardInfo.CardBlockSize;  //块大小
                    res = RES_OK;
				    break;
                case GET_SECTOR_COUNT:
					*(DWORD*)buff = SDCardInfo.CardCapacity/512;
				    res = RES_OK;
					break;
				default: break;
			}
		default:break;
	}
	return res;
}

/*------------------------------------------------------------------
*
*-------------------------------------------------------------------*/
DWORD get_fattime(void)
{
	#if  USE_RTC == 1
	
	
	#else 
	
	     return 0;
	
	#endif
}

/******************************文件结束*********************************/



