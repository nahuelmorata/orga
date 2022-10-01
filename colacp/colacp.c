#include <stdlib.h>
#include "colacp.h"
#include "../constantes.h";

TEntrada obtener_ultima_entrada_derecha(TColaCP cola);
void burbujeo_abajo(TColaCP cola);
boolean es_hoja(TNodo n);
boolean esta_lleno(TNodo n);
boolean insertar(TNodo n, TNodo nuevo);
void eliminar_pos_orden(TNodo n, void (*fEliminar)(TEntrada));

TColaCP crear_cola_cp(int (*f)(TEntrada, TEntrada)) {
    TColaCP cola = (TColaCP) malloc(sizeof(struct cola_con_prioridad));
    cola->raiz = ELE_NULO;
    cola->comparador = f;
    cola->cantidad_elementos = 0;
    return cola;
}

int cp_cantidad(TColaCP cola) {
    return cola->cantidad_elementos;
}

boolean cp_insertar(TColaCP cola, TEntrada entr) {
    TNodo nuevo_nodo = (TNodo) malloc(sizeof(struct nodo));
    nuevo_nodo->entrada = entr;
    nuevo_nodo->hijo_derecho = ELE_NULO;
    nuevo_nodo->hijo_izquierdo = ELE_NULO;
    boolean exito = FALSE;

    if (cola->cantidad_elementos == 0){
       cola->raiz = nuevo_nodo;
       exito = TRUE;
    }
    else {
        exito = insertar(cola->raiz, nuevo_nodo);
    }
    if (exito)
        cola->cantidad_elementos++;

    return exito;
}

TEntrada cp_eliminar(TColaCP cola) {
    if (cola->cantidad_elementos == 0) {
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

void cp_destruir(TColaCP cola, void (*fEliminar)(TEntrada)){
    eliminar_pos_orden(cola->raiz, fEliminar);
    free(cola);
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

/**
 * Aplica burbujeo hacia abajo para reordenar la cola basado en la funcion de prioridad
 * 
 * @param cola Cola con prioridad
 */
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
void eliminar_pos_orden(TNodo n, void (*fEliminar)(TEntrada)) {
    if (n->hijo_izquierdo != ELE_NULO){
        eliminar_pos_orden(n->hijo_izquierdo, fEliminar);
    }
    if (n->hijo_derecho != ELE_NULO){
        eliminar_pos_orden(n->hijo_derecho, fEliminar);
    }
    fEliminar(n->entrada);
    free(n);
}

/**
 * Retorna si el nodo pasado por parametro es hoja
 * 
 * @param n Nodo a verificar
 * @returns Verdadero si es hoja sino Falso
 */
boolean es_hoja(TNodo n) {
    return n->hijo_izquierdo == ELE_NULO;
}

/**
 * Retorna si el nodo pasado por parametro es tiene su ultimo hijo lleno
 * 
 * @param n Nodo a verificar
 * @returns Verdadero si el ultimo hijo esta lleno sino Falso
 */
boolean esta_lleno(TNodo n) {
    return n->hijo_derecho != ELE_NULO;
}

/**
 * Inserta el nodo en el ultimo lugar que encuentra siguiendo el recorrido de por niveles
 * 
 * @param n Nodo de inicio
 * @param nuevo Nodo a agregar
 * @returns Verdadero si lo inserta sino Falso
 */
boolean insertar(TNodo n, TNodo nuevo) {
    if (esta_lleno(n)) {
        insertar(n->hijo_izquierdo, nuevo);
        insertar(n->hijo_derecho, nuevo);
    } else {
        if (es_hoja(n)){
            n->hijo_izquierdo = nuevo;
            nuevo->padre = n;
            return TRUE;
        } else{
            n->hijo_derecho = nuevo;
            nuevo->padre = n;
            return TRUE;
        }
    }
}
