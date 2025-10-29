#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "KEY.h"  

uint8_t KeyNum;
uint16_t ArraryWrite[]={0x0001,0x0001};
uint16_t ArraryRead[2];

int main(void)
{
	
	OLED_Init();
  KEY_Init();
	
	OLED_ShowString(1, 1, "W:");
  OLED_ShowString(2, 1, "R:");
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);

	while(1)
	{
		KeyNum=Key_GetNum();//»ñÈ¡¼üÖµ
		
		if(KeyNum==1)
		{
			ArraryWrite[0]++;
			ArraryWrite[1]++;
			
			BKP_WriteBackupRegister(BKP_DR1,ArraryWrite[0]);
			BKP_WriteBackupRegister(BKP_DR2,ArraryWrite[1]);
			
			OLED_ShowHexNum(1,3,ArraryWrite[0],4);
			OLED_ShowHexNum(1,8,ArraryWrite[1],4);
		}
		ArraryRead[0]=BKP_ReadBackupRegister(BKP_DR1);
		ArraryRead[1]=BKP_ReadBackupRegister(BKP_DR2);

		OLED_ShowHexNum(2,3,ArraryRead[0],4);
		OLED_ShowHexNum(2,8,ArraryRead[1],4);
	}
}
