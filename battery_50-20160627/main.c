/*******************************************************************************
** �ļ���:            main.c
** ��󱣴�����:     2016��6��19��
** ����оƬ�ͺţ�    MSP430FR5949
** �޸�����:         1.���ӳ�纯��2.������������
** �޸���:           weirongluan
** Author:           л����
** ˵����TBCLK=SMCK=MCLK=DCO=1M Hz��P1.4��P1.5�ֱ����75%��25%��PWM
*******************************************************************************/
#include <msp430fr5949.h>
#include"stdio.h"
#include "battery_50.h"
char string[5];
uint8_t data=0x00;
uint8_t Charg_flag=0;                   //����־λ
char S1_flag=0;
char S2_flag=0;
float DATA_ADC = 0;
int PWM_Duty=0;                         //PWMռ�ձ�
//float Sum_ADC=0;//�洢һ�����ܵ�ѹ
float Adc_value[5][10];                 //�ֱ�洢5������10��ͨ����ADC��ֵ
char temp=0;
int error_flag =0;
char discharge_flag=0;
void main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                     //�رտ��Ź�
  Port_Init();                                  //��Ƭ�����ų�ʼ��
  Init_Clk();                                   //��ʼ��ʱ��.ACLK=32768��MCLK=SMCLK=1M
  Uart_Init();                                  //��ʼ�����ڣ�Tx--P2.5;Rx--P2.6
  ADC12_Init();                                 //��ʼ��ADC12
  PWM_Init();                                   //PWM��ʼ��
  Send_Str("Init have complete\n");  
  error_flag=Self_Test();                       //�״��Լ�
  if(error_flag == 1)//�Լ�ͨ��
  {
    Send_Str("Self test have pass\n");
    error_flag =0;
  }
  else if(error_flag < 0)//�Լ�ʧ��
  {
    Send_Str("Self test have failure\n");
    error_flag =0;
  }
    __bis_SR_register(GIE); 
  S1_flag=0;
  S2_flag=0;
  while(1)
  {
    if(S1_flag ==1)//���S1��������
    {
      S1_flag=0;
      error_flag=Self_Test();                   //�Լ�
      if(error_flag == 1)//�Լ�ͨ��
      {
        error_flag = 0;
        Send_Str("Self test have pass\n");
        //�������IO1-IO5ͬʱ�ŵ����
        On_IO1;//��I01,�ŵ�
        On_IO2;//��I02,�ŵ�
        On_IO4;//��I03,�ŵ�
        On_IO4;//��I04,�ŵ�
        On_IO5;//��I05,�ŵ�   
        discharge_flag=0;//�ŵ��־λ����
        Send_Str("Start Discharge... \n");
        while(!discharge_flag)//���û�ŵ����
        {
          for(char i=0;i<5;i++)//�жϸ�·��ѹ�Ƿ��е���3.v�ĵ��
          {
            Samples_Adc(i);                       //�Ե�i����в���
            if(3.0>Adc_value[i][0])//����е���3V�ĵ��
              Send_Str("Stop Discharge \n");      //ֹͣ�ŵ�
            //�������ֹͣ�ŵ����
            Off_IO1;//�ر�I01,ֹͣ�ŵ�
            Off_IO2;//�ر�I02,ֹͣ�ŵ�
            Off_IO4;//�ر�I03,ֹͣ�ŵ�
            Off_IO4;//�ر�I04,ֹͣ�ŵ�
            Off_IO5;//�ر�I05,ֹͣ�ŵ�
            discharge_flag=1;//�ŵ���ɱ�־λ
            for(char k=0;k<9;k++)//ȡ��ÿһ�����ߵ�ѹ
            {
              if(3.0>(Adc_value[i][k+1]-Adc_value[i][k]))//����е���3V�ĵ��
                Send_Str("Stop Discharge \n");    //ֹͣ�ŵ�
              //�������ֹͣ�ŵ����
              Off_IO1;//�ر�I01,ֹͣ�ŵ�
              Off_IO2;//�ر�I02,ֹͣ�ŵ�
              Off_IO4;//�ر�I03,ֹͣ�ŵ�
              Off_IO4;//�ر�I04,ֹͣ�ŵ�
              Off_IO5;//�ر�I05,ֹͣ�ŵ�
              discharge_flag=1;//�ŵ���ɱ�־λ
            }
          }
        }
      }
      else if(error_flag < 0)//�Լ�ʧ��
      {
        Send_Str("Self test have failure,please try again\n");//��ʾ�����Լ�
        error_flag =0;
      }
      
    }
    if(S2_flag ==1)//���S2��������
    {
      S2_flag = 0;
      Send_Str("The battery is on charge...\n");//������ڳ��
      Charging();                               //ʵ�ֳ�����������  
      Send_Str("Charging is complete\n");       //��ʾ������
    }

//    DATA_ADC=Get_ADC(2);
//    sprintf(string,"%1.3f",DATA_ADC);//�ѵ�ѹֵת�����ַ��������㴮�����,3λС���㣬һ������λ����1.23V
//    Send_Str(string);
//    Send_Str("V\n");
//    __delay_cycles(5000);                    // Delay between conversions
//    __bis_SR_register(LPM0_bits+GIE);             // ����˯��ģʽ�����ܴ��ж�
//    __no_operation();                         // For debugger
  }
  
}
