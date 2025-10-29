#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "KEY.h"  


int main(void)
{
	OLED_Init();
	KEY_Init();
	
	OLED_ShowString(1,1,"IWDG TEST");
	
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET)//�鿴ʱ�ӱ�־λ�Ƿ��ǿ��Ź��ı�־λ
		{
			 OLED_ShowString(2,1,"IWDGRESET");
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
		//�������Ź���ʼ��
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//��дʹ��
		IWDG_SetPrescaler(IWDG_Prescaler_16);//����Ԥ��Ƶ��Ԥ��ƵСʱ�ӿ�ȡ��������С
		IWDG_SetReload(2499);//�����ʽ����RL,��ʱʱ��=1/40 000*16*(2499+1)=1000ms
		IWDG_ReloadCounter();//��ι��������ι�����ھ���1000ms
		IWDG_Enable();//�������Ź�
		
		while(1)
		{
			Key_GetNum();//������ѭ���������ܼ�ʱι�������Ź��Ḵλ
			
			IWDG_ReloadCounter();//ι��
			OLED_ShowString(4,1,"FEED");
			Delay_ms(200);
			OLED_ShowString(4,1,"     ");
			Delay_ms(600);
		}
}
