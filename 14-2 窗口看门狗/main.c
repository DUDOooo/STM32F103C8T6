#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "KEY.h"  


int main(void)
{
	OLED_Init();
	KEY_Init();
	
	OLED_ShowString(1,1,"WWDG TEST");
	
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST)==SET)//查看时钟标志位是否是看门狗的标志位
		{
			 OLED_ShowString(2,1,"WWDGRESET");
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
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);//根据超时时间和窗口时间公式和表格计算，设置预分频写入窗口
		
		WWDG_SetPrescaler(WWDG_Prescaler_8);//预分频,超时50ms
    WWDG_SetWindowValue(0X40|21);//写入窗口值30ms
		WWDG_Enable(0X40|54);//'0X40|'：T6，或0X40+54；、
		
		while(1)
		{
			Key_GetNum();//按下主循环阻塞不能及时喂狗，看门狗会复位
			
		 
			OLED_ShowString(4,1,"FEED");
			Delay_ms(20);
			OLED_ShowString(4,1,"     ");
			Delay_ms(20);
			
			WWDG_SetCounter(0X40|54);//不断写入计数器喂狗,放在延时函数后，以免和第一次喂狗间隔时间太短
		}
}
