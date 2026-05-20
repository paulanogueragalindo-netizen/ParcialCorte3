#include "Audio.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

static void beepTon(int freq, int ms) {
#ifdef _WIN32
    Beep(freq, ms);
#else
    std::cout << '\a';
    (void)freq;
    (void)ms;
#endif
}

void audioMovimiento() {
    beepTon(520, 40);
}

void audioAtaque() {
    beepTon(740, 90);
}

void audioEnemigo() {
    beepTon(280, 60);
}

void audioDano() {
    beepTon(180, 140);
}

void audioVictoria() {
    beepTon(600, 80);
    beepTon(800, 80);
    beepTon(1000, 120);
}

void audioDerrota() {
    beepTon(400, 100);
    beepTon(250, 200);
}

void audioRecoger() {
    beepTon(900, 50);
}
