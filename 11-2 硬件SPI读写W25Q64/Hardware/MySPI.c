#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);
}


void MySPI_Iint(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//开启硬件SPI1
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//通用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//不能少，不能和下面共用
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//不能少，不能和下面共用
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//开漏输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;//指定当前为主机
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//SPI裁剪引脚：双线全双工
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;//8位数据帧
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;//高位先行
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;//波特率分频器，配置SCK时钟
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;//空闲状态低电平
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;//第1个边沿开始采样输入
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;//不会用到NSS一般采用软件模式
	SPI_InitStructure.SPI_CRCPolynomial=7;//CRC采样用不上，随便给7 
	SPI_Init(SPI1 ,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE);
	
	MySPI_W_SS(1);//置1不做从机
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)!=SET);
	
	SPI_I2S_SendData(SPI1,ByteSend);
	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)!=SET);
	
	return SPI_I2S_ReceiveData(SPI1);
}
