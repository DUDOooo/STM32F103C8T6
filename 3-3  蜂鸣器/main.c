#include "stm32f10x.h"                  // Device header
#include "Delay.h"  

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//所有GPIO都在APB2上
	GPIO_InitTypeDef GPIO_InitStructure;//结构体也是一种局部变量，初始化命名initstructure
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//结构体成员mode,pin,speed右键复制出参数，8种输出模式，点灯用推挽
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;//LED所用引脚
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	while(1)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);//
		Delay_ms(100);
		GPIO_SetBits(GPIOB,GPIO_Pin_12);//
		Delay_ms(100);
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);//
		Delay_ms(100);
		GPIO_SetBits(GPIOB,GPIO_Pin_12);//
		Delay_ms(700);
	}
	
}
