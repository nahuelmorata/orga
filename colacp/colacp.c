#include <stdlib.h>
#include "colacp.h"
#include "../constantes.h";

TEntrada obtener_ultima_entrada_derecha(TColaCP cola);
void burbujeo_abajo(TColaCP cola);

TColaCP crear_cola_cp(int (*f)(TEntrada, TEntrada)) {
    TColaCP cola = (TColaCP) malloc(sizeof(struct cola_con_prioridad));
    cola->raiz = ELE_NULO;
    cola->comparador = f;
    cola->cantidad_elementos = 0;
    return cola;
}

TEntrada cp_eliminar(TColaCP cola) {
    if (cola->cantidad_elementos = 0) {
        return ELE_NULO;
    }

    TEntrada entrada = cola->raiz->entrada;

    if (cola->cantidad_elementos == 1) {
        cola->raiz = ELE_NULO;
        cola->cantidad_elementos = 0;
        return entrada;
    }

    TEntrada nueva_raiz = obtener_ultima_entrada_derecha(cola);
    cola->raiz->entrada = nueva_raiz;

    burbujeo_abajo(cola);

    return entrada;
}

/**
 * Obtiene la entrada del nodo mas profundo y mas a la derecha, al padre se le borra este hijo
 * 
 * @param cola Referencia de la cola con prioridad
 * @returns Entrada del nodo mas profundo y mas a la derecha
 */
TEntrada obtener_ultima_entrada_derecha(TColaCP cola) {
    TNodo nodo_actual = cola->raiz;
    boolean seguir = TRUE;

    while (seguir) {
        if (nodo_actual->hijo_derecho != ELE_NULO) {
            nodo_actual = nodo_actual->hijo_derecho;
        } else if (nodo_actual->hijo_izquierdo != ELE_NULO) {
            nodo_actual = nodo_actual->hijo_izquierdo;
        } else {
            seguir = FALSE;
        }
    }
}

void burbujeo_abajo(TColaCP cola) {
    boolean seguir = TRUE;
    TNodo nodo_actual = cola->raiz, nodo_minimo = NULL;
    TEntrada aux_entrada = NULL;
    
    while(seguir) {
        if (nodo_actual->hijo_izquierdo == ELE_NULO) {
            seguir = FALSE;
        } else {
            if (nodo_actual->hijo_derecho != ELE_NULO) {
                if (cola->comparador(nodo_actual->hijo_izquierdo->entrada, nodo_actual->hijo_derecho->entrada) == -1) {
                    nodo_minimo = nodo_actual->hijo_izquierdo;
                } else {
                    nodo_minimo = nodo_actual->hijo_derecho;
                }
            } else {
                nodo_minimo = nodo_actual->hijo_izquierdo;
            }

            if (cola->comparador(nodo_actual->entrada, nodo_minimo->entrada) == -1) {
                aux_entrada = nodo_actual->entrada;
                nodo_actual->entrada = nodo_minimo->entrada;
                nodo_minimo->entrada = aux_entrada;
            } else {
                seguir = FALSE;
            }
        }
    }
}
