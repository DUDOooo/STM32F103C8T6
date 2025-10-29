#include "stm32f10x.h"                  // Device header
#include <stdio.h>//printf
#include <stdarg.h>//封装sprintf

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//接收可上拉可浮空
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;//波特率，内部会自动算好分频给BRR寄存器
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//不使用流控，ctrl+alt+空格引出联想参数
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No; //不校验
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//字长
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启RXNE标志位到NVIC的输出
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//手册中规定USART_FLAG_TXE写入后自动清零
}

void Serial_SendArray(uint8_t *Array,uint16_t Lenght)//传递数组需要用指针,*指向首地址
{
	uint16_t i;
	for(i=0;i<Lenght;i++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for(i=0;String[i]!='\0';i++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result=1;
	while(Y--)
	{
		Result*=X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number,uint8_t Lenght)
{
	uint8_t i;
	for(i=0;i<Lenght;i++)
	{
		Serial_SendByte(Number/Serial_Pow(10,Lenght-i-1)%10+'0');
	}
}

int fputc(int ch,FILE*f)//printf函数移植
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format,...)//...用于接收后面的可变量参数列表
{
	char String[100];//定义输出字符串
	va_list arg;//va_list类型名，arg是变量名
	va_start(arg,format);//从foemat开始接收参数表，放在arg里面
	vsprintf(String ,format,arg);//封装格式用vsprintf
	va_end(arg);//释放参数列表
	Serial_SendString(String);//把string发送出去
}

uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag==1)
	{
		Serial_RxFlag=0;
		return 1;
	}
	return 0;
}
	
	
uint8_t Serial_GetRxData()
{
	return Serial_RxData;
}

void USART1_IRQHandler(void)//mds中找，在中断函数里面接收数据
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET )//先判断中断标志位
	{
		Serial_RxData=USART_ReceiveData(USART1); 
		Serial_RxFlag=1;
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
