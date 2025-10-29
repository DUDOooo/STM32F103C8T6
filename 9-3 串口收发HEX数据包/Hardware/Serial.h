#ifndef __SERIAL_H_
#define __SERIAL_H_
#include <stdio.h>
#include <stdarg.h>//·â×°sprintf

extern uint8_t Serial_TxPacket[];
extern uint8_t Serial_RxPacket[];


void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array,uint16_t Lenght);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number,uint8_t Lenght);
void Serial_Printf(char *format,...);

uint8_t Serial_GetRxData(void);
void USART1_IRQHandler(void);
uint8_t Serial_GetRxFlag(void);

void Serial_SendPacket(void);

#endif
