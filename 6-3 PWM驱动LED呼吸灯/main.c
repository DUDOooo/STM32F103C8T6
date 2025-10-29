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
		for(i=0;i<=100;i++)//���ϼ���������
		{
			PWM_SetCompare1(i);//CCR,ռ�ձ�Ҫ�ù�ʽ����
			Delay_ms(10);
		}
		for(i=0;i<=100;i++)//�䰵
		{
			PWM_SetCompare1(100-i);
			Delay_ms(10);
		}
	}
}
