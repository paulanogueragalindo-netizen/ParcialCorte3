#ifndef CONSOLA_H
#define CONSOLA_H

void consolaColorTitulo();
void consolaColorReset();
void consolaImprimirCasilla(char c);
void consolaColorVida(int vida, int vidaMax);
void consolaColorAlerta(const char* texto);
void consolaColorDerrota();
void consolaLimpiarCompleto();
void consolaRefrescoRapido();
void consolaActivarRefrescoRapido();

#endif
