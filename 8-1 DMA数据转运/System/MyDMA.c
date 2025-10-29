#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;

void MyDMA_Init(uint32_t AddrA,uint32_t AddrB,uint16_t Size)//Ҫת���ĸ�����ĵ�ַ�����ڴ˳�ʼ��
{
	MyDMA_Size=Size;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr=AddrA;//0x20000000���Ե�ַһ�㲻�ã�ͨ������������ȡ��ַ
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Enable;//����Ҫ����
	DMA_InitStructure.DMA_MemoryBaseAddr= AddrB;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;//����վ����Ϊ����Դ
	DMA_InitStructure.DMA_BufferSize=Size;//���������,uint16_t��ֵ��0-65535
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;//�Ƿ�������װ���Զ���װ�������������ͬʱʹ�÷������Զͣ������,�˴��Ǵ洢�����洢������ת��һ��ͣ�¾���
	DMA_InitStructure.DMA_M2M=DMA_M2M_Enable;//Ӳ�����������������,�洢�����洢��ѡ���������
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium;//���ȼ�
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
}

void MyMDA_Transfer(void)//������DMAת������
{
	DMA_Cmd(DMA1_Channel1,DISABLE);//�ر�DMA��������дֵ�����������
	DMA_SetCurrDataCounter(DMA1_Channel1,MyDMA_Size); //
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);//ת����ɱ�־λ,ת����Ҫʱ��RESET��ʾ��û����ɵȴ�ת�˽��
	DMA_ClearFlag(DMA1_FLAG_TC1);
}
