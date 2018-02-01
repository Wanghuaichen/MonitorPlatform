#ifndef _TASK_DEBUG_H
#define _TASK_DEBUG_H

#include "DriveUSART.h"
#include "UserFreeRTOS.h"
#include "SysParam.h"

#if (DEBUG_MODE == 1)

void Task_Debug(void const *argument);

#endif

#endif

