#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����ʱ�ӣ�timer2APB2��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//�ⲿ�ж���Ҫ�õ�GPIO

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);//�����ֲ��еĹ涨���ݲ�����ӳ��,P15ԭ��JTRST������
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//P15ԭ��JTRST�����ţ��Ӵ�����
//	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//һ�����ſ���Ȩ�ǼĴ����ģ��ö�ʱ������IO��Ҫ�ø��ÿ�©/�����������ʱ����Ĵ������Ͽ��������Ȩת�Ƹ�Ƭ������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_InternalClockConfig(TIM2);//�ڲ�ʱ�ӵ�Ԫ 
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=100-1;//����ARR�Զ���װ����ֵ,Ԥ��Ƶ���ͼ���������1������ƫ�����10����
	TIM_TimeBaseInitStructure.TIM_Prescaler=720-1;//PSCԤ��Ƶ����ֵ��1s=1hz�����ù�ʽ���õľ�����ֵ�ɱ�,0����Ƶ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//�ظ����������߼���ʱ������
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//���ṹ�帳��ʼֵ
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//�ߵ�ƽ��Ч
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;//TIM_Pulse���壬��������CCR�Ĵ�����ֵ
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);//�ṹ���Աû�����������ᵼ����Щ�������������	
	
	TIM_Cmd(TIM2,ENABLE);//���������ʱ�� 
}
void  PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);//�����й����и���CCRʵ�ֺ�����Ч��
}
