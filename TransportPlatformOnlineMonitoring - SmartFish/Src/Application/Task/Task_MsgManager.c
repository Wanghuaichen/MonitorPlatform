#include "Task_MsgManager.h"

void Task_MsgManager(void const *argument)
{
	(void) argument;
	const TickType_t delay = 50 / portTICK_PERIOD_MS; //50ms
	
	uint8_t res;
	MSGStructTypeDef msg;
	
	for (;;)
	{
		//队列一定要注意收一次就收干净
		res = 0;
		res |= xQueueReceive(Queue_AttiAndAccMsg,&msg.atti_acc_msg,0) << 0;
		res |= xQueueReceive(Queue_TempMsg,&msg.temp_msg,0) << 1;
		res |= xQueueReceive(Queue_GpsMsg,&msg.gps_msg,0) << 2;
		
//		if ((res & 0x02) == 0x02)
//			printf("res:%d msg.temp_msg:%f\r\n",res,msg.temp_msg);
		
		xEventGroupSetBits(EventGroup_SystemStatus, NAND_FLASH_BIT_4);
		vTaskDelay(delay);
	}
}
