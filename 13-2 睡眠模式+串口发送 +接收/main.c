#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "SERIAL.h"  

uint8_t RxData;//收到的数据

int main(void)
{
	OLED_Init();
	Serial_Init();
	
	OLED_ShowString(1,1,"RxData:");
	
	while(1)
	{
		if(Serial_GetRxFlag()==1)//中断用法
		{
			RxData=Serial_GetRxData();
			Serial_SendByte(RxData);
			OLED_ShowHexNum(1,8, RxData,2);
		}
		OLED_ShowString(2,1,"Running");
		Delay_ms(100);
		OLED_ShowString(2,1,"       ");
		Delay_ms(100);
		
		__WFI();//睡眠模式中断唤醒
	}
}
//	while(1)
//{
//			if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)//读取到RXNE表示接收到数据
//		{
//			RxData=USART_ReceiveData(USART1);
//			OLED_ShowHexNum(1,1, RxData,2);
//		}
//	}
//直接扫描读取
