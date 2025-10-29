#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_Time[]={2023, 1, 1,23,31,23};//C����ʮ����ǰλ��0�����bug

void MyRTC_SetTime(void);
	
void MyRTC_Init(void)//ע��Init
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//ע��ע��ע��ʱ�Ӻ���
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	
	if(BKP_ReadBackupRegister(BKP_DR1)!=0XA5A5)
	{
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)!=SET);//�ȴ�LSE������ɣ��������Ϊ40Khz��LSI
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//ѡ��RCCCLKʱ��Դ
		RCC_RTCCLKCmd(ENABLE);
		
		RTC_WaitForSynchro();//�ȴ�ͬ��
		RTC_WaitForLastTask();//�ȴ��ϴ�д��������
		
		RTC_SetPrescaler(32768-1);//����0
		RTC_WaitForLastTask();//
		
		MyRTC_SetTime();
		
		BKP_WriteBackupRegister(BKP_DR1,0XA5A5);//ִ�й��ĳ����ֶ�����ǣ������ϵ�û�е��綪���ݾͲ������³�ʼ��
	}
	else
	{
		RTC_WaitForSynchro();//
		RTC_WaitForLastTask();//
	}
}

void MyRTC_SetTime(void)
{
	time_t time_cnt;//time_cnt������
	struct tm time_data;
	//������ָ����ʱ����䵽struct tm�ṹ����
	time_data.tm_year = MyRTC_Time[0]-1900;//ƫ��-1900
	time_data.tm_mon = MyRTC_Time[1]-1;//0-11����12����
	time_data.tm_mday = MyRTC_Time[2];
	time_data.tm_hour = MyRTC_Time[3];
	time_data.tm_min = MyRTC_Time[4];
	time_data.tm_sec = MyRTC_Time[5];
	
	time_cnt = mktime(&time_data)-8 * 60 * 60;//����ʱ�䵽������ת����ʹ��mktime�õ�����
	
	RTC_SetCounter(time_cnt);//������д�뵽RCT��
	RTC_WaitForLastTask();//д��Ĵ�����ȴ��������
}


void MyRTC_ReadTime(void)
{
	time_t time_cnt;//time_cnt������
	struct tm time_data;
	//�ȶ����������������ʱ��ı���

	time_cnt = RTC_GetCounter()+ 8 *60 * 60;//��ȡCNT������,0ʱ��ת��������
	
	time_data= *localtime(&time_cnt);//��locaitime�õ�����ʱ��
	
	MyRTC_Time[0]=time_data.tm_year + 1900;
	MyRTC_Time[1]=time_data.tm_mon + 1;
  MyRTC_Time[2]=time_data.tm_mday;
  MyRTC_Time[3]=time_data.tm_hour;
  MyRTC_Time[4]=time_data.tm_min;
  MyRTC_Time[5]=time_data.tm_sec;
}
