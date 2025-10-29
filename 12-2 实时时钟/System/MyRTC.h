#ifndef __MYRTC_H_
#define __MYRTC_H_

extern uint16_t MyRTC_Time[];

void MyRTC_Init(void);
void MyRTC_SetTime(void);
void MyRTC_ReadTime(void);
	
#endif
