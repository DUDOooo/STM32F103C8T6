#include "stm32f10x.h"                  // Device header


void IC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//外部中断需要用到GPIO
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//一般引脚控制权是寄存器的，用定时器控制IO口要用复用开漏/推挽输出，此时输出寄存器被断开输出控制权转移给片上外设
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_InternalClockConfig(TIM3);//内部时钟单元 
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=65536-1;//周期ARR自动重装器的值,预分频器和计数器都有1个数的偏差，数到清零设置大一点的数防止溢出
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;//72M/72=1MHZ,PSC预分频器的值，1s=1hz，套用公式设置的具体数值可变,0不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//重复计数器，高级定时器才用
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);//给结构体赋初始值
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter=0x0F;//数值越高滤波效率越好
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);//结构体成员没有配置完整会导致有些不能输出等问题	
	
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);//触发源函数
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);//从模式设置为RESET
	
	TIM_Cmd(TIM3,ENABLE);//最后启动定时器 
}

uint32_t IC_GetFreq(void)
{
	return 10000/(TIM_GetCapture1(TIM3)+1);//hz,TIM_GetCapture1(TIM3)捕获到的CCR的值
}
