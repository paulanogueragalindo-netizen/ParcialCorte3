#ifndef JUEGO_H
#define JUEGO_H

#include "Mapa.h"
#include "Jugador.h"
#include "Enemigo.h"
#include "Objeto.h"

class Juego {
public:
    Juego();
    void ejecutar();

private:
    Habitacion habitaciones[NUM_HABITACIONES];
    Habitacion* habitacionActual;
    Jugador jugador;
    Enemigo enemigos[MAX_ENEMIGOS];
    Objeto objetos[MAX_OBJETOS];

    int estado;
    int enemigoCerca;

    void pantallaTitulo();
    void pantallaControles();
    void limpiarPantalla();
    void dibujar();
    void panelEstado();
    void leyenda();
    void lineaControles();
    void bucleJuego();
    void procesarTecla(char c);
    int intentarMover(int dx, int dy);
    void intentarAtaque();
    void recogerObjeto();
    void recogerAutomatico();
    void usarPocion();
    void soltarObjeto();
    int atacarCelda(int tx, int ty, int dano);
    void cambiarHabitacion(int dest, int nx, int ny);
    int revisarVictoria();
    int revisarDerrota();
};

#endif
