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

void Juego::pantallaControles() {
    std::cout << "\nControles:\n";
    std::cout << "  WASD o flechas = mover\n";
    std::cout << "  J / Espacio    = atacar (2 casillas si tienes latigo)\n";
    std::cout << "  F = usar pocion del inventario (+30 vida)\n";
    std::cout << "  T = soltar objeto del inventario\n";
    std::cout << "  Recoges al pisar llave/pocion/latigo (pocion no se usa sola)\n";
    std::cout << "  Q = salir del juego\n\n";
    std::cout << "Objetivo: llave en sala 3 (^ desde 2), puerta + en 4, altar en 8.\n";
    std::cout << "Ruta: 1>2>4>6>8. Ramas: 3 arriba de 2, 5 abajo de 4, 7 arriba de 6.\n";
    std::cout << "Pulsa ENTER...\n";
    std::cin.get();
}

static char leerTecla() {
#ifdef _WIN32
    int c = _getch();
    if (c == 224 || c == 0) {
        c = _getch();
        if (c == 72) {
            return 'w';
        }
        if (c == 80) {
            return 's';
        }
        if (c == 75) {
            return 'a';
        }
        if (c == 77) {
            return 'd';
        }
        return 0;
    }
    return (char)c;
#else
    struct termios viejo;
    struct termios nuevo;
    char c = 0;
    char seq[2];

    if (tcgetattr(STDIN_FILENO, &viejo) != 0) {
        return 0;
    }
    nuevo = viejo;
    nuevo.c_lflag &= (tcflag_t)~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &nuevo);

    if (read(STDIN_FILENO, &c, 1) != 1) {
        tcsetattr(STDIN_FILENO, TCSANOW, &viejo);
        return 0;
    }
    if (c == 27) {
        if (read(STDIN_FILENO, &seq[0], 1) != 1) {
            tcsetattr(STDIN_FILENO, TCSANOW, &viejo);
            return 0;
        }
        if (read(STDIN_FILENO, &seq[1], 1) != 1) {
            tcsetattr(STDIN_FILENO, TCSANOW, &viejo);
            return 0;
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &viejo);
        if (seq[0] == '[') {
            if (seq[1] == 'A') {
                return 'w';
            }
            if (seq[1] == 'B') {
                return 's';
            }
            if (seq[1] == 'C') {
                return 'd';
            }
            if (seq[1] == 'D') {
                return 'a';
            }
        }
        return 0;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &viejo);
    return c;
#endif
}

void Juego::leyenda() {
    std::cout << "--- leyenda ---\n";
    std::cout << "[] heroe  L llave  + puerta (con llave)  H altar  <>^v salidas\n";
}

void Juego::panelEstado() {
    std::cout << "Vida: ";
    consolaColorVida(jugador.vida, jugador.vidaMax);
    std::cout << jugador.vida;
    consolaColorReset();
    std::cout << "/" << jugador.vidaMax;
    std::cout << "  |  Sala: " << habitacionActual->nombre << "\n";
    std::cout << "Inventario: ";
    if (jugador.inventario == OBJ_NINGUNO) {
        std::cout << "(vacio)";
    } else if (jugador.inventario == OBJ_LLAVE) {
        std::cout << "Llave";
    } else {
        std::cout << "Pocion";
    }
    if (jugador.equipoLatigo == 1) {
        std::cout << "  |  Latigo: SI (alcance 2)";
    } else {
        std::cout << "  |  Latigo: no";
    }
    if (enemigoCerca == 1) {
        std::cout << "  |  ";
        consolaColorAlerta("!Enemigo cerca!");
        std::cout << "\n";
    } else {
        std::cout << "\n";
    }
}

void Juego::lineaControles() {
    std::cout << "WASD mover | J atacar | F pocion | auto recoger | T soltar | Q salir\n";
}

