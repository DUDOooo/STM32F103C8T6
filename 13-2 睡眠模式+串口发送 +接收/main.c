#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "SERIAL.h"  

uint8_t RxData;//�յ�������

int main(void)
{
	OLED_Init();
	Serial_Init();
	
	OLED_ShowString(1,1,"RxData:");
	
	while(1)
	{
		if(Serial_GetRxFlag()==1)//�ж��÷�
		{
			RxData=Serial_GetRxData();
			Serial_SendByte(RxData);
			OLED_ShowHexNum(1,8, RxData,2);
		}
		OLED_ShowString(2,1,"Running");
		Delay_ms(100);
		OLED_ShowString(2,1,"       ");
		Delay_ms(100);
		
		__WFI();//˯��ģʽ�жϻ���
	}
}
//	while(1)
//{
//			if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)//��ȡ��RXNE��ʾ���յ�����
//		{
//			RxData=USART_ReceiveData(USART1);
//			OLED_ShowHexNum(1,1, RxData,2);
//		}
//	}
//ֱ��ɨ���ȡ
