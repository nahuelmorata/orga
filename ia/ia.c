#include <stdlib.h>
#include <stdio.h>

#include "lista.h"
#include "arbol.h"
#include "ia.h"

// Prototipos de funciones auxiliares.
static void ejecutar_min_max(tBusquedaAdversaria b);
static void crear_sucesores_min_max(tArbol a, tNodo n, int es_max, int alpha, int beta, int jugador_max, int jugador_min);
static int valor_utilidad(tEstado e, int jugador_max);
static tLista estados_sucesores(tEstado e, int ficha_jugador);
static void diferencia_estados(tEstado anterior, tEstado nuevo, int * x, int * y);
static tEstado clonar_estado(tEstado e);
static void eliminar_tEstado(tElemento e);
static void eliminar_vacio(tElemento e);

// -------------------

void crear_busqueda_adversaria(tBusquedaAdversaria * b, tPartida p){
    int i, j;
    tEstado estado;

    (*b) = (tBusquedaAdversaria) malloc(sizeof(struct busqueda_adversaria));
    if ((*b) == NULL) exit(IA_ERROR_MEMORIA);

    estado = (tEstado) malloc(sizeof(struct estado));
    if (estado == NULL) exit(IA_ERROR_MEMORIA);

    // Se clona el estado del tablero de la partida, al estado inicial de la búsqueda adversaria.
    for(i=0; i<3; i++){
        for(j=0; j<3; j++){
            estado->grilla[i][j] = p->tablero->grilla[i][j];
        }
    }

	// Se asume que el estado de la partida es PART_EN_JUEGO por lo que, la utilidad del estado
	// inicialmente es IA_NO_TERMINO
    estado->utilidad = IA_NO_TERMINO;

    // Inicializa los valores que representarón a los jugadores MAX y MIN respectivamente.
    (*b)->jugador_max = p->turno_de;
    (*b)->jugador_min = (p->turno_de == PART_JUGADOR_1) ? PART_JUGADOR_2 : PART_JUGADOR_1;

    // Inicializa un árbol para la búsqueda adversaria inicialmente vacío.
    crear_arbol(&((*b)->arbol_busqueda));

    // Inicializa la raíz del árbol de búsqueda con el estado del tablero T.
    crear_raiz((*b)->arbol_busqueda, estado);

    // Ejecuta algoritmo Min-Max con podas Alpha-Beta.
    ejecutar_min_max((*b));
}

/**
>>>>>  A IMPLEMENTAR   <<<<<
 Computa y retorna el próximo movimiento a realizar por el jugador MAX.
 Para esto, se tiene en cuenta el árbol creado por el algoritmo de búsqueda adversaria Min-max con podas Alpha-Beta.
 Siempre que sea posible, se indicará un movimiento que permita que MAX gane la partida.
 Si no existe un movimiento ganador para MAX, se indicará un movimiento que permita que MAX empate la partida.
 En caso contrario, se indicará un movimiento que lleva a MAX a perder la partida.
**/
void proximo_movimiento(tBusquedaAdversaria b, int * x, int * y){


        tNodo raiz = a_raiz(b->arbol_busqueda);
        tEstado estado_actual = (tEstado) a_recuperar(b->arbol_busqueda, raiz); //guarda el estado actual de la partida
        tLista sucesores = a_hijos(b->arbol_busqueda, raiz); //lista con las posibles siguientes jugadas
        tNodo mejor_sucesor = NULL; //guarda la mejor jugada encontrada hasta el momento
        int mejor_valor = IA_INFINITO_NEG; //guarda la utilidad de mejor_sucesor
        tPosicion cursor = l_primera(sucesores);
        tPosicion fin = l_fin(sucesores);
        int utilidad_cursor; //guarda la utilidad del estado en el cursor
        tNodo nodo_cursor; //guarda el nodo corrspondiente al cursor

        while(cursor != fin){
            nodo_cursor = (tNodo) l_recuperar(sucesores, cursor);
            utilidad_cursor = ((tEstado) a_recuperar(b->arbol_busqueda, nodo_cursor))->utilidad;
            if(mejor_valor < utilidad_cursor){ //si encunentra un estado mejor a la actual mejor jugada, la reemplaza
                mejor_sucesor = nodo_cursor;
                mejor_valor = utilidad_cursor;
            }
            cursor = l_siguiente(sucesores, cursor);
        }

        //calcula la casilla correspondiente a la mejor jugada encontrada
        diferencia_estados(estado_actual, (tEstado) a_recuperar(b->arbol_busqueda, mejor_sucesor), x, y);
}

