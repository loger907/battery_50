/*******************************************************************************
** �ļ���:            main.c
** ��󱣴�����:     2016��6��19��
** ����оƬ�ͺţ�    MSP430FR5949
** �޸�����:         1.�����˴��ڴ�ӡ�ַ������ܣ�2.������PWM�������
** �޸���:           weirongluan
** Author:           л����
** ˵����TBCLK=SMCK=MCLK=DCO=1M Hz��P1.4��P1.5�ֱ����75%��25%��PWM
*******************************************************************************/
#include <msp430fr5949.h>
#include"stdio.h"
#include "battery_50.h"
char string[5];
uint8_t data=0x00;
uint8_t Charg_flag=0;//����־λ
float DATA_ADC = 0;
int PWM_Duty=0;//PWMռ�ձ�
//float Sum_ADC=0;//�洢һ�����ܵ�ѹ
float Adc_value[5][10];//�ֱ�洢5������10��ͨ����ADC��ֵ
char temp=0;
void main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 //�رտ��Ź�
   Port_Init();//��Ƭ�����ų�ʼ��
   Init_Clk();//��ʼ��ʱ��.ACLK=32768��MCLK=SMCLK=1M
   Uart_Init();//��ʼ�����ڣ�Tx--P2.5;Rx--P2.6
   ADC12_Init();//��ʼ��ADC12
   PWM_Init();//PWM��ʼ��
   __bis_SR_register(GIE);  
   while(1)
   {
     DATA_ADC=Get_ADC(2);
     sprintf(string,"%1.3f",DATA_ADC);//�ѵ�ѹֵת�����ַ��������㴮�����,3λС���㣬һ������λ����1.23V
     Send_Str(string);
     Send_Str("V\n");
     __delay_cycles(5000);                    // Delay between conversions
      __bis_SR_register(LPM0_bits+GIE);             // ����˯��ģʽ�����ܴ��ж�
      __no_operation();                         // For debugger
   }

}
