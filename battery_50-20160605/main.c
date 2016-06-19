//*******************************************************************************
//  MSP430FR59x Demo - TimerB, PWM TB0.1-2, Up Mode, DCO SMCLK
//
//  Description: This program generates two PWM outputs on P1.4,P1.5 using
//  TimerB configured for up mode. The value in CCR0, 1000-1, defines the PWM
//  period and the values in CCR1 and CCR2 the PWM duty cycles. Using ~1MHz
//  SMCLK as TACLK, the timer period is ~1ms with a 75% duty cycle on P1.4
//  and 25% on P1.5.
//  ACLK = n/a, SMCLK = MCLK = TACLK = 1MHz
//
//
//           MSP430FR5969
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |     P1.4/TB0.1|--> CCR1 - 75% PWM
//        |     P1.5/TB0.2|--> CCR2 - 25% PWM
//
//   T. Witt/ P. Thanigai
//   Texas Instruments Inc.
//   November 2011
//   Built with IAR Embedded Workbench V5.30 & Code Composer Studio V5.5
//******************************************************************************
/*******************************************************************************
** 文件名:            main.c
** 最后保存日期:     2016年6月5日
** 兼容芯片型号：    MSP430FR5949
** 修改内容:         
** 修改人:           weirongluan
** Author:           谢信奇
** 说明：TBCLK=SMCK=MCLK=DCO=1M Hz，P1.4、P1.5分别输出75%、25%的PWM
*******************************************************************************/
#include <msp430fr5949.h>
#include "battery_50.h"
uint8_t data=0x00;
uint8_t Charg_flag=0;//充电标志位
void main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 //关闭看门狗
   Port_Init();//单片机引脚初始化
   Init_Clk();//初始化时钟.ACLK=32768，MCLK=SMCLK=1M
   Uart_Init();//初始化串口，Tx--P2.5;Rx--P2.6
//   PWM_Init();//PWM初始化
   while(1)
   {
//     data=Get_char();//从串口获取一个字节的数据
     Send_char(data++);//发送一个字节数据
      __bis_SR_register(LPM0_bits+GIE);             // 进入睡眠模式并打开总打开中断
      __no_operation();                         // For debugger
   }

}
