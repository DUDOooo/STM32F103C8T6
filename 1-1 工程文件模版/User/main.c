#include "stm32f10x.h"                  // Device header

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʱ��
	GPIO_InitTypeDef GPIO_InitStructure;//�ṹ�����������MODE,PIN,SPEED
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//ͨ���������ģʽ,�һ���ͷ�ļ�������CtrlF������������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;//ѡIO��,PC13
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);//&ȡ��ַ����
	// GPIO_SetBits(GPIOC,GPIO_Pin_13);//�øߵ�ƽ
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//�õ͵�ƽ
	while(1)
	{
		
	}
}
 
//�Ĵ�������
//int main(void)//int�ͷ���ֵ��void�����ĺ���
//{
//	RCC->APB2ENR=0X00000010;//GPIO����APB2�����裬�ֲ���IOPCEN��ʹ��GPIO��ʱ�ӵ���λ1��ʱ�ӡ�������27��0+10000=ʮ������0010
//	GPIOC->CRH=0X00300000;//P13��LED����ģʽ	����+50mhz
//	GPIOC->ODR=0X00002000;//�����������LED������ͬʱ����������λ����|=��&=��Ӱ������λ
//	while(1)
//	{
//		
//	}
//}
//�ļ����һ�������У�����ᱨ���棬������Encode in UTF-8��ֹ����
