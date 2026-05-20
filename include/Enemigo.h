#ifndef ENEMIGO_H
#define ENEMIGO_H

#define MAX_ENEMIGOS 12

#define ENEM_GOBlin 1
#define ENEM_ESQUELETO 2
#define ENEM_RATA 3

struct Enemigo {
    int activo;
    int tipo;
    int habitacion;
    int x;
    int y;
    int vida;
    int turnosRapido;
};

void enemigosInicializar(Enemigo* lista);
void enemigosCrearIniciales(Enemigo* lista);
Enemigo* enemigoEnPos(Enemigo* lista, int hab, int x, int y);
Enemigo* enemigoSlotLibre(Enemigo* lista);
void enemigoSpawn(Enemigo* e, int tipo, int hab, int px, int py);
void enemigosTurno(Enemigo* lista, int habJugador, int jx, int jy, char mapa[][21]);
int enemigoDanoAlJugador(Enemigo* e);
char enemigoSimbolo(int tipo);
const char* enemigoNombre(int tipo);

#endif
