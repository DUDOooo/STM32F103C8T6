#include "stm32f10x.h"                  // Device header


void IC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//�ⲿ�ж���Ҫ�õ�GPIO
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//һ�����ſ���Ȩ�ǼĴ����ģ��ö�ʱ������IO��Ҫ�ø��ÿ�©/�����������ʱ����Ĵ������Ͽ��������Ȩת�Ƹ�Ƭ������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_InternalClockConfig(TIM3);//�ڲ�ʱ�ӵ�Ԫ 
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=65536-1;//����ARR�Զ���װ����ֵ,Ԥ��Ƶ���ͼ���������1������ƫ������������ô�һ�������ֹ���
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;//72M/72=1MHZ,PSCԤ��Ƶ����ֵ��1s=1hz�����ù�ʽ���õľ�����ֵ�ɱ�,0����Ƶ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//�ظ����������߼���ʱ������
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ�帳��ʼֵ
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter=0x0F;//��ֵԽ���˲�Ч��Խ��
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);//�ṹ���Աû�����������ᵼ����Щ�������������	
	
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);//����Դ����
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);//��ģʽ����ΪRESET
	
	TIM_Cmd(TIM3,ENABLE);//���������ʱ�� 
}

uint32_t IC_GetFreq(void)
{
	return 10000/(TIM_GetCapture1(TIM3)+1);//hz,TIM_GetCapture1(TIM3)���񵽵�CCR��ֵ
}
