#ifndef __TASK_MONITOR_H
#define __TASK_MONITOR_H

#include "UserFreeRTOS.h"
#include "SysParam.h"

/*各传感器标志位*/
#define ADXL345_1_BIT_0  (1 << 0)
#define ADXL345_2_BIT_1  (1 << 1)
#define DS18B20_BIT_2    (1 << 2)
#define SIM868_BIT_3     (1 << 3)
#define NAND_FLASH_BIT_4 (1 << 4)
#define BIT_ALL          (ADXL345_1_BIT_0 | ADXL345_2_BIT_1 | DS18B20_BIT_2 | SIM868_BIT_3 | NAND_FLASH_BIT_4)

#endif
