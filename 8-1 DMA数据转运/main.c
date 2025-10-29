#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "MyDMA.h"  

uint8_t DataA[]={0x01,0x02,0x03,0x04};//const可转移到SRAM中数据只读
uint8_t DataB[]={0,0,0,0};

int main(void)
{
	OLED_Init();

	MyDMA_Init((uint32_t)DataA,(uint32_t)DataB,4);
	
	OLED_ShowString(1,1,"DataA");
	OLED_ShowString(3,1,"DataB");
	OLED_ShowHexNum(1,8,(uint32_t)DataA, 8);
	OLED_ShowHexNum(3,8,(uint32_t)DataB, 8);

	OLED_ShowHexNum(2,1, DataA[0], 2);
	OLED_ShowHexNum(2,4, DataA[1], 2);
	OLED_ShowHexNum(2,7, DataA[2], 2);
	OLED_ShowHexNum(2,10, DataA[3], 2);
	OLED_ShowHexNum(4,1, DataB[0], 2);
	OLED_ShowHexNum(4,4, DataB[1], 2);
	OLED_ShowHexNum(4,7, DataB[2], 2);
	OLED_ShowHexNum(4,10, DataB[3], 2);
	
	while(1)
	{
		DataA[0]++;
		DataA[1]++;
		DataA[2]++;
		DataA[3]++;
		
		Delay_ms(2000);
		
		MyMDA_Transfer();
		
		OLED_ShowHexNum(2,1, DataA[0], 2);
		OLED_ShowHexNum(2,4, DataA[1], 2);
		OLED_ShowHexNum(2,7, DataA[2], 2);
		OLED_ShowHexNum(2,10, DataA[3], 2);
		OLED_ShowHexNum(4,1, DataB[0], 2);
		OLED_ShowHexNum(4,4, DataB[1], 2);
		OLED_ShowHexNum(4,7, DataB[2], 2);
		OLED_ShowHexNum(4,10, DataB[3], 2);
		
		Delay_ms(1000);
	}
}
//系统地址都是32位，&aa表示读取aa的地址，(unint32_t)&aa显示出所读取的aa的地址
//const只读变量存储在FLASH中，用于查找表和字库节省sram空间，flash只能读不能写（保存常量数据和代码）
//基地址+偏移=实际地址
