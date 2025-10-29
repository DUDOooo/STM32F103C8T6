#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
 
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//��ģʽģ������ר�ã���ʱGPIO��Ч
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//��ͨ�����·������л�
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

	
	ADC_InitTypeDef  ADC_InitStructure;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//NONE��ʹ���ⲿ�������ڲ�����
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;;//��������
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_NbrOfChannel=1;//��ɨ�赥�δ���ģʽ
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);//��ʼУ׼��1
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//���ظ�λУ׼״̬(==SET����)���ȴ���λ���Ҫ��whileѭ��;����ñ�־λ1��Ӳ����ʼУ׼��ɺ��Զ�����
	ADC_StartCalibration(ADC1);//��ʼУ׼���ڲ���·�Զ�У׼����Ҫ����
	while(ADC_GetCalibrationStatus(ADC1)==SET);//��ȡУ׼״̬

	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//��ȡ����ת��������
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);//����ת����ɨ��ģʽ���ֶ�����ͨ���ڴ����ɲ���DMA,��ADC_Channel��ͨ��ѡ���ܼ�_x��ɵ�ѡ��
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//��ȡ����ת��������
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);
	return ADC_GetConversionValue(ADC1);  
}
