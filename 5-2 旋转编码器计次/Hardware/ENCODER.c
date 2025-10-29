#include "stm32f10x.h"                  // Device header

int16_t Encoder_Count;

void  Encoder_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//RCC设置内核外的外设，NVIC是内核内的不需要另外设置RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;//1.配置GPIO
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//EXIT在手册中说明可上拉，下拉，浮空
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);//2.配置EXTI外设中断AFIO 
	
	EXTI_InitTypeDef EXTI_InitStructure;//3.EXTI
	EXTI_InitStructure.EXTI_Line=EXTI_Line0|EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);//1.中断中不执行过长函数，2.不同时执行主函数中的硬件配置，不调用相同函数
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//4.NVIC

	NVIC_InitTypeDef  NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
  NVIC_Init(&NVIC_InitStructure);
}

int16_t Encoder_Get(void)//定义临时变量temp，否则count=0直接返回函数会终止程序运行
{
	uint16_t Temp;
	Temp= Encoder_Count;
	 Encoder_Count=0;
	return Temp;
}
	

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
		{
			
			Encoder_Count--;//多用变量和标志位减少代码的耦合性
		}
			EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
		{
			Encoder_Count++;
		}
			EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
