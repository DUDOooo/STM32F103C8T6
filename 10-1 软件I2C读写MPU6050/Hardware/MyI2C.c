#include "stm32f10x.h"                  // Device header
#include "Delay.h"


//#define SCL_PORT   GPIOB
//#define SCL_PIN    GPIO_Pin_10
void MyI2C_W_SCL(uint8_t BitValue)//SCL����λд����
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);//P10=BitValue;51��Ƭ����SCL��P10
	Delay_us(10);//оƬ��Ƶ�Ͽ�Ҫ����ʱ,������ms�����ǻ������
}

void MyI2C_W_SDA(uint8_t BitValue)//BitValue�������ݷ�0��1������0x80�൱�ڴ���1������0x00�൱�ڴ���0
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);
	Delay_us(10);//
}

uint8_t MyI2C_R_SCL(void)//SCL��λ
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
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;//SCL��SDA���ڶ˿�
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void MyI2C_Start(void)//��ʼ����
{
	 MyI2C_W_SCL(1);
	 MyI2C_W_SDA(1);
	 MyI2C_W_SDA(0);
	 MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)//��ʼ����
{
// MyI2C_W_SCL(0);����Ӧ���Ĭ�ϵ͵�ƽ
	 MyI2C_W_SDA(0);//����ʱ��ͼҪ������SDAȷ��SDA���ڵ͵�ƽ������������
	 MyI2C_W_SCL(1);
	 MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)//������ȷ��Ҫ���͵��ֽ�,һ���ֽڰ�λ
{		
	uint8_t i;
	for(i=0;i<8;i++)
	{
		MyI2C_W_SDA(Byte&(0x80>>i));
		MyI2C_W_SCL(1);//��ȡ�ղ�д�������
		MyI2C_W_SCL(0);//�����ڽ���ʱҪ�ߵ�ƽ�����൥Ԫʱ��͵�ƽ���������ν�
	}
}

uint8_t MyI2C_ReceiveByte(void)//����������Ҫ����ֵ
{		
	uint8_t i, Byte=0x00;
	for(i=0;i<8;i++)
	{
	  MyI2C_W_SDA(1);//ȷ�������Ѿ��������еö�ȡ
		MyI2C_W_SCL(1);//������SCL�ߵ�ƽʱ��ȡ����
		if(MyI2C_R_SDA()==1){Byte|=(0x80>>i);}//�ѸղŶ�ȡ����ֵ�������λ,������ѭ��ʱ��SCLʱ���ӻ�������ı�SDA����ʱ��ȡ����SDA���Ǵӻ����Ʒ����
		MyI2C_W_SCL(0);//�͵�ƽ������λ��
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)//����Ӧ��
{		
		MyI2C_W_SDA(AckBit);
		MyI2C_W_SCL(1);//������AckBit����SDA�ϣ��ӻ���ȡӦ��
		MyI2C_W_SCL(0);//������һ��ʱ��Ԫ
}

uint8_t MyI2C_ReceiveAck(void)//����������Ҫ����ֵ
{		
		uint8_t AckBit;
		MyI2C_W_SDA(1);//�����ͷ�SDA����ֹ�ӻ����ţ�����ǿ��SDAΪ�ߵ�ƽ��I2C��������  
		MyI2C_W_SCL(1);//������SCL�ߵ�ƽʱ��ȡӦ��
		AckBit=MyI2C_R_SDA();//������ȡӦ��λ��0��ʾ�ӻ�����Ӧ��1��ʾ�ӻ�û��Ӧ��
		MyI2C_W_SCL(0);//�͵�ƽ������һ����Ԫ
	  return AckBit;
}
