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
** �ļ���:            main.c
** ��󱣴�����:     2016��6��5��
** ����оƬ�ͺţ�    MSP430FR5949
** �޸�����:         
** �޸���:           weirongluan
** Author:           л����
** ˵����TBCLK=SMCK=MCLK=DCO=1M Hz��P1.4��P1.5�ֱ����75%��25%��PWM
*******************************************************************************/
#include <msp430fr5949.h>
#include "battery_50.h"
uint8_t data=0x00;
uint8_t Charg_flag=0;//����־λ
void main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 //�رտ��Ź�
   Port_Init();//��Ƭ�����ų�ʼ��
   Init_Clk();//��ʼ��ʱ��.ACLK=32768��MCLK=SMCLK=1M
   Uart_Init();//��ʼ�����ڣ�Tx--P2.5;Rx--P2.6
//   PWM_Init();//PWM��ʼ��
   while(1)
   {
//     data=Get_char();//�Ӵ��ڻ�ȡһ���ֽڵ�����
     Send_char(data++);//����һ���ֽ�����
      __bis_SR_register(LPM0_bits+GIE);             // ����˯��ģʽ�����ܴ��ж�
      __no_operation();                         // For debugger
   }

}
