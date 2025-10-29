#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "PWM.h"  
#include "Motor.h"  
#include "Key.h" 
//人耳能听到的频率20hz-20khz，加大频率就可以减弱风扇的噪声（减小预分频PSC

	uint8_t KeyNum;
	int8_t Speed;

int main(void)
{
	
	OLED_Init();
  Motor_Init();
  KEY_Init();
	
	OLED_ShowString(1, 1, "Speed:");

	while(1)
	{
		
		KeyNum=Key_GetNum();
		if(KeyNum==1)
		{
			Speed+=20;
			
			if(Speed>=100)
			{
				Speed=-100;
			}
		}
		Motor_SetSpeed(Speed);
	  OLED_ShowSignedNum(1, 7, Speed,3);
		
	}
}

