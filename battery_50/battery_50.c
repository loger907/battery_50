#include<msp430fr5949.H>
#include"battery_50.h"
/*******************************************************************************
** 函数名:   Port_Init()
** 功能  :   IO初始化函数
** 输入参数: 无
** 返回参数: 无
** Author:   weirongluan
*******************************************************************************/
void Port_Init(void)
{
  // 把所有IO配置成输出低电平，保证最低功耗
  P1OUT = 0;                                // All P1.x reset
  P1DIR = 0xFF;                             // All P1.x outputs
  P2OUT = 0;                                // All P2.x reset
  P2DIR = 0xFF;                             // All P2.x outputs
  P3OUT = 0;                                // All P3.x reset
  P3DIR = 0xFF;                             // All P3.x outputs
  P4OUT = 0;                                // All P4.x reset
  P4DIR = 0xFF;                             // All P4.x outputs
  PJOUT = 0;                                // All PJ.x reset
  PJDIR = 0xFFFF;                           // All PJ.x outputs
  PJSEL0 = BIT4 | BIT5;                     // 配置低频晶振引脚
  //初始化为高电平引脚
  P3DIR |= BIT3;
  P3OUT |= BIT3;//DQ1   温度传感器总线
  P1DIR |= BIT6+BIT7;
  P1OUT |= BIT6+BIT7;//按键3  自检按键;按键2  电池板块切换按键
  P2DIR |= BIT7;
  P2OUT |= BIT7;//IN/KEY  充电识别脚
  P4DIR |= BIT4;
  P4OUT |= BIT4;//按键1  开机按键
  //初始化为低电平引脚
  P2DIR |= BIT0+BIT1+BIT2;
  P2OUT &=~(BIT0+BIT1+BIT2);//P2.0 PWM充电电流控制脚;P2.1  IN/GNDS 充电允许脚;P2.2  电池板块1 激活脚
  P3DIR |= BIT4+BIT5+BIT6+BIT7;
  P3OUT &=~(BIT4+BIT5+BIT6+BIT7);//P3.4  电池板块2激活脚;P3.5  电池板块3激活脚;P3.6  电池板块4激活脚;P3.7  电池板块5激活脚
  //ADC功能引脚配置
  P1SEL0 |=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;
  P1SEL1 |=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;//配置A0、A1、A2、A3、A4、A5引脚功能
  P2SEL0 |=BIT3+BIT4;
  P2SEL1 |=BIT3+BIT4;//配置A6、A7引脚功能
  P3SEL0 |=BIT0+BIT1+BIT2;
  P3SEL1 |=BIT0+BIT1+BIT2;//配置A12、A13、A14引脚功能
  
}
/*******************************************************************************
** 函数名:   Self_Test()
** 功能  :   开机后自检函数
** 输入参数: 无
** 返回参数: 无
** Author:   weirongluan
*******************************************************************************/
void Self_Test(void)
{
  ;
}






























