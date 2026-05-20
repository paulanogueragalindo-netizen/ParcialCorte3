#include "Jugador.h"
#include "Objeto.h"

void jugadorIniciar(Jugador* j) {
    j->x = 3;
    j->y = 6;
    j->vida = 100;
    j->vidaMax = 100;
    j->habitacion = 0;
    j->inventario = OBJ_NINGUNO;
    j->equipoLatigo = 0;
}

int jugadorTieneLlave(Jugador* j) {
    if (j->inventario == OBJ_LLAVE) {
        return 1;
    }
    return 0;
}

void jugadorCurar(Jugador* j, int cant) {
    j->vida = j->vida + cant;
    if (j->vida > j->vidaMax) {
        j->vida = j->vidaMax;
    }
}
