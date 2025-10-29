#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "PWM.h"  

uint8_t i;

int main(void)
{
	OLED_Init();
  PWM_Init();

	while(1)
	{
		for(i=0;i<=100;i++)//往上加量，变亮
		{
			PWM_SetCompare1(i);//CCR,占空比要用公式计算
			Delay_ms(10);
		}
		for(i=0;i<=100;i++)//变暗
		{
			PWM_SetCompare1(100-i);
			Delay_ms(10);
		}
	}
}
