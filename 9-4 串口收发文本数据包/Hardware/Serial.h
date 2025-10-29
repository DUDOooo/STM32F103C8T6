#ifndef __SERIAL_H_
#define __SERIAL_H_
#include <stdio.h>
#include <stdarg.h>//��װsprintf

extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array,uint16_t Lenght);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number,uint8_t Lenght);
void Serial_Printf(char *format,...);


#endif
