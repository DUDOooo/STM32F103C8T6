#ifndef __MYSPI_H_
#define __MYSPI_H_

void MySPI_W_SS(uint8_t BitValue);
void MySPI_Iint(void);
void MySPI_Start(void);
void MySPI_Stop(void);	
uint8_t MySPI_SwapByte(uint8_t ByteSend);

	
#endif
