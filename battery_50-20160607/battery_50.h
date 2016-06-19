/*******************************************************************************
** �ļ���:            battery.h
** ��󱣴�����:     2016��6��5��
** ����оƬ�ͺţ�    MSP430FR5949
** �޸�����:         
** �޸���:           weirongluan
** Author:           л����
** ˵�����������̵�ͷ�ļ�������
          1.IO�˿ڳ�ʼ������������       
          3.ȫ�ֱ������壻
          4.�궨��
          
*******************************************************************************/
#ifndef _battery_50_H_
#define _battery_50_H_
#include <msp430FR5949.h>
typedef unsigned char  uint8_t;
typedef unsigned int   uint16_t;
typedef unsigned long  uint32_t;
extern uint8_t Charg_flag;//����־λ
extern float DATA_ADC;
void Init_Clk(void);//��ʼ��ʱ��
void Uart_Init(void);//���ڳ�ʼ��
uint8_t Get_char(void);//����һ���ֽڵĴ�������
void Send_char(uint8_t DATA);//����һ���ֽڵĴ�������
void Port_Init(void);//��Ƭ�����ų�ʼ��
void Self_Test(void);//������ʱ���Լ�
void PWM_Init(void);//PWM��ʼ��
void ADC12_Init();//��ʼ��ADC12
float Get_ADC(char channel);//��ȡĳ��ͨ���ĵ�ѹֵ

#endif