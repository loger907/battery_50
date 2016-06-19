/*******************************************************************************
** 文件名:            battery_50.c
** 最后保存日期:     2016年6月5日
** 兼容芯片型号：    MSP430FR5949
** 修改内容:         
** 修改人:           谢信奇      
** Author:           xiexinqi
** 功能说明：
**程序大小为：
*******************************************************************************/
#include<msp430fr5949.H>
#include"battery_50.h"
/*******************************************************************************
** 函数名:   Init_Clk()
** 功能  :   配置时钟MCLK=SMCLK=1M，ACLK=32768
** 输入参数: 无
** 返回参数: 无
** Author:   谢信奇
*******************************************************************************/
void Init_Clk(void)
{
    // Clock System Setup
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz，
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;//ACLK=32768;SMCLK=MCLK=DCO=1M
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // 不分频
  CSCTL4 &= ~LFXTOFF;
  do//检测LFXT错误标志
  {
    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  
}
/*******************************************************************************
** 函数名:   Port_Init()
** 功能  :   IO初始化函数,上电之后MCU的初始状态
** 输入参数: 无
** 返回参数: 无
** Author:   谢信奇
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
  //按键1  开机按键,配置P4.4为下降沿中断
  P4DIR &= ~BIT4;//P4.4为输入状态
  P4IE |=  BIT4;                                   // P4.4 中断使能
  P4IES |= BIT4;                                   // P4.4 下降沿中断
  P4REN |= BIT4;		                        // P4.4 上拉使能
  P4IFG &= ~BIT4;                                // P4.4 中断标志位清除
  //按键2  电池板块切换按键,配置P1.7为下降沿中断
  P1DIR &= ~BIT7;//P1.7为输入状态
  P1IE |=  BIT7;                                   // P1.7 中断使能
  P1IES |= BIT7;                                   // P1.7  下降沿中断
  P1REN |= BIT7;		                        // P1.7  上拉使能
  P1IFG &= ~BIT7;                                // P1.7 中断标志位清除
  //按键3  自检按键,配置P1.6为下降沿中断
  P1DIR &= ~BIT6;//P1.6为输入状态
  P1IE |=  BIT6;                                   // P1.6 中断使能
  P1IES |= BIT6;                                   // P1.6  下降沿中断
  P1REN |= BIT6;		                        // P1.6  上拉使能
  P1IFG &= ~BIT6;                                // P1.6中断标志位清除
  //充电中断,配置P2.7为下降沿中断
  P2DIR &= ~BIT7;//P2.7为输入状态
  P2IE |=  BIT7;                                   // P2.7中断使能
  P2IES |= BIT7;                                   // P2.7  下降沿中断
  P2REN |= BIT7;		                        // P2.7  上拉使能
  P2IFG &= ~BIT7;                                // P2.7中断标志位清除
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;
  
}
/*******************************************************************************
** 函数名:   Uart_Init()
** 功能  :   串口初始化，UCA1，波特率9600
** 输入参数: 无
** 返回参数: 无
** Author:   谢信奇
*******************************************************************************/
void Uart_Init(void)
{
    P2SEL1 |= BIT5 | BIT6;                    // 配置UCA1功能引脚，Tx---P2.5;Rx---P2.6
    P2SEL0 &= ~(BIT5 | BIT6);
    PM5CTL0 &= ~LOCKLPM5;
    UCA1CTL1 |= UCSWRST;     //Set UCSERST               
    UCA1CTL1 = UCSSEL__ACLK;               
    UCA1BR0 = 3;          //波特率9600           
    UCA1BR1 = 0x00;                          
    UCA1MCTLW = 0x5300;    
    UCA1CTL1 &= ~UCSWRST;     
}
/*******************************************************************************
** 函数名:   PWM_Init()
** 功能  :   PWM初始化，P2.0，定时器输出周期为：1mS;占空比为75%的PWM
** 输入参数: 无
** 返回参数: 无
** Author:   谢信奇
*******************************************************************************/
void PWM_Init(void)
{
    // P2.0为PWM输出引脚
  P2DIR |= BIT0 ;                     // P2.0output
  P2SEL0 |= BIT0;                    // P2.0options select

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;
  TB0CCR0 = 1000-1;                         // PWM Period
  TB0CCTL6 = OUTMOD_7;                      // CCR6 reset/set
  TB0CCR6 = 750;                            // CCR6 PWM 占空比75%
  TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR
}
/*******************************************************************************
** 函数名:   Get_char()
** 功能  :   通过串口获取一个字节的数据
** 输入参数: 无
** 返回参数: 无
** Author:   谢信奇
*******************************************************************************/
uint8_t Get_char(void)
{
    uint8_t data;
    while (!(UCA1IFG & UCRXIFG));
    data=UCA1RXBUF;
    return data;
}

/*******************************************************************************
** 函数名:   Send_char()
** 功能  :   利用串口发送一个字节的数据
** 输入参数: 无
** 返回参数: 无
** Author:   谢信奇
*******************************************************************************/
void Send_char(uint8_t DATA)
{   
    UCA1TXBUF=DATA;
    while (!(UCA1IFG & UCTXIFG));//等待发送缓存为空（也就是说发送缓存中的数据已经被移出）
    while (UCA1STATW & UCBUSY); //等待移位寄存器的数据发送完毕，如果UCBUSY = 1说明USCI模块正在发送数据
}
/*******************************************************************************
** 函数名:   Self_Test()
** 功能  :   开机后自检函数
** 输入参数: 无
** 返回参数: 无
** Author:   谢信奇
*******************************************************************************/
void Self_Test(void)
{
  ;
}

//按键中断服务程序,P1.6、P1.7
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  P1IE &=  ~(BIT6+BIT7);                                // BIT6+BIT7关掉中断
  if((P1IFG & BIT6) == BIT6)                            //如果是自检按键中断
  {
    P1IFG &= ~BIT6;                                     // P1.6IFG cleared
  }
  else if((P1IFG & BIT7) == BIT7)//如果是电池切换按键中断
  {
    P1IFG &= ~BIT7; // P1.7 IFG cleared
  }
  P1IE |=BIT6+BIT7;//打开中断  
}
//按键中断服务程序,P2.7,充电的时候有一个下降沿过来，充电的过程中为低电平
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  P2IE &=  ~BIT7;                                // BIT7关掉中断
  if((P2IFG & BIT7) == BIT7)                            //如果是自检按键中断
  {
    P2IFG &= ~BIT7;                                     // P2.7IFG cleared
    __delay_cycles(20000);//延时20mS
    if((P2IN & BIT7)!=BIT7)//如果P2.7是低电平
    {
      Charg_flag = 1;//充电标志位置位
    }
  }
  P2IE |=BIT7;//打开中断  
}
//按键中断服务程序,P4.4
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
  P4IE &=  ~BIT4;                                // BIT4关掉中断
  if((P4IFG & BIT4) == BIT4)                            //如果是开机按键中断
  {
    P4IFG &= ~BIT4;                                     // P4.4IFG cleared
    LPM0_EXIT;                                        // 唤醒低功耗LPM0
  }
  P4IE |=BIT4;//打开中断  
}




























