#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
 
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//��ģʽģ������ר�ã���ʱGPIO��Ч
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);//��ɨ��ͨ��ֻ��һ����Ч����rank1,������ʱ��Ҫ�����ת����С�Ĳ��������ȶ��ô����

	
	ADC_InitTypeDef  ADC_InitStructure;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//NONE��ʹ���ⲿ�������ڲ�����
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;//��������
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_NbrOfChannel=1;//��ɨ�赥�δ���ģʽ
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);//��ʼУ׼��1
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//���ظ�λУ׼״̬(==SET����)���ȴ���λ���Ҫ��whileѭ��;����ñ�־λ1��Ӳ����ʼУ׼��ɺ��Զ�����
	ADC_StartCalibration(ADC1);//��ʼУ׼���ڲ���·�Զ�У׼����Ҫ����
	while(ADC_GetCalibrationStatus(ADC1)==SET);//��ȡУ׼״̬
	
}

uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//��ȡ����ת��������
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);//ת����Ҫ�ȴ�ʱ�䣬RESET0ת��δ���ѭ�����������Ӳ���Զ���1,EOC�������ע������ɶ�����1
//	//ADCCLK����������72MHZ6��Ƶ=12mhz��ͨ��ת������������55.5��ת�����ڹ̶���12.5��55.5+12.5=68��12mhz����68������ת��������ɣ�1/12*68=5.6usת��ʱ��
	return ADC_GetConversionValue(ADC1);//����������Զ����DR��־λ
}
