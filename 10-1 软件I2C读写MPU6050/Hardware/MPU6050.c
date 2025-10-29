#include "stm32f10x.h"                  // Device header
#include "MYI2C.h"
#include "MPU6050_Reg.h"  

#define MPU6050_ADDRESS   0XD0

void MPU6050_WriteReg(uint8_t RegAdress,uint8_t Data)//根据时序图
{
	MyI2C_Start();						//I2C起始
	MyI2C_SendByte(MPU6050_ADDRESS);//起始后紧跟发送一个字节，指定从机地址和读写位
	MyI2C_ReceiveAck();//返回应答位
	MyI2C_SendByte(RegAdress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAdress)//根据时序图
{
	uint8_t  Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);//读写地址改为读，接下来读取从机数据
	MyI2C_ReceiveAck();//返回应答位
	MyI2C_SendByte(RegAdress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte((MPU6050_ADDRESS)|0X01);//读写地址改为读，接下来读取从机数据
	MyI2C_ReceiveAck();//返回应答位
	Data=MyI2C_ReceiveByte();//主机接收字节，加for循环读多位，最后应答1结束
	MyI2C_SendAck(1);//主机接收到数据给从机应答,0表示发了应答从机会继续发下一个数据，1没应答从机停止发送数据
	MyI2C_Stop();
	
	return Data;
}

void MPU6050_Init(void)
{
	MyI2C_Initn();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);//电源管理寄存器1，，解除第二位睡眠模式给0，000是内部时钟，001陀螺仪时钟
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);//电源管理寄存器1，00是循环唤醒模式不需要，后6个轴都不待机给0
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);//分频，0-9档越小越快，0x09十分频
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);//配置寄存器，3-5外部同步不需要就都给0，最后3位数字低通滤波器110滤波
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);//陀螺仪配置寄存器，前3位自测使能不测给0,45满量选择最大成11，
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);//加速度配置寄存器，前3位自测使能不测给0，45满量选择最大成11，最后3位数字低通滤波器000滤波
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}
//C语言中函数只能有一个返回值
//在函数外定义6个全局变量，子函数读到的数据直接写到全局变量里，6个全局变量在主函数中共享
//法2：用指针进行变量的地址传递来实现多返回值
//法3：用结构体对多个变量进行打包输入参数，输出参数和返回值也可以
void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)//在主函数中定义变量,通过指针将主函数变量地址传递到子函数，子函数操控主函数的变量
{
	uint16_t DataH,DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);//加速度寄存器X轴的高8位,子函数读取指定地址
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);//加速度寄存器X轴的低8位
	*AccX=(DataH<<8)|DataL;//将高8位和低8位凑成16位
	
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

