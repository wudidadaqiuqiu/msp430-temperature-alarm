#include <msp430.h> 
#include <stdio.h>
#include "adc.h"
#include "key.h"
#include "buzzer.h"
// #include "Include.h"
// #include "../Paper_Display.h"
#include "Paper_Display.h"

// #include "lib/epaper/src/EPD_2in9.h"
// #include "EPD_2in9.h"
#define TimesNewRoman 0
#define Arial 1
#define ArialBlack 2
#define size8 0
#define size16 1

#define MODE_NUMBER 3
volatile unsigned char DisBuffer[250*16];

char number[10];
char number2[10];
uint32_t tem;
uint32_t REF;
uint8_t enable_change_mode = 0;
uint8_t modes = 0;

int calc_mode(int temm, int ref);
// float calc_fre(int tem);
float calc_fre(int tem, float ref);

float adc2tem(unsigned int t);
void float2str(float a, char* s);
void initClock()
{
	 UCSCTL6 &= ~XT1OFF; 
	 P5SEL |= BIT2 + BIT3; 
	 UCSCTL6 &= ~XT2OFF;        
	 __bis_SR_register(SCG0);
	 UCSCTL0 = DCO0+DCO1+DCO2+DCO3+DCO4;
	 UCSCTL1 = DCORSEL_4;       
	 UCSCTL2 = FLLD_5 + 1;       
	 UCSCTL3 = SELREF_5 + FLLREFDIV_3;    
	 UCSCTL4 = SELA_4 + SELS_3 +SELM_3;   
	 UCSCTL5 = DIVA_5 +DIVS_1;      
	             //MCLK:16MHZ,SMCLK:8MHZ,ACLK:32KHZ

//	 __bic_SR_register(SCG0);                   //Enable the FLL control loop

}

#define DUTY TA2CCR1
#define LED_DUTY TB0CCR2
unsigned int Period=50000; // 50ms
int step = 200;
void timer_init()
{
    TA2CTL = MC_1 + TASSEL_2 + TACLR ;  // SMCLK(8M), up mode, clear TAR
    TA2CCR0 =Period;                    // PWM Period
    TA2CCTL1 = OUTMOD_7;                // CCR3 toggle/set
    TA2CCR1 = 0;                    // CCR3 PWM duty cycle  0
    TA2CCTL0 = CCIE;
    TB0CTL = MC_1 + TASSEL_2 + TACLR ;  // SMCLK(8M), up mode, clear TAR
    TB0CCR0 = Period;                    // PWM Period
    TB0CCTL2 = OUTMOD_7;                // CCR3 toggle/set
    TB0CCR2 = 0;                    // CCR3 PWM duty cycle  0

    TA0CTL = MC_1 + TASSEL_2 + TACLR ;  // SMCLK(8M), up mode, clear TAR
    TA0CCR0 = Period;                    // PWM Period
    TA0CCTL1 = OUTMOD_7;                // CCR3 toggle/set
    TA0CCR1 = 0;                    // CCR3 PWM duty cycle  0

    
}


void IO_Init(void)
{

    // ����
    // ����
    PULL_UP_INT(2,6);
    PULL_UP_INT(1,3);
    
    // 6.1 in
    PULL_DOWN(6, 3);

    // 7.4 L3 TB0.2
    PIN_OUT_SEL(7,4);
    
    // 1.4 buzzer -> 3.6
    PIN_OUT_SEL(1,2);

    //��������
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
    
    ADInit(ADC12INCH_2);
    initClock();

    timer_init();
    buzzer_init();

    PaperIO_Int();

    INIT_SSD1673();
    // paper_init();
    Init_buff();

    // display("tem:", 8, 16, TimesNewRoman,size8,1,0);
    // display("M O D E:", 8, 32, TimesNewRoman,size8,1,0);
    DIS_IMG(1);
    __enable_interrupt();
    // DIS_IMG(254);
//    display("                        ", 42, 0,TimesNewRoman,size8,1,0);
//   display(" Huatsing Instruments   ", 42, 16,TimesNewRoman,size8,1,3);
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


#define XX 50
#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer_A (void)
{
    static char str1[10];
    static char str2[10]; 
    static int M;
    static int ccnt;
    ccnt = (ccnt + 1) % 20;
    
    count_temperature(ccnt);
    // tem = get_temperature();
    if (ccnt == 19)
        tem = get_temperature();
    REF = get_ref();


    if (tem > REF) {
        PIN_HIGH(1,5);
        DUTY = Period / 3 - Period / 3 * (float)(tem - REF) / (float)(4096 - REF);

        
        // DUTY = Period / 3 - Period / 3 * (float)(tem - tem) / (float)(4096 - tem);
        if (enable_change_mode) {
            DUTY = Period / 3 * (MODE_NUMBER - (float) modes) / (float) MODE_NUMBER;

            M = modes + 1;
        } else {
            M = calc_mode(tem, REF);
        }
        buzzer_start();
    } else {
        M = 0;
        // DUTY = Period;
        PIN_LOW(1,5);
        buzzer_stop();
    }

    if (ccnt == 19) {
        // sprintf(number, "%5.2f", adc2tem(tem));
        // sprintf(number2, "%5.2f", adc2tem(REF));

        // sprintf(number, "%-4d", (tem));
        // sprintf(number2, "%-4d", (REF));
        float2str(adc2tem(tem), number);
        float2str(adc2tem(REF), number2);
        
        display(number2, XX, 64, TimesNewRoman, size8, 1, 0);
        display(number, XX, 16, TimesNewRoman, size8, 1, 0);
        
        sprintf(str1, "%-4d", enable_change_mode);
        display(str1, XX, 16 + 16, TimesNewRoman, size8, 1, 0);

        sprintf(str2, "%d", M);
        display(str2, XX, 48, TimesNewRoman, size8, 1, 0);
    }

    static int cnt;
    cnt = (cnt + 1) % 16;
    if (cnt == 0) {
        buzzer_update();
    }

    static int up = 0;

    static float r;
    r = calc_fre(tem, REF);
    if (r < 1) {
        LED_DUTY = 0;
        return;
    }

    if ((float)LED_DUTY - step * r <= 0) {
        up = 1;
    } else if ((float)LED_DUTY + step * r >= Period) {
        up = 0;
    }
    if (up == 0) {
        LED_DUTY -= step * r;
    } else {
        LED_DUTY += step * r;
    }


}


#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(IS_PIN_LOW(2,6)){
        INT_IN(2,6);
        enable_change_mode = !enable_change_mode;

    }

}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(IS_PIN_LOW(1,3)){
        INT_IN(1,3);
        modes = (modes + 1) % MODE_NUMBER;
    }

}

int calc_mode(int tem, int ref) {
    int step = (4095 - ref) / 3;
    if (tem <= ref) {
        return 0;
    }
    if (tem < ref + step) {
        return 1;
    }
    if (tem < ref + 2*step)
        return 2;
    return 3;
}

float calc_fre(int tem, float ref) {
    if (tem <= ref) return 0;
    return 4 * (tem - ref) / (4095 - ref) + 1;
    // if (tem <= 2000) return 0;
    // return 4 * (tem - 2000.0) / (4095 - 2000) + 1;
}

float adc2tem(unsigned int t) {
    return ((float)t - 2200.0) /100.0 + 24.0;
}

void float2str(float a, char* s) {
    char d = '.';
    uint16_t b = ((uint16_t)(a*100))  % 100;
    uint16_t c = ((uint16_t)(a*100)) / 100;
    sprintf(s, "%d%c%d", c, d, b);
}