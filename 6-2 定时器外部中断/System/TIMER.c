#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����ʱ�ӣ�timer2APB2��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//�ⲿ�ж���Ҫ�õ�GPIO
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x00); //�ⲿ������Ƶ��TIM_ExtTRGPrescaler, 	TIM_ExtTRGPSC_OFF����Ƶ
																													//TIM_ExtTRGPolarity_NonInverted�����򼴸ߵ�ƽ����������Ч
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=10-1;//����ARR�Զ���װ����ֵ,Ԥ��Ƶ���ͼ���������1������ƫ�����10����
	TIM_TimeBaseInitStructure.TIM_Prescaler=1-1;//PSCԤ��Ƶ����ֵ��1s=1hz�����ù�ʽ���õľ�����ֵ�ɱ�,0����Ƶ
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

uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
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
