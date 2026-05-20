#ifndef OBJETO_H
#define OBJETO_H

#define MAX_OBJETOS 8

#define OBJ_NINGUNO 0
#define OBJ_LLAVE 1
#define OBJ_POCION 2
#define OBJ_LATIGO 3

struct Objeto {
    int activo;
    int tipo;
    int habitacion;
    int x;
    int y;
};

void objetosInicializar(Objeto* lista);
Objeto* objetoEnSuelo(Objeto* lista, int hab, int x, int y);
Objeto* objetoSlotLibre(Objeto* lista);
void objetoCrear(Objeto* obj, int tipo, int hab, int px, int py);
char objetoNombre(int tipo);

#endif
