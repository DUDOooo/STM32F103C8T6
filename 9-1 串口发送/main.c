#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "SERIAL.h"  


int main(void)
{
	OLED_Init();
	Serial_Init();
//	Serial_SendByte(0x41);
	
//	uint8_t MyArray[]={0x40,0x41,0x42,0x43};
//	Serial_SendArray(MyArray,4);
//	
//	Serial_SendString("HelloWorld\r\n");//'\r\n'换行
	
//	Serial_SendNumber(12345,5);
	
	//printf("Num=%d\r\n",666);
	
//	char String[100];
//	sprintf(String,"Num=%d\r\n",666);
//	Serial_SendString(String);
	//可封装多个
	
	Serial_Printf("你好");
	
	
	while(1)
	{

	}
}
