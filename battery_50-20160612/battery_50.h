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
extern float Adc_value[5][10];//�ֱ�洢5������10��ͨ����ADC��ֵ
void Init_Clk(void);//��ʼ��ʱ��
void Uart_Init(void);//���ڳ�ʼ��
uint8_t Get_char(void);//����һ���ֽڵĴ�������
void Send_char(uint8_t DATA);//����һ���ֽڵĴ�������
void Send_Str(const char *str);//����һ���ַ����Ĵ�������
void Port_Init(void);//��Ƭ�����ų�ʼ��
uint8_t Self_Test(void);//������ʱ���Լ�
uint8_t IsOver_3V(void);//��������ذ���ѹ���Ƿ񳬹�3V����������򷵻�-1
void PWM_Init(void);//PWM��ʼ��
void ADC12_Init();//��ʼ��ADC12
float Get_ADC(char channel);//��ȡĳ��ͨ���ĵ�ѹֵ
void Samples_Adc(uint8_t count);//����ͨ��ADC����10�β�ȡƽ��ֵ���浽һ����������
//�궨��
#define On_IO1   P2OUT |= BIT2;//ʹ��IO1��ذ��
#define Off_IO1  P2OUT &=~BIT2;//��ֹIO1��ذ��
#define On_IO2   P3OUT |= BIT4;//ʹ��IO2��ذ��
#define Off_IO2  P3OUT &=~BIT4;//��ֹIO2��ذ��
#define On_IO3   P3OUT |= BIT5;//ʹ��IO3��ذ��
#define Off_IO3  P3OUT &=~BIT5;//��ֹIO3��ذ��
#define On_IO4   P3OUT |= BIT6;//ʹ��IO4��ذ��
#define Off_IO4  P3OUT &=~BIT6;//��ֹIO4��ذ��
#define On_IO5   P3OUT |= BIT7;//ʹ��IO5��ذ��
#define Off_IO5  P3OUT &=~BIT7;//��ֹIO5��ذ��

#endif