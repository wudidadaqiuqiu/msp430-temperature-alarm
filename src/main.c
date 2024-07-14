#include <msp430.h> 
#include <stdio.h>
#include "adc.h"
#include "key.h"
// #include "Include.h"
// #include "../Paper_Display.h"
#include "Paper_Display.h"

// #include "lib/epaper/src/EPD_2in9.h"
#include "EPD_2in9.h"
#define TimesNewRoman 0
#define Arial 1
#define ArialBlack 2
#define size8 0
#define size16 1

#define MODE_NUMBER 3
volatile unsigned char DisBuffer[250*16];

char number[10];
uint32_t tem;
uint8_t enable_change_mode = 0;
uint8_t modes = 0;


void initClock()
{
	 UCSCTL6 &= ~XT1OFF; //启动XT1
	 P5SEL |= BIT2 + BIT3; //XT2引脚功能选择
	 UCSCTL6 &= ~XT2OFF;          //打开XT2
	 __bis_SR_register(SCG0);
	 UCSCTL0 = DCO0+DCO1+DCO2+DCO3+DCO4;
	 UCSCTL1 = DCORSEL_4;       //DCO频率范围在28.2MHZ以下
	 UCSCTL2 = FLLD_5 + 1;       //D=16，N=1
	 UCSCTL3 = SELREF_5 + FLLREFDIV_3;    //n=8,FLLREFCLK时钟源为XT2CLK；DCOCLK=D*(N+1)*(FLLREFCLK/n);DCOCLKDIV=(N+1)*(FLLREFCLK/n);
	 UCSCTL4 = SELA_4 + SELS_3 +SELM_3;    //ACLK的时钟源为DCOCLKDIV,MCLK\SMCLK的时钟源为DCOCLK
	 UCSCTL5 = DIVA_5 +DIVS_1;      //ACLK由DCOCLKDIV的32分频得到，SMCLK由DCOCLK的2分频得到
	             //最终MCLK:16MHZ,SMCLK:8MHZ,ACLK:32KHZ

//	 __bic_SR_register(SCG0);                   //Enable the FLL control loop

}

#define DUTY TA2CCR1
unsigned int Period=50000; // 50ms
int step = 200;
void timer_init()
{
    TA2CTL = MC_1 + TASSEL_2 + TACLR ;  // SMCLK(8M), up mode, clear TAR
    TA2CCR0 =Period;                    // PWM Period
    TA2CCTL1 = OUTMOD_7;                // CCR3 toggle/set
    TA2CCR1 = 0;                    // CCR3 PWM duty cycle  0
    TA2CCTL0 = CCIE;                        //比较器中断使能

    
}

#define PIN_HIGH(a, b) P##a##OUT |= BIT##b
#define PIN_LOW(a, b) P##a##OUT &= ~BIT##b

#define IS_PIN_HIGH(a, b) (P##a##IN & BIT##b)
void IO_Init(void)
{

    // 按键
    // 上拉
    PULL_UP_INT(2,1);
    PULL_UP_INT(1,1);
    

    //风扇驱动
    P1DIR |= BIT5;
    P2DIR |= BIT4;
    P2DIR |= BIT5;

    P2SEL |= BIT4;

    P1OUT &= ~BIT5;
    P2OUT |= BIT5;



}
// void paper_init() {
//     EPD_2IN9_Init(EPD_2IN9_FULL);
//     EPD_2IN9_Clear();
//     // DEV_Delay_ms(500);
//     DELAY_mS(500);
// }
// int main1();
int main(void) {
    #if 1
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    
    IO_Init();
    // DUTY = Period;
    
    // P8DIR |= BIT1;
    // P8OUT &=~ BIT1;
    
    ADInit(ADC12INCH_5);
    initClock();

    timer_init();
    

    PaperIO_Int();

    INIT_SSD1673();
    // paper_init();
    Init_buff();

    display("tem:", 8, 16, TimesNewRoman,size8,1,0);
    display("M O D E:", 8, 32, TimesNewRoman,size8,1,0);
    DIS_IMG(1);
    __enable_interrupt();
    // DIS_IMG(254);
//    display("                        ", 42, 0,TimesNewRoman,size8,1,0);
//   display(" Huatsing Instruments   ", 42, 16,TimesNewRoman,size8,1,1);
//    display("012abcABC",80, 0,TimesNewRoman,size16,0,0);
//    display("012abcABC", 0,50,Arial,size8,1,0);
//    display("012abcABC",80,50,Arial,size16,1,0);
//    display("012abcABC", 0,90,ArialBlack,size8,1,0);
//    display("012abcABC",80,90,ArialBlack,size16,1,0);

//    DIS_IMG(1);
    // DIS_IMG(2);
//    DIS_IMG(3);
    
    while(1)
    {
        // display(" Huatsing Instruments   ", 42, 16,TimesNewRoman,size8,1,0);
        
        DIS_IMG(1);
        // EPD_2IN9_Display((uint8_t*)DisBuffer);
        // __delay_cycles(1e5);


        
    }
    #endif
    #if 0
    main1();
    #endif
}



#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer_A (void)
{
    static char str1[10];
    static char str2[10]; 
    tem = get_temperature();
    sprintf(number, "%-4d", tem);
    display(number, 100, 16, TimesNewRoman, size8, 1, 0);
    
    sprintf(str1, "%-4d", enable_change_mode);
    display(str1, 100, 16 + 16, TimesNewRoman, size8, 1, 0);


    // 
    // if (!IS_PIN_HIGH(2,1)) {
    //     enable_change_mode
    // }
    if (tem > 2000) {
        PIN_HIGH(1,5);
        DUTY = Period / 3 - Period / 3 * (float)(tem - 2000) / (float)(4095 - 2000);
        if (enable_change_mode) {
            DUTY = Period / 3 * (MODE_NUMBER - (float) modes) / (float) MODE_NUMBER;
        }
        // DUTY = Period / 2;
        // DUTY = Period;
    } else {
        // DUTY = Period;
        PIN_LOW(1,5);
    }
//       static int up = 0;
//       if (DUTY >= Period)
//       {
//           up = 0;
//       }
//
//       else if (DUTY == 0)
//       {
//           up = 1;
//       }
//
//
//       if ((P2IN & BIT1) == 0)
//       {
//           up = 1;
//       }
//       if ((P1IN & BIT1) == 0)
//       {
//           up = 0;
//       }
//       if (DUTY >= Period)
//       {
//           if (up != 0) DUTY = 0;
//       }
//       else if (DUTY == 0)
//       {
//           if (up != 1) DUTY = Period;
//       }
//
//
//       if (up == 0)
//       {
//           DUTY -= step;
//       }
//       else
//       {
//           DUTY += step;
//       }

}


#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(IS_PIN_HIGH(2,1)){
        INT_IN(2,1);
        enable_change_mode = !enable_change_mode;

    }

}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(IS_PIN_HIGH(1,1)){
        INT_IN(1,1);
        modes = (modes + 1) % MODE_NUMBER;
    }

}