/**
>>>>>  A IMPLEMENTAR   <<<<<
**/
void destruir_busqueda_adversaria(tBusquedaAdversaria * b) {
    a_destruir(&((*b)->arbol_busqueda), eliminar_tEstado);
    free(*b);

    b = NULL;
}

// ===============================================================================================================
// FUNCIONES Y PROCEDEMIENTOS AUXILIARES
// ===============================================================================================================

/**
Ordena la ejecución del algoritmo Min-Max para la generación del árbol de búsqueda adversaria, considerando como
estado inicial el estado de la partida almacenado en el árbol almacenado en B.
**/
static void ejecutar_min_max(tBusquedaAdversaria b){
    tArbol a = b->arbol_busqueda;
    tNodo r = a_raiz(a);
    int jugador_max = b->jugador_max;
    int jugador_min = b->jugador_min;

    crear_sucesores_min_max(a, r, 1, IA_INFINITO_NEG, IA_INFINITO_POS, jugador_max, jugador_min);
}

/**
>>>>>  A IMPLEMENTAR   <<<<<
Implementa la estrategia del algoritmo Min-Max con podas Alpha-Beta, a partir del estado almacenado en N.
- A referencia al árbol de búsqueda adversaria.
- N referencia al nodo a partir del cual se construye el subárbol de búsqueda adversaria.
- ES_MAX indica si N representa un nodo MAX en el árbol de búsqueda adversaria.
- ALPHA y BETA indican sendos valores correspondientes a los nodos ancestros a N en el árbol de búsqueda A.
- JUGADOR_MAX y JUGADOR_MIN indican las fichas con las que juegan los respectivos jugadores.
**/
static void crear_sucesores_min_max(tArbol a, tNodo n, int es_max, int alpha, int beta, int jugador_max, int jugador_min){
    tEstado estado = (tEstado) a_recuperar(a,n);
    tEstado sucesor;
    tPosicion cursor, fin;
    tLista sucesores;

    if(estado->utilidad == IA_NO_TERMINO){ //si el estado recibido no corresponde a una hoja

        if(es_max == 1){ //si el nodo recibido corresponde a un turno de max
            sucesores = estados_sucesores(estado, jugador_max);
            cursor = l_primera(sucesores);
            fin = l_fin(sucesores);
            while(cursor != fin){
                sucesor = (tEstado) l_recuperar(sucesores, cursor); //recupera el estado del cursor
                sucesor->utilidad = valor_utilidad(sucesor, jugador_max); //calcula y asigna la utilidad del sucesor
                a_insertar(a, n, NULL, sucesor); //inserta el sucesor en el arbol como hijo de n al final de la lista de hijos

                //llama crear_sucesores_min_max con el nodo correspondiente al sucesor insertado
                crear_sucesores_min_max(a, l_recuperar(a_hijos(a, n), l_ultima(a_hijos(a, n))), 0, alpha, beta, jugador_max, jugador_min);

                //verifica si la utilidad del cursor es mayor a alpha y si lo es actualiza el valor
                alpha = (alpha > sucesor->utilidad) ? alpha : sucesor->utilidad;

                //si alpha es mayor o igual a beta detiene el recorrido de los sucesores
                if(alpha>=beta){
                    break;
                }
                cursor = l_siguiente(sucesores, cursor);
            }

            //actualiza la utilidad del estado correspondiente a n con la mejor utilidad de sus sucesores
            estado->utilidad = alpha;
        }

        else{//si el nodo recibido corresponde a un turno de min
            sucesores = estados_sucesores(estado, jugador_min);
            cursor = l_primera(sucesores);
            fin = l_fin(sucesores);
            while(cursor != fin){
                sucesor = (tEstado) l_recuperar(sucesores, cursor); //recupera el estado del cursor
                sucesor->utilidad = valor_utilidad(sucesor, jugador_max); //calcula y asigna la utilidad del sucesor
                a_insertar(a, n, NULL, sucesor); //inserta el sucesor en el arbol como hijo de n al final de la lista de hijos

                //llama crear_sucesores_min_max con el nodo correspondiente al sucesor insertado
                crear_sucesores_min_max(a, l_recuperar(a_hijos(a, n), l_ultima(a_hijos(a, n))), 1, alpha, beta, jugador_max, jugador_min);

                //verifica si la utilidad del cursor es menor a beta y si lo es actualiza el valor
                beta = (beta < sucesor->utilidad) ? beta : sucesor->utilidad;

                //si alpha es mayor o igual a beta detiene el recorrido de los sucesores
                if(alpha>=beta){
                    break;
                }
                cursor = l_siguiente(sucesores, cursor);
            }

            //actualiza la utilidad del estado correspondiente a n con la mejor utilidad de sus sucesores
            estado->utilidad = beta;
        }

        //elimina los estados que fueron podados
        if (cursor != fin) {
            cursor = l_siguiente(sucesores, cursor);

            while(cursor != l_fin(sucesores)) {
                l_eliminar(sucesores, cursor, &eliminar_tEstado);
            }
        }

        //destruye la lista de sucesor sin afectar los estados
        l_destruir(&sucesores, &eliminar_vacio);
    }
}

