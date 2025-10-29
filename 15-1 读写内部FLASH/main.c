#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
//#include "MyFLASH.h" 
#include "Key.h" 
#include "Store.h" 

uint8_t  KeyNum;


int main(void)
{
	OLED_Init();
  KEY_Init();
	Store_Iint();
	
//	OLED_ShowHexNum(1,1,MyFLASH_ReadWord(0X08000000), 8);
//	OLED_ShowHexNum(2,1,MyFLASH_ReadHalfWord(0X08000000), 4);
//	OLED_ShowHexNum(3,1,MyFLASH_ReadByte(0X08000000), 2);
//	
//  MyFLASH_ErasePage(0X0800FC00);
//	MyFLASH_ProgramWord(0X0800FC00,0X12345678);
//	MyFLASH_ProgramHalfWord(0X0800FC10 ,0XABCD);
	OLED_ShowString(1, 1, "Flag:");
	OLED_ShowString(2, 1, "Data:");
	
	while(1)
	{
		KeyNum = Key_GetNum();
		
		if(KeyNum==1)
		{
			Store_Data[1]++;																		//			Store_Data[1]=0X1234;				//			MyFLASH_EraseAllPages();
			Store_Data[2]+=2;																			//			Store_Data[2]=0XABCD;
			Store_Data[3]+=3;
			Store_Data[4]+=4;
			Store_Save();
		}
		if(KeyNum==2)
		{
			Store_Clear();																																			////			MyFLASH_ErasePage(0X08000000);	
		}
		OLED_ShowHexNum(1,6,Store_Data[0],4);
		OLED_ShowHexNum(3,1,Store_Data[1],4);
		OLED_ShowHexNum(3,6,Store_Data[2],4);
		OLED_ShowHexNum(4,1,Store_Data[3],4);
		OLED_ShowHexNum(4,6,Store_Data[4],4);
	}
}
