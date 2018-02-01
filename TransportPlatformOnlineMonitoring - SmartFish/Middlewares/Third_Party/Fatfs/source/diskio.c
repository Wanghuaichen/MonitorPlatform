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


FATFS *g_pfs[2];        //�ļ�ϵͳ����

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
            stat = NandFlash_Init(); //��ʼ��
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
		 	if(res) //STM32 SPI��bug,��sd������ʧ�ܵ�ʱ�������ִ����������,���ܵ���SPI��д�쳣
			{
//				SD_SPI_SpeedLow();
//				SD_SPI_ReadWriteByte(0xff);//�ṩ�����8��ʱ��
//				SD_SPI_SpeedHigh();
			}
  			break;
        }

        case DEV_FLASH :
        {
			for(;count>0;count--)
			{
                /**
                 * @brief (sector * NAND_SECTOR_SIZE)��ȡ������ַ
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
    return RES_OK; //��ʼ���ɹ�
    
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
                 * @brief (sector * NAND_SECTOR_SIZE)��ȡ������ַ
                 */    
                NandFlash_WriteOnePage((uint8_t*)buff, (sector * NAND_SECTOR_SIZE));
				sector++;
				buff += NAND_SECTOR_SIZE;
			}
			res=0;
			break;
        }
	}
    //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
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
	if(pdrv == DEV_SD_CARD)//SD��
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
    else if(pdrv == DEV_FLASH)	//�ⲿFLASH  
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
        res=RES_ERROR;//�����Ĳ�֧��
    }
    return res;
}

/**
 * @fn InitIO()
 * @brief IO��ʼ��
 * @return ����ʱ��
 */                                                                                         
DWORD get_fattime (void)
{				 
	return 0;
}

/**
 * @fn FAT_Mount()
 * @brief ���豸�������ļ�ϵͳ��
 * @param equipmentType ��Ҫ���ص�����
 * @param pathName ָ��Ҫ��װ/ж�ص��߼���������
 * @return ����ʱ��
 */      
uint8_t FAT_Mount(uint8_t equipmentType, char* pathName)
{
    uint8_t result = 0;
    FIL fil;
    FRESULT res;        //API�������
    BYTE workFlash[FF_MAX_SS];    //�������򣨴���ʱ��Խ��Խ��
    DWORD fre_clust, fre_sect, tot_sect;
    TCHAR str [256]; 
    
    switch(equipmentType)
    {
        case DEV_SD_CARD:
        {
//            res = f_mkfs("1:", FM_FAT32, 8, work, sizeof(work));
//            if(res == 0)
//            {
//                printf("SD CARD Disk Format Finish\r\n");	    //!< ��ʽ�����
//            }
//            else
//            {
//                result = 0xF1;
//                printf("SD CARD Disk Format Error \r\n");	    //!< ��ʽ��ʧ��
//            }
            res = f_mount(g_pfs[0], "1:", 1);    //!< ����SD
            if (res)
            {
                result = 0xF2;
                printf("File system mount failed.\r\n");
            }
            else
            {
                printf("SD CARD File system mount successfully.\r\n");
                res = f_getcwd(str,sizeof str / sizeof * str);  /*��ȡ��ǰĿ¼·�� */
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
                    
                    /*��ӡ���пռ䣨�ٶ�512�ֽ�/����)*/
                    printf("%10lu MB__Total drive space. \n %10lu MB__Available space.\n", tot_sect, fre_sect);
                }
            }
            break;
        }
        case DEV_FLASH:
        {
            res = f_mount(g_pfs[1], "0:", 1);    //!< ����FLASH.   
            if(res) //!< �����ز��ɹ������¸�ʽ�������
            {
                res = f_mkfs("0:", FM_ANY, 0, workFlash, sizeof(workFlash));
                if(res == FR_OK)
                {
                    f_setlabel((const TCHAR *)"0:0");	        //!< ����Flash���̵�����Ϊ��D
                    res = f_mount(g_pfs[1], "0:", 1);    //!< ����FLASH.            
                    if (res) 
                    {
                        return (int)res;
                    }
                    printf("Flash Disk Format Finish\r\n");	    //!< ��ʽ�����
                }
                else
                {
                    result = 0xF1;
                    printf("Flash Disk Format Error \r\n");	    //!< ��ʽ��ʧ��
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
            
            /*����ܿռ�����ɿռ�*/
            res = f_getfree("1:", &fre_clust, &g_pfs[1]);
            if(res == FR_OK)
            {
                tot_sect = (g_pfs[1]-> n_fatent - 2) * g_pfs[1]->csize;
                tot_sect = (tot_sect * 4) / 1024; //MB
                fre_sect = fre_clust * g_pfs[1]->csize;
                fre_sect = (fre_sect * 4) / 1024; //MB
                
                /*��ӡ���пռ䣨�ٶ�4096�ֽ�/����)*/
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

