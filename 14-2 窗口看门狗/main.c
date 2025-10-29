#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "KEY.h"  


int main(void)
{
	OLED_Init();
	KEY_Init();
	
	OLED_ShowString(1,1,"WWDG TEST");
	
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST)==SET)//�鿴ʱ�ӱ�־λ�Ƿ��ǿ��Ź��ı�־λ
		{
			 OLED_ShowString(2,1,"WWDGRESET");
			 Delay_ms(500);
			 OLED_ShowString(2,1,"         ");
			 Delay_ms(100);
			
			 RCC_ClearFlag();//���Ź���־λ��Ҫ�ֶ������ֱ�Ӱ���λ����������
		}
		else
		{
			 OLED_ShowString(3,1,"RESET");
			 Delay_ms(500);
			 OLED_ShowString(3,1,"     ");
			 Delay_ms(100);
		}
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);//���ݳ�ʱʱ��ʹ���ʱ�乫ʽ�ͱ����㣬����Ԥ��Ƶд�봰��
		
		WWDG_SetPrescaler(WWDG_Prescaler_8);//Ԥ��Ƶ,��ʱ50ms
    WWDG_SetWindowValue(0X40|21);//д�봰��ֵ30ms
		WWDG_Enable(0X40|54);//'0X40|'��T6����0X40+54����
		
		while(1)
		{
			Key_GetNum();//������ѭ���������ܼ�ʱι�������Ź��Ḵλ
			
		 
			OLED_ShowString(4,1,"FEED");
			Delay_ms(20);
			OLED_ShowString(4,1,"     ");
			Delay_ms(20);
			
			WWDG_SetCounter(0X40|54);//����д�������ι��,������ʱ����������͵�һ��ι�����ʱ��̫��
		}
}
