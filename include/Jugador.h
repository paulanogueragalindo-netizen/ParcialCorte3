#ifndef JUGADOR_H
#define JUGADOR_H

struct Jugador {
    int x;
    int y;
    int vida;
    int vidaMax;
    int habitacion;
    int inventario;
    int equipoLatigo;
};

void jugadorIniciar(Jugador* j);
int jugadorTieneLlave(Jugador* j);
void jugadorCurar(Jugador* j, int cant);

#endif
