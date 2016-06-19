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
extern float Adc_value[5][10];//分别存储5个区域10个通道的ADC的值
void Init_Clk(void);//初始化时钟
void Uart_Init(void);//串口初始化
uint8_t Get_char(void);//接收一个字节的串口数据
void Send_char(uint8_t DATA);//发送一个字节的串口数据
void Send_Str(const char *str);//发送一个字符串的串口数据
void Port_Init(void);//单片机引脚初始化
uint8_t Self_Test(void);//开机的时候自检
uint8_t IsOver_3V(void);//检查各个电池板块的压差是否超过3V，如果超过则返回-1
void PWM_Init(void);//PWM初始化
void ADC12_Init();//初始化ADC12
float Get_ADC(char channel);//获取某个通道的电压值
void Samples_Adc(uint8_t count);//所有通道ADC采样10次并取平均值保存到一个数组里面
//宏定义
#define On_IO1   P2OUT |= BIT2;//使能IO1电池板块
#define Off_IO1  P2OUT &=~BIT2;//禁止IO1电池板块
#define On_IO2   P3OUT |= BIT4;//使能IO2电池板块
#define Off_IO2  P3OUT &=~BIT4;//禁止IO2电池板块
#define On_IO3   P3OUT |= BIT5;//使能IO3电池板块
#define Off_IO3  P3OUT &=~BIT5;//禁止IO3电池板块
#define On_IO4   P3OUT |= BIT6;//使能IO4电池板块
#define Off_IO4  P3OUT &=~BIT6;//禁止IO4电池板块
#define On_IO5   P3OUT |= BIT7;//使能IO5电池板块
#define Off_IO5  P3OUT &=~BIT7;//禁止IO5电池板块

#endif