/**
>>>>>  A IMPLEMENTAR   <<<<<
Computa el valor de utilidad correspondiente al estado E, y la ficha correspondiente al JUGADOR_MAX, retornado:
- IA_GANA_MAX si el estado E refleja una jugada en el que el JUGADOR_MAX ganó la partida.
- IA_EMPATA_MAX si el estado E refleja una jugada en el que el JUGADOR_MAX empató la partida.
- IA_PIERDE_MAX si el estado E refleja una jugada en el que el JUGADOR_MAX perdió la partida.
- IA_NO_TERMINO en caso contrario.
**/
static int valor_utilidad(tEstado e, int jugador_max) {

    int grilla_llena=1;
    int jugador_min = (jugador_max == PART_JUGADOR_1) ? PART_JUGADOR_2 : PART_JUGADOR_1;

    if( //chequea si max gana
       //horizontales
       (e->grilla[0][0] == jugador_max && e->grilla[0][1] == jugador_max && e->grilla[0][2] == jugador_max) ||
       (e->grilla[1][0] == jugador_max && e->grilla[1][1] == jugador_max && e->grilla[1][2] == jugador_max) ||
       (e->grilla[2][0] == jugador_max && e->grilla[2][1] == jugador_max && e->grilla[2][2] == jugador_max) ||

       //verticales
       (e->grilla[0][0] == jugador_max && e->grilla[1][0] == jugador_max && e->grilla[2][0] == jugador_max) ||
       (e->grilla[0][1] == jugador_max && e->grilla[1][1] == jugador_max && e->grilla[2][1] == jugador_max) ||
       (e->grilla[0][2] == jugador_max && e->grilla[1][2] == jugador_max && e->grilla[2][2] == jugador_max) ||

       //diagonales
       (e->grilla[0][0] == jugador_max && e->grilla[1][1] == jugador_max && e->grilla[2][2] == jugador_max) ||
       (e->grilla[2][0] == jugador_max && e->grilla[1][1] == jugador_max && e->grilla[0][2] == jugador_max)
       )
       return IA_GANA_MAX;

    else if( //chequea si max pierde
       //horizontales
       (e->grilla[0][0] == jugador_min && e->grilla[0][1] == jugador_min && e->grilla[0][2] == jugador_min) ||
       (e->grilla[1][0] == jugador_min && e->grilla[1][1] == jugador_min && e->grilla[1][2] == jugador_min) ||
       (e->grilla[2][0] == jugador_min && e->grilla[2][1] == jugador_min && e->grilla[2][2] == jugador_min) ||

       //verticales
       (e->grilla[0][0] == jugador_min && e->grilla[1][0] == jugador_min && e->grilla[2][0] == jugador_min) ||
       (e->grilla[0][1] == jugador_min && e->grilla[1][1] == jugador_min && e->grilla[2][1] == jugador_min) ||
       (e->grilla[0][2] == jugador_min && e->grilla[1][2] == jugador_min && e->grilla[2][2] == jugador_min) ||

       //diagonales
       (e->grilla[0][0] == jugador_min && e->grilla[1][1] == jugador_min && e->grilla[2][2] == jugador_min) ||
       (e->grilla[2][0] == jugador_min && e->grilla[1][1] == jugador_min && e->grilla[0][2] == jugador_min)
       )
       return IA_PIERDE_MAX;

    else{ //chequea si la grilla esta llena
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (e->grilla[i][j] == PART_SIN_MOVIMIENTO)
                    grilla_llena = 0;
            }
        }

        if(grilla_llena == 0)
            return IA_NO_TERMINO;
        else
            return IA_EMPATA_MAX;
    }

}

