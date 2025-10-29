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

void MyFLASH_EraseAllPages(void)//ȫ����
{
	FLASH_Unlock();//1.����
	FLASH_EraseAllPages();//�ɶ��ط���ֵ
	FLASH_Lock();
}

void MyFLASH_ErasePage(uint32_t PageAddress)//ҳ����
{
	FLASH_Unlock();//1.����
	FLASH_ErasePage(PageAddress);//�ɶ��ط���ֵ
	FLASH_Lock();
}

void MyFLASH_ProgramWord(uint32_t Address,uint32_t Data)//д��һ����
{
	FLASH_Unlock();//1.����
	FLASH_ProgramWord(Address,Data);//�ɶ��ط���ֵ
	FLASH_Lock();
}

void MyFLASH_ProgramHalfWord(uint32_t Address,uint16_t Data)//д��һ������
{
	FLASH_Unlock();//1.����
	FLASH_ProgramHalfWord(Address,Data);//�ɶ��ط���ֵ
	FLASH_Lock();
}
