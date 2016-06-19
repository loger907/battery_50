/*******************************************************************************
** 文件名:            ADC12.c
** 最后保存日期:     2016年6月5日
** 兼容芯片型号：    MSP430FR5949
** 修改内容:         
** 修改人:           谢信奇      
** Author:           xiexinqi
** 功能说明：
**程序大小为：
*******************************************************************************/
#include "battery_50.h"
/*******************************************************************************
** 函数名:   ADC12_Init()
** 功能  :   ADC12模块初始化
** 输入参数: 无
** 返回参数: 无
** Author:   weirongluan
*******************************************************************************/
void ADC12_Init()
{
  // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
  REFCTL0 |= REFVSEL_3 | REFON;             // Select internal ref = 2.5V
                                            // Internal Reference ON

  // Configure ADC12
  ADC12CTL0 = ADC12SHT0_2 | ADC12ON;
  ADC12CTL1 = ADC12SHP;                     // ADCCLK = MODOSC; sampling timer
  ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
  ADC12IER0 |= ADC12IE0;                    // Enable ADC conv complete interrupt
  ADC12MCTL0 |= ADC12INCH_1 | ADC12VRSEL_1; // A1 ADC input select; Vref=1.2V

  while(!(REFCTL0 & REFGENRDY));            // Wait for reference generator
                                            // to settle
}
/******************************************************************************
** 函数名：Get_ADC()
** 功能 ： 获取某个通道的电压值
** 参数：  通道
** 返回值：浮点数的电压值
** Author:   weirongluan
*******************************************************************************/
float Get_ADC(char channel)
{
  ADC12CTL0 &= ~ADC12ENC;
  switch(channel)
  {
    case 0:
      ADC12MCTL1 = ADC12INCH_0;
      ADC12CTL0 |= ADC12ENC + ADC12SC;             // Sampling and conversion start   
      break;
    case 1: 
      ADC12MCTL1 = ADC12INCH_1;
      ADC12CTL0 |= ADC12ENC + ADC12SC;             // Sampling and conversion start   
      break;
    case 2:
      ADC12MCTL1 = ADC12INCH_2;
      ADC12CTL0 |= ADC12ENC + ADC12SC;             // Sampling and conversion start   
      break;
  }
   __bis_SR_register(LPM0_bits + GIE);                                      // CPU off, await USI interrupt，进入低功耗等待写入完成
  _NOP();                                     // Used for IAR 
  return (ADC12MEM0*2.5/4096);                 //处理转换数据
}
//ADC12的中断服务程序
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
  switch (__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
  {
    case ADC12IV_NONE:        break;        // Vector  0:  No interrupt
    case ADC12IV_ADC12OVIFG:  break;        // Vector  2:  ADC12MEMx Overflow
    case ADC12IV_ADC12TOVIFG: break;        // Vector  4:  Conversion time overflow
    case ADC12IV_ADC12HIIFG:  break;        // Vector  6:  ADC12BHI
    case ADC12IV_ADC12LOIFG:  break;        // Vector  8:  ADC12BLO
    case ADC12IV_ADC12INIFG:  break;        // Vector 10:  ADC12BIN
    case ADC12IV_ADC12IFG0:                 // Vector 12:  ADC12MEM0 Interrupt
      if (ADC12MEM0 >= 0x6B4)               // ADC12MEM = A1 > 0.5V?
        P1OUT |= BIT0;                      // P1.0 = 1
      else
        P1OUT &= ~BIT0;                     // P1.0 = 0
      DATA_ADC=(ADC12MEM0*2.5/4096);
      LPM0_EXIT; ; // Exit active CPU
      break;                                // Clear CPUOFF bit from 0(SR)

    case ADC12IV_ADC12IFG1:   break;        // Vector 14:  ADC12MEM1
    case ADC12IV_ADC12IFG2:   break;        // Vector 16:  ADC12MEM2
    case ADC12IV_ADC12IFG3:   break;        // Vector 18:  ADC12MEM3
    case ADC12IV_ADC12IFG4:   break;        // Vector 20:  ADC12MEM4
    case ADC12IV_ADC12IFG5:   break;        // Vector 22:  ADC12MEM5
    case ADC12IV_ADC12IFG6:   break;        // Vector 24:  ADC12MEM6
    case ADC12IV_ADC12IFG7:   break;        // Vector 26:  ADC12MEM7
    case ADC12IV_ADC12IFG8:   break;        // Vector 28:  ADC12MEM8
    case ADC12IV_ADC12IFG9:   break;        // Vector 30:  ADC12MEM9
    case ADC12IV_ADC12IFG10:  break;        // Vector 32:  ADC12MEM10
    case ADC12IV_ADC12IFG11:  break;        // Vector 34:  ADC12MEM11
    case ADC12IV_ADC12IFG12:  break;        // Vector 36:  ADC12MEM12
    case ADC12IV_ADC12IFG13:  break;        // Vector 38:  ADC12MEM13
    case ADC12IV_ADC12IFG14:  break;        // Vector 40:  ADC12MEM14
    case ADC12IV_ADC12IFG15:  break;        // Vector 42:  ADC12MEM15
    case ADC12IV_ADC12IFG16:  break;        // Vector 44:  ADC12MEM16
    case ADC12IV_ADC12IFG17:  break;        // Vector 46:  ADC12MEM17
    case ADC12IV_ADC12IFG18:  break;        // Vector 48:  ADC12MEM18
    case ADC12IV_ADC12IFG19:  break;        // Vector 50:  ADC12MEM19
    case ADC12IV_ADC12IFG20:  break;        // Vector 52:  ADC12MEM20
    case ADC12IV_ADC12IFG21:  break;        // Vector 54:  ADC12MEM21
    case ADC12IV_ADC12IFG22:  break;        // Vector 56:  ADC12MEM22
    case ADC12IV_ADC12IFG23:  break;        // Vector 58:  ADC12MEM23
    case ADC12IV_ADC12IFG24:  break;        // Vector 60:  ADC12MEM24
    case ADC12IV_ADC12IFG25:  break;        // Vector 62:  ADC12MEM25
    case ADC12IV_ADC12IFG26:  break;        // Vector 64:  ADC12MEM26
    case ADC12IV_ADC12IFG27:  break;        // Vector 66:  ADC12MEM27
    case ADC12IV_ADC12IFG28:  break;        // Vector 68:  ADC12MEM28
    case ADC12IV_ADC12IFG29:  break;        // Vector 70:  ADC12MEM29
    case ADC12IV_ADC12IFG30:  break;        // Vector 72:  ADC12MEM30
    case ADC12IV_ADC12IFG31:  break;        // Vector 74:  ADC12MEM31
    case ADC12IV_ADC12RDYIFG: break;        // Vector 76:  ADC12RDY
    default: break;
  }
}

















