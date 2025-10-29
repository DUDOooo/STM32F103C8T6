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
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//�����٣����ܺ����湲��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��©���
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	MySPI_W_SS(1);
	MySPI_W_SCK(0);//��ʼ�����й涨������Ҫ�øߵ͵�ƽ
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)//ģʽ0
{
	uint8_t i,ByteReceive=0x00;//�ӻ���0x00��λ
	for(i=0;i<8;i++)
	{
		MySPI_W_MOSI(ByteSend&(0x80>>i));//0x80>>�������λ���޳��޹�λ������������ķ�ʽ
		MySPI_W_SCK(1);//�ӻ���������
		if(MySPI_R_MISO()==1){ByteReceive|=(0x80>>i);}//�����������1���Ƶ����λ��0�����Ƴ�ʼֵ��������0
		MySPI_W_SCK(0);	
	}
	return ByteReceive;
}

//uint8_t MySPI_SwapByte(uint8_t ByteSend)
//{
//	uint8_t i;//�ӻ���0x00��λ
//	for(i=0;i<8;i++)
//	{
//		MySPI_W_MOSI(ByteSend&0x80);//�������̽ṹͼ�е���λ����
//		ByteSend<<=1;
//		MySPI_W_SCK(1);//�ӻ���������
//		if(MySPI_R_MISO()==1){ByteSend|=0x01;}
//		MySPI_W_SCK(0);	
//	}
//	return ByteSend;//���ʼ�����ByteSend��ֵͬ��
//}
