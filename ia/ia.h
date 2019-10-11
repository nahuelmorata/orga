#ifndef IA_H_INCLUDED
#define IA_H_INCLUDED

#include "arbol.h"
#include "partida.h"

// Importante: IA_INFINITO_NEG < IA_PIERDE_MAX < IA_EMPATA_MAX < IA_GANA_MAX < IA_INFINITO_POS

#define IA_INFINITO_NEG     1000
#define IA_PIERDE_MAX       1001
#define IA_EMPATA_MAX       1002
#define IA_GANA_MAX         1003
#define IA_INFINITO_POS     1004
#define IA_NO_TERMINO       1005
#define IA_ERROR_MEMORIA    1006

struct busqueda_adversaria{
    tArbol arbol_busqueda;
    int jugador_max;
    int jugador_min;
};

struct estado{
    int grilla [3][3];
    int utilidad;
};

typedef struct busqueda_adversaria * tBusquedaAdversaria;
typedef struct estado * tEstado;

/**
 Inicializa la estructura correspondiente a una búsqueda adversaria, a partir del estado actual de la partida parametrizada.
 Se asume la partida parametrizada con estado PART_EN_JUEGO.
 Los datos del tablero de la partida parametrizada son clonados, por lo que P no se ve modificada.
 Una vez esto, se genera el árbol de búsqueda adversaria siguiendo el algoritmo Min-Max con podas Alpha-Beta.
**/
extern void crear_busqueda_adversaria(tBusquedaAdversaria * b, tPartida p);

/**
 Computa y retorna el próximo movimiento a realizar por el jugador MAX.
 Para esto, se tiene en cuenta el árbol creado por el algoritmo de búsqueda adversaria Min-max con podas Alpha-Beta.
 Siempre que sea posible, se indicará un movimiento que permita que MAX gane la partida.
 Si no existe un movimiento ganador para MAX, se indicará un movimiento que permita que MAX empate la partida.
 En caso contrario, se indicará un movimiento que lleva a MAX a perder la partida.
**/
extern void proximo_movimiento(tBusquedaAdversaria b, int * x, int * y);

/**
 Libera el espacio asociado a la estructura correspondiente para la búsqueda adversaria.
**/
extern void destruir_busqueda_adversaria(tBusquedaAdversaria * b);

#endif // IA_H_INCLUDED
