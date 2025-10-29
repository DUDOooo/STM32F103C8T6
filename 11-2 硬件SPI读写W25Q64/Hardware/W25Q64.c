#include "stm32f10x.h"                  // Device header
#include "MYSPI.h"
#include "W25Q64_INS.h"


void W25Q64_Init(void)
{
	MySPI_Iint();//调用底层函数
}

void W25Q64_ReadID(uint8_t *MID,uint16_t *DID)//指针返回多个值
{	
	MySPI_Start();//开始拼接时序（业务代码）
	MySPI_SwapByte(W25Q64_JEDEC_ID);
	*MID=MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//用0XFF置换出想要的数据，或用其他值代替0XFF只是为了换出数据
	*DID=MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID<<=8;//移到高8位
	*DID|=MySPI_SwapByte(W25Q64_DUMMY_BYTE);//低8位
	MySPI_Stop();
}

void W25Q64_WriteEnable(void)//写使能
{
	MySPI_Start();//开始拼接时序（业务代码）
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}

void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	Timeout=100000;
	while((MySPI_SwapByte(W25Q64_DUMMY_BYTE)&0x01)==0x01)//busy=1时等待，=0时跳出死循环执行其他命令
	{
		Timeout--;
		if(Timeout==0)
		{
			break ;
		}
	}
	MySPI_Stop();
}

void W25Q64_PageProgram(uint32_t Address,uint8_t *DataArray,uint16_t count)//3个8位地址，C语没有24位所以定义32,要写入的数据8位一组，count最多一页可写256个，uint_8只能255位
{
	uint32_t i;
	W25Q64_WriteEnable();//Flash需要写入的地方要先打开写使能
		
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	MySPI_SwapByte(Address>>16);//byte是8位，只发送最低8位地址其余高位舍弃
	MySPI_SwapByte(Address>>8);	
	MySPI_SwapByte(Address);
	for(i=0;i<count;i++)
	{
		MySPI_SwapByte(DataArray[i]);
	}

	MySPI_Stop();
	
	W25Q64_WaitBusy();//Flash写完后要置busy等待
}

void W25Q64_SectorErase(uint32_t Address)//发送指令码20H和3个8位地址就可以擦除一个扇区
{
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	MySPI_SwapByte(Address>>16);//byte是8位，只发送最低8位地址其余高位舍弃
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
	MySPI_SwapByte(Address>>16);//byte是8位，只发送最低8位地址其余高位舍弃
	MySPI_SwapByte(Address>>8);	
	MySPI_SwapByte(Address);
	for(i=0;i<count;i++)
	{
		DataArray[i]=MySPI_SwapByte(W25Q64_DUMMY_BYTE);//用W25Q64_DUMMY_BYTE换读出目标数据
	}
	MySPI_Stop();
}
