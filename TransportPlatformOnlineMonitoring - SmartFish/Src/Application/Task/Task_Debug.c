#include "Task_Debug.h"

#if (DEBUG_MODE == 1)

void Task_Debug(void const *argument)
{
	(void)argument;
	
	const TickType_t delay = 1000 / portTICK_PERIOD_MS; //1000ms
	
	uint8_t pcWriteBuffer[500];
	
	for (;;)
	{
		
		printf("===================TaskMsgReport=====================\r\n\r\n");
		printf("任务名                 任务状态 优先级 剩余栈 任务序号\r\n");
		vTaskList((char *)&pcWriteBuffer);
		printf("%s\r\n", pcWriteBuffer);
		printf("\r\n任务名                  运行计数       使用率\r\n");
		vTaskGetRunTimeStats((char *)&pcWriteBuffer);
		printf("%s\r\n", pcWriteBuffer);
		printf("=====================================================\r\n");

		vTaskDelay(delay);
	}
}

#endif

