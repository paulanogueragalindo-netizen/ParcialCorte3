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

#else
#define ANSI_RESET      "\033[0m"
#define ANSI_BOLD       "\033[1m"
#define ANSI_RED        "\033[31m"
#define ANSI_GREEN      "\033[32m"
#define ANSI_YELLOW     "\033[33m"
#define ANSI_BLUE       "\033[34m"
#define ANSI_CYAN       "\033[36m"
#define ANSI_WHITE      "\033[37m"
#define ANSI_BRED       "\033[1;31m"
#define ANSI_BGREEN     "\033[1;32m"
#define ANSI_BYELLOW    "\033[1;33m"
#define ANSI_BCYAN      "\033[1;36m"
#endif

void consolaColorReset() {
#ifdef _WIN32
    ponerAtributo(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    std::cout << ANSI_RESET;
#endif
}

void consolaColorTitulo() {
#ifdef _WIN32
    ponerAtributo(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
    std::cout << ANSI_BYELLOW;
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
    if (vida <= vidaMax / 4) {
        std::cout << ANSI_BRED;
    } else if (vida <= vidaMax / 2) {
        std::cout << ANSI_BYELLOW;
    } else {
        std::cout << ANSI_BGREEN;
    }
#endif
}

void consolaColorAlerta(const char* texto) {
#ifdef _WIN32
    ponerAtributo(FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::cout << texto;
    consolaColorReset();
#else
    std::cout << ANSI_BRED << texto << ANSI_RESET;
#endif
}

void consolaColorDerrota() {
#ifdef _WIN32
    ponerAtributo(FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
    std::cout << ANSI_BRED;
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
#else
    if (refrescoRapidoActivo == 1) {
        std::cout << "\033[H" << std::flush;
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
    const char* color = ANSI_RESET;
    char imprime = c;

    if (c == GRAF_HEROE) {
        color = ANSI_BGREEN; imprime = '@';
    } else if (c == GRAF_GOBLIN) {
        color = ANSI_BRED;   imprime = 'G';
    } else if (c == GRAF_ESQUELETO) {
        color = "\033[1;34m"; imprime = 'S';
    } else if (c == GRAF_PARED) {
        color = "\033[90m";  imprime = '#';
    } else if (c == GRAF_SUELO) {
        color = "\033[36m";  imprime = '.';
    } else if (c == GRAF_LLAVE) {
        color = ANSI_BYELLOW; imprime = 'K';
    } else if (c == GRAF_POCION) {
        color = ANSI_BCYAN;  imprime = 'P';
    } else if (c == GRAF_ALTAR) {
        color = ANSI_BYELLOW; imprime = 'H';
    } else if (c == GRAF_PUERTA_CERRADA) {
        color = ANSI_BYELLOW; imprime = '+';
    } else if (graficosEsPuerta(c) == 1) {
        color = ANSI_BGREEN;
        // imprime el propio caracter (>, <, ^, v) que ya es ASCII
    } else if (c == GRAF_LATIGO) {
        color = ANSI_YELLOW; imprime = '-';
    } else if (c == GRAF_RATA) {
        color = ANSI_WHITE;  imprime = 'r';
    }

    std::cout << color << imprime << ANSI_RESET;
#endif
}