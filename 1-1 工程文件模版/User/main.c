#include "stm32f10x.h"                  // Device header

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//时钟
	GPIO_InitTypeDef GPIO_InitStructure;//结构体的三个参数MODE,PIN,SPEED
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//通用推挽输出模式,右击打开头文件定义再CtrlF搜索定义内容
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;//选IO口,PC13
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);//&取地址符号
	// GPIO_SetBits(GPIOC,GPIO_Pin_13);//置高电平
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//置低电平
	while(1)
	{
		
	}
}
 
//寄存器配置
//int main(void)//int型返回值，void参数的函数
//{
//	RCC->APB2ENR=0X00000010;//GPIO都是APB2的外设，手册中IOPCEN是使能GPIO的时钟第四位1打开时钟→二级制27个0+10000=十六进制0010
//	GPIOC->CRH=0X00300000;//P13口LED引脚模式	推挽+50mhz
//	GPIOC->ODR=0X00002000;//数据输出点亮LED，但是同时控制了其他位，用|=和&=不影响其他位
//	while(1)
//	{
//		
//	}
//}
//文件最后一行留空行，否则会报警告，字体用Encode in UTF-8防止乱码
