
#define PULL_UP_INT(a, b) \
    P##a##DIR &= ~BIT##b;   \
    P##a##REN |= BIT##b;   \
    P##a##OUT |= BIT##b;   \
    P##a##IE |= BIT##b;   \
    P##a##IES |= BIT##b;   \
    P##a##IFG &= ~BIT##b;   

#define INT_IN(a, b) P##a##IFG &= ~BIT##b;   

#define PIN_HIGH(a, b) P##a##OUT |= BIT##b
#define PIN_LOW(a, b) P##a##OUT &= ~BIT##b

#define IS_PIN_HIGH(a, b) (P##a##IN & BIT##b)
#define IS_PIN_LOW(a, b) (!(P##a##IN & BIT##b))

#define PIN_OUT_SEL(a, b) P##a##DIR |= BIT##b; P##a##SEL |= BIT##b;

#define PULL_DOWN(a,b) \
    P##a##DIR &= ~BIT##b;   \
    P##a##REN |= BIT##b;   \
    P##a##OUT &= ~BIT##b;   