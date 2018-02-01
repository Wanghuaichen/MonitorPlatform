#ifndef __TASK_MSG_MANAGER_H
#define __TASK_MSG_MANAGER_H

#include "UserFreeRTOS.h"
#include "Task_GetAttiAndAccMsg.h"
#include "Task_GetGpsMsg.h"
#include "Task_Monitor.h"

typedef struct
{
	ADXL345StructTypeDef atti_acc_msg;
	SIM868StuctTypeDef gps_msg;
	float temp_msg;
}MSGStructTypeDef;

#endif
