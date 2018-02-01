/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

#include "NandFlash.h"
#include "SD_ReadWrite.h"
#include "DriveIO.h"


FATFS *g_pfs[2];        //文件系统对象

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) 
    {
        case DEV_FLASH :
        case DEV_SD_CARD :
        {
            stat = RES_OK;
            break;
        }
        case DEV_USB:
        default:
        {
            stat = STA_NOINIT;
            break;
        }
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
    
	switch (pdrv) 
    {
        case DEV_SD_CARD:
        {
			stat = SD_Init();    //SD_Initialize() 
  			break;
        }

        case DEV_FLASH:
        {
            stat = NandFlash_Init(); //初始化
 			break;
        }
        case DEV_USB:
        default:
        {
            stat = STA_NOINIT;
        }
	}
    
    return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	uint8_t res;

	switch (pdrv) 
    {
        case DEV_SD_CARD :
        {
			res = SD_ReadDisk(buff, sector, count);   //SD_Initialize() 
		 	if(res) //STM32 SPI的bug,在sd卡操作失败的时候如果不执行下面的语句,可能导致SPI读写异常
			{
//				SD_SPI_SpeedLow();
//				SD_SPI_ReadWriteByte(0xff);//提供额外的8个时钟
//				SD_SPI_SpeedHigh();
			}
  			break;
        }

        case DEV_FLASH :
        {
			for(;count>0;count--)
			{
                /**
                 * @brief (sector * NAND_SECTOR_SIZE)获取扇区地址
                 */    
				NandFlash_ReadPageData(buff, (sector * NAND_SECTOR_SIZE));
				sector++;
				buff += NAND_SECTOR_SIZE;
			}
			res=0;
            break;
        }
        default:
        {
            res = 1;
            break;
        }
	}

	if(res)
    {
        return  RES_PARERR;
    }
    return RES_OK; //初始化成功
    
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	uint8_t res;

	switch (pdrv) 
    {
        case DEV_SD_CARD :
        {
            res = SD_WriteDisk((uint8_t*)buff, sector, count);
            break;
        }
        case DEV_FLASH :
        {
            for(;count>0;count--)
			{
                /**
                 * @brief (sector * NAND_SECTOR_SIZE)获取扇区地址
                 */    
                NandFlash_WriteOnePage((uint8_t*)buff, (sector * NAND_SECTOR_SIZE));
				sector++;
				buff += NAND_SECTOR_SIZE;
			}
			res=0;
			break;
        }
	}
    //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res == 0x00)
    {
        return RES_OK;	 
    }
    return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;
//	DWORD nFrom,nTo;		
	if(pdrv == DEV_SD_CARD)//SD卡
	{
	    switch(cmd)
	    {
		    case CTRL_SYNC:
				SD_CS=0;
		        if(SD_WaitReady()==0)res = RES_OK; 
		        else res = RES_ERROR;	  
				SD_CS=1;
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = 512;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = 8;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = SD_GetSectorCount();
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}
    else if(pdrv == DEV_FLASH)	//外部FLASH  
	{
	    switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;
		    case GET_SECTOR_SIZE:
		        *(DWORD*)buff = NAND_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(DWORD*)buff = NAND_BLOCK_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = NAND_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}
    else 
    {
        res=RES_ERROR;//其他的不支持
    }
    return res;
}

/**
 * @fn InitIO()
 * @brief IO初始化
 * @return 返回时间
 */                                                                                         
DWORD get_fattime (void)
{				 
	return 0;
}

/**
 * @fn FAT_Mount()
 * @brief 将设备挂载在文件系统内
 * @param equipmentType 需要挂载的类型
 * @param pathName 指向要安装/卸载的逻辑驱动器号
 * @return 返回时间
 */      
uint8_t FAT_Mount(uint8_t equipmentType, char* pathName)
{
    uint8_t result = 0;
    FIL fil;
    FRESULT res;        //API结果代码
    BYTE workFlash[FF_MAX_SS];    //工作区域（处理时间越长越好
    DWORD fre_clust, fre_sect, tot_sect;
    TCHAR str [256]; 
    
    switch(equipmentType)
    {
        case DEV_SD_CARD:
        {
//            res = f_mkfs("1:", FM_FAT32, 8, work, sizeof(work));
//            if(res == 0)
//            {
//                printf("SD CARD Disk Format Finish\r\n");	    //!< 格式化完成
//            }
//            else
//            {
//                result = 0xF1;
//                printf("SD CARD Disk Format Error \r\n");	    //!< 格式化失败
//            }
            res = f_mount(g_pfs[0], "1:", 1);    //!< 挂载SD
            if (res)
            {
                result = 0xF2;
                printf("File system mount failed.\r\n");
            }
            else
            {
                printf("SD CARD File system mount successfully.\r\n");
                res = f_getcwd(str,sizeof str / sizeof * str);  /*获取当前目录路径 */
                res = f_open(&fil, "1:Test.txt", FA_READ);
                if (res) 
                {
                    return (int)res;
                }
                
                f_close(&fil);

                res = f_getfree("1:", &fre_clust, &g_pfs[0]);

                if(res == FR_OK)
                {
                    tot_sect = (g_pfs[0]-> n_fatent - 2) * g_pfs[0]->csize;
                    tot_sect /= 2048; //MB
                    fre_sect = fre_clust * g_pfs[0]->csize;
                    fre_sect /= 2048; //MB
                    
                    /*打印空闲空间（假定512字节/扇区)*/
                    printf("%10lu MB__Total drive space. \n %10lu MB__Available space.\n", tot_sect, fre_sect);
                }
            }
            break;
        }
        case DEV_FLASH:
        {
            res = f_mount(g_pfs[1], "0:", 1);    //!< 挂载FLASH.   
            if(res) //!< 若挂载不成功则重新格式化后挂载
            {
                res = f_mkfs("0:", FM_ANY, 0, workFlash, sizeof(workFlash));
                if(res == FR_OK)
                {
                    f_setlabel((const TCHAR *)"0:0");	        //!< 设置Flash磁盘的名字为：D
                    res = f_mount(g_pfs[1], "0:", 1);    //!< 挂载FLASH.            
                    if (res) 
                    {
                        return (int)res;
                    }
                    printf("Flash Disk Format Finish\r\n");	    //!< 格式化完成
                }
                else
                {
                    result = 0xF1;
                    printf("Flash Disk Format Error \r\n");	    //!< 格式化失败
                }
            }
            else
            {
                printf("File system mount successfully.\r\n");
            }
            res = f_open(&fil, "0:/Test.txt",  FA_READ);
            if (res) 
            {
                return (int)res;
            }
            f_close(&fil);
            
            /*获得总空间和自由空间*/
            res = f_getfree("1:", &fre_clust, &g_pfs[1]);
            if(res == FR_OK)
            {
                tot_sect = (g_pfs[1]-> n_fatent - 2) * g_pfs[1]->csize;
                tot_sect = (tot_sect * 4) / 1024; //MB
                fre_sect = fre_clust * g_pfs[1]->csize;
                fre_sect = (fre_sect * 4) / 1024; //MB
                
                /*打印空闲空间（假定4096字节/扇区)*/
                printf("%10lu MB__Total drive space. \n %10lu MB__Available space.\n", tot_sect, fre_sect);
            }
            break;
        }
        default:
        {
            result = 0x01;
        }
    }
    return result;
}

