#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_Time[]={2023, 1, 1,23,31,23};//C语中十进制前位补0会产生bug

void MyRTC_SetTime(void);
	
void MyRTC_Init(void)//注意Init
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//注意注意注意时钟函数
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	
	if(BKP_ReadBackupRegister(BKP_DR1)!=0XA5A5)
	{
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)!=SET);//等待LSE启动完成，不起振改为40Khz的LSI
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择RCCCLK时钟源
		RCC_RTCCLKCmd(ENABLE);
		
		RTC_WaitForSynchro();//等待同步
		RTC_WaitForLastTask();//等待上次写入操作完成
		
		RTC_SetPrescaler(32768-1);//减掉0
		RTC_WaitForLastTask();//
		
		MyRTC_SetTime();
		
		BKP_WriteBackupRegister(BKP_DR1,0XA5A5);//执行过的程序手动做标记，重新上电没有掉电丢数据就不用重新初始化
	}
	else
	{
		RTC_WaitForSynchro();//
		RTC_WaitForLastTask();//
	}
}

void MyRTC_SetTime(void)
{
	time_t time_cnt;//time_cnt变量名
	struct tm time_data;
	//把数组指定的时间填充到struct tm结构体中
	time_data.tm_year = MyRTC_Time[0]-1900;//偏移-1900
	time_data.tm_mon = MyRTC_Time[1]-1;//0-11代表12个月
	time_data.tm_mday = MyRTC_Time[2];
	time_data.tm_hour = MyRTC_Time[3];
	time_data.tm_min = MyRTC_Time[4];
	time_data.tm_sec = MyRTC_Time[5];
	
	time_cnt = mktime(&time_data)-8 * 60 * 60;//日期时间到秒数的转换，使用mktime得到秒数
	
	RTC_SetCounter(time_cnt);//将秒数写入到RCT中
	RTC_WaitForLastTask();//写入寄存器后等待操作完成
}


void MyRTC_ReadTime(void)
{
	time_t time_cnt;//time_cnt变量名
	struct tm time_data;
	//先定义秒计数器和日期时间的变量

	time_cnt = RTC_GetCounter()+ 8 *60 * 60;//读取CNT的秒数,0时区转换东八区
	
	time_data= *localtime(&time_cnt);//用locaitime得到日期时间
	
	MyRTC_Time[0]=time_data.tm_year + 1900;
	MyRTC_Time[1]=time_data.tm_mon + 1;
  MyRTC_Time[2]=time_data.tm_mday;
  MyRTC_Time[3]=time_data.tm_hour;
  MyRTC_Time[4]=time_data.tm_min;
  MyRTC_Time[5]=time_data.tm_sec;
}
