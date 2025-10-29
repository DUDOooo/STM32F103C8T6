#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
 
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//此模式模拟输入专用，此时GPIO无效
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);//非扫描通道只第一个有效所以rank1,最后采样时间要更快的转换用小的参数，更稳定用大参数

	
	ADC_InitTypeDef  ADC_InitStructure;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//NONE不使用外部触发即内部触发
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;//连续触发
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_NbrOfChannel=1;//非扫描单次触发模式
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);//开始校准置1
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//返回复位校准状态(==SET可略)，等待复位完成要加while循环;软件置标志位1，硬件开始校准完成后自动清零
	ADC_StartCalibration(ADC1);//开始校准，内部电路自动校准不需要操作
	while(ADC_GetCalibrationStatus(ADC1)==SET);//获取校准状态
	
}

uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//读取触发转换的数据
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);//转化需要等待时间，RESET0转换未完成循环继续，完成硬件自动置1,EOC规则组或注入组完成都会置1
//	//ADCCLK上面配置了72MHZ6分频=12mhz，通道转换周期配置了55.5，转换周期固定是12.5，55.5+12.5=68；12mhz进行68个周期转化才能完成，1/12*68=5.6us转换时间
	return ADC_GetConversionValue(ADC1);//这个函数会自动清除DR标志位
}
