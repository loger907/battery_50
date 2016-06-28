/*******************************************************************************
** 文件名:            main.c
** 最后保存日期:     2016年6月19日
** 兼容芯片型号：    MSP430FR5949
** 修改内容:         1.增加了串口打印字符串功能；2.增加了PWM输出函数
** 修改人:           weirongluan
** Author:           谢信奇
** 说明：TBCLK=SMCK=MCLK=DCO=1M Hz，P1.4、P1.5分别输出75%、25%的PWM
*******************************************************************************/
#include <msp430fr5949.h>
#include"stdio.h"
#include "battery_50.h"
char string[5];
uint8_t data=0x00;
uint8_t Charg_flag=0;//充电标志位
float DATA_ADC = 0;
int PWM_Duty=0;//PWM占空比
//float Sum_ADC=0;//存储一组电池总电压
float Adc_value[5][10];//分别存储5个区域10个通道的ADC的值
char temp=0;
void main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 //关闭看门狗
   Port_Init();//单片机引脚初始化
   Init_Clk();//初始化时钟.ACLK=32768，MCLK=SMCLK=1M
   Uart_Init();//初始化串口，Tx--P2.5;Rx--P2.6
   ADC12_Init();//初始化ADC12
   PWM_Init();//PWM初始化
   __bis_SR_register(GIE);  
   while(1)
   {
     DATA_ADC=Get_ADC(2);
     sprintf(string,"%1.3f",DATA_ADC);//把电压值转换成字符串，方便串口输出,3位小数点，一个整型位，如1.23V
     Send_Str(string);
     Send_Str("V\n");
     __delay_cycles(5000);                    // Delay between conversions
      __bis_SR_register(LPM0_bits+GIE);             // 进入睡眠模式并打开总打开中断
      __no_operation();                         // For debugger
   }

}
