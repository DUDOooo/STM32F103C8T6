#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  

int main(void)
{
	OLED_Init();

	OLED_ShowString(1, 1, "SYSCLK:");
	OLED_ShowNum(1,8, SystemCoreClock, 10);
	while(1)
	{
		OLED_ShowString(2, 8, "RUNNING");
		Delay_ms(100);
		OLED_ShowString(2, 8, "       ");
		Delay_ms(100);
	}
}
//在system.h中修改主频，一般情况下不改会影响到很多地方
