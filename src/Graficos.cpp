#include "Graficos.h"

#ifdef _WIN32
#include <windows.h>
#endif

void graficosInicializar() {
#ifdef _WIN32
    SetConsoleOutputCP(437);
    SetConsoleCP(437);
#endif
}

char graficosConvertir(char tile) {
    if (tile == '#') {
        return GRAF_PARED;
    }
    if (tile == '.') {
        return GRAF_SUELO;
    }
    if (tile == 'A' || tile == 'H') {
        return GRAF_ALTAR;
    }
    return tile;
}

int graficosEsPuerta(char tile) {
    if (tile == GRAF_PUERTA_ESTE) {
        return 1;
    }
    if (tile == GRAF_PUERTA_OESTE) {
        return 1;
    }
    if (tile == GRAF_PUERTA_NORTE) {
        return 1;
    }
    if (tile == GRAF_PUERTA_SUR) {
        return 1;
    }
    return 0;
}
