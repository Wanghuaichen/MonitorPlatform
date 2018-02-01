#include "Task_GetAttiAndAccMsg.h"

void Task_GetAttiAndAccMsg(void const *argument)
{
	(void) argument;
	
	const TickType_t delay = 500 / portTICK_PERIOD_MS;   //500ms
	const TickType_t time_out = 10 / portTICK_PERIOD_MS; //��Ϣ���з��ͳ�ʱʱ��10ms

	ADXL345StructTypeDef atti_acc_msg[3];
	
	uint8_t res[2] = {0xFF,0xFF};

	int16_t data[3];
	
  for (;(res[0] != 0)&&(res[1] != 0);)                //!< ADXL345��ʼ�����
  {
      res[0] = ADXL345_Init(ADXL345_FIRST_ID);
			res[1] = ADXL345_Init(ADXL345_SECOND_ID);
			RUN_LED = !RUN_LED;    //!< LED0״̬�л�
      vTaskDelay(delay);     //!< ��ʱ500ms
  }
	
	
	for (;;)
	{
		//�����ٽ���
		taskENTER_CRITICAL();

		//Step1:��鴫����״̬
		if (!ADXL345_Check(ADXL345_FIRST_ID))
			xEventGroupSetBits(EventGroup_SystemStatus, ADXL345_1_BIT_0);
		if (!ADXL345_Check(ADXL345_SECOND_ID))
			xEventGroupSetBits(EventGroup_SystemStatus, ADXL345_2_BIT_1);
		
		//Step2:��ȡ���ٶ�
		//ADXL345_ReadAllAxisAverageValue(&atti_acc_msg[1].acc_x,&atti_acc_msg[1].acc_y,&atti_acc_msg[1].acc_z,ADXL_FIRST_ADDRESS);
		//ADXL345_ReadAllAxisAverageValue(&atti_acc_msg[2].acc_x,&atti_acc_msg[2].acc_y,&atti_acc_msg[2].acc_z,ADXL_SECEND_ADDRESS);
		
		ADXL345_ReadTheEntireAxis(data,ADXL_FIRST_ADDRESS);
		atti_acc_msg[1].acc_x = data[0];
		atti_acc_msg[1].acc_y = data[1];
		atti_acc_msg[1].acc_z = data[2];
		ADXL345_ReadTheEntireAxis(data,ADXL_SECEND_ADDRESS);
		atti_acc_msg[2].acc_x = data[0];
		atti_acc_msg[2].acc_y = data[1];
		atti_acc_msg[2].acc_z = data[2];
		printf("ax1:%f ay1:%f az1:%f    ax2:%f ay2:%f az2:%f\r\n",atti_acc_msg[1].acc_x,atti_acc_msg[1].acc_y,atti_acc_msg[1].acc_z,
																															atti_acc_msg[2].acc_x,atti_acc_msg[2].acc_y,atti_acc_msg[2].acc_z);

		
		//Step3:��ȡ�Ƕ�
		//atti_acc_msg.acc_x = atti_acc_msg.acc_y = atti_acc_msg.acc_z = atti_acc_msg.pitch = atti_acc_msg.roll = 11.0f;
		
		//Step4:��Ϣ����
		xQueueSend(Queue_AttiAndAccMsg, &atti_acc_msg[0], time_out);

		//�˳��ٽ���
		taskEXIT_CRITICAL();
		
		vTaskDelay(delay);
	}
}


