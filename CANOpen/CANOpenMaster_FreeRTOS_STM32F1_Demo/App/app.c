/**
  ******************************************************************************
  * @�ļ���     �� app.c
  * @����       �� strongerHuang
  * @�汾       �� V1.0.0
  * @����       �� 2018��11��14��
  * @ժҪ       �� Ӧ�ó���Դ�ļ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2018-11-14 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "app.h"
#include "bsp.h"
#include "bsp_usart.h"
#include "canopen_app.h"


/* ��̬���� ------------------------------------------------------------------*/
static void APP_Init(void);
static void APP_Task(void *pvParameters);


/************************************************
�������� �� AppTaskCreate
��    �� �� ��������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
void AppTaskCreate(void)
{
  BaseType_t xReturn;

  /* ����Ӧ�ó���(��ʼ����) */
  xReturn = xTaskCreate(APP_Task, "APP_Task", APP_STACK_SIZE, NULL, APP_TASK_PRIORITY, NULL);
  if(pdPASS != xReturn)
  {
    printf("APP_Task create failed");
    return;                                      //��������ʧ��
  }
}

/************************************************
�������� �� APP_Init
��    �� �� Ӧ�ó����ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
static void APP_Init(void)
{
  CANOpen_App_Init();
}

/************************************************
�������� �� APP_Task
��    �� �� Ӧ�ó���(��ʼ����)
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
static void APP_Task(void *pvParameters)
{
  BSP_Init();                                    //�ײ�������ʼ��
  APP_Init();                                    //Ӧ�ó����ʼ��

  for(;;)
  {
    vTaskDelay(250);
    LED_TOGGLE();                                //LED��˸
  }
}


/**** Copyright (C)2018 strongerHuang. All Rights Reserved **** END OF FILE ****/