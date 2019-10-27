#include <stdio.h>
#include <stdlib.h>

#include "partida/partida.h";

void mostrar_tablero(tTablero tablero);

int main() {
    tPartida partida;

    printf("Juego de Ta-Te-Ti\n\n");

    iniciar_juego(&partida);

    return 0;
}

void iniciar_juego(tPartida *partida) {
    int modo_juego = 0, modo_juego_partida, comienza;
    char nombre_jugador_1[100], nombre_jugador_2[100];

    while (modo_juego == 1 || modo_juego == 2) {
        printf("Modos de juego\n");
        printf("1 - Usuario vs. Usuario\n");
        printf("2 - Usuario vs. Agente IA\n");
        printf("Elija el modo de juego (1-2): ");

        scanf("%d", &modo_juego);
    }

    printf("Ingrese el nombre del jugador 1(max 100): ");
    scanf("%100s", &nombre_jugador_1);

    if (modo_juego == 1) {
        printf("Ingrese el nombre del jugador 2(max 100): ");
        scanf("%100s", &nombre_jugador_2);
    } else {
        printf("Ingrese el nombre del jugador ia(max 100): ");
        scanf("%100s", &nombre_jugador_2);
    }

    elegir_quien_comienza(modo_juego, &comienza);

    switch (modo_juego) {
    case 1:
        modo_juego_partida = PART_MODO_USUARIO_VS_USUARIO;
        break;
    case 2:
        modo_juego_partida = PART_MODO_USUARIO_VS_AGENTE_IA;
        break;
    default:
        break;
    }

    nueva_partida(partida, modo_juego_partida, comienza, nombre_jugador_1, nombre_jugador_2);
}

void elegir_quien_comienza(int modo_juego, int *quien_comienza) {
    int comienza;

    while (comienza >= 1 && comienza <= 3) {
        printf("Quien comienza?\n");

        printf("1) Jugador 1\n");
        if (modo_juego == 1) {
            printf("2) Jugador 2\n");
        }

        if (modo_juego == 2) {
            printf("2) Jugador IA\n");
        }
        
        printf("3) Jugador al azar\n");

        scanf("%d", &comienza);
    }

    switch (comienza) {
    case 1:
        *quien_comienza = PART_JUGADOR_1;
        break;
    case 2:
        *quien_comienza = PART_JUGADOR_2;
        break;
    case 3:
        *quien_comienza = PART_JUGADOR_RANDOM;
        break;
    default:
        break;
    }
}

void mostrar_tablero(tTablero tablero) {
    printf("-------------\n");

    for (int i = 0; i < 3; i++) {
        printf("|");
        for (int j = 0; j < 3; j++) {
            printf(" ");

            if (tablero->grilla[i][j] == PART_JUGADOR_1) {
                printf("X");
            }

            if (tablero->grilla[i][j] == PART_JUGADOR_2) {
                printf("O");
            }

            if (tablero->grilla[i][j] != PART_JUGADOR_1 && tablero->grilla[i][j] != PART_JUGADOR_2) {
                printf(" ");
            }

            printf(" |");
        }

        printf("\n");
    }

    printf("-------------\n");
}