void Juego::dibujar() {
    int x;
    int y;
    int i;
    char mapaVista[MAP_ALTO][MAP_ANCHO + 1];
    Enemigo* en;
    Objeto* ob;

    consolaRefrescoRapido();

    for (y = 0; y < MAP_ALTO; y++) {
        for (x = 0; x < MAP_ANCHO; x++) {
            mapaVista[y][x] = graficosConvertir(mapaTileBase(habitacionActual, x, y));
        }
        mapaVista[y][MAP_ANCHO] = '\0';
    }

    for (y = 0; y < MAP_ALTO; y++) {
        for (x = 0; x < MAP_ANCHO; x++) {
            ob = objetoEnSuelo(objetos, jugador.habitacion, x, y);
            if (ob != 0) {
                mapaVista[y][x] = objetoNombre(ob->tipo);
            }
            en = enemigoEnPos(enemigos, jugador.habitacion, x, y);
            if (en != 0) {
                mapaVista[y][x] = enemigoSimbolo(en->tipo);
            }
        }
    }

    if (jugador.habitacion == habitacionActual->id) {
        mapaVista[jugador.y][jugador.x] = GRAF_HEROE;
    }

    for (y = 0; y < MAP_ALTO; y++) {
        for (x = 0; x < MAP_ANCHO; x++) {
            consolaImprimirCasilla(mapaVista[y][x]);
        }
        std::cout << "\n";
    }
    consolaColorReset();

    std::cout << "\n";
    panelEstado();
    leyenda();
    lineaControles();
    for (i = 0; i < 4; i++) {
        std::cout << "\n";
    }
}

int Juego::revisarVictoria() {
    if (jugador.habitacion != 7) {
        return 0;
    }
    if (jugadorTieneLlave(&jugador) == 0) {
        return 0;
    }
    char t = mapaTileBase(habitacionActual, jugador.x, jugador.y);
    if (mapaEsAltar(t) == 1) {
        return 1;
    }
    return 0;
}

int Juego::revisarDerrota() {
    if (jugador.vida <= 0) {
        return 1;
    }
    return 0;
}

void Juego::cambiarHabitacion(int dest, int nx, int ny) {
    if (dest < 0 || dest >= NUM_HABITACIONES) {
        return;
    }
    jugador.habitacion = dest;
    jugador.x = nx;
    jugador.y = ny;
    habitacionActual = &habitaciones[dest];
}

int Juego::intentarMover(int dx, int dy) {
    int nx = jugador.x + dx;
    int ny = jugador.y + dy;
    int dest;
    int sx;
    int sy;
    Enemigo* en;

    if (mapaTileBloquea(habitacionActual, nx, ny, jugadorTieneLlave(&jugador))) {
        return 0;
    }

    en = enemigoEnPos(enemigos, jugador.habitacion, nx, ny);
    if (en != 0) {
        return 0;
    }

    jugador.x = nx;
    jugador.y = ny;
    audioMovimiento();

    if (mapaProbarSalida(habitacionActual, nx, ny, &dest, &sx, &sy) == 1) {
        cambiarHabitacion(dest, sx, sy);
    }

    recogerAutomatico();
    return 1;
}

int Juego::atacarCelda(int tx, int ty, int dano) {
    Enemigo* en;

    en = enemigoEnPos(enemigos, jugador.habitacion, tx, ty);
    if (en == 0) {
        return 0;
    }
    audioAtaque();
    en->vida = en->vida - dano;
    if (en->vida <= 0) {
        en->activo = 0;
    }
    return 1;
}

void Juego::intentarAtaque() {
    int dx;
    int dy;
    int i;
    int d;
    int alcance;
    int dano = 25;

    if (atacarCelda(jugador.x, jugador.y, dano) == 1) {
        return;
    }

    alcance = 1;
    if (jugador.equipoLatigo == 1) {
        alcance = 2;
    }

    for (d = 1; d <= alcance; d++) {
        for (i = 0; i < 4; i++) {
            dx = 0;
            dy = 0;
            if (i == 0) {
                dx = 1;
            }
            if (i == 1) {
                dx = -1;
            }
            if (i == 2) {
                dy = 1;
            }
            if (i == 3) {
                dy = -1;
            }

            if (atacarCelda(jugador.x + dx * d, jugador.y + dy * d, dano) == 1) {
                return;
            }
        }
    }
}

void Juego::recogerAutomatico() {
    Objeto* ob;

    ob = objetoEnSuelo(objetos, jugador.habitacion, jugador.x, jugador.y);
    if (ob == 0) {
        return;
    }

    if (ob->tipo == OBJ_LATIGO) {
        jugador.equipoLatigo = 1;
        ob->activo = 0;
        audioRecoger();
        return;
    }

    if (ob->tipo == OBJ_LLAVE) {
        jugador.inventario = OBJ_LLAVE;
        ob->activo = 0;
        audioRecoger();
        return;
    }

    if (jugador.inventario != OBJ_NINGUNO) {
        return;
    }

    jugador.inventario = ob->tipo;
    ob->activo = 0;
    audioRecoger();
}

void Juego::usarPocion() {
    if (jugador.inventario != OBJ_POCION) {
        return;
    }
    jugadorCurar(&jugador, 30);
    jugador.inventario = OBJ_NINGUNO;
    audioRecoger();
}

