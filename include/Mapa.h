#ifndef MAPA_H
#define MAPA_H

#define MAP_ANCHO 20
#define MAP_ALTO 12
#define NUM_HABITACIONES 8

struct SalidaHab {
    int x;
    int y;
    int destino;
    int spawnX;
    int spawnY;
};

struct Habitacion {
    int id;
    char nombre[40];
    char tiles[MAP_ALTO][MAP_ANCHO + 1];
    SalidaHab salidas[4];
    int numSalidas;
};

void mapaCargarHabitaciones(Habitacion* habs);
int mapaEsPared(char tile);
int mapaEsPuerta(char tile);
int mapaTileBloquea(Habitacion* hab, int x, int y, int tieneLlave);
char mapaTileBase(Habitacion* hab, int x, int y);
int mapaProbarSalida(Habitacion* hab, int x, int y, int* dest, int* sx, int* sy);
int mapaEsAltar(char tile);

#endif
