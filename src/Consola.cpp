#include "Consola.h"
#include "Graficos.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>

static HANDLE consolaHandle() {
    return GetStdHandle(STD_OUTPUT_HANDLE);
}

static void ponerAtributo(WORD color) {
    SetConsoleTextAttribute(consolaHandle(), color);
}
#endif

void consolaColorReset() {
#ifdef _WIN32
    ponerAtributo(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
}

void consolaColorTitulo() {
#ifdef _WIN32
    ponerAtributo(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
    (void)0;
#endif
}

void consolaColorVida(int vida, int vidaMax) {
#ifdef _WIN32
    if (vida <= vidaMax / 4) {
        ponerAtributo(FOREGROUND_RED | FOREGROUND_INTENSITY);
    } else if (vida <= vidaMax / 2) {
        ponerAtributo(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    } else {
        ponerAtributo(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }
#else
    (void)vida;
    (void)vidaMax;
#endif
}

void consolaColorAlerta(const char* texto) {
#ifdef _WIN32
    ponerAtributo(FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::cout << texto;
    consolaColorReset();
#else
    std::cout << texto;
#endif
}

void consolaColorDerrota() {
#ifdef _WIN32
    ponerAtributo(FOREGROUND_RED | FOREGROUND_INTENSITY);
#endif
}

static int refrescoRapidoActivo = 0;

void consolaActivarRefrescoRapido() {
    refrescoRapidoActivo = 1;
}

void consolaLimpiarCompleto() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void consolaRefrescoRapido() {
#ifdef _WIN32
    COORD origen;
    if (refrescoRapidoActivo == 1) {
        origen.X = 0;
        origen.Y = 0;
        SetConsoleCursorPosition(consolaHandle(), origen);
        return;
    }
#endif
    consolaLimpiarCompleto();
}

void consolaImprimirCasilla(char c) {
#ifdef _WIN32
    WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    if (c == GRAF_HEROE) {
        color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    } else if (c == GRAF_GOBLIN) {
        color = FOREGROUND_RED | FOREGROUND_INTENSITY;
    } else if (c == GRAF_ESQUELETO) {
        color = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    } else if (c == GRAF_PARED) {
        color = FOREGROUND_INTENSITY;
    } else if (c == GRAF_SUELO) {
        color = FOREGROUND_GREEN | FOREGROUND_BLUE;
    } else if (c == GRAF_LLAVE) {
        color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    } else if (c == GRAF_POCION) {
        color = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    } else if (c == GRAF_ALTAR) {
        color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    } else if (c == GRAF_PUERTA_CERRADA) {
        color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    } else if (graficosEsPuerta(c) == 1) {
        color = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
    } else if (c == GRAF_LATIGO) {
        color = FOREGROUND_RED | FOREGROUND_GREEN;
    } else if (c == GRAF_RATA) {
        color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }

    ponerAtributo(color);
    std::cout << c;
    consolaColorReset();
#else
    if (c == GRAF_HEROE) {
        std::cout << '@';
    } else if (c == GRAF_GOBLIN) {
        std::cout << 'G';
    } else if (c == GRAF_ESQUELETO) {
        std::cout << 'S';
    } else if (c == GRAF_PARED) {
        std::cout << '#';
    } else if (c == GRAF_SUELO) {
        std::cout << '.';
    } else if (c == GRAF_LLAVE) {
        std::cout << 'K';
    } else if (c == GRAF_POCION) {
        std::cout << 'P';
    } else if (c == GRAF_ALTAR) {
        std::cout << 'A';
    } else {
        std::cout << c;
    }
#endif
}
