#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)BitValue);
}

uint8_t  MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
}

void MySPI_Iint(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//不能少，不能和下面共用
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//开漏输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	MySPI_W_SS(1);
	MySPI_W_SCK(0);//初始化后有规定的引脚要置高低电平
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)//模式0
{
	uint8_t i,ByteReceive=0x00;//从机放0x00换位
	for(i=0;i<8;i++)
	{
		MySPI_W_MOSI(ByteSend&(0x80>>i));//0x80>>提出单独位，剔除无关位，用掩码操作的方式
		MySPI_W_SCK(1);//从机读走数据
		if(MySPI_R_MISO()==1){ByteReceive|=(0x80>>i);}//如果读到数据1，移到最高位，0不用移初始值本来就是0
		MySPI_W_SCK(0);	
	}
	return ByteReceive;
}

//uint8_t MySPI_SwapByte(uint8_t ByteSend)
//{
//	uint8_t i;//从机放0x00换位
//	for(i=0;i<8;i++)
//	{
//		MySPI_W_MOSI(ByteSend&0x80);//按照流程结构图中的移位操作
//		ByteSend<<=1;
//		MySPI_W_SCK(1);//从机读走数据
//		if(MySPI_R_MISO()==1){ByteSend|=0x01;}
//		MySPI_W_SCK(0);	
//	}
//	return ByteSend;//与初始输入的ByteSend不同值了
//}
