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


int es_hoja (TNodo n){
    return (n->hijo_izquierdo==ELE_NULO);
}

int esta_lleno(TNodo n){
    return (n->hijo_derecho!=ELE_NULO);
}


int insertar(TNodo n, TNodo nuevo){
        if (esta_lleno(n)){
                insertar(n->hijo_izquierdo, nuevo);
                insertar(n->hijo_derecho, nuevo);
        }
        else {
            if (es_hoja(n)){
                n->hijo_izquierdo=nuevo;
                nuevo->padre=n;
                return TRUE;
            }
            else{
                n->hijo_derecho=nuevo;
                nuevo->padre=n;
                return TRUE;
            }
            return FALSE;
        }
}

int cp_insertar(TColaCP cola, TEntrada entr){
    TNodo nuevo_nodo=(TNodo) malloc(sizeof(struct nodo));
    nuevo_nodo->entrada=entr;
    nuevo_nodo->hijo_derecho=ELE_NULO;
    nuevo_nodo->hijo_izquierdo=ELE_NULO;
    int exito=FALSE;

    if (cola->cantidad_elementos==0){
       cola->raiz=nuevo_nodo;
       exito=TRUE;
    }
    else {
        exito=insertar(cola->raiz, nuevo_nodo);
    }
    if (exito)
        cola->cantidad_elementos++;

    return exito;
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

/**
 * Elimina cada nodo y su respectiva entrada en pos orden
 *
 * @param n nodo a eliminar
 * @param fEliminar funcion para eliminar una entrada dada
 */
void pos_orden(TNodo n, void (*fEliminar)(TEntrada)){
    if (n->hijo_izquierdo != ELE_NULO){
        pos_orden(n->hijo_izquierdo, fEliminar);
    }
    if (n->hijo_derecho != ELE_NULO){
        pos_orden(n->hijo_derecho, fEliminar);
    }
    fEliminar(n->entrada);
    free(n);
}

void cp_destruir(TColaCP cola, void (*fEliminar)(TEntrada)){
    pos_orden(cola->raiz,fEliminar);
    cola->cantidad_elementos=0;
    free(cola);
}