/**
>>>>>  A IMPLEMENTAR   <<<<<
Computa y retorna una lista con aquellos estados que representan estados sucesores al estado E.
Un estado sucesor corresponde a la clonación del estado E, junto con la incorporación de un nuevo movimiento
realizado por el jugador cuya ficha es FICHA_JUGADOR por sobre una posición que se encuentra libre en el estado E.
La lista de estados sucesores se debe ordenar de forma aleatoria, de forma tal que una doble invocación de la función
estados_sucesores(estado, ficha) retornaría dos listas L1 y L2 tal que:
- L1 y L2 tienen exactamente los mismos estados sucesores de ESTADO a partir de jugar FICHA.
- El orden de los estado en L1 posiblemente sea diferente al orden de los estados en L2.
**/
static tLista estados_sucesores(tEstado e, int ficha_jugador){
    tLista lista_estados;
    crear_lista(&lista_estados);
    tEstado nuevo_estado;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            //si el espacio (i,j) esta vacio crea un nuevo estado con una ficha en esa casilla
            if(e->grilla[i][j] == PART_SIN_MOVIMIENTO){
                nuevo_estado = clonar_estado(e);
                nuevo_estado->grilla[i][j] = ficha_jugador;

                //agrega el nuevo estado al comienzo o al fin de la lista de manera aleatoria
                if(rand() % 2 == 0)
                    l_insertar(lista_estados, l_primera(lista_estados), nuevo_estado);
                else
                    l_insertar(lista_estados, l_fin(lista_estados), nuevo_estado);
            }
        }
    }

    return lista_estados;
}

/**
>>>>>  A IMPLEMENTAR   <<<<<
Inicializa y retorna un nuevo estado que resulta de la clonación del estado E.
Para esto copia en el estado a retornar los valores actuales de la grilla del estado E, como su valor
de utilidad.
**/
static tEstado clonar_estado(tEstado e) {
    tEstado clon = (tEstado) malloc(sizeof(struct estado));

    for(int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            clon->grilla[i][j] = e->grilla[i][j];
        }
    }

    clon->utilidad = e->utilidad;

    return clon;
}

/**
Computa la diferencia existente entre dos estados.
Se asume que entre ambos existe sólo una posición en el que la ficha del estado anterior y nuevo difiere.
La posición en la que los estados difiere, es retornada en los parámetros *X e *Y.
**/
static void diferencia_estados(tEstado anterior, tEstado nuevo, int * x, int * y){
    int i,j, hallado = 0;
    for(i=0; i<3 && !hallado; i++){
        for(j=0; j<3 && !hallado; j++){
            if (anterior->grilla[i][j] != nuevo->grilla[i][j]){
                *x = j;
                *y = i;
                hallado = 1;
            }
        }
    }
}

// -----------------------

/**
 Elimina el estado recibido
 */
static void eliminar_tEstado(tElemento e){
    tEstado estado_borrar = (tEstado) e;
    free(estado_borrar);
}

/**
 Metodo auxiliar para destruit lista sin alterar los estados en ella
 */
static void eliminar_vacio(tElemento e){}
