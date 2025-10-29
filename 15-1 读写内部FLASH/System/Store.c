#include "stm32f10x.h"                  // Device header
#include "MyFLASH.h" 

#define  STORE_START_ADDRESS   0X0800FC00
#define  STORE_COUNT           512
 
uint16_t Store_Data[512];

void Store_Iint(void)
{
	if(MyFLASH_ReadHalfWord(0X0800FC00)!=0XA5A5)
	{
		MyFLASH_ErasePage(0X0800FC00);
		MyFLASH_ProgramHalfWord(0X0800FC00 ,0XA5A5);
		for(uint16_t i=0;i<512;i++)
		{
			MyFLASH_ProgramHalfWord(0X0800FC00+i*2,0x0000);
		}
	}
		for(uint16_t i=0;i<512;i++)
		{
			Store_Data[i] = MyFLASH_ReadHalfWord(0X800FC00+i*2);//�൱�����������SRAM�ж�д����
		}
}

void Store_Save(void)//���浽����
{
		MyFLASH_ErasePage(0X0800FC00);

		for(uint16_t i=0;i<512;i++)
		{
			MyFLASH_ProgramHalfWord(0X0800FC00+i*2,Store_Data[i]);
		}
}

void Store_Clear(void)//���浽����
{
		for(uint16_t i=0;i<512;i++)
		{
			Store_Data[i]=0x0000;//
		}
		Store_Save();
}


//��SRAM �Ļ�������������FLASH�����һҳ
