#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"  

#define MPU6050_ADDRESS   0XD0

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)//替代大量死循环，死循环出不来下面的程序都卡死
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

void MPU6050_WriteReg(uint8_t RegAdress,uint8_t Data)//根据时序图
{
	I2C_GenerateSTART(I2C2,ENABLE);
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);//非阻塞式程序要加标志位查看执行结果，用状态监控式函数，手册中用EV5标志位检测起始函数

	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);//发送和接收自带接收发送过程
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//EV6
	
	I2C_SendData(I2C2,RegAdress);
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING);//EV8
	
	I2C_SendData(I2C2,Data);
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);//EV82

	I2C_GenerateSTOP(I2C2,ENABLE);
}

uint8_t MPU6050_ReadReg(uint8_t RegAdress)//根据时序图
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

		I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Receiver);//现在只读一个字节，要读多个直接可直接添加FOR循环
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
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;//复用开漏输出，I2C需要开漏不能强上拉，复用GPIO的控制权要给外设；软件I2C通过程序控制引脚是通用开漏
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;
	I2C_InitStructure.I2C_ClockSpeed=50000;//频率
	I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;//2；1时钟占空比在频率大鱼板100KHZ才有用，小于100k都是1：1，低：高
	I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;//指定stm32为从机可以响应几位地址
	I2C_InitStructure.I2C_OwnAddress1=0x00;//stm32不做从机随意天但要避免重复其他地址
	I2C_Init(I2C2,&I2C_InitStructure);
	
	I2C_Cmd(I2C2,ENABLE);
	
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

