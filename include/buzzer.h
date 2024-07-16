#pragma once


#include "stdint.h"
#include <msp430.h>


#define BUZZER_FRE TA0CCR0
#define BUZZER_MODE TA0CCTL1
#define BUZZER_DUTY TA0CCR1

#define MUSIC_INDEX 1


void buzzer_init();
void buzzer_start();
void buzzer_update();
void buzzer_stop();