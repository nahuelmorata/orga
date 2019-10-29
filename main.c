#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ia/ia.h"
#include "partida/partida.h"

void mostrar_tablero(tPartida partida);
void iniciar_juego(tPartida *partida);
void elegir_quien_comienza(int modo_juego, int *quien_comienza);
void chequear_estado(tPartida partida);
void jugar_partida_2jugadores(tPartida partida);
void jugar_partida_vs_ia(tPartida partida);
void actualizar_arbol(tBusquedaAdversaria busqueda_ia, int x, int y);
static void eliminar_tEstado(tElemento e);

int main() {
    srand(time(0));
    tPartida partida;
    int jugar_de_nuevo = 1;
    char leido;

    printf("Juego de Ta-Te-Ti\n");
    while (jugar_de_nuevo) {
        iniciar_juego(&partida);

        printf("\nQuiere jugar de nuevo? (S/N): ");
        scanf("%s", &leido);

        if (leido == 'N' || leido == 'n') {
            jugar_de_nuevo = 0;
        }
        if (leido == 'S' || leido == 's') { // De la nada se cambia jugar_de_nuevo a 0
            jugar_de_nuevo = 1;
        }
    }

    finalizar_partida(&partida);

    return 0;
}

void iniciar_juego(tPartida *partida) {
    int modo_juego = 0, modo_juego_partida = 0, comienza;
    char nombre_jugador_1[100], nombre_jugador_2[100];

    while (modo_juego < 1 || modo_juego > 2) {
        printf("\nModos de juego:\n");
        printf("1 - Usuario vs. Usuario\n");
        printf("2 - Usuario vs. Agente IA\n");
        printf("\nElija el modo de juego (1-2): ");

        scanf("%d", &modo_juego);
    }

    printf("\nIngrese el nombre del jugador 1(max 100) (X): ");
    scanf("%s", nombre_jugador_1);

    if (modo_juego == 1) {
        printf("Ingrese el nombre del jugador 2(max 100) (O): ");
        scanf("%s", nombre_jugador_2);
    } else {
        printf("Ingrese el nombre del jugador ia(max 100) (O): ");
        scanf("%s", nombre_jugador_2);
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

    switch (modo_juego_partida) {
        case PART_MODO_USUARIO_VS_USUARIO:
            jugar_partida_2jugadores(*partida);
            break;
        case PART_MODO_USUARIO_VS_AGENTE_IA:
            jugar_partida_vs_ia(*partida);
            break;
        default:
            break;
    }
}

void elegir_quien_comienza(int modo_juego, int *quien_comienza) {
    int comienza = 0;

    while (comienza < 1 || comienza > 3) {
        printf("\nQuien comienza?\n");

        printf("1) Jugador 1\n");
        if (modo_juego == 1) {
            printf("2) Jugador 2\n");
        }

        if (modo_juego == 2) {
            printf("2) Jugador IA\n");
        }

        printf("3) Jugador al azar\n");

        printf("Elija opcion(1-3): ");

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

void mostrar_tablero(tPartida partida) {
    printf("-------------\n");

    for (int i = 0; i < 3; i++) {
        printf("|");
        for (int j = 0; j < 3; j++) {
            printf(" ");

            if (partida->tablero->grilla[i][j] == PART_JUGADOR_1) {
                printf("X");
            }

            if (partida->tablero->grilla[i][j] == PART_JUGADOR_2) {
                printf("O");
            }

            if (partida->tablero->grilla[i][j] != PART_JUGADOR_1 && partida->tablero->grilla[i][j] != PART_JUGADOR_2) {
                printf(" ");
            }

            printf(" |");
        }

        printf("\n-------------\n");
    }

    printf("\n");
}

void jugar_partida_2jugadores(tPartida partida){
    int x, y, mov_ok;

    while(partida->estado == PART_EN_JUEGO){

        do{
            if(partida->turno_de == PART_JUGADOR_1)
                printf("Jugador 1 ingrese su jugada (x,y): ");
            else
                printf("Jugador 2 ingrese su jugada (x,y): ");

            scanf("%d,%d", &x, &y);

            mov_ok = nuevo_movimiento(partida, x, y);
        } while(mov_ok != PART_MOVIMIENTO_OK);


        mostrar_tablero(partida);

        chequear_estado(partida);

        if(partida->turno_de == PART_JUGADOR_1)
            partida->turno_de = PART_JUGADOR_2;
        else
            partida->turno_de = PART_JUGADOR_1;

    }

    if(partida->estado == PART_EMPATE)
        printf("\nPARTIDA TERMINO EN EMPATE\n");
    else if(partida->estado == PART_GANA_JUGADOR_1)
        printf("\nJUGADOR 1 GANO\n");
    else if(partida->estado == PART_GANA_JUGADOR_2)
        printf("\nJUGADOR 2 GANO\n");
}

void jugar_partida_vs_ia(tPartida partida) {
    int x, y, mov_ok;
    tBusquedaAdversaria busqueda_ia = NULL;

    while(partida->estado == PART_EN_JUEGO) {
        do{
            if(partida->turno_de == PART_JUGADOR_1) {
                printf("Jugador 1 ingrese su jugada (x,y): ");
                scanf("%d,%d", &x, &y);
            } else {
                if(busqueda_ia == NULL)
                    crear_busqueda_adversaria(&busqueda_ia, partida);
                proximo_movimiento(busqueda_ia, &x, &y);
            }

            mov_ok = nuevo_movimiento(partida, x, y);
        } while(mov_ok != PART_MOVIMIENTO_OK);

        if(partida->turno_de == PART_JUGADOR_1 && busqueda_ia != NULL){
            actualizar_arbol(busqueda_ia, x, y);
        }

        mostrar_tablero(partida);

        chequear_estado(partida);

        if(partida->turno_de == PART_JUGADOR_1)
            partida->turno_de = PART_JUGADOR_2;
        else
            partida->turno_de = PART_JUGADOR_1;
    }

    if(partida->estado == PART_EMPATE)
        printf("\nPARTIDA TERMINO EN EMPATE\n");
    else if(partida->estado == PART_GANA_JUGADOR_1)
        printf("\nJUGADOR 1 GANO\n");
    else if(partida->estado == PART_GANA_JUGADOR_2)
        printf("\nJUGADOR IA GANO\n");
    destruir_busqueda_adversaria(&busqueda_ia);
}


void actualizar_arbol(tBusquedaAdversaria busqueda_ia, int x, int y){
    tNodo raiz = busqueda_ia->arbol_busqueda->raiz, nodo_cursor = NULL;
    tLista sucesores = a_hijos(busqueda_ia->arbol_busqueda, raiz);
    tEstado estado_cursor;
    tPosicion cursor = l_primera(sucesores), fin = l_fin(sucesores);

    while(cursor != fin){
        nodo_cursor = l_recuperar(sucesores, cursor);
        estado_cursor = a_recuperar(busqueda_ia->arbol_busqueda, nodo_cursor);
        if(estado_cursor->grilla[y][x] == PART_JUGADOR_1 || estado_cursor->grilla[y][x] == PART_JUGADOR_2)
            break;
        cursor = l_siguiente(sucesores, cursor);
    }

    tArbol nuevo_arbol;
    a_sub_arbol(busqueda_ia->arbol_busqueda, nodo_cursor, &nuevo_arbol);
    a_destruir(&(busqueda_ia->arbol_busqueda), &eliminar_tEstado);
    busqueda_ia->arbol_busqueda = nuevo_arbol;
}

static void eliminar_tEstado(tElemento e){
    tEstado estado_borrar = (tEstado) e;
    free(estado_borrar);
}

void chequear_estado(tPartida partida){
    int ganador, grilla_llena=1;
    tTablero tablero = partida->tablero;

    if(
       (tablero->grilla[0][0] == tablero->grilla[0][1] && tablero->grilla[0][0] == tablero->grilla[0][2]) ||
       (tablero->grilla[0][0] == tablero->grilla[1][1] && tablero->grilla[0][0] == tablero->grilla[2][2]) ||
       (tablero->grilla[0][0] == tablero->grilla[1][0] && tablero->grilla[0][0] == tablero->grilla[2][0]) )
       ganador = tablero->grilla[0][0];

    else if
       (tablero->grilla[1][0] == tablero->grilla[1][1] && tablero->grilla[1][0] == tablero->grilla[1][2])
       ganador = tablero->grilla[1][0];

    else if(
       (tablero->grilla[2][0] == tablero->grilla[2][1] && tablero->grilla[2][0] == tablero->grilla[2][2]) ||
       (tablero->grilla[2][0] == tablero->grilla[1][1] && tablero->grilla[2][0] == tablero->grilla[0][2]) )
       ganador = tablero->grilla[2][0];

    else if
       (tablero->grilla[0][1] == tablero->grilla[1][1] && tablero->grilla[0][1] == tablero->grilla[2][1])
       ganador = tablero->grilla[0][1];

    else if
       (tablero->grilla[0][2] == tablero->grilla[1][2] && tablero->grilla[0][2] == tablero->grilla[2][2])
       ganador = tablero->grilla[0][2];

    else
        ganador=0;



    if(ganador == PART_JUGADOR_1)
        partida->estado = PART_GANA_JUGADOR_1;

    else if(ganador == PART_JUGADOR_2)
        partida->estado = PART_GANA_JUGADOR_2;

    else{
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (tablero->grilla[i][j] != PART_JUGADOR_1 && tablero->grilla[i][j] != PART_JUGADOR_2)
                    grilla_llena = 0;
            }
        }

        if(grilla_llena == 0)
            partida->estado = PART_EN_JUEGO;
        else
            partida->estado = PART_EMPATE;
    }


}
