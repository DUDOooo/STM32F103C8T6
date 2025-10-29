#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启时钟，timer2APB2上
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//外部中断需要用到GPIO
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x00); //外部触发分频器TIM_ExtTRGPrescaler, 	TIM_ExtTRGPSC_OFF不分频
																													//TIM_ExtTRGPolarity_NonInverted不返向即高电平和上升沿有效
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=10-1;//周期ARR自动重装器的值,预分频器和计数器都有1个数的偏差，数到10清零
	TIM_TimeBaseInitStructure.TIM_Prescaler=1-1;//PSC预分频器的值，1s=1hz，套用公式设置的具体数值可变,0不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//重复计数器，高级定时器才用
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_IT_Update);//手册中定义默认开始给1，所以手动清零让数值从0开始
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;//定时器2在nvic里的通道
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);//最后启动定时器 
}

uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
}
//用seycount和getcount操作计时器CNT

//void TIM2_IRQHandler(void)//启动文件中找
//{
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)//先检查中断标志位,更新中断的标志位
//	{
//		
//		TIM_ClearFlag(TIM2, TIM_IT_Update);
//	}
//}
