#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "OLED.h"  
#include "MPU6050_REG.h"  
#include "MPU6050.h"  
#include "MYI2C.h"


uint8_t  ID;
int16_t AccX,AccY,AccZ,GyroX,GyroY,GyroZ;

int main(void)
{
	MPU6050_Init();
	OLED_Init(); 
	
	OLED_ShowString(1,1,"ID:");
	ID = MPU6050_GetID();
	OLED_ShowHexNum(1,4,ID,2);
	 
	while(1)
	{
		MPU6050_GetData(&AccX,&AccY,&AccZ,&GyroX,&GyroY,&GyroZ);
		OLED_ShowSignedNum(2,1,AccX,5);
		OLED_ShowSignedNum(3,1,AccY,5);
		OLED_ShowSignedNum(4,1,AccZ,5);
		OLED_ShowSignedNum(2,8,GyroX,5);
		OLED_ShowSignedNum(3,8,GyroY,5);
		OLED_ShowSignedNum(4,8,GyroZ,5);
	}
}
