#ifndef __ENCODER_H_
#define __ENCODER_H_

void  Encoder_Init(void);
int16_t Encoder_Get(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
	
#endif
