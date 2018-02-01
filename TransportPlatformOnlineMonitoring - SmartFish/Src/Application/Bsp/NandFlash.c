/**
 * @file NandFlash.c
 * @brief NandFlash读写操作
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2017.12.27
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
 
#include "DriveIO.h"
#include "DriveSPI.h"
#include "NandFlash.h"

#define JEDEC_ID    0x00EFAA21
/**
 * @brief 读取NandFlash的JEDEC ID 号
 */
static void NandFlash_ReadID(uint32_t *pID); 
static void NandFlash_ReadStatusRegister(uint8_t registerAddress, uint8_t *pReadRegisterValue);
static void NandFlash_WriteStatusRegister(uint8_t registerAddress, uint8_t writeRegisterValue);
static void NandFlash_DeviceRest(void);
static void NandFlash_WriteEnable(void);
static void NandFlash_WriteDisable(void);
static void NandFlash_WaitBusy(void);

/**
 * @fn NandFlash_Init()
 * @brief 初始化NandFlash
 * @note 
 * 
 * 
 *
 * @return 若初始化成功返回0,否则失败
 */
uint8_t NandFlash_Init(void)
{
    uint32_t id = 0;
	NandFlash_CS = 1;			            //!< FLASH不选中
	Init_SPI();		   			            //!< 初始化SPI
	SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_2); //!< 设置高速模式
    NandFlash_ReadID(&id);	                //!< 读取FLASH ID.
    if(id == JEDEC_ID)
    {
        return 0;
    }
    return 1;
}  

/**
 * @fn NandFlash_ReadID()
 * @brief 读取芯片JEDEC ID
 * @param pID 芯片的JEDEC ID 号
 */
void NandFlash_ReadID(uint32_t *pID)
{
	NandFlash_CS = 0;   //!< 使能NandFlash	    
    *pID = 0;            //!< Clear
	SPI1_ReadWriteByte(READ_JEDEC_ID);   //!< 发送读取ID命令	    
	SPI1_ReadWriteByte(SPI_DUMMY);	 			   
	*pID |= SPI1_ReadWriteByte(SPI_READ) << 16;  	   
	*pID |= SPI1_ReadWriteByte(SPI_READ) << 8;  
	*pID |= SPI1_ReadWriteByte(SPI_READ);	 
	NandFlash_CS = 1;	        //!< 取消片选
}

/**
 * @fn NandFlash_ReadStatusRegister()
 * @brief 读取芯片状态寄存器值
 * @param pReadRegisterValue 指向从寄存器中读取的值
 */
void NandFlash_ReadStatusRegister(uint8_t registerAddress, uint8_t *pReadRegisterValue)
{
	NandFlash_CS = 0;   //!< 使能NandFlash	 
    SPI1_ReadWriteByte(READ_REGISTER);
    SPI1_ReadWriteByte(registerAddress);
    *pReadRegisterValue = SPI1_ReadWriteByte(SPI_READ);  //!< 读取相应状态寄存器的值
	NandFlash_CS = 1;   //!< 取消片选
}

/**
 * @fn NandFlash_WriteStatusRegister()
 * @brief 向芯片寄存器中写入值
 * @param writeRegisterValue 要向寄存器中写入的值
 */
void NandFlash_WriteStatusRegister(uint8_t registerAddress, uint8_t writeRegisterValue)
{
	NandFlash_CS = 0;   //!< 使能NandFlash	 
    SPI1_ReadWriteByte(WRITE_REGISTER);
    SPI1_ReadWriteByte(registerAddress);
    SPI1_ReadWriteByte(writeRegisterValue);  //!< 读取相应状态寄存器的值
	NandFlash_CS = 1;   //!< 取消片选
}

/**
 * @fn NandFlash_DeviceRest()
 * @brief 复位器件
 */
void NandFlash_DeviceRest(void)
{
	NandFlash_CS = 0;   //!< 使能NandFlash	 
    SPI1_ReadWriteByte(DEVICE_RESET);
    // TODO 需要增加延时函数
	NandFlash_CS = 1;   //!< 取消片选
}

/**
 * @fn NandFlash_WriteEnable()
 * @brief 写使能
 */
void NandFlash_WriteEnable(void)
{
	NandFlash_CS = 0;   //!< 使能NandFlash	 
    SPI1_ReadWriteByte(WRITE_ENABLE);
    // TODO 需要增加延时函数
	NandFlash_CS = 1;   //!< 取消片选
}

/**
 * @fn NandFlash_WriteDisable()
 * @brief 禁止写入
 */
void NandFlash_WriteDisable(void)
{
	NandFlash_CS = 0;   //!< 使能NandFlash	 
    SPI1_ReadWriteByte(WRITE_DISABLE);
    // TODO 需要增加延时函数
	NandFlash_CS = 1;   //!< 取消片选
}

/**
 * @fn NandFlash_WaitBusy()
 * @brief 等待BUSY信号空闲
 */
void NandFlash_WaitBusy(void)
{
    uint8_t registerData = 0;
    do
    {
        NandFlash_ReadStatusRegister(REGISTER_3_ADDERSS, &registerData);
    }while((registerData & 0x01) == 0x01);  //!< 等待空闲
}

/**
 * @fn NandFlash_WriteOnePage()
 * @brief 在指定地址开始写入最大256字节的数据，一页
 * @param pDataBuffer 数据存储区
 * @param writeAdderss 写入的数据地址
 * 
 */
void NandFlash_WriteOnePage(uint8_t *pDataBuffer, uint16_t writeAdderss)
{
    NandFlash_WriteEnable();
	NandFlash_CS = 0;   //!< 使能NandFlash	
    SPI1_ReadWriteByte(LOAD_PROGRAM_DATA);  //!< 发送写页命令
    /**
     * @brief 写入的地址
     */
    SPI1_ReadWriteByte((uint8_t)((writeAdderss >> 8) & 0x00FF));    //!< 写入高8位地址
    SPI1_ReadWriteByte((uint8_t)(writeAdderss & 0x00FF));           //!< 写入低8位地址
    for(uint16_t i = 0; i < NAND_PAGE_SIZE; i++)
    {
        SPI1_ReadWriteByte(pDataBuffer[i]);  //!< 循环写入缓冲区内的数据
    }
	NandFlash_CS = 1;           //!< 取消片选
    NandFlash_WaitBusy();       //!< 等待Flash写操作完成
    NandFlash_WriteDisable();   //!< 禁止写
}

/**
 * @fn NandFlash_ReadPageData()
 * @brief 在指定地址开始读取最大2112字节的数据，一页
 * @param pDataBuffer 数据存储区
 * @param writeAdderss 读取的数据地址
 * 
 */
void NandFlash_ReadPageData(uint8_t *pDataBuffer, uint16_t writeAdderss)
{
	NandFlash_CS = 0;   //!< 使能NandFlash	
    SPI1_ReadWriteByte(PAGE_DATA_READ);  //!< 发送读页命令
    /**
     * @brief 写入的地址
     */
    SPI1_ReadWriteByte((uint8_t)((writeAdderss >> 8) & 0x00FF));    //!< 写入高8位地址
    SPI1_ReadWriteByte((uint8_t)(writeAdderss & 0x00FF));           //!< 写入低8位地址
    for(uint16_t i = 0; i < NAND_PAGE_SIZE; i++)
    {
        pDataBuffer[i] = SPI1_ReadWriteByte(SPI_READ);  //!< 循环写入缓冲区内的数据
    }
	NandFlash_CS = 1;       //!< 取消片选
    NandFlash_WaitBusy();   //!< 等待Flash读操作完成
}

