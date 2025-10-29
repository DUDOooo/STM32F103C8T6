#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);
}


void MySPI_Iint(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//����Ӳ��SPI1
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//ͨ���������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//�����٣����ܺ����湲��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//�����٣����ܺ����湲��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��©���
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;//ָ����ǰΪ����
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//SPI�ü����ţ�˫��ȫ˫��
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;//8λ����֡
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;//��λ����
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;//�����ʷ�Ƶ��������SCKʱ��
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;//����״̬�͵�ƽ
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;//��1�����ؿ�ʼ��������
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;//�����õ�NSSһ��������ģʽ
	SPI_InitStructure.SPI_CRCPolynomial=7;//CRC�����ò��ϣ�����7 
	SPI_Init(SPI1 ,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE);
	
	MySPI_W_SS(1);//��1�����ӻ�
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
