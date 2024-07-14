// #include <msp430.h>
// #include "adc.h"	

// // //ADC 初始化
// // void ADInit(char channel)
// // {
// // 	ADC12CTL0 |= ADC12MSC;										//自动循环采样转换
// // 	ADC12CTL0 |= ADC12ON;										//启动ADC12模块
// // 	ADC12CTL1 |= ADC12CONSEQ1 ;									//选择单通道循环采样转换
// // 	ADC12CTL1 |= ADC12SHP;										//采样保持模式
// // 	switch (channel)
// // 	{
// //             case 0:ADC12MCTL0 |= ADC12INCH_0;    break;			//选择通道0，P6.0
// // 	        case 1:ADC12MCTL0 |= ADC12INCH_1; 	break;			//选择通道1，P6.1
// // 	        case 2:ADC12MCTL0 |= ADC12INCH_2; 	break;			//选择通道2，P6.2
// // 	        case 3:ADC12MCTL0 |= ADC12INCH_3; 	break;			//选择通道3，P6.3
// // 	        case 4:ADC12MCTL0 |= ADC12INCH_4; 	break; 			//选择通道4，P6.4
// // 	        case 5:ADC12MCTL0 |= ADC12INCH_5; 	break; 			//选择通道5，P6.5  连接拨码电位器
// // 	        case 6:ADC12MCTL0 |= ADC12INCH_6; 	break;			//选择通道6，P6.6
// // 	        case 12:ADC12MCTL0 |= ADC12INCH_12; break;	 		//选择通道12，P7.0
// // 	        default: ;break;
// // 	}

// // 	ADC12CTL0 |= ADC12ENC;
// // }

// // #define N_POINT 13
// // unsigned int  GetAD()
// // {
// // 	unsigned int temp = 0;			//设置变量
// // 	ADC12CTL0 |= ADC12SC;			//开始采样转换
// // 	temp= ADC12MEM0;				//把结果赋给变量
// // 	return temp;
// // }

// // unsigned int Filter( )
// // {
// //      char count,i,j;
// //      unsigned int value_buf[N_POINT];
// //      unsigned int temp=0;
// //      int sum=0;
// // 	  for (count=0;count<N_POINT;count++)
// // 	  {
// // 	    	value_buf[count] = GetAD();
// // 	  }
// // 	  for (j=0;j<N_POINT-1;j++)
// // 	  {
// // 	    	for (i=0;i<N_POINT-j-1;i++)
// // 		   {
// // 			    if ( value_buf[i]>value_buf[i+1] )
// // 			  {
// // 				   temp = value_buf[i];
// // 				   value_buf[i] = value_buf[i+1];
// // 				   value_buf[i+1] = temp;
// // 			   }
// // 		   }
// // 	}
// // 	for(count=1;count<N_POINT-1;count++)
// // 	sum += value_buf[count];
// // 	//如果为2的n次方等分，则可以用>>n的减少计算量  a=a*8;  可以为 a=a<<3;
// // 	//b=b/8; 可以为b=b>>3;
// // 	return (unsigned int)(sum/(N_POINT-2));
// // }


// // unsigned int get_temperature() {
// //     return GetAD();
// // }


// #define PIN_OUT(a, b) _PIN_OUT(a,b)
// #define _PIN_OUT(a, b) P##a##DIR |= BIT##b
// #define PIN_HIGH(a, b) P##a##OUT |= BIT##b
// unsigned int icnt;
// void IO_Init(void)
// {
//     P8DIR |= BIT1;                          // 设置P8.1口为输出模式  控制LED灯
//     P8OUT &= ~BIT1;                          // 选中P8.1为输出方式
//     P3DIR |= BIT7;                          // 设置P8.1口为输出模式  控制LED灯
//     P3OUT &= ~BIT7;                          // 选中P8.1为输出方式

//     PIN_OUT(7,4);
//     PIN_OUT(6,3);
//     PIN_OUT(6,4);
//     PIN_OUT(3,5);
    
//     // P3DIR |= BIT6;                          // 设置P3.6口为输出模式  控制蜂鸣器
//     // P3OUT |= BIT6;                          // 选中P3.6为输出方式

//     // P2DIR &= ~(BIT3+BIT6);
//     // P2REN |= BIT3+BIT6;
//     // P2OUT |= BIT3+BIT6;

//     // P1DIR &=~( BIT3+BIT2);
//     // P1REN = BIT3+BIT2;
//     // P1OUT |= BIT3+BIT2;
// }

// int apInterface_Key( void )
// {
//    return 0;
// }

// // /**
// //  * main.c
// //  */
// #define DIF (650 - 280)
// #define ONE (int)(280 + DIF/6)
// #define TWO (int)(280+DIF/3)
// #define THREE (int)(280+DIF/2)
// #define FOUR (int)(280+DIF/6*4)
// #define FIVE (int)(280+DIF/6*5)
// #define SIX (int)(280+DIF)

// static int arr[] = {0, ONE, TWO, THREE, FOUR, FIVE};
// int main(void)
// {
//     WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
//     IO_Init();

//     ADInit(ADC12INCH_2);

//     TA0CTL |= MC_1 + TASSEL_2 + TACLR;      //时钟为SMCLK,比较模式，开始时清零计数器
//     TA0CCTL0 = CCIE;                        //比较器中断使能
//     TA0CCR0  = 50000;                       //比较值设为50000，相当于50ms的时间间隔
//     __enable_interrupt();

//     unsigned int ivalue = 0;                                             //设置判断变量
//     while(1)
//     {
//         //  ivalue = GetAD();                                            //没有软件滤波
//         ivalue = Filter( );                                              //软件滤波
//         if(ivalue>=2500)
//         {
//             P8OUT |= BIT1;
//             P3OUT |= BIT7;
//             TA0CTL |= MC_1 + TASSEL_2 + TACLR;      //时钟为SMCLK,比较模式，开始时清零计数器
//             icnt=0;
//         }
//         // if (0 < ivalue && ivalue <= ONE) {

//         // } else if (ONE < ivalue && ivalue <= TWO) {

//         // } else if (TWO < ivalue && ivalue <= THREE) {
            
//         // } else if (THREE < ivalue && ivalue <= FOUR) {
            
//         // } else if (FOUR < ivalue && ivalue <= FIVE) {
            
//         // } else if (FIVE < ivalue && ivalue <= SIX) {
            
//         // } else {

//         // }
//         // if (ivalue > ONE) {
//         //     PIN_HIGH(8,1);
//         // }
//         // if (ivalue > TWO) {
//         //     PIN_HIGH(3,7);
//         // }
//         // if (ivalue > THREE) {
//         //     PIN_HIGH(7,4);
//         // }
//         // if (ivalue > FOUR) {
//         //     PIN_HIGH(6,3);
//         // }
//         // if (ivalue > FIVE) {
//         //     PIN_HIGH(6,4);
//         // }
//         // if (ivalue > SIX) {
//         //     PIN_HIGH(3,5);
//         // }
//         // __delay_cycles(10);
        
//     }

// }

// #pragma vector = TIMER0_A0_VECTOR
// __interrupt void Timer_A (void)
// {
//     icnt++;
//     if(icnt==200)
//     {//10s关灯
//        P8OUT &= ~BIT1;
//        P3OUT &= ~BIT7;
//        icnt=0;
//     }
// }