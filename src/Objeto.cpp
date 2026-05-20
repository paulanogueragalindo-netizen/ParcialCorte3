#include "Objeto.h"
#include "Graficos.h"

void objetosInicializar(Objeto* lista) {
    int i;
    for (i = 0; i < MAX_OBJETOS; i++) {
        lista[i].activo = 0;
        lista[i].tipo = OBJ_NINGUNO;
        lista[i].habitacion = -1;
        lista[i].x = 0;
        lista[i].y = 0;
    }
}

Objeto* objetoEnSuelo(Objeto* lista, int hab, int x, int y) {
    int i;
    for (i = 0; i < MAX_OBJETOS; i++) {
        if (lista[i].activo == 1 && lista[i].habitacion == hab) {
            if (lista[i].x == x && lista[i].y == y) {
                return &lista[i];
            }
        }
    }
    return 0;
}

Objeto* objetoSlotLibre(Objeto* lista) {
    int i;
    for (i = 0; i < MAX_OBJETOS; i++) {
        if (lista[i].activo == 0) {
            return &lista[i];
        }
    }
    return 0;
}

void objetoCrear(Objeto* obj, int tipo, int hab, int px, int py) {
    obj->activo = 1;
    obj->tipo = tipo;
    obj->habitacion = hab;
    obj->x = px;
    obj->y = py;
}

char objetoNombre(int tipo) {
    if (tipo == OBJ_LLAVE) {
        return GRAF_LLAVE;
    }
    if (tipo == OBJ_POCION) {
        return GRAF_POCION;
    }
    if (tipo == OBJ_LATIGO) {
        return GRAF_LATIGO;
    }
    return '?';
}
