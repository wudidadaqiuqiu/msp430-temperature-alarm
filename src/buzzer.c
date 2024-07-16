#include "buzzer.h"
#define L_Do 3822
#define L_Re 3405
#define L_Mi 3033
#define L_Fa 2863
#define L_So 2551
#define L_La 2272
#define L_Xi 2024
#define M_Do 1911
#define M_Re 1702
#define M_Mi 1526
#define M_Fa 1431
#define M_So 1275
#define M_La 1136
#define M_Xi 1012
#define H_Do 955
#define H_Re 851
#define H_Mi 758
#define H_Fa 715
#define H_So 637
#define H_La 568
#define H_Xi 506



uint16_t music1[] = {M_Re, M_Mi, M_So, H_Do, 0, M_La, H_Do, H_Do};

uint16_t music2[] = {H_Do, 0, M_So, 0, M_Mi, 0, M_Xi,
                     M_Xi, M_Xi, M_Xi, M_La, 0, M_Xi, H_Do};

uint16_t music3[] = {L_So, 0, M_Do, 0, 0, M_Mi, M_Re, 0, M_Do, 0, 0, 0,
                     M_Mi, 0, M_So, 0, 0, M_La, M_Xi, 0, H_Do, H_Do, H_Do};

uint16_t music4[] = {M_Do, 0, M_Do, M_Mi, M_Mi, M_Mi, M_Mi, 0, M_Do,
                     0, M_Do, M_Mi, M_Mi, M_Mi, M_Mi, 0, M_La, 0,
                     M_La, 0, M_So, 0, M_La, 0, M_So, M_Do, 0,
                     M_Mi, M_Mi, 0, 0, H_Do, 0, M_La, M_La, M_So,
                     0, M_La, 0, 0, M_So, 0, M_Do, M_Re, M_Re,
                     0, 0, M_Xi, M_Xi, M_Xi, M_Xi, M_Xi, 0, M_Xi,
                     0, M_So, M_Mi, 0, M_So, M_So, M_So};

uint16_t music5[] = {H_Do, 0, M_Xi, 0, M_So, 0, M_So, 0, M_La,
                     1145, M_La, 0, M_So, 0, M_So, 0, M_La, 1145,
                     M_La, 0, M_So, 0, M_So, 0, M_La, 1145, M_La,
                     0, M_La, 0, M_La, M_La, M_Xi, M_Xi, H_Do, H_Do,
                     H_Re, H_Re, M_Xi, M_Xi, M_Xi, M_Xi, M_So, M_So, M_So,
                     M_So, H_Fa, H_Fa, H_Fa, H_Fa, H_Re, H_Re, H_Re, 0,
                     H_Re, H_Re, 0, H_Re, H_Re, H_Re, H_Mi, H_Mi};

uint16_t music6[] = {
    H_Do, H_Do, H_Do, 0, H_Do, 0, M_Xi, H_Do, H_Do, 0, H_Re, 0,
    H_Mi, 0, H_Fa, 0, H_Mi, H_Mi, 0, H_Mi, H_Mi, 0, M_Xi, 0,
    M_Xi, M_Xi, M_Xi, 0, 0, M_La, M_La, M_La, 0, M_La, 0, M_So,
    M_La, M_La, 0, H_Fa, 0, H_Mi, 0, H_Re, 0, H_Re, H_Re, 0,
    H_Do, H_Do, 0, H_Re, 0, H_Mi, H_Mi, H_Mi};

uint16_t music_nxt[] = {M_Do, M_Do, M_Do, M_Do, M_Do, M_Do, M_Do, M_Do,
                        M_La, M_La, M_La, M_La, M_La, M_La, M_La, M_Fa,
                        M_Fa, M_Fa, M_Fa, M_Fa, M_Do, M_Do, M_Do, M_Do,
                        M_Do, M_Do, M_Do, M_Do, H_Do, H_Do, H_Do, H_Do,
                        H_Do, H_Do, H_Do, H_Do, H_Do, H_Do, H_Do, H_Do};

uint16_t music_thankyou[] = {M_Do, 0, M_Re, 0, M_Mi, 0, M_So, 0, M_Mi, 0, M_Mi, M_Mi, 0, M_Re, 0, M_Re, M_Do, 0, M_Re, M_Re, 0, M_Do, 0, L_La, 0, M_Do, 0, M_Re, 0, M_Mi, M_Mi, M_Mi, M_Mi, 0,
                             M_Do, 0, L_La, 0, M_Do, M_Do, 0, L_So, 0, L_So, M_Re, 0, M_Do, M_Do, 0, M_Mi, 0, M_Re, 0, M_Re, 0, M_Do, 0, M_Re, M_Re, M_Re, M_Re};

uint16_t* musics[] = {music1, music2, music3, music4, music5, music6, music_nxt, music_thankyou};
uint16_t music_lens[] = {sizeof(music1) / 2, sizeof(music2) / 2, sizeof(music3) / 2, sizeof(music4) / 2,
                         sizeof(music5) / 2, sizeof(music6) / 2, sizeof(music_nxt) / 2, sizeof(music_thankyou) / 2};  //因为是uint16_t，实际元素数为字节数的两倍


typedef struct Buzzer {
    uint8_t finished;
    uint8_t started;
    uint16_t count;
    uint16_t* music;
    uint16_t len;
} Buzzer;

Buzzer buzzer;

void buzzer_init() {
    BUZZER_MODE = OUTMOD_7;
    BUZZER_DUTY = 0;

    buzzer.music = musics[MUSIC_INDEX];
    buzzer.len = music_lens[MUSIC_INDEX];
}

void buzzer_start() {
    buzzer.finished = 0;
    buzzer.started = 1;
    // buzzer.count = 0;
}

void buzzer_stop() {
    BUZZER_DUTY = 0;
    buzzer.finished = 1;
}

void buzzer_update() {
    if (!buzzer.started) return;
    if (buzzer.finished) {
        if (buzzer.finished == 1) {
            buzzer_stop();
            buzzer.finished++;
            buzzer.started = 0;  //开始标志位置0
            buzzer.count = 0;
        }
        return;
    }

    BUZZER_FRE = buzzer.music[buzzer.count] * 10;
    // BSP_PWM_SetARR(buzzer.config.bsp_pwm_index, buzzer.config.music[(int)buzzer.count]);  //通过改变arr即自动重装载值来改变频率实现蜂鸣器发出不同音高的声音
    // if (music_nxt[buzzer.count] != 0) {  //零即为休止符
    BUZZER_DUTY = BUZZER_FRE / 2;
        // BSP_PWM_SetCCR(buzzer.config.bsp_pwm_index, buzzer.config.music[buzzer.count] / 2);  //保持占空比为50%（占空比控制音量）
    // } else {
        // BSP_PWM_SetCCR(buzzer.config.bsp_pwm_index, buzzer.config.music[buzzer.count]);  //令ccr=arr，即占空比为0%或100%。蜂鸣器需要有电平变化才能发声，故能做到不发声
    // }
    buzzer.count++;  //放下一个音
    if (buzzer.count >= buzzer.len) {
        buzzer.finished = 1;  //放完歌就将停止标志位置为1，在下次更新本蜂鸣器时就不进行驱动
        buzzer.count = 0;
    }
}