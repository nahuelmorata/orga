#include <stdlib.h>
#include "colacp.h"
#include "../constantes.h"

TNodo obtener_ultimo_nodo(TColaCP cola);
void burbujeo_abajo(TColaCP cola);
boolean es_hoja(TNodo n);
boolean esta_lleno(TNodo n);
boolean insertar(TNodo n, TNodo nuevo);
void eliminar_pos_orden(TNodo n, void (*fEliminar)(TEntrada));
void burbujeo_arriba(TColaCP cola, TNodo nodo);

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
    nuevo_nodo->padre = ELE_NULO;
    nuevo_nodo->hijo_derecho = ELE_NULO;
    nuevo_nodo->hijo_izquierdo = ELE_NULO;
    boolean exito = FALSE;

    if (cola->cantidad_elementos == 0){
       cola->raiz = nuevo_nodo;
       exito = TRUE;
    } else {
        exito = insertar(cola->raiz, nuevo_nodo);
    }

    if (exito)
        cola->cantidad_elementos++;

    burbujeo_arriba(cola, nuevo_nodo);

    return exito;
}

TEntrada cp_eliminar(TColaCP cola) {
    if (cola->cantidad_elementos == 0) {
        return ELE_NULO;
    }

    TEntrada entrada = cola->raiz->entrada;

    if (cola->cantidad_elementos == 1) {
        free(cola->raiz);
        cola->raiz = ELE_NULO;
        cola->cantidad_elementos = 0;
        return entrada;
    }

    TNodo ultimo_nodo = obtener_ultimo_nodo(cola);
    cola->raiz->entrada = ultimo_nodo->entrada;

    if (ultimo_nodo->padre->hijo_derecho == ultimo_nodo) {
        ultimo_nodo->padre->hijo_derecho = ELE_NULO;
    } else {
        ultimo_nodo->padre->hijo_izquierdo = ELE_NULO;
    }
    free(ultimo_nodo);
    cola->cantidad_elementos--;
    burbujeo_abajo(cola);

    return entrada;
}

void cp_destruir(TColaCP cola, void (*fEliminar)(TEntrada)){
    if (cp_cantidad(cola)>0 ){
        eliminar_pos_orden(cola->raiz, fEliminar);
    }
    free(cola);
}

/**
 * Obtiene la entrada del nodo mas profundo y mas a la derecha, al padre se le borra este hijo
 *
 * @param cola Referencia de la cola con prioridad
 * @returns Entrada del nodo mas profundo y mas a la derecha
 */
TNodo obtener_ultimo_nodo(TColaCP cola) {
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

    return nodo_actual;
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
 * Aplica burbujeo hacia arriba para reordenar la cola basado en la funcion de prioridad
 *
 * @param nodo
 */
void burbujeo_arriba(TColaCP cola, TNodo nodo) {
    TNodo nodo_actual = nodo;
    while(nodo_actual->padre != ELE_NULO) {
        if (cola->comparador(nodo_actual->entrada, nodo_actual->padre->entrada) == 1) {
            TEntrada entrada_aux;

            entrada_aux = nodo_actual->padre->entrada;
            nodo_actual->padre->entrada = nodo_actual->entrada;
            nodo_actual->entrada = entrada_aux;
        }

        nodo_actual = nodo_actual->padre;
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
        n->hijo_izquierdo = ELE_NULO;
    }
    if (n->hijo_derecho != ELE_NULO){
        eliminar_pos_orden(n->hijo_derecho, fEliminar);
        n->hijo_derecho = ELE_NULO;
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
 * Retorna si el nodo pasado por parametro tiene los hijos ocupados
 *
 * @param n Nodo a verificar
 * @returns Verdadero si el tiene todos los hijos ocupados sino Falso
 */
boolean esta_lleno(TNodo n) {
    return n->hijo_izquierdo != ELE_NULO && n->hijo_derecho != ELE_NULO;
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
        if (esta_lleno(n->hijo_izquierdo) && esta_lleno(n->hijo_derecho)) {
            return insertar(n->hijo_izquierdo, nuevo);
        }
        if (esta_lleno(n->hijo_izquierdo)) {
            return insertar(n->hijo_derecho, nuevo);
        }
        return insertar(n->hijo_izquierdo, nuevo);
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
