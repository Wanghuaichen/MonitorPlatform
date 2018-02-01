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
		printf("������                 ����״̬ ���ȼ� ʣ��ջ �������\r\n");
		vTaskList((char *)&pcWriteBuffer);
		printf("%s\r\n", pcWriteBuffer);
		printf("\r\n������                  ���м���       ʹ����\r\n");
		vTaskGetRunTimeStats((char *)&pcWriteBuffer);
		printf("%s\r\n", pcWriteBuffer);
		printf("=====================================================\r\n");

		vTaskDelay(delay);
	}
}

#endif

