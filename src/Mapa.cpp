#include "Mapa.h"
#include "Graficos.h"
#include <cstring>

static void copiarFila(Habitacion* h, int fila, const char* linea) {
    int i;
    for (i = 0; i < MAP_ANCHO; i++) {
        h->tiles[fila][i] = linea[i];
    }
    h->tiles[fila][MAP_ANCHO] = '\0';
}

static void ponerTile(Habitacion* h, int x, int y, char c) {
    h->tiles[y][x] = c;
}

static void muroConPuerta(Habitacion* h, int x, int yInicio, int yFin, int yPuerta) {
    int y;
    for (y = yInicio; y <= yFin; y++) {
        if (y == yPuerta) {
            ponerTile(h, x, y, GRAF_PUERTA_CERRADA);
        } else {
            ponerTile(h, x, y, '#');
        }
    }
}

static void agregarSalida(Habitacion* h, int x, int y, int dest, int sx, int sy) {
    int n = h->numSalidas;
    h->salidas[n].x = x;
    h->salidas[n].y = y;
    h->salidas[n].destino = dest;
    h->salidas[n].spawnX = sx;
    h->salidas[n].spawnY = sy;
    h->numSalidas = n + 1;
}

static char simboloPuertaEn(int x, int y) {
    if (x >= MAP_ANCHO - 3) {
        return GRAF_PUERTA_ESTE;
    }
    if (x <= 2) {
        return GRAF_PUERTA_OESTE;
    }
    if (y <= 2) {
        return GRAF_PUERTA_NORTE;
    }
    if (y >= MAP_ALTO - 3) {
        return GRAF_PUERTA_SUR;
    }
    return GRAF_PUERTA_ESTE;
}

static void marcarPuertasHabitacion(Habitacion* hab) {
    int i;
    int px;
    int py;
    for (i = 0; i < hab->numSalidas; i++) {
        px = hab->salidas[i].x;
        py = hab->salidas[i].y;
        hab->tiles[py][px] = simboloPuertaEn(px, py);
    }
}

static void salaAbierta(Habitacion* h, int id, const char* nombre) {
    int f;
    h->id = id;
    std::strcpy(h->nombre, nombre);
    h->numSalidas = 0;
    for (f = 0; f < MAP_ALTO; f++) {
        if (f == 0 || f == MAP_ALTO - 1) {
            copiarFila(h, f, "####################");
        } else {
            copiarFila(h, f, "#..................#");
        }
    }
}

void mapaCargarHabitaciones(Habitacion* habs) {
    Habitacion* h;

    h = &habs[0];
    salaAbierta(h, 0, "1 - Entrada");
    agregarSalida(h, 18, 6, 1, 1, 6);

    h = &habs[1];
    salaAbierta(h, 1, "2 - Pasillo");
    agregarSalida(h, 1, 6, 0, 17, 6);
    agregarSalida(h, 18, 6, 3, 1, 6);
    agregarSalida(h, 10, 1, 2, 10, 10);

    h = &habs[2];
    salaAbierta(h, 2, "3 - Cripta (LLAVE)");
    agregarSalida(h, 10, 10, 1, 10, 2);

    h = &habs[3];
    salaAbierta(h, 3, "4 - Puerta sellada (latigo)");
    muroConPuerta(h, 10, 1, 10, 6);
    agregarSalida(h, 1, 6, 1, 17, 6);
    agregarSalida(h, 18, 6, 5, 1, 6);
    agregarSalida(h, 6, 10, 4, 6, 2);

    h = &habs[4];
    salaAbierta(h, 4, "5 - Camara de huesos");
    agregarSalida(h, 6, 1, 3, 6, 10);

    h = &habs[5];
    salaAbierta(h, 5, "6 - Vestibulo");
    agregarSalida(h, 1, 6, 3, 17, 6);
    agregarSalida(h, 18, 6, 7, 1, 6);
    agregarSalida(h, 10, 1, 6, 10, 10);

    h = &habs[6];
    salaAbierta(h, 6, "7 - Torre alta");
    agregarSalida(h, 10, 10, 5, 10, 2);

    h = &habs[7];
    salaAbierta(h, 7, "8 - Santuario (altar)");
    copiarFila(h, 4, "#......HHH.........#");
    copiarFila(h, 5, "#......H#H.........#");
    copiarFila(h, 6, "#......HHH.........#");
    agregarSalida(h, 1, 6, 5, 17, 6);

    {
        Habitacion* ptr;
        for (ptr = habs; ptr < habs + NUM_HABITACIONES; ptr++) {
            marcarPuertasHabitacion(ptr);
        }
    }
}

int mapaEsPared(char tile) {
    if (tile == '#') {
        return 1;
    }
    if (tile == GRAF_PARED) {
        return 1;
    }
    return 0;
}

int mapaEsPuerta(char tile) {
    return graficosEsPuerta(tile);
}

int mapaTileBloquea(Habitacion* hab, int x, int y, int tieneLlave) {
    char t;
    if (x < 0 || y < 0 || x >= MAP_ANCHO || y >= MAP_ALTO) {
        return 1;
    }
    t = hab->tiles[y][x];
    if (mapaEsPuerta(t) == 1) {
        return 0;
    }
    if (mapaEsPared(t) == 1) {
        return 1;
    }
    if (t == GRAF_PUERTA_CERRADA) {
        if (tieneLlave == 1) {
            return 0;
        }
        return 1;
    }
    return 0;
}

char mapaTileBase(Habitacion* hab, int x, int y) {
    if (x < 0 || y < 0 || x >= MAP_ANCHO || y >= MAP_ALTO) {
        return '#';
    }
    return hab->tiles[y][x];
}

int mapaEsAltar(char tile) {
    if (tile == 'A' || tile == 'H') {
        return 1;
    }
    return 0;
}

int mapaProbarSalida(Habitacion* hab, int x, int y, int* dest, int* sx, int* sy) {
    int i;
    for (i = 0; i < hab->numSalidas; i++) {
        if (hab->salidas[i].x == x && hab->salidas[i].y == y) {
            *dest = hab->salidas[i].destino;
            *sx = hab->salidas[i].spawnX;
            *sy = hab->salidas[i].spawnY;
            return 1;
        }
    }
    return 0;
}
