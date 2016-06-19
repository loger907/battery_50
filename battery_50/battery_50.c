#include<msp430fr5949.H>
#include"battery_50.h"
/*******************************************************************************
** ������:   Port_Init()
** ����  :   IO��ʼ������
** �������: ��
** ���ز���: ��
** Author:   weirongluan
*******************************************************************************/
void Port_Init(void)
{
  // ������IO���ó�����͵�ƽ����֤��͹���
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
  PJSEL0 = BIT4 | BIT5;                     // ���õ�Ƶ��������
  //��ʼ��Ϊ�ߵ�ƽ����
  P3DIR |= BIT3;
  P3OUT |= BIT3;//DQ1   �¶ȴ���������
  P1DIR |= BIT6+BIT7;
  P1OUT |= BIT6+BIT7;//����3  �Լ찴��;����2  ��ذ���л�����
  P2DIR |= BIT7;
  P2OUT |= BIT7;//IN/KEY  ���ʶ���
  P4DIR |= BIT4;
  P4OUT |= BIT4;//����1  ��������
  //��ʼ��Ϊ�͵�ƽ����
  P2DIR |= BIT0+BIT1+BIT2;
  P2OUT &=~(BIT0+BIT1+BIT2);//P2.0 PWM���������ƽ�;P2.1  IN/GNDS ��������;P2.2  ��ذ��1 �����
  P3DIR |= BIT4+BIT5+BIT6+BIT7;
  P3OUT &=~(BIT4+BIT5+BIT6+BIT7);//P3.4  ��ذ��2�����;P3.5  ��ذ��3�����;P3.6  ��ذ��4�����;P3.7  ��ذ��5�����
  //ADC������������
  P1SEL0 |=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;
  P1SEL1 |=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;//����A0��A1��A2��A3��A4��A5���Ź���
  P2SEL0 |=BIT3+BIT4;
  P2SEL1 |=BIT3+BIT4;//����A6��A7���Ź���
  P3SEL0 |=BIT0+BIT1+BIT2;
  P3SEL1 |=BIT0+BIT1+BIT2;//����A12��A13��A14���Ź���
  
}
/*******************************************************************************
** ������:   Self_Test()
** ����  :   �������Լ캯��
** �������: ��
** ���ز���: ��
** Author:   weirongluan
*******************************************************************************/
void Self_Test(void)
{
  ;
}






























