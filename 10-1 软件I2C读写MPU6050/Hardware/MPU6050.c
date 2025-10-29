#include "stm32f10x.h"                  // Device header
#include "MYI2C.h"
#include "MPU6050_Reg.h"  

#define MPU6050_ADDRESS   0XD0

void MPU6050_WriteReg(uint8_t RegAdress,uint8_t Data)//����ʱ��ͼ
{
	MyI2C_Start();						//I2C��ʼ
	MyI2C_SendByte(MPU6050_ADDRESS);//��ʼ���������һ���ֽڣ�ָ���ӻ���ַ�Ͷ�дλ
	MyI2C_ReceiveAck();//����Ӧ��λ
	MyI2C_SendByte(RegAdress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAdress)//����ʱ��ͼ
{
	uint8_t  Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);//��д��ַ��Ϊ������������ȡ�ӻ�����
	MyI2C_ReceiveAck();//����Ӧ��λ
	MyI2C_SendByte(RegAdress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte((MPU6050_ADDRESS)|0X01);//��д��ַ��Ϊ������������ȡ�ӻ�����
	MyI2C_ReceiveAck();//����Ӧ��λ
	Data=MyI2C_ReceiveByte();//���������ֽڣ���forѭ������λ�����Ӧ��1����
	MyI2C_SendAck(1);//�������յ����ݸ��ӻ�Ӧ��,0��ʾ����Ӧ��ӻ����������һ�����ݣ�1ûӦ��ӻ�ֹͣ��������
	MyI2C_Stop();
	
	return Data;
}

void MPU6050_Init(void)
{
	MyI2C_Initn();
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
//C�����к���ֻ����һ������ֵ
//�ں����ⶨ��6��ȫ�ֱ������Ӻ�������������ֱ��д��ȫ�ֱ����6��ȫ�ֱ������������й���
//��2����ָ����б����ĵ�ַ������ʵ�ֶ෵��ֵ
//��3���ýṹ��Զ���������д�������������������ͷ���ֵҲ����
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

