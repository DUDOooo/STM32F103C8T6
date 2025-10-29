#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;

void MyDMA_Init(uint32_t AddrA,uint32_t AddrB,uint16_t Size)//要转运哪个数组的地址将他在此初始化
{
	MyDMA_Size=Size;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr=AddrA;//0x20000000绝对地址一般不用，通过数组名来获取地址
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Enable;//数组要自增
	DMA_InitStructure.DMA_MemoryBaseAddr= AddrB;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;//外设站点作为数据源
	DMA_InitStructure.DMA_BufferSize=Size;//传输计数器,uint16_t的值是0-65535
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;//是否启用重装，自动重装和软件触发不能同时使用否则会永远停不下来,此次是存储器到存储器传输转运一次停下就行
	DMA_InitStructure.DMA_M2M=DMA_M2M_Enable;//硬件触发还是软件触发,存储器到存储器选择软件触发
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium;//优先级
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
}

void MyMDA_Transfer(void)//二次用DMA转移数据
{
	DMA_Cmd(DMA1_Channel1,DISABLE);//关闭DMA才能重新写值给传输计数器
	DMA_SetCurrDataCounter(DMA1_Channel1,MyDMA_Size); //
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);//转运完成标志位,转运需要时间RESET表示还没有完成等待转运结果
	DMA_ClearFlag(DMA1_FLAG_TC1);
}
