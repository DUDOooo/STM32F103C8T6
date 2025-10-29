#include "stm32f10x.h"                  // Device header
#include "Delay.h"


//#define SCL_PORT   GPIOB
//#define SCL_PIN    GPIO_Pin_10
void MyI2C_W_SCL(uint8_t BitValue)//SCL进行位写操作
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);//P10=BitValue;51单片机的SCL是P10
	Delay_us(10);//芯片主频较快要加延时,这里用ms陀螺仪会检测很慢
}

void MyI2C_W_SDA(uint8_t BitValue)//BitValue传入数据非0即1，传入0x80相当于传入1，传入0x00相当于传入0
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);
	Delay_us(10);//
}

uint8_t MyI2C_R_SCL(void)//SCL读位
{
	uint8_t BitValue;
	BitValue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10);
	Delay_us(10);
	return BitValue;
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	Delay_us(10);
	return BitValue;
}

void MyI2C_Initn(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;//SCL和SDA所在端口
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void MyI2C_Start(void)//起始条件
{
	 MyI2C_W_SCL(1);
	 MyI2C_W_SDA(1);
	 MyI2C_W_SDA(0);
	 MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)//起始条件
{
// MyI2C_W_SCL(0);接收应答后默认低电平
	 MyI2C_W_SDA(0);//根据时序图要先拉低SDA确保SDA处于低电平后续才能拉高
	 MyI2C_W_SCL(1);
	 MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)//参数明确是要发送的字节,一个字节八位
{		
	uint8_t i;
	for(i=0;i<8;i++)
	{
		MyI2C_W_SDA(Byte&(0x80>>i));
		MyI2C_W_SCL(1);//提取刚才写入的数据
		MyI2C_W_SCL(0);//除了在结束时要高电平，其余单元时间低电平操作方便衔接
	}
}

uint8_t MyI2C_ReceiveByte(void)//接收数据需要返回值
{		
	uint8_t i, Byte=0x00;
	for(i=0;i<8;i++)
	{
	  MyI2C_W_SDA(1);//确保数据已经放上来有得读取
		MyI2C_W_SCL(1);//主机在SCL高电平时读取数据
		if(MyI2C_R_SDA()==1){Byte|=(0x80>>i);}//把刚才读取到的值放在最高位,当主机循环时钟SCL时，从机有义务改变SDA，此时读取到的SDA都是从机控制放入的
		MyI2C_W_SCL(0);//低电平结束此位读
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)//发送应答
{		
		MyI2C_W_SDA(AckBit);
		MyI2C_W_SCL(1);//主机把AckBit放在SDA上，从机读取应答
		MyI2C_W_SCL(0);//进入下一个时序单元
}

uint8_t MyI2C_ReceiveAck(void)//接收数据需要返回值
{		
		uint8_t AckBit;
		MyI2C_W_SDA(1);//主机释放SDA，防止从机干扰，不是强制SDA为高电平，I2C是弱上拉  
		MyI2C_W_SCL(1);//主机在SCL高电平时读取应答
		AckBit=MyI2C_R_SDA();//主机读取应答位，0表示从机给了应答，1表示从机没给应答
		MyI2C_W_SCL(0);//低电平进入下一个单元
	  return AckBit;
}
