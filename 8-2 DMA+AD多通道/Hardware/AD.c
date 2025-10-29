#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4];

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
 
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//此模式模拟输入专用，此时GPIO无效
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//多通道供下方函数切换
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	
	ADC_InitTypeDef  ADC_InitStructure;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//NONE不使用外部触发即内部触发
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;;//连续模式
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;//ENABLE连续扫描
	ADC_InitStructure.ADC_NbrOfChannel=4;//触发4个通道
	ADC_Init(ADC1,&ADC_InitStructure);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&ADC1->DR;//AddrA;//0x4001244C绝对地址一般不用，通过数组名来获取地址
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;//16位
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//不自增,始终转运同一个位置
	DMA_InitStructure.DMA_MemoryBaseAddr= (uint32_t)AD_Value;//存储器地址，这次存放在SRAM中
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;//存储器地址自增
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;//外设站点作为数据源
	DMA_InitStructure.DMA_BufferSize=4;//传输数量4个
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;//是否启用重装，自动重装和软件触发不能同时使用否则会永远停不下来,循环模式
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;//disable硬件触发
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium;//优先级
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);//开启DMA触发信号
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);//开始校准置1
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//返回复位校准状态(==SET可略)，等待复位完成要加while循环;软件置标志位1，硬件开始校准完成后自动清零
	ADC_StartCalibration(ADC1);//开始校准，内部电路自动校准不需要操作
	while(ADC_GetCalibrationStatus(ADC1)==SET);//获取校准状态
//还有另外自动循环的方法，ADC用单次触发，额外添加定时器去触发定时，实现定时器触发ADC,ADC触发DMA
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//读取触发转换的数据
}

//void AD_GetValue(void)
//{
//	DMA_Cmd(DMA1_Channel1,DISABLE);//关闭DMA才能重新写值给传输计数器
//	DMA_SetCurrDataCounter(DMA1_Channel1,4); //
//	DMA_Cmd(DMA1_Channel1,ENABLE);
//	
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//读取触发转换的数据
//	
//	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);//转运完成标志位,转运需要时间RESET表示还没有完成等待转运结果
//	DMA_ClearFlag(DMA1_FLAG_TC1);
//}ADC连续扫描+DMA循环就不需要
