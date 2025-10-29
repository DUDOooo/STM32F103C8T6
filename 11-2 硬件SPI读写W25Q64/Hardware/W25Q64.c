#include "stm32f10x.h"                  // Device header
#include "MYSPI.h"
#include "W25Q64_INS.h"


void W25Q64_Init(void)
{
	MySPI_Iint();//���õײ㺯��
}

void W25Q64_ReadID(uint8_t *MID,uint16_t *DID)//ָ�뷵�ض��ֵ
{	
	MySPI_Start();//��ʼƴ��ʱ��ҵ����룩
	MySPI_SwapByte(W25Q64_JEDEC_ID);
	*MID=MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//��0XFF�û�����Ҫ�����ݣ���������ֵ����0XFFֻ��Ϊ�˻�������
	*DID=MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID<<=8;//�Ƶ���8λ
	*DID|=MySPI_SwapByte(W25Q64_DUMMY_BYTE);//��8λ
	MySPI_Stop();
}

void W25Q64_WriteEnable(void)//дʹ��
{
	MySPI_Start();//��ʼƴ��ʱ��ҵ����룩
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}

void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	Timeout=100000;
	while((MySPI_SwapByte(W25Q64_DUMMY_BYTE)&0x01)==0x01)//busy=1ʱ�ȴ���=0ʱ������ѭ��ִ����������
	{
		Timeout--;
		if(Timeout==0)
		{
			break ;
		}
	}
	MySPI_Stop();
}

void W25Q64_PageProgram(uint32_t Address,uint8_t *DataArray,uint16_t count)//3��8λ��ַ��C��û��24λ���Զ���32,Ҫд�������8λһ�飬count���һҳ��д256����uint_8ֻ��255λ
{
	uint32_t i;
	W25Q64_WriteEnable();//Flash��Ҫд��ĵط�Ҫ�ȴ�дʹ��
		
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	MySPI_SwapByte(Address>>16);//byte��8λ��ֻ�������8λ��ַ�����λ����
	MySPI_SwapByte(Address>>8);	
	MySPI_SwapByte(Address);
	for(i=0;i<count;i++)
	{
		MySPI_SwapByte(DataArray[i]);
	}

	MySPI_Stop();
	
	W25Q64_WaitBusy();//Flashд���Ҫ��busy�ȴ�
}

void W25Q64_SectorErase(uint32_t Address)//����ָ����20H��3��8λ��ַ�Ϳ��Բ���һ������
{
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	MySPI_SwapByte(Address>>16);//byte��8λ��ֻ�������8λ��ַ�����λ����
	MySPI_SwapByte(Address>>8);	
	MySPI_SwapByte(Address);
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

void W25Q64_W25Q64_ReadData(uint32_t Address,uint8_t *DataArray,uint32_t count)//
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address>>16);//byte��8λ��ֻ�������8λ��ַ�����λ����
	MySPI_SwapByte(Address>>8);	
	MySPI_SwapByte(Address);
	for(i=0;i<count;i++)
	{
		DataArray[i]=MySPI_SwapByte(W25Q64_DUMMY_BYTE);//��W25Q64_DUMMY_BYTE������Ŀ������
	}
	MySPI_Stop();
}
