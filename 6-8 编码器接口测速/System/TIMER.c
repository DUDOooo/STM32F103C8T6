#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����ʱ�ӣ�timer2APB2��
	
	TIM_InternalClockConfig(TIM2);//ѡ��ʱ����Ԫ��ʱ�ӣ�������ҲĬ��ʹ���ڲ�ʱ�ӣ�ͨ��234
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=10000-1;//����ARR�Զ���װ����ֵ,Ԥ��Ƶ���ͼ���������1������ƫ��
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;//PSCԤ��Ƶ����ֵ��1s=1hz�����ù�ʽ���õľ�����ֵ�ɱ�
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//�ظ����������߼���ʱ������
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_IT_Update);//�ֲ��ж���Ĭ�Ͽ�ʼ��1�������ֶ���������ֵ��0��ʼ
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;//��ʱ��2��nvic���ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);//���������ʱ�� 
}
//��seycount��getcount������ʱ��CNT

//void TIM2_IRQHandler(void)//�����ļ�����
//{
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)//�ȼ���жϱ�־λ,�����жϵı�־λ
//	{
//		
//		TIM_ClearFlag(TIM2, TIM_IT_Update);
//	}
//}
