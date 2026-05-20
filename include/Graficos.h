#ifndef GRAFICOS_H
#define GRAFICOS_H

#define GRAF_HEROE      ((char)254)
#define GRAF_PARED      ((char)219)
#define GRAF_SUELO      ((char)176)
#define GRAF_GOBLIN     ((char)177)
#define GRAF_ESQUELETO  ((char)223)
#define GRAF_LLAVE      'L'
#define GRAF_POCION     ((char)250)
#define GRAF_LATIGO     ((char)196)
#define GRAF_RATA       ((char)234)
#define GRAF_ALTAR      ((char)178)
#define GRAF_PUERTA_CERRADA '+'

#define GRAF_PUERTA_ESTE  '>'
#define GRAF_PUERTA_OESTE '<'
#define GRAF_PUERTA_NORTE '^'
#define GRAF_PUERTA_SUR   'v'

void graficosInicializar();
char graficosConvertir(char tile);
int graficosEsPuerta(char tile);

#endif
