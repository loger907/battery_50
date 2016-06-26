/*******************************************************************************
** 文件名:            main.c
** 最后保存日期:     2016年6月19日
** 兼容芯片型号：    MSP430FR5949
** 修改内容:         1.增加充电函数2.完善了主流程
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
char S1_flag=0;
char S2_flag=0;
float DATA_ADC = 0;
int PWM_Duty=0;//PWM占空比
//float Sum_ADC=0;//存储一组电池总电压
float Adc_value[5][10];//分别存储5个区域10个通道的ADC的值
char temp=0;
int error_flag =0;
void main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 //关闭看门狗
  Port_Init(); //单片机引脚初始化
  Init_Clk();//初始化时钟.ACLK=32768，MCLK=SMCLK=1M
  Uart_Init();//初始化串口，Tx--P2.5;Rx--P2.6
  ADC12_Init();//初始化ADC12
  PWM_Init();//PWM初始化
  Send_Str("Init have complete\n");  
  error_flag=Self_Test();//首次自检
  if(error_flag == 1)//自检通过
  {
    Send_Str("Self test have pass\n");
    error_flag =0;
  }
  else if(error_flag < 0)//自检失败
  {
    Send_Str("Self test have failure\n");
    error_flag =0;
  }
    __bis_SR_register(GIE); 
  S1_flag=0;
  S2_flag=0;
  while(1)
  {
    if(S1_flag ==1)//如果S1按键按下
    {
      S1_flag=0;
      error_flag=Self_Test();//自检
      if(error_flag == 1)//自检通过
      {
        error_flag = 0;
        Send_Str("Self test have pass\n");
        //这里添加IO1-IO5同时放电操作
        Send_Str("Start Discharge \n");
        for(char i=0;i<5;i++)//判断各路电压是否有低于3.v的电池
        {
          Samples_Adc(i);//对第i组进行采样
          if(3.0>Adc_value[i][0])//如果有低于3V的电池
            Send_Str("Stop Discharge \n");//停止放电
            //这里添加停止放点操作
          for(char k=0;k<9;k++)//取出每一组的最高电压
          {
            if(3.0>(Adc_value[i][k+1]-Adc_value[i][k]))//如果有低于3V的电池
              Send_Str("Stop Discharge \n");//停止放电
            //这里添加停止放点操作
          }
        }
      }
      else if(error_flag < 0)//自检失败
      {
        Send_Str("Self test have failure\n");
        error_flag =0;
      }
      
    }
    if(S2_flag ==1)//如果S2按键按下
    {
      S2_flag = 0;
      Send_Str("The battery is on charge...\n");//电池正在充电
      Charging();//实现充电的整个过程  
      Send_Str("Charging is complete\n");//充电完成
    }

//    DATA_ADC=Get_ADC(2);
//    sprintf(string,"%1.3f",DATA_ADC);//把电压值转换成字符串，方便串口输出,3位小数点，一个整型位，如1.23V
//    Send_Str(string);
//    Send_Str("V\n");
//    __delay_cycles(5000);                    // Delay between conversions
//    __bis_SR_register(LPM0_bits+GIE);             // 进入睡眠模式并打开总打开中断
//    __no_operation();                         // For debugger
  }
  
}
