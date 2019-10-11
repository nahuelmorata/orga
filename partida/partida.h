#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#define PART_JUGADOR_1                      100
#define PART_JUGADOR_2                      101
#define PART_JUGADOR_RANDOM                 102

#define PART_MODO_USUARIO_VS_USUARIO        103
#define PART_MODO_USUARIO_VS_AGENTE_IA      104
#define PART_MODO_AGENTE_IA_VS_AGENTE_IA    105

#define PART_MOVIMIENTO_OK                  106
#define PART_MOVIMIENTO_ERROR               107
#define PART_SIN_MOVIMIENTO                 108
#define PART_GANA_JUGADOR_1                 109
#define PART_GANA_JUGADOR_2                 110
#define PART_EN_JUEGO                       111
#define PART_EMPATE                         112

#define PART_ERROR_MEMORIA                  113

struct tablero{
    int grilla [3][3];
};

struct partida{
    int modo_partida;
    int turno_de;
    int estado;
    struct tablero * tablero;
    char nombre_jugador_1 [50];
    char nombre_jugador_2 [50];
};

typedef struct tablero * tTablero;
typedef struct partida * tPartida;

/**
Inicializa una nueva partida, indicando:
 - Modo de partida (Usuario vs. Usuario o Usuario vs. Agente IA).
 - Jugador que comienza la partida (Jugador 1, Jugador 2, o elección al azar).
 - Nombre que representa al Jugador 1.
 - Nombre que representa al Jugador 2.
**/
extern void nueva_partida(tPartida * p, int modo_partida, int comienza, char * j1_nombre, char * j2_nombre);

/**
Actualiza, si corresponde, el estado de la partida considerando que el jugador al que le corresponde jugar, decide hacerlo en la posición indicada (X,Y).
En caso de que el movimiento a dicha posición sea posible, retorna PART_MOVIMIENTO_OK; en caso contrario, retorna PART_MOVIMIENTO_ERROR.
Las posiciones (X,Y) deben corresponderse al rango [0-2]; X representa el número de fila, mientras Y el número de columna.
**/
extern int nuevo_movimiento(tPartida p, int mov_x, int mov_y);

/**
Finaliza la partida referenciada por P, liberando toda la memoria utilizada.
**/
extern void finalizar_partida(tPartida * p);

#endif // PARTIDA_H_INCLUDED