void Juego::recogerObjeto() {
    Objeto* ob;

    if (jugador.inventario != OBJ_NINGUNO) {
        return;
    }

    ob = objetoEnSuelo(objetos, jugador.habitacion, jugador.x, jugador.y);
    if (ob == 0) {
        return;
    }

    if (ob->tipo == OBJ_LATIGO) {
        jugador.equipoLatigo = 1;
    } else {
        jugador.inventario = ob->tipo;
    }
    ob->activo = 0;
    audioRecoger();
}

void Juego::soltarObjeto() {
    Objeto* ob;

    if (jugador.inventario == OBJ_NINGUNO) {
        return;
    }

    ob = objetoEnSuelo(objetos, jugador.habitacion, jugador.x, jugador.y);
    if (ob != 0) {
        return;
    }

    ob = objetoSlotLibre(objetos);
    if (ob == 0) {
        return;
    }

    objetoCrear(ob, jugador.inventario, jugador.habitacion, jugador.x, jugador.y);
    jugador.inventario = OBJ_NINGUNO;
}

void Juego::procesarTecla(char c) {
    if (c == 'q' || c == 'Q') {
        estado = EST_DERROTA;
        jugador.vida = 0;
        return;
    }
    if (c == 'w' || c == 'W') {
        intentarMover(0, -1);
    } else if (c == 's' || c == 'S') {
        intentarMover(0, 1);
    } else if (c == 'a' || c == 'A') {
        intentarMover(-1, 0);
    } else if (c == 'd' || c == 'D') {
        intentarMover(1, 0);
    } else if (c == 'j' || c == 'J' || c == ' ') {
        intentarAtaque();
    } else if (c == 'f' || c == 'F') {
        usarPocion();
    } else if (c == 'r' || c == 'R') {
        recogerObjeto();
    } else if (c == 't' || c == 'T') {
        soltarObjeto();
    }
}

void Juego::bucleJuego() {
    char tecla;
    int i;
    Enemigo* en;
    int dano;

    consolaActivarRefrescoRapido();
    consolaLimpiarCompleto();

    while (estado == EST_JUGANDO) {
        enemigoCerca = 0;
        for (i = 0; i < MAX_ENEMIGOS; i++) {
            if (enemigos[i].activo == 1 && enemigos[i].habitacion == jugador.habitacion) {
                if (enemigos[i].x - jugador.x <= 2 && enemigos[i].x - jugador.x >= -2) {
                    if (enemigos[i].y - jugador.y <= 2 && enemigos[i].y - jugador.y >= -2) {
                        enemigoCerca = 1;
                    }
                }
            }
        }

        dibujar();
        tecla = leerTecla();

        procesarTecla(tecla);

        if (revisarVictoria() == 1) {
            estado = EST_VICTORIA;
            break;
        }
        if (revisarDerrota() == 1) {
            break;
        }

        enemigosTurno(enemigos, jugador.habitacion, jugador.x, jugador.y, habitacionActual->tiles);
        audioEnemigo();

        for (i = 0; i < MAX_ENEMIGOS; i++) {
            int sepX;
            int sepY;
            if (enemigos[i].activo == 0) {
                continue;
            }
            if (enemigos[i].habitacion != jugador.habitacion) {
                continue;
            }
            sepX = enemigos[i].x - jugador.x;
            if (sepX < 0) {
                sepX = -sepX;
            }
            sepY = enemigos[i].y - jugador.y;
            if (sepY < 0) {
                sepY = -sepY;
            }
            if (sepX + sepY == 1) {
                en = &enemigos[i];
                dano = enemigoDanoAlJugador(en);
                jugador.vida = jugador.vida - dano;
                audioDano();
            }
        }

        if (jugador.vida <= 0) {
            estado = EST_DERROTA;
        }
    }
}

void Juego::ejecutar() {
    pantallaTitulo();
    pantallaControles();
    bucleJuego();

    limpiarPantalla();
    if (estado == EST_VICTORIA) {
        audioVictoria();
        consolaColorTitulo();
        std::cout << "========================================\n";
        std::cout << "         VICTORIA - LO LOGRASTE\n";
        std::cout << "   La llave abrio el altar sagrado\n";
        std::cout << "========================================\n";
        consolaColorReset();
    } else if (jugador.vida <= 0 && estado == EST_DERROTA) {
        audioDerrota();
        consolaColorDerrota();
        std::cout << "========================================\n";
        std::cout << "              GAME OVER\n";
        std::cout << "         Tu heroe ha caido...\n";
        std::cout << "========================================\n";
        consolaColorReset();
    } else {
        std::cout << "Partida terminada.\n";
    }
}
