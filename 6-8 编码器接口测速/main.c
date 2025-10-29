#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "Encoder.h" 	
#include "Timer.h" 	

int16_t Speed;

int main(void)
{
	OLED_Init();
	Timer_Init();
	Encoder_Init();
	
	OLED_ShowString(1, 1, "Speed:");	
//	OLED_ShowString(1, 1, "CNT:");

	while(1)
	{
		OLED_ShowSignedNum(1,7,Speed,5);
//		OLED_ShowSignedNum(1,5,Encoder_Get(),5);
	}
}

void TIM2_IRQHandler(void)//�����ļ�����
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)//�ȼ���жϱ�־λ,�����жϵı�־λ
	{
		Speed=Encoder_Get();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

