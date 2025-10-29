#include "stm32f10x.h"                  // Device header

uint32_t MyFLASH_ReadWord(uint32_t Address)
{
	return *((__IO uint32_t *)(Address));
}

uint16_t MyFLASH_ReadHalfWord(uint32_t Address)
{
	return *((__IO uint16_t *)(Address));
}

uint8_t MyFLASH_ReadByte(uint32_t Address)
{
	return *((__IO uint8_t *)(Address));
}

void MyFLASH_EraseAllPages(void)//全擦除
{
	FLASH_Unlock();//1.解锁
	FLASH_EraseAllPages();//可读回返回值
	FLASH_Lock();
}

void MyFLASH_ErasePage(uint32_t PageAddress)//页擦除
{
	FLASH_Unlock();//1.解锁
	FLASH_ErasePage(PageAddress);//可读回返回值
	FLASH_Lock();
}

void MyFLASH_ProgramWord(uint32_t Address,uint32_t Data)//写入一个字
{
	FLASH_Unlock();//1.解锁
	FLASH_ProgramWord(Address,Data);//可读回返回值
	FLASH_Lock();
}

void MyFLASH_ProgramHalfWord(uint32_t Address,uint16_t Data)//写入一个半字
{
	FLASH_Unlock();//1.解锁
	FLASH_ProgramHalfWord(Address,Data);//可读回返回值
	FLASH_Lock();
}
