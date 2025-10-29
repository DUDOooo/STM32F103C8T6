#include "stm32f10x.h"                  // Device header
#include "Delay.h"  

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//所有GPIO都在APB2上
	GPIO_InitTypeDef GPIO_InitStructure;//结构体也是一种局部变量，初始化命名initstructure
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//结构体成员mode,pin,speed右键复制出参数，8种输出模式，点灯用推挽
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;//LED所用引脚
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	while(1)
	{
		GPIO_Write(GPIOA,~0X0001);//0000 0000 0000 0001，也可以用GPIO_Pin_0|GPIO_Pin_1|....
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
		GPIO_Write(GPIOA,~0X0080);//0000 0000 1000 0000接了0-7,8个灯跑流水
		Delay_ms(100);
	}
	
}
