#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Count;

void  CountSensor_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//RCC设置内核外的外设，NVIC是内核内的不需要另外设置RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;//1.配置GPIO
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//EXIT在手册中说明可上拉，下拉，浮空
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);//2.配置EXTI外设中断AFIO 
	
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

	
void EXTI15_10_IRQHandler()//启动文件中找中断函数，名字固定写错不能进入中断，无参数无返回值;中断函数不需要放在头文件调用，可以自动执行
{
	if(EXTI_GetITStatus(EXTI_Line14)==SET)//先进行一个中断标志位，确保是想要的中断源
	{
		CountSensor_Count++;
		EXTI_ClearITPendingBit(EXTI_Line14);//最后清除标志位否则程序会一直进入中断卡死
	}
}	
