#include "Task_GetTempMsg.h"

void Task_GetTempMsg(void const *argument)
{
	(void) argument;
	
	const TickType_t delay = 1000 / portTICK_PERIOD_MS; //1000ms
	const TickType_t time_out = 10 / portTICK_PERIOD_MS; //消息队列发送超时时间10ms
	float tempValue = 0;
	
	/**
	 * @note 由于DS18B20时序要求严格，所以需要进行临界区处理
	 */
	taskENTER_CRITICAL();           //!< 进入临界区
	while(DS18B20_Init())
	{
			RUN_LED = !RUN_LED;   //!< LED0状态切换
			DelayMs(400);
	}
	taskEXIT_CRITICAL();            //!< 退出临界区
	
	for (;;)
	{
		//进入临界区
		taskENTER_CRITICAL();
		
		//Step1 :查看传感器状态
		if (!DS18B20_Check())
		{
			xEventGroupSetBits(EventGroup_SystemStatus, DS18B20_BIT_2);
		}
		
		//Step2:获取温度
		tempValue = DS18B20_Get_Temp();
		
		//Step3:发出温度
		xQueueSend(Queue_TempMsg, &tempValue, time_out);
		
		//退出临界区
		taskEXIT_CRITICAL();

		vTaskDelay(delay);
	}	
}
