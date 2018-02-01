/**
 * @file NandFlash.h
 * @brief NandFlash读写操作
 * copyright: Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * company: SOJO
 * @date 2017.12.27
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef _NAND_FLASH_H
#define _NAND_FLASH_H
#include "SystemConfig.h"

#define	NandFlash_CS    F_CS    //W25QXX的片选信号

#define NAND_SECTOR_SIZE    2048        //!< 一个扇区的大小,由于NandFlash的特殊性，故将其扇区大小设置为页的大小
#define NAND_PAGE_SIZE      NAND_SECTOR_SIZE
#define NAND_BLOCK_SIZE     64          //块大小
#define NAND_SECTOR_COUNT   NAND_BLOCK_SIZE * NAND_PAGE_SIZE * 1024

#define SPI_DUMMY   0x00
#define SPI_READ    0xFF

/**
 * @brief W25N01G指令表
 */ 

#define DEVICE_RESET    0xFF            //!< 复位芯片
#define READ_JEDEC_ID   0x9F            //!< 读取JEDEC ID
#define READ_REGISTER   0x05            //!< datasheet中提到读取寄存器值使用指令为0Fh/05h
#define WRITE_REGISTER  0x01            //!< datasheet中提到写入寄存器值使用指令为1Fh/01h
#define WRITE_ENABLE    0x06            //!< 写入使能
#define WRITE_DISABLE   0x04            //!< 写入禁止
#define LOAD_PROGRAM_DATA   0x02        //!< 写入程序数据
#define RANDOM_LOAD_PROGRAM_DATA  0x84  //!< 随机写入程序数据
#define PAGE_DATA_READ  0x13            //!< 按页读取数据
#define READ_DATA       0x03            //!< 读取数据
#define FAST_READ_DATA  0x0B            //!< 快速读取数据


//!< 手册中提到三个寄存器的地址为0xAxh、0xBxh、0xCxh
#define REGISTER_1_ADDERSS  0xA0        
#define REGISTER_2_ADDERSS  0xB0
#define REGISTER_3_ADDERSS  0xC0



/***********************************************************************************
 *                                   函数申明区                                    *
 **********************************************************************************/
extern uint8_t NandFlash_Init(void);
extern void NandFlash_DeviceRest(void);
extern void NandFlash_WriteOnePage(uint8_t *pDataBuffer, uint16_t writeAdderss);
extern void NandFlash_ReadPageData(uint8_t *pDataBuffer, uint16_t writeAdderss);

/***********************************************************************************
 *                                  全局变量声明区                                 *
 **********************************************************************************/


#endif

