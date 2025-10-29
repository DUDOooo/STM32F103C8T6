#include "stm32f10x.h"                  // Device header
#include "Delay.h"  

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//����GPIO����APB2��
	GPIO_InitTypeDef GPIO_InitStructure;//�ṹ��Ҳ��һ�־ֲ���������ʼ������initstructure
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�ṹ���Աmode,pin,speed�Ҽ����Ƴ�������8�����ģʽ�����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;//LED��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	while(1)
	{
		GPIO_Write(GPIOA,~0X0001);//0000 0000 0000 0001��Ҳ������GPIO_Pin_0|GPIO_Pin_1|....
		Delay_ms(100);
		GPIO_Write(GPIOA,~0X0004);//0000 0000 0000 0100
		Delay_ms(100);
		GPIO_Write(GPIOA,~0X0008);//0000 0000 0000 1000
		Delay_ms(100);
		GPIO_Write(GPIOA,~0X0010);//0000 0000 0001 0000
		Delay_ms(100);
		GPIO_Write(GPIOA,~0X0020);//0000 0000 0010 0000
		Delay_ms(100);
		GPIO_Write(GPIOA,~0X0040);//0000 0000 0100 0000
		Delay_ms(100);
		GPIO_Write(GPIOA,~0X0080);//0000 0000 1000 0000����0-7,8��������ˮ
		Delay_ms(100);
	}
	
}
