/*******************************************************************************
** �ļ���:            battery_50.c
** ��󱣴�����:     2016��6��5��
** ����оƬ�ͺţ�    MSP430FR5949
** �޸�����:         
** �޸���:           л����      
** Author:           xiexinqi
** ����˵����
**�����СΪ��
*******************************************************************************/
#include<msp430fr5949.H>
#include<math.h>
#include"battery_50.h"
/*******************************************************************************
** ������:   Init_Clk()
** ����  :   ����ʱ��MCLK=SMCLK=1M��ACLK=32768
** �������: ��
** ���ز���: ��
** Author:   л����
*******************************************************************************/
void Init_Clk(void)
{
    // Clock System Setup
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz��
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;//ACLK=32768;SMCLK=MCLK=DCO=1M
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // ����Ƶ
  CSCTL4 &= ~LFXTOFF;
  do//���LFXT�����־
  {
    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  
}
/*******************************************************************************
** ������:   Port_Init()
** ����  :   IO��ʼ������,�ϵ�֮��MCU�ĳ�ʼ״̬
** �������: ��
** ���ز���: ��
** Author:   л����
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
  //����1  ��������,����P4.4Ϊ�½����ж�
  P4DIR &= ~BIT4;//P4.4Ϊ����״̬
  P4IE |=  BIT4;                                   // P4.4 �ж�ʹ��
  P4IES |= BIT4;                                   // P4.4 �½����ж�
  P4REN |= BIT4;		                        // P4.4 ����ʹ��
  P4IFG &= ~BIT4;                                // P4.4 �жϱ�־λ���
  //����2  ��ذ���л�����,����P1.7Ϊ�½����ж�
  P1DIR &= ~BIT7;//P1.7Ϊ����״̬
  P1IE |=  BIT7;                                   // P1.7 �ж�ʹ��
  P1IES |= BIT7;                                   // P1.7  �½����ж�
  P1REN |= BIT7;		                        // P1.7  ����ʹ��
  P1IFG &= ~BIT7;                                // P1.7 �жϱ�־λ���
  //����3  �Լ찴��,����P1.6Ϊ�½����ж�
  P1DIR &= ~BIT6;//P1.6Ϊ����״̬
  P1IE |=  BIT6;                                   // P1.6 �ж�ʹ��
  P1IES |= BIT6;                                   // P1.6  �½����ж�
  P1REN |= BIT6;		                        // P1.6  ����ʹ��
  P1IFG &= ~BIT6;                                // P1.6�жϱ�־λ���
  //����ж�,����P2.7Ϊ�½����ж�
  P2DIR &= ~BIT7;//P2.7Ϊ����״̬
  P2IE |=  BIT7;                                   // P2.7�ж�ʹ��
  P2IES |= BIT7;                                   // P2.7  �½����ж�
  P2REN |= BIT7;		                        // P2.7  ����ʹ��
  P2IFG &= ~BIT7;                                // P2.7�жϱ�־λ���
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;
  
}
/*******************************************************************************
** ������:   Uart_Init()
** ����  :   ���ڳ�ʼ����UCA1��������9600
** �������: ��
** ���ز���: ��
** Author:   л����
*******************************************************************************/
void Uart_Init(void)
{
    P2SEL1 |= BIT5 | BIT6;                    // ����UCA1�������ţ�Tx---P2.5;Rx---P2.6
    P2SEL0 &= ~(BIT5 | BIT6);
    PM5CTL0 &= ~LOCKLPM5;
    UCA1CTL1 |= UCSWRST;     //Set UCSERST               
    UCA1CTL1 = UCSSEL__ACLK;               
    UCA1BR0 = 3;          //������9600           
    UCA1BR1 = 0x00;                          
    UCA1MCTLW = 0x5300;    
    UCA1CTL1 &= ~UCSWRST;     
}
/*******************************************************************************
** ������:   PWM_Init()
** ����  :   PWM��ʼ����P2.0����ʱ���������Ϊ��1mS;ռ�ձ�Ϊ75%��PWM
** �������: ��
** ���ز���: ��
** Author:   л����
*******************************************************************************/
void PWM_Init(void)
{
    // P2.0ΪPWM�������
  P2DIR |= BIT0 ;                     // P2.0output
  P2SEL0 |= BIT0;                    // P2.0options select

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;
  TB0CCR0 = 1000-1;                         // PWM Period
  TB0CCTL6 = OUTMOD_7;                      // CCR6 reset/set
  TB0CCR6 = 750;                            // CCR6 PWM ռ�ձ�75%
  TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR
}
/*******************************************************************************
** ������:   PWM()
** ����  :   PWM��������
** �������: PWM������period��ռ�ձ�duty
** ���ز���: ��
** Author:   л����
*******************************************************************************/
void PWM(long period,int duty)
{
  TB0CCR0 = (int)(1000000/period);
  TB0CCR6 = (int)((1000000/period)*(duty/100.0));
}
/*******************************************************************************
** ������:   Get_char()
** ����  :   ͨ�����ڻ�ȡһ���ֽڵ�����
** �������: ��
** ���ز���: ��
** Author:   л����
*******************************************************************************/
uint8_t Get_char(void)
{
    uint8_t data;
    while (!(UCA1IFG & UCRXIFG));
    data=UCA1RXBUF;
    return data;
}

/*******************************************************************************
** ������:   Send_char()
** ����  :   ���ô��ڷ���һ���ֽڵ�����
** �������: ��
** ���ز���: ��
** Author:   л����
*******************************************************************************/
void Send_char(uint8_t DATA)
{   
    UCA1TXBUF=DATA;
    while (!(UCA1IFG & UCTXIFG));//�ȴ����ͻ���Ϊ�գ�Ҳ����˵���ͻ����е������Ѿ����Ƴ���
    while (UCA1STATW & UCBUSY); //�ȴ���λ�Ĵ��������ݷ�����ϣ����UCBUSY = 1˵��USCIģ�����ڷ�������
}
/*******************************************************************************
** ������:   Send_Str()
** ����  :   ���ô��ڷ���һ���ַ�������
** �������: Ҫ���͵��ַ���
** ���ز���: ��
** Author:   л����
*******************************************************************************/
void Send_Str(const char *str)
{
    if(str != _NULL)
    {
        while(*str!='\0')
        {
            Send_char(*str++);
        }
    }
}
/*******************************************************************************
** ������:   IsOver_3V()
** ����  :   ���Ƚ�IO1-IO5��·����߽ڵ�ص�ѹ֮���ѹ���Ƿ񳬹�3V
** �������: ��
** ���ز���: ����3V����-1��û�г����򷵻�1
** Author:   л����
*******************************************************************************/
uint8_t IsOver_3V(void)
{
  float temp[5]={0};//����һ����ʱ�����������Ÿ�·����ߵ�ѹֵ
  for(char i=0;i<5;i++)
  {
    temp[i]=Adc_value[i][0];//��һ����ֵ�������Ƚ�
    for(char k=0;k<9;k++)//ȡ��ÿһ�����ߵ�ѹ
    {
      if(temp[i]<(Adc_value[i][k+1]-Adc_value[i][k]))
        temp[i] = (Adc_value[i][k+1]-Adc_value[i][k]);
    }
  }
  for(char i=0;i<4;i++)
  {
    for(char k=0;k<(5-i-1);k++)
    {
      if(fabs(temp[i]-temp[k]) >=3.0 )
        return -1;//���س���3v��Ϣ
    }
  }
  return 1;//û�г���3V
}
/*******************************************************************************
** ������:   Self_Test()
** ����  :   �������Լ캯��
** �������: ��
** ���ز���: �Լ�ͨ���򷵻�1��δͨ���򷵻�-1
** Author:   л����
*******************************************************************************/
uint8_t Self_Test(void)
{
  for(char i=0;i<5;i++)//��������IO1-IO5
  {
    switch(i)
    {
    case 0:
      On_IO1;//ʹ��IO1
      Off_IO2;//��ֹIO2
      Off_IO3;//��ֹIO3
      Off_IO4;//��ֹIO4
      Off_IO5;//��ֹIO5
      Samples_Adc(i);//�Ը�������ĵ�ѹ���в������ұ��浽ȫ������Adc_value[i][]��
      break;
    case 1:
      On_IO2;//ʹ��IO2
      Off_IO1;//��ֹIO1
      Off_IO3;//��ֹIO3
      Off_IO4;//��ֹIO4
      Off_IO5;//��ֹIO5
      Samples_Adc(i);//�Ը�������ĵ�ѹ���в������ұ��浽ȫ������Adc_value[i][]��
      break;
    case 2:
      On_IO3;//ʹ��IO3
      Off_IO2;//��ֹIO2
      Off_IO1;//��ֹIO1
      Off_IO4;//��ֹIO4
      Off_IO5;//��ֹIO5
      Samples_Adc(i);//�Ը�������ĵ�ѹ���в������ұ��浽ȫ������Adc_value[i][]��
      break;
    case 3:
      On_IO4;//ʹ��IO4
      Off_IO2;//��ֹIO2
      Off_IO3;//��ֹIO3
      Off_IO1;//��ֹIO1
      Off_IO5;//��ֹIO5
      Samples_Adc(i);//�Ը�������ĵ�ѹ���в������ұ��浽ȫ������Adc_value[i][]��
      break;
    case 4:
      On_IO5;//ʹ��IO5
      Off_IO2;//��ֹIO2
      Off_IO3;//��ֹIO3
      Off_IO4;//��ֹIO4
      Off_IO1;//��ֹIO1
      Samples_Adc(i);//�Ը�������ĵ�ѹ���в������ұ��浽ȫ������Adc_value[i][]��
      break;
    }
//    __delay_cycles(5000);//�Ƿ���Ҫ��ʱ�����Ե�ʱ���ʵ�����
  }
   if(IsOver_3V()>0)//���ѹ��û�г���3V
     return 1;//�Լ�ͨ��
   else 
     return -1;//�Լ�û��ͨ��
}
/*******************************************************************************
** ������:   Step_8()
** ����  :   ��������8���׶�
** �������: ��
** ���ز���: void
** Author:   л����
*******************************************************************************/
void Step_8(void)
{
  char state = 1;//8�����׶�
  while(state)//���state=0���������ѭ��
  {
    switch(state)
    {
    case 1:                                                     //���׶�1
      Off_IO5;//�ر�I05,�رյ�ذ��5
      On_IO1;//��I01,�����ذ��1
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(35>Get_ADC(13))//�������ܵ�ѹ����35V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO1;//�ر�I01,�رյ�ذ��1
      On_IO2;//��I02,�����ذ��2
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(35>Get_ADC(13))//�������ܵ�ѹ����35V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO2;//�ر�I02,�رյ�ذ��2
      On_IO3;//��I03,�����ذ��3
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(35>Get_ADC(13))//�������ܵ�ѹ����35V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO3;//�ر�I03,�رյ�ذ��3
      On_IO4;//��I04,�����ذ��4
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(35>Get_ADC(13))//�������ܵ�ѹ����35V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO4;//�ر�I04,�رյ�ذ��1
      On_IO5;//��I05,�����ذ��5
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(35>Get_ADC(13))//�������ܵ�ѹ����35V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      state = 2;//�л������ĵ�2�׶�
      break;
    case 2:                                                     //���׶�2
      Off_IO5;//�ر�I05,�رյ�ذ��5
      On_IO1;//��I01,�����ذ��1
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(36>Get_ADC(13))//�������ܵ�ѹ����35V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO1;//�ر�I01,�رյ�ذ��1
      On_IO2;//��I02,�����ذ��2
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(36>Get_ADC(13))//�������ܵ�ѹ����35V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO2;//�ر�I02,�رյ�ذ��2
      On_IO3;//��I03,�����ذ��3
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(36>Get_ADC(13))//�������ܵ�ѹ����35V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO3;//�ر�I03,�رյ�ذ��3
      On_IO4;//��I04,�����ذ��4
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(36>Get_ADC(13))//�������ܵ�ѹ����35V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO4;//�ر�I04,�رյ�ذ��1
      On_IO5;//��I05,�����ذ��5
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(36>Get_ADC(13))//�������ܵ�ѹ����35V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      state = 3;//�л������ĵ�3�׶�
      break;  
    case 3:                                                     //���׶�3
      Off_IO5;//�ر�I05,�رյ�ذ��5
      On_IO1;//��I01,�����ذ��1
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(37>Get_ADC(13))//�������ܵ�ѹ����37V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO1;//�ر�I01,�رյ�ذ��1
      On_IO2;//��I02,�����ذ��2
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(37>Get_ADC(13))//�������ܵ�ѹ����37V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO2;//�ر�I02,�رյ�ذ��2
      On_IO3;//��I03,�����ذ��3
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(37>Get_ADC(13))//�������ܵ�ѹ����37V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO3;//�ر�I03,�رյ�ذ��3
      On_IO4;//��I04,�����ذ��4
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(37>Get_ADC(13))//�������ܵ�ѹ����37V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO4;//�ر�I04,�رյ�ذ��1
      On_IO5;//��I05,�����ذ��5
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(37>Get_ADC(13))//�������ܵ�ѹ����37V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      state = 4;//�л������ĵ�4�׶�
      break;
    case 4:                                                     //���׶�4
      Off_IO5;//�ر�I05,�رյ�ذ��5
      On_IO1;//��I01,�����ذ��1
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(38>Get_ADC(13))//�������ܵ�ѹ����38V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO1;//�ر�I01,�رյ�ذ��1
      On_IO2;//��I02,�����ذ��2
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(38>Get_ADC(13))//�������ܵ�ѹ����38V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO2;//�ر�I02,�رյ�ذ��2
      On_IO3;//��I03,�����ذ��3
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(38>Get_ADC(13))//�������ܵ�ѹ����38V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO3;//�ر�I03,�رյ�ذ��3
      On_IO4;//��I04,�����ذ��4
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(38>Get_ADC(13))//�������ܵ�ѹ����38V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO4;//�ر�I04,�رյ�ذ��1
      On_IO5;//��I05,�����ذ��5
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(38>Get_ADC(13))//�������ܵ�ѹ����38V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      state = 5;//�л������ĵ�5�׶�
      break;  
    case 5:                                                     //���׶�5
      Off_IO5;//�ر�I05,�رյ�ذ��5
      On_IO1;//��I01,�����ذ��1
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(39>Get_ADC(13))//�������ܵ�ѹ����39V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO1;//�ر�I01,�رյ�ذ��1
      On_IO2;//��I02,�����ذ��2
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(39>Get_ADC(13))//�������ܵ�ѹ����39V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO2;//�ر�I02,�رյ�ذ��2
      On_IO3;//��I03,�����ذ��3
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(39>Get_ADC(13))//�������ܵ�ѹ����39V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO3;//�ر�I03,�رյ�ذ��3
      On_IO4;//��I04,�����ذ��4
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(39>Get_ADC(13))//�������ܵ�ѹ����39V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO4;//�ر�I04,�رյ�ذ��1
      On_IO5;//��I05,�����ذ��5
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(39>Get_ADC(13))//�������ܵ�ѹ����39V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      state = 6;//�л������ĵ�6�׶�
      break;
    case 6:                                                     //���׶�6
      Off_IO5;//�ر�I05,�رյ�ذ��5
      On_IO1;//��I01,�����ذ��1
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(40>Get_ADC(13))//�������ܵ�ѹ����40V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO1;//�ر�I01,�رյ�ذ��1
      On_IO2;//��I02,�����ذ��2
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(40>Get_ADC(13))//�������ܵ�ѹ����40V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO2;//�ر�I02,�رյ�ذ��2
      On_IO3;//��I03,�����ذ��3
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(40>Get_ADC(13))//�������ܵ�ѹ����40V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO3;//�ر�I03,�رյ�ذ��3
      On_IO4;//��I04,�����ذ��4
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(40>Get_ADC(13))//�������ܵ�ѹ����40V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO4;//�ر�I04,�رյ�ذ��1
      On_IO5;//��I05,�����ذ��5
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(40>Get_ADC(13))//�������ܵ�ѹ����40V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      state = 7;//�л������ĵ�7�׶�
      break;  
    case 7:                                                     //���׶�7
      Off_IO5;//�ر�I05,�رյ�ذ��5
      On_IO1;//��I01,�����ذ��1
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(41>Get_ADC(13))//�������ܵ�ѹ����41V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO1;//�ر�I01,�رյ�ذ��1
      On_IO2;//��I02,�����ذ��2
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(41>Get_ADC(13))//�������ܵ�ѹ����41V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO2;//�ر�I02,�رյ�ذ��2
      On_IO3;//��I03,�����ذ��3
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(41>Get_ADC(13))//�������ܵ�ѹ����41V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO3;//�ر�I03,�رյ�ذ��3
      On_IO4;//��I04,�����ذ��4
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(41>Get_ADC(13))//�������ܵ�ѹ����41V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO4;//�ر�I04,�رյ�ذ��1
      On_IO5;//��I05,�����ذ��5
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(41>Get_ADC(13))//�������ܵ�ѹ����41V
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      state = 8;//�л������ĵ�8�׶�
      break; 
    case 8://�ý׶β�ͬ����7���׶�                              //���׶�8 
      Off_IO5;//�ر�I05,�رյ�ذ��5
      On_IO1;//��I01,�����ذ��1
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(0.015>Get_ADC(14))//��������������С��100mA
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO1;//�ر�I01,�رյ�ذ��1
      On_IO2;//��I02,�����ذ��2
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(0.015>Get_ADC(14))//��������������С��100mA
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO2;//�ر�I02,�رյ�ذ��2
      On_IO3;//��I03,�����ذ��3
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(0.015>Get_ADC(14))//��������������С��100mA
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO3;//�ر�I03,�رյ�ذ��3
      On_IO4;//��I04,�����ذ��4
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(0.015>Get_ADC(14))//��������������С��100mA
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO4;//�ر�I04,�رյ�ذ��1
      On_IO5;//��I05,�����ذ��5
      PWM_Duty = 5;//��ռ�ձȵ�Ϊ5%
      while(0.015>Get_ADC(14))//��������������С��100mA
      {
        On_PWM;//ʹ��PWM
        PWM(2000000,(int)PWM_Duty);//200k Hz
        if(0.15>Get_ADC(14))//�����������ĵ�ѹ<0.15v
        {
          PWM_Duty +=5;
          if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
          PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%      
        }
        else //�����������ĵ�ѹ>0.15v
        {
          PWM_Duty -=5;
          PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
        }
      }
      Off_IO1;//�ر�I01,�رյ�ذ��1
      Off_IO2;//�ر�I02,�رյ�ذ��2
      Off_IO4;//�ر�I03,�رյ�ذ��3
      Off_IO4;//�ر�I04,�رյ�ذ��4
      Off_IO5;//�ر�I05,�رյ�ذ��5
      Off_PWM;//�ر�PWM���
      PWM(2000000,0);//PWMֱ������͵�ƽ��ֹͣ���
      state = 0;//�л������ĵ�0�׶�
      break;  
    default:
      state = 0;
      break;
    }
  }
}

/*******************************************************************************
** ������:   Charging()
** ����  :   ��纯��
** �������: ��
** ���ز���: void
** Author:   л����
*******************************************************************************/
void Charging(void)
{
  On_IO1;//��I01,�����ذ��1
  if(1.5>Get_ADC(13))//����ܵ�ص�ѹ����30V����ѹ����Ϊ1:20
  {
    Send_Str("The battery is under 30V\n");//����ܵ�ѹ����30V
    On_PWM;//ʹ��PWM
    PWM(2000000,PWM_Duty);//200k Hz
    if(0.015>Get_ADC(14))//�����������ĵ�ѹ<0.015v
    {
      PWM_Duty +=5;
      if(PWM_Duty>100)PWM_Duty=100;//��֤ռ�ձȲ�����100%
      PWM(2000000,PWM_Duty);//�ϵ�ռ�ձ�5%
    }
    else //�����������ĵ�ѹ>0.015v
    {
      PWM_Duty -=5;
      PWM(2000000,PWM_Duty);//�µ�ռ�ձ�5%
    }
  }
  else //����ܵ�ѹ����30V,�������
  {
    Send_Str("The battery is beyond 30V\n");//����ܵ�ѹ����30V
    Step_8();//����������
  }
}
//�����жϷ������,P1.6��P1.7
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  P1IE &=  ~(BIT6+BIT7);                                // BIT6+BIT7�ص��ж�
  if((P1IFG & BIT6) == BIT6)                            //S3����
  {
    P1IFG &= ~BIT6;                                     // P1.6IFG cleared
  }
  else if((P1IFG & BIT7) == BIT7)//S2����
  {
    P1IFG &= ~BIT7; // P1.7 IFG cleared
    S2_flag =1;
  }
  P1IE |=BIT6+BIT7;//���ж�  
}
//�����жϷ������,P2.7,����ʱ����һ���½��ع��������Ĺ�����Ϊ�͵�ƽ
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  P2IE &=  ~BIT7;                                // BIT7�ص��ж�
  if((P2IFG & BIT7) == BIT7)                            
  {
    P2IFG &= ~BIT7;                                     // P2.7IFG cleared
    __delay_cycles(20000);//��ʱ20mS
    if((P2IN & BIT7)!=BIT7)//���P2.7�ǵ͵�ƽ
    {
      Charg_flag = 1;//����־λ��λ
    }
  }
  P2IE |=BIT7;//���ж�  
}
//�����жϷ������,P4.4
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
  P4IE &=  ~BIT4;                                // BIT4�ص��ж�
  if((P4IFG & BIT4) == BIT4)                            //S1����
  {
    S1_flag = 1;
    P4IFG &= ~BIT4;                                     // P4.4IFG cleared
    LPM0_EXIT;                                        // ���ѵ͹���LPM0
  }
  P4IE |=BIT4;//���ж�  
}




























