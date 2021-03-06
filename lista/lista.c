#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

/**
 Inicializa una lista vacia.
 Una referencia a la lista creada es referenciada en *L.
**/
void crear_lista(tLista * lista) {
    (*lista) = (tPosicion) malloc(sizeof(struct celda));

    if ((*lista) == NULL) {
        exit(LST_ERROR_MEMORIA);
    }

    (*lista)->elemento = NULL;
    (*lista)->siguiente = NULL;
}

/**
 Inserta el elemento E, en la posición P, en L.
 Con L = A,B,C,D y la posición P direccionando C, luego:
 L' = A,B,E,C,D
**/
void l_insertar(tLista l, tPosicion p, tElemento e){
    tPosicion nueva_celda = (tPosicion) malloc(sizeof(struct celda));

    if (nueva_celda == NULL) {
        exit(LST_ERROR_MEMORIA);
    }

    nueva_celda -> elemento = e;
    nueva_celda -> siguiente = p -> siguiente;
    p -> siguiente = nueva_celda;
}

/**
 Elimina la celda P de L. El elemento almacenado en la posicion P es eliminado mediante la funcion fEliminar parametrizada.
 Si P es fin(L), finaliza indicando LST_POSICION_INVALIDA.
**/
void l_eliminar(tLista lista, tPosicion posicion, void (*fEliminar)(tElemento)) {
    tPosicion auxSiguiente = NULL; //variable auxiliar para guardar la posicion siguiente a la eliminada

    if (posicion == l_fin(lista)) {
        exit(LST_POSICION_INVALIDA);
    }

    fEliminar(posicion->siguiente->elemento);

    auxSiguiente = posicion->siguiente->siguiente;

    posicion->siguiente->elemento = NULL;
    posicion->siguiente->siguiente = NULL;

    free(posicion->siguiente);

    posicion->siguiente = auxSiguiente;
}

/**
 Destruye la lista L, elimininando cada una de sus celdas. Los elementos almacenados en las celdas son eliminados mediante la función fEliminar parametrizada.
**/
void l_destruir(tLista * l, void (*fEliminar)(tElemento)){
    tPosicion pos_actual = *l; //primera posicion de la lista
    tPosicion pos_aux = pos_actual ->siguiente;//guarda la posicion siguiente

    //elimina el centinela
    pos_actual->siguiente = NULL;
    free(pos_actual);
    pos_actual = pos_aux;

    //elimina el resto de la lista
    while(pos_actual != NULL) {
        pos_aux = pos_actual ->siguiente;
        fEliminar(pos_actual ->elemento);
        pos_actual->elemento = NULL;
        pos_actual->siguiente = NULL;
        free(pos_actual);
        pos_actual = pos_aux;
    }

    l = NULL;
}

 /**
 Recupera y retorna el elemento en la posicion P.
 Si P es fin(L), finaliza indicando LST_POSICION_INVALIDA.
**/
tElemento l_recuperar(tLista l, tPosicion p) {
    if (p == l_fin(l)) {
        exit(LST_POSICION_INVALIDA);
    }

    return p->siguiente->elemento;
}

/**
 Recupera y retorna la primera posición de L.
 Si L es vacía, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_primera(tLista l){
    return l;
}

/**
 Recupera y retorna la posicion siguiente a P en L.
 Si P es fin(L), finaliza indicando LST_NO_EXISTE_SIGUIENTE.
**/
tPosicion l_siguiente(tLista l, tPosicion p) {
    if (p == l_fin(l)) {
        exit(LST_NO_EXISTE_SIGUIENTE);
    }

    return p->siguiente;
}

/**
 Recupera y retorna la posición anterior a P en L.
 Si P es primera(L), finaliza indicando LST_NO_EXISTE_ANTERIOR.
**/
tPosicion l_anterior(tLista l, tPosicion p){
    if(l_primera(l) == p)
        exit(LST_NO_EXISTE_ANTERIOR);

    else{
        tPosicion pos_actual = l; //primera posicion de la lista
        while(pos_actual ->siguiente != p){
            pos_actual = pos_actual->siguiente;
        }
        return pos_actual;
    }
}

 /**
 Recupera y retorna la ultima posicion de L.
 Si L es vacia, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_ultima(tLista l) {
    if(l->siguiente == NULL)
        return l;
    else{
        tPosicion pos_actual = l; //primera posicion de la lista
        while(pos_actual ->siguiente->siguiente != NULL){
            pos_actual = pos_actual->siguiente;
        }
        return pos_actual;
    }
}


 /**
 Recupera y retorna la posición fin de L.
 Si L es vacía, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_fin(tLista l){
    tPosicion pos_actual = l; //primera posicion de la lista
    while(pos_actual ->siguiente != NULL){
        pos_actual = pos_actual->siguiente;
    }
    return pos_actual;
}

/**
 Retorna la longitud actual de la lista.
**/
int l_longitud(tLista l) {
    int contador = 0;
    tPosicion cursor = l->siguiente;

    while (cursor != NULL) {
        contador++;

        cursor = cursor->siguiente;
    }

    return contador;
}
