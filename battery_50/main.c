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
** ��󱣴�����:     2016��1��21��
** ����оƬ�ͺţ�    MSP430FR5949
** �޸�����:         
** �޸���:           weirongluan
** Author:           л����
** ˵����TBCLK=SMCK=MCLK=DCO=1M Hz��P1.4��P1.5�ֱ����75%��25%��PWM
*******************************************************************************/
#include <msp430fr5949.h>
#include "battery_50.h"
void main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure GPIO
  P1DIR |= BIT4 | BIT5;                     // P1.4 and P1.5 output
  P1SEL0 |= BIT4 | BIT5;                    // P1.4 and P1.5 options select

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  TB0CCR0 = 1000-1;                         // PWM Period
  TB0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
  TB0CCR1 = 750;                            // CCR1 PWM duty cycle
  TB0CCTL2 = OUTMOD_7;                      // CCR2 reset/set
  TB0CCR2 = 250;                            // CCR2 PWM duty cycle
  TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR

  __bis_SR_register(LPM0_bits);             // Enter LPM0
  __no_operation();                         // For debugger
}
