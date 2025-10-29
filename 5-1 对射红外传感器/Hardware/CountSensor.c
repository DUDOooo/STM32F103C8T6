#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Count;

void  CountSensor_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//RCC�����ں�������裬NVIC���ں��ڵĲ���Ҫ��������RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;//1.����GPIO
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//EXIT���ֲ���˵��������������������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);//2.����EXTI�����ж�AFIO 
	
	EXTI_InitTypeDef EXTI_InitStructure;//3.EXTI
	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//4.NVIC

	NVIC_InitTypeDef  NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
  NVIC_Init(&NVIC_InitStructure);
}

uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}

	
void EXTI15_10_IRQHandler()//�����ļ������жϺ��������̶ֹ�д���ܽ����жϣ��޲����޷���ֵ;�жϺ�������Ҫ����ͷ�ļ����ã������Զ�ִ��
{
	if(EXTI_GetITStatus(EXTI_Line14)==SET)//�Ƚ���һ���жϱ�־λ��ȷ������Ҫ���ж�Դ
	{
		CountSensor_Count++;
		EXTI_ClearITPendingBit(EXTI_Line14);//��������־λ��������һֱ�����жϿ���
	}
}	
