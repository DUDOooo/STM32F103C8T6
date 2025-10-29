#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "MYSPI.h"
#include "W25Q64_INS.h"
#include "W25Q64.h"

uint16_t DID;
uint8_t MID;

uint8_t ArrayWrite[]={0x01,0x02,0x030,0x04};
uint8_t ArrayRead[4];

int main(void)
{
	OLED_Init();
  W25Q64_Init();
	MySPI_Iint();
	
	OLED_ShowString(1 , 1 ,"MID:");
	OLED_ShowString(1 , 8 ,"DID:");
	OLED_ShowString(2 , 1 ,"W:");
	OLED_ShowString(3 , 1 ,"R:");
	
	W25Q64_ReadID(&MID,&DID);
	OLED_ShowHexNum(1,5,MID,2);
	OLED_ShowHexNum(1,12,DID,4);
	
	W25Q64_SectorErase(0X000000);//Flash写入前要先擦除否则，下次写入的数据=原数据&现数据，且遇0则0
	W25Q64_PageProgram(0X000000,ArrayWrite,4);
	W25Q64_W25Q64_ReadData(0X000000,ArrayRead,4);
	
	OLED_ShowHexNum(2,3,ArrayWrite[0],2);
	OLED_ShowHexNum(2,6,ArrayWrite[1],2);
	OLED_ShowHexNum(2,9,ArrayWrite[2],2);
	OLED_ShowHexNum(2,12,ArrayWrite[3],2);
	
	OLED_ShowHexNum(3,3,ArrayRead[0],2);
	OLED_ShowHexNum(3,6,ArrayRead[1],2);
	OLED_ShowHexNum(3,9,ArrayRead[2],2);
	OLED_ShowHexNum(3,12,ArrayRead[3],2);
		
	while(1)
	{
		
	}
}
