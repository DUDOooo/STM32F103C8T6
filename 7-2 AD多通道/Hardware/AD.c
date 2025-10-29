#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
 
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//此模式模拟输入专用，此时GPIO无效
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//多通道供下方函数切换
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

	
	ADC_InitTypeDef  ADC_InitStructure;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//NONE不使用外部触发即内部触发
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;;//连续触发
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_NbrOfChannel=1;//非扫描单次触发模式
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);//开始校准置1
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//返回复位校准状态(==SET可略)，等待复位完成要加while循环;软件置标志位1，硬件开始校准完成后自动清零
	ADC_StartCalibration(ADC1);//开始校准，内部电路自动校准不需要操作
	while(ADC_GetCalibrationStatus(ADC1)==SET);//获取校准状态

	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//读取触发转换的数据
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);//单次转换非扫描模式，手动更改通道在触发可不用DMA,改ADC_Channel多通道选择不能加_x变成单选择
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//读取触发转换的数据
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);
	return ADC_GetConversionValue(ADC1);  
}
