#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"  

#define MPU6050_ADDRESS   0XD0

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)//���������ѭ������ѭ������������ĳ��򶼿���
{
	uint32_t Timeout;
	Timeout=10000;
	while(I2C_CheckEvent(I2Cx,I2C_EVENT)!=SUCCESS)
	{
		Timeout--;
		if(Timeout==0)
		{
			break ;
		}
	}
}

void MPU6050_WriteReg(uint8_t RegAdress,uint8_t Data)//����ʱ��ͼ
{
	I2C_GenerateSTART(I2C2,ENABLE);
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);//������ʽ����Ҫ�ӱ�־λ�鿴ִ�н������״̬���ʽ�������ֲ�����EV5��־λ�����ʼ����

	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);//���ͺͽ����Դ����շ��͹���
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//EV6
	
	I2C_SendData(I2C2,RegAdress);
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING);//EV8
	
	I2C_SendData(I2C2,Data);
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);//EV82

	I2C_GenerateSTOP(I2C2,ENABLE);
}

uint8_t MPU6050_ReadReg(uint8_t RegAdress)//����ʱ��ͼ
{
	uint8_t  Data;
	
		I2C_GenerateSTART(I2C2,ENABLE);
		MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);

		I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);
		MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
		
		I2C_SendData(I2C2,RegAdress);
		MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);
		
		I2C_GenerateSTART(I2C2,ENABLE);
		MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);

		I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Receiver);//����ֻ��һ���ֽڣ�Ҫ�����ֱ�ӿ�ֱ�����FORѭ��
		MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
		
		I2C_AcknowledgeConfig(I2C2,DISABLE);//ACK=0
		I2C_GenerateSTOP(I2C2,ENABLE);//STOP=1
		
		MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED);
		Data=I2C_ReceiveData(I2C2);
		
		I2C_AcknowledgeConfig(I2C2,ENABLE);//ACK=1
		
		return Data;
}

void MPU6050_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;//���ÿ�©�����I2C��Ҫ��©����ǿ����������GPIO�Ŀ���ȨҪ�����裻���I2Cͨ���������������ͨ�ÿ�©
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;
	I2C_InitStructure.I2C_ClockSpeed=50000;//Ƶ��
	I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;//2��1ʱ��ռ�ձ���Ƶ�ʴ����100KHZ�����ã�С��100k����1��1���ͣ���
	I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;//ָ��stm32Ϊ�ӻ�������Ӧ��λ��ַ
	I2C_InitStructure.I2C_OwnAddress1=0x00;//stm32�����ӻ������쵫Ҫ�����ظ�������ַ
	I2C_Init(I2C2,&I2C_InitStructure);
	
	I2C_Cmd(I2C2,ENABLE);
	
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);//��Դ����Ĵ���1��������ڶ�λ˯��ģʽ��0��000���ڲ�ʱ�ӣ�001������ʱ��
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);//��Դ����Ĵ���1��00��ѭ������ģʽ����Ҫ����6���ᶼ��������0
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);//��Ƶ��0-9��ԽСԽ�죬0x09ʮ��Ƶ
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);//���üĴ�����3-5�ⲿͬ������Ҫ�Ͷ���0�����3λ���ֵ�ͨ�˲���110�˲�
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);//���������üĴ�����ǰ3λ�Բ�ʹ�ܲ����0,45����ѡ������11��
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);//���ٶ����üĴ�����ǰ3λ�Բ�ʹ�ܲ����0��45����ѡ������11�����3λ���ֵ�ͨ�˲���000�˲�
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)//���������ж������,ͨ��ָ�뽫������������ַ���ݵ��Ӻ������Ӻ����ٿ��������ı���
{
	uint16_t DataH,DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);//���ٶȼĴ���X��ĸ�8λ,�Ӻ�����ȡָ����ַ
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);//���ٶȼĴ���X��ĵ�8λ
	*AccX=(DataH<<8)|DataL;//����8λ�͵�8λ�ճ�16λ
	
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY=(DataH<<8)|DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ=(DataH<<8)|DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX=(DataH<<8)|DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY=(DataH<<8)|DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ=(DataH<<8)|DataL;
}

