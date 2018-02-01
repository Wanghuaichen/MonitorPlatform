/**
 * @file SD_ReadWrite.h
 * @brief SD_ReadWrite读写操作
 * copyright: Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * company: SOJO
 * @date 2017.12.27
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef _SD_READ_WRITE_H
#define _SD_READ_WRITE_H
#include "SystemConfig.h"

/**
 * @brief SD卡指令表
 */
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   
/**
 * @brief SD卡指令表
 */
#define CMD0    0       //!< 卡复位
#define CMD1    1
#define CMD8    8       //!< 命令8 ，SEND_IF_COND
#define CMD9    9       //!< 命令9 ，读CSD数据
#define CMD10   10      //!< 命令10，读CID数据
#define CMD12   12      //!< 命令12，停止数据传输
#define CMD16   16      //!< 命令16，设置SectorSize 应返回0x00
#define CMD17   17      //!< 命令17，读sector
#define CMD18   18      //!< 命令18，读Multi sector
#define CMD23   23      //!< 命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //!< 命令24，写sector
#define CMD25   25      //!< 命令25，写Multi sector
#define CMD41   41      //!< 命令41，应返回0x00
#define CMD55   55      //!< 命令55，应返回0x01
#define CMD58   58      //!< 命令58，读OCR信息
#define CMD59   59      //!< 命令59，使能/禁止CRC，应返回0x00
/**
 * @brief 数据写入回应字意义
 */
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
/**
 * @brief SD卡回应标记字
 */
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF

/***********************************************************************************
 *                                   函数申明区                                    *
 **********************************************************************************/
/**
 * @brief 硬件设备初始化,使用SIP模式驱动SD卡
 * Hardware initialisation.
 */
uint8_t SD_Init(void);								//!< 初始化
uint8_t SD_WaitReady(void);							//!< 等待SD卡准备
uint8_t SD_GetResponse(uint8_t Response);           //!< 获得回应
uint8_t SD_ReadDisk(uint8_t*buf, uint32_t sector, uint32_t cnt);    //!< 读块
uint8_t SD_WriteDisk(uint8_t*buf, uint32_t sector, uint32_t cnt);   //!< 写块
uint32_t SD_GetSectorCount(void);       //!< 读扇区数
uint8_t SD_GetCID(uint8_t *cid_data);   //!< 读SD卡CID
uint8_t SD_GetCSD(uint8_t *csd_data);   //!< 读SD卡CSD

/***********************************************************************************
 *                                   全局变量声明区                                *
 **********************************************************************************/
extern uint8_t  SD_Type;			//!< SD卡的类型


#endif

