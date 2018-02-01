#include "Task_Monitor.h"

void Task_Monitor(void const *argument)
{
	(void) argument;
	
	const TickType_t time_out = 5000 / portTICK_PERIOD_MS; //事件等待超时时间发送超时时间5s
	
	uint8_t sysStatus = 0;
	
	
	for (;;)
	{
		sysStatus = xEventGroupWaitBits(EventGroup_SystemStatus,
                                    BIT_ALL,
                                    pdTRUE,
                                    pdTRUE,
                                    time_out);

		if ((sysStatus & BIT_ALL) == BIT_ALL)
		{
		//	printf("Everything is OK!\r\n");
		}
		else if (!(sysStatus & ADXL345_1_BIT_0))
		{
			printf("ADXL354_1 was dead!\r\n");
		}
		else if (!(sysStatus & ADXL345_2_BIT_1))
		{
			printf("ADXL354_2 was dead!\r\n");
		}
		else if (!(sysStatus & DS18B20_BIT_2))
		{
			printf("DS18B20 was dead!\r\n");
		}
		else if (!(sysStatus & SIM868_BIT_3))
		{
			printf("SIM868 was dead!\r\n");
		}
		else if (!(sysStatus & NAND_FLASH_BIT_4))
		{
			printf("NAND_FLASH was dead!\r\n");
		}

	}
}
