
#define PULL_UP_INT(a, b) \
    P##a##DIR &= ~BIT##b;   \
    P##a##REN |= BIT##b;   \
    P##a##OUT |= BIT##b;   \
    P##a##IE |= BIT##b;   \
    P##a##IES |= BIT##b;   \
    P##a##IFG &= ~BIT##b;   

#define INT_IN(a, b) P##a##IFG &= ~BIT##b;   