#include "Juego.h"
#include "Audio.h"
#include "Consola.h"
#include "Graficos.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#define EST_JUGANDO 1
#define EST_VICTORIA 2
#define EST_DERROTA 3

Juego::Juego() {
    std::srand((unsigned int)std::time(0));
    graficosInicializar();
    mapaCargarHabitaciones(habitaciones);
    habitacionActual = &habitaciones[0];
    jugadorIniciar(&jugador);
    enemigosInicializar(enemigos);
    enemigosCrearIniciales(enemigos);
    objetosInicializar(objetos);

    Objeto* o = objetoSlotLibre(objetos);
    if (o != 0) {
        objetoCrear(o, OBJ_LLAVE, 2, 8, 5);
    }
    o = objetoSlotLibre(objetos);
    if (o != 0) {
        objetoCrear(o, OBJ_POCION, 4, 14, 6);
    }
    o = objetoSlotLibre(objetos);
    if (o != 0) {
        objetoCrear(o, OBJ_LATIGO, 3, 5, 4);
    }

    estado = EST_JUGANDO;
    enemigoCerca = 0;
}

void Juego::limpiarPantalla() {
    consolaLimpiarCompleto();
}

void Juego::pantallaTitulo() {
    limpiarPantalla();
    consolaColorTitulo();
    std::cout << "========================================\n";
    std::cout << "     DUNGEON CRAWLER - PARCIAL C++\n";
    std::cout << "========================================\n";
    consolaColorReset();
    std::cout << "\nINNOVACIONES:\n";
    std::cout << "  - Graficos geometricos (bloques, no solo letras)\n";
    std::cout << "  - Colores en consola y sonidos por accion\n";
    std::cout << "  - 3 enemigos: goblin, esqueleto, rata (mov. aleatorio)\n";
    std::cout << "  - Puertas visibles > < ^ v entre 8 habitaciones\n";
    std::cout << "  - Recogida automatica al pisar objetos\n";
    std::cout << "  - Latigo: ataque a 2 casillas de distancia\n";
    std::cout << "  - Pocion: se recoge sola, usala con F (+30 vida)\n";
    std::cout << "  - Panel de vida, inventario y alerta enemigo cerca\n";
    std::cout << "  - Refresco rapido sin cls (cursor en 0,0)\n";
    std::cout << "\nSimbolos: [] heroe  bloques enemigos  Omega=rata\n";
    std::cout << "  linea=latigo  rombo=llave  punto=pocion  <>^v puertas\n";
    std::cout << "========================================\n";
    std::cout << "Pulsa ENTER para empezar...\n";
    std::cin.get();
}
