// #include <msp430.h>				

// void init();
// void LED_init();
// void LED_ON();
// void LED_OFF();
// void ClockInit();

// #define LED_SLEEP {1,5}

// /**
//  * blink.c
//  */
// int man()
// {
// 	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
// //	P3DIR |= BIT7;					// configure P1.0 as output
// //
// //	P1REN |= BIT3;
// //	P1OUT |= BIT3;
// //	volatile unsigned int i;		// volatile to prevent optimization
// //
// //	while(1)
// //	{
// ////		P1OUT ^= 0x01;				// toggle P1.0
// ////	    	for(i=10000; i>0; i--);     // delay
// //	    if (!(P1IN & BIT3)) {
// //	        P3OUT &= ~BIT7;
// //	    } else {
// //	        P3OUT |= BIT7;
// //	    }
// //	}
// 	LED_init();
// 	init();
// 	ClockInit();

// 	while(1){
// 	    static int a;
// 	    a++;
// //	    if (a < 10000) {
// //	        LED_OFF();
// //	    } else if (a <20000) {
// //	        LED_ON();
// //	    } else {
// //	        a = 0;
// //	    }
// 	    P2OUT ^= BIT4;
// 	    __delay_cycles(16e6);
// 	}
// }
// #define PIN_OUT(a, b) _PIN_OUT(a,b)
// #define _PIN_OUT(a, b) P##a##DIR |= BIT##b
// #define PIN_HIGH(a, b) P##a##OUT |= BIT##b
// #define PIN_LOW(a, b) P##a##OUT &= ~BIT##b
// //#define MYOUT(a) { static int v[2]= a; PIN_OUT(v[0], v[1]);}
// void init() {
//     P8DIR |= BIT1;
//     P3DIR |= BIT7;
//     P7DIR |= BIT4;
//     P6DIR |= BIT3;

//     PIN_OUT(8,1);

//     P1REN |= BIT2;
//     P1OUT |= BIT2;
//     P1IE |= BIT2;
//     P1IES |= BIT2;
//     P1IFG &= ~BIT2;


//     P1REN |= BIT3;
//     P1OUT |= BIT3;
//     P1IE |= BIT3;
//     P1IES |= BIT3;
//     P1IFG &= ~BIT3;

//     P2REN |= BIT3;
//     P2OUT |= BIT3;
//     P2IE |= BIT3;
//     P2IES |= BIT3;
//     P2IFG &= ~BIT3;

//     P2REN |= BIT6;
//     P2OUT |= BIT6;
//     P2IE |= BIT6;
//     P2IES |= BIT6;
//     P2IFG &= ~BIT6;

//     // _enable_interrupt();
//     __enable_interrupt();
// }

// #pragma vector=PORT1_VECTOR
// __interrupt void p1() {
//     if (P1IFG&BIT2){
//         P8OUT ^= BIT1;

//         P1IFG &= ~BIT2;
//     }

//     if (P1IFG&BIT3) {
//         P3OUT ^= BIT7;

//         P1IFG &= ~BIT3;
//     }
// }

// #pragma vector=PORT2_VECTOR

// __interrupt void p2() {
//     if (P2IFG&BIT3){
//         P7OUT ^= BIT4;

//         P2IFG &= ~BIT3;
//     }

//     if (P2IFG&BIT6) {
//         P6OUT ^= BIT3;

//         P2IFG &= ~BIT6;
//     }
// }

// void LED_init() {
//     PIN_OUT(1,5);

// //    MYOUT(LED_SLEEP);
//     PIN_OUT(2,5);
//     PIN_OUT(2,4);
// }

// void LED_ON() {
//     PIN_HIGH(1,5);
//     PIN_HIGH(2,5);
//     PIN_HIGH(2,4);
// }

// void LED_OFF() {
//     PIN_HIGH(1,5);
//     PIN_HIGH(2,5);
//     PIN_LOW(2,4);
// }

// void ClockInit()
// {//最终MCLK:16MHZ,SMCLK:8MHZ,ACLK:32KHZ
//     UCSCTL6 &= ~XT1OFF; //启动XT1
//     P5SEL |= BIT2 + BIT3; //XT2引脚功能选择
//     UCSCTL6 &= ~XT2OFF; //打开XT2
//     __bis_SR_register(SCG0);
//     UCSCTL0 = DCO0+DCO1+DCO2+DCO3+DCO4;
//     UCSCTL1 = DCORSEL_4; //DCO频率范围在28.2MHZ以下
//     UCSCTL2 = FLLD_5 + 1; //D=16，N=1
//     //n=8,FLLREFCLK时钟源为XT2CLK；
//     //DCOCLK=D*(N+1)*(FLLREFCLK/n);
//     //DCOCLKDIV=(N+1)*(FLLREFCLK/n);
//     UCSCTL3 = SELREF_5 + FLLREFDIV_3;
//     //ACLK的时钟源为DCOCLKDIV,
//     //MCLK\SMCLK的时钟源为DCOCLK
//     UCSCTL4 = SELA_4 + SELS_3 +SELM_3;
//     //ACLK由DCOCLKDIV的32分频得到，
//     //SMCLK由DCOCLK的2分频得到
//     UCSCTL5 = DIVA_5 +DIVS_1;
    
// }