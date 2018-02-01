#include "Task_GetGpsMsg.h"

void Task_GetGpsMsg(void const *argument)
{
	(void) argument;
	
	const TickType_t delay = 1000 / portTICK_PERIOD_MS; //1000ms
	
	for (;;)
	{
		xEventGroupSetBits(EventGroup_SystemStatus, SIM868_BIT_3);
		vTaskDelay(delay);
	}	
}
