#include "stm32f10x.h"                  // Device header
#include "PWM.h"   

void Servo_Init(void)
{
	PWM_Init();
}

void Servo_SetAngle(float Angle)
{
	PWM_SetCompare2(Angle/180*2000+500);//补500作为初始值
}
//0-500ms
//180-2500ms
