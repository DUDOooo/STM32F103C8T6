#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启时钟，timer2APB2上
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//外部中断需要用到GPIO

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);//按照手册中的规定内容部分重映射,P15原是JTRST的引脚
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//P15原是JTRST的引脚，接触锁定
//	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//一般引脚控制权是寄存器的，用定时器控制IO口要用复用开漏/推挽输出，此时输出寄存器被断开输出控制权转移给片上外设
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_InternalClockConfig(TIM2);//内部时钟单元 
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=100-1;//周期ARR自动重装器的值,预分频器和计数器都有1个数的偏差，数到10清零
	TIM_TimeBaseInitStructure.TIM_Prescaler=720-1;//PSC预分频器的值，1s=1hz，套用公式设置的具体数值可变,0不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//重复计数器，高级定时器才用
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//给结构体赋初始值
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//高电平有效
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;//TIM_Pulse脉冲，用于设置CCR寄存器的值
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);//结构体成员没有配置完整会导致有些不能输出等问题	
	
	TIM_Cmd(TIM2,ENABLE);//最后启动定时器 
}
void  PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);//在运行过程中更改CCR实现呼吸灯效果
}
