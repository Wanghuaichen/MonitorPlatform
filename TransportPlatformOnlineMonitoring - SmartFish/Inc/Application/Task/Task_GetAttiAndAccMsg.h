#ifndef __TASK_ATTI_ACC_H
#define __TASK_ATTI_ACC_H

#include "UserFreeRTOS.h"
#include "Task_Monitor.h"
#include "ADXL345.h"
#include "DriveIO.h"

typedef struct
{
	float pitch;
	float roll;
	float acc_x;
	float acc_y;
	float acc_z;
}ADXL345StructTypeDef;

#endif

