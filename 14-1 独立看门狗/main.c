#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "KEY.h"  


int main(void)
{
	OLED_Init();
	KEY_Init();
	
	OLED_ShowString(1,1,"IWDG TEST");
	
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET)//查看时钟标志位是否是看门狗的标志位
		{
			 OLED_ShowString(2,1,"IWDGRESET");
			 Delay_ms(500);
			 OLED_ShowString(2,1,"         ");
			 Delay_ms(100);
			
			 RCC_ClearFlag();//看门狗标志位需要手动清除，直接按复位键不能消除
		}
		else
		{
			 OLED_ShowString(3,1,"RESET");
			 Delay_ms(500);
			 OLED_ShowString(3,1,"     ");
			 Delay_ms(100);
		}
		//独立看门狗初始化
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//打开写使能
		IWDG_SetPrescaler(IWDG_Prescaler_16);//设置预分频，预分频小时钟快取整后的误差小
		IWDG_SetReload(2499);//看表格公式计算RL,超时时间=1/40 000*16*(2499+1)=1000ms
		IWDG_ReloadCounter();//先喂狗，这样喂狗周期就是1000ms
		IWDG_Enable();//启动看门狗
		
		while(1)
		{
			Key_GetNum();//按下主循环阻塞不能及时喂狗，看门狗会复位
			
			IWDG_ReloadCounter();//喂狗
			OLED_ShowString(4,1,"FEED");
			Delay_ms(200);
			OLED_ShowString(4,1,"     ");
			Delay_ms(600);
		}
}
