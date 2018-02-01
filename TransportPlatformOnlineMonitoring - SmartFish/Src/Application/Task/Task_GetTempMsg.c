#include "Task_GetTempMsg.h"

void Task_GetTempMsg(void const *argument)
{
	(void) argument;
	
	const TickType_t delay = 1000 / portTICK_PERIOD_MS; //1000ms
	const TickType_t time_out = 10 / portTICK_PERIOD_MS; //��Ϣ���з��ͳ�ʱʱ��10ms
	float tempValue = 0;
	
	/**
	 * @note ����DS18B20ʱ��Ҫ���ϸ�������Ҫ�����ٽ�������
	 */
	taskENTER_CRITICAL();           //!< �����ٽ���
	while(DS18B20_Init())
	{
			RUN_LED = !RUN_LED;   //!< LED0״̬�л�
			DelayMs(400);
	}
	taskEXIT_CRITICAL();            //!< �˳��ٽ���
	
	for (;;)
	{
		//�����ٽ���
		taskENTER_CRITICAL();
		
		//Step1 :�鿴������״̬
		if (!DS18B20_Check())
		{
			xEventGroupSetBits(EventGroup_SystemStatus, DS18B20_BIT_2);
		}
		
		//Step2:��ȡ�¶�
		tempValue = DS18B20_Get_Temp();
		
		//Step3:�����¶�
		xQueueSend(Queue_TempMsg, &tempValue, time_out);
		
		//�˳��ٽ���
		taskEXIT_CRITICAL();

		vTaskDelay(delay);
	}	
}
