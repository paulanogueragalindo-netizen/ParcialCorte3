#include "Enemigo.h"
#include "Mapa.h"
#include "Graficos.h"
#include <cstdlib>

static int tileLibre(char mapa[][21], int x, int y) {
    char t;
    if (x < 0 || y < 0 || x >= MAP_ANCHO || y >= MAP_ALTO) {
        return 0;
    }
    t = mapa[y][x];
    if (mapaEsPared(t) == 1) {
        return 0;
    }
    if (t == GRAF_PUERTA_CERRADA) {
        return 0;
    }
    return 1;
}

void enemigosInicializar(Enemigo* lista) {
    int i;
    for (i = 0; i < MAX_ENEMIGOS; i++) {
        lista[i].activo = 0;
        lista[i].tipo = 0;
        lista[i].habitacion = -1;
        lista[i].x = 0;
        lista[i].y = 0;
        lista[i].vida = 0;
        lista[i].turnosRapido = 0;
    }
}

void enemigoSpawn(Enemigo* e, int tipo, int hab, int px, int py) {
    e->activo = 1;
    e->tipo = tipo;
    e->habitacion = hab;
    e->x = px;
    e->y = py;
    e->turnosRapido = 0;
    if (tipo == ENEM_GOBlin) {
        e->vida = 20;
    } else if (tipo == ENEM_ESQUELETO) {
        e->vida = 35;
    } else {
        e->vida = 15;
    }
}

void enemigosCrearIniciales(Enemigo* lista) {
    Enemigo* e;

    e = enemigoSlotLibre(lista);
    if (e != 0) {
        enemigoSpawn(e, ENEM_GOBlin, 1, 12, 4);
    }
    e = enemigoSlotLibre(lista);
    if (e != 0) {
        enemigoSpawn(e, ENEM_RATA, 1, 16, 8);
    }
    e = enemigoSlotLibre(lista);
    if (e != 0) {
        enemigoSpawn(e, ENEM_GOBlin, 2, 4, 8);
    }
    e = enemigoSlotLibre(lista);
    if (e != 0) {
        enemigoSpawn(e, ENEM_ESQUELETO, 3, 14, 4);
    }
    e = enemigoSlotLibre(lista);
    if (e != 0) {
        enemigoSpawn(e, ENEM_ESQUELETO, 4, 10, 6);
    }
    e = enemigoSlotLibre(lista);
    if (e != 0) {
        enemigoSpawn(e, ENEM_RATA, 5, 14, 4);
    }
    e = enemigoSlotLibre(lista);
    if (e != 0) {
        enemigoSpawn(e, ENEM_GOBlin, 5, 6, 8);
    }
    e = enemigoSlotLibre(lista);
    if (e != 0) {
        enemigoSpawn(e, ENEM_GOBlin, 6, 10, 6);
    }
}

Enemigo* enemigoEnPos(Enemigo* lista, int hab, int x, int y) {
    int i;
    for (i = 0; i < MAX_ENEMIGOS; i++) {
        if (lista[i].activo == 1 && lista[i].habitacion == hab) {
            if (lista[i].x == x && lista[i].y == y) {
                return &lista[i];
            }
        }
    }
    return 0;
}

Enemigo* enemigoSlotLibre(Enemigo* lista) {
    int i;
    for (i = 0; i < MAX_ENEMIGOS; i++) {
        if (lista[i].activo == 0) {
            return &lista[i];
        }
    }
    return 0;
}

static void moverUnPaso(Enemigo* e, int jx, int jy, char mapa[][21]) {
    int dx = 0;
    int dy = 0;
    int nx;
    int ny;

    if (e->x < jx) {
        dx = 1;
    } else if (e->x > jx) {
        dx = -1;
    } else if (e->y < jy) {
        dy = 1;
    } else if (e->y > jy) {
        dy = -1;
    }

    nx = e->x + dx;
    ny = e->y + dy;

    if (nx == jx && ny == jy) {
        return;
    }

    if (tileLibre(mapa, nx, ny)) {
        e->x = nx;
        e->y = ny;
    } else if (dx != 0 && tileLibre(mapa, e->x + dx, e->y)) {
        e->x = e->x + dx;
    } else if (dy != 0 && tileLibre(mapa, e->x, e->y + dy)) {
        e->y = e->y + dy;
    }
}

static void moverRataAleatoria(Enemigo* e, int jx, int jy, char mapa[][21]) {
    int paso;
    int intento;
    int dir;
    int dx;
    int dy;
    int nx;
    int ny;

    for (paso = 0; paso < 2; paso++) {
        for (intento = 0; intento < 8; intento++) {
            dir = rand() % 4;
            dx = 0;
            dy = 0;
            if (dir == 0) {
                dx = 1;
            } else if (dir == 1) {
                dx = -1;
            } else if (dir == 2) {
                dy = 1;
            } else {
                dy = -1;
            }

            nx = e->x + dx;
            ny = e->y + dy;

            if (nx == jx && ny == jy) {
                continue;
            }
            if (tileLibre(mapa, nx, ny) == 1) {
                e->x = nx;
                e->y = ny;
                break;
            }
        }
    }
}

void enemigosTurno(Enemigo* lista, int habJugador, int jx, int jy, char mapa[][21]) {
    int i;
    int dist;

    for (i = 0; i < MAX_ENEMIGOS; i++) {
        if (lista[i].activo == 0) {
            continue;
        }
        if (lista[i].habitacion != habJugador) {
            continue;
        }

        if (lista[i].tipo == ENEM_RATA) {
            moverRataAleatoria(&lista[i], jx, jy, mapa);
            continue;
        }

        dist = lista[i].x - jx;
        if (dist < 0) {
            dist = -dist;
        }
        if (lista[i].y - jy < 0) {
            if (-(lista[i].y - jy) > dist) {
                dist = -(lista[i].y - jy);
            }
        } else {
            if (lista[i].y - jy > dist) {
                dist = lista[i].y - jy;
            }
        }
        if (dist > 8) {
            continue;
        }

        if (lista[i].x == jx && lista[i].y == jy) {
            continue;
        }

        moverUnPaso(&lista[i], jx, jy, mapa);
    }
}

int enemigoDanoAlJugador(Enemigo* e) {
    if (e->tipo == ENEM_ESQUELETO) {
        return 12;
    }
    if (e->tipo == ENEM_RATA) {
        return 5;
    }
    return 7;
}

char enemigoSimbolo(int tipo) {
    if (tipo == ENEM_GOBlin) {
        return GRAF_GOBLIN;
    }
    if (tipo == ENEM_RATA) {
        return GRAF_RATA;
    }
    return GRAF_ESQUELETO;
}

const char* enemigoNombre(int tipo) {
    if (tipo == ENEM_GOBlin) {
        return "Goblin";
    }
    if (tipo == ENEM_RATA) {
        return "Rata";
    }
    return "Esqueleto";
}