/*******************************************************************************
** 文件名:            battery.h
** 最后保存日期:     2016年6月5日
** 兼容芯片型号：    MSP430FR5949
** 修改内容:         
** 修改人:           weirongluan
** Author:           谢信奇
** 说明：整个工程的头文件，包含
          1.IO端口初始化函数声明；       
          3.全局变量定义；
          4.宏定义
          
*******************************************************************************/
#ifndef _battery_50_H_
#define _battery_50_H_
#include <msp430FR5949.h>
typedef unsigned char  uint8_t;
typedef unsigned int   uint16_t;
typedef unsigned long  uint32_t;
extern uint8_t Charg_flag;//充电标志位
extern float DATA_ADC;
void Init_Clk(void);//初始化时钟
void Uart_Init(void);//串口初始化
uint8_t Get_char(void);//接收一个字节的串口数据
void Send_char(uint8_t DATA);//发送一个字节的串口数据
void Port_Init(void);//单片机引脚初始化
void Self_Test(void);//开机的时候自检
void PWM_Init(void);//PWM初始化
void ADC12_Init();//初始化ADC12
float Get_ADC(char channel);//获取某个通道的电压值

#endif