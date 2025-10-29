#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//外部中断需要用到GPIO
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//一般引脚控制权是寄存器的，用定时器控制IO口要用复用开漏/推挽输出，此时输出寄存器被断开输出控制权转移给片上外设
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//编码器接口就是一个带方向控制的外部时钟，不需要再配置时钟单元函数
	
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
	TIM_ICInitStructure.TIM_ICFilter=0x0F;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter=0x0F;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);//两个通道的滤波器和极性
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM3,ENABLE);//最后启动定时器 
}	
	
int16_t Encoder_Get(void)//读取速度
{
	int16_t Temp;
	Temp= TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
	return Temp;
}

//int16_t Encoder_Get(void)
//{
//	return  TIM_GetCounter(TIM3);
//}
