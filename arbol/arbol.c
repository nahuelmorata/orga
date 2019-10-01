#include "arbol.h"
#include <stdlib.h>

void (*fEliminarUsuario)(tElemento);

tPosicion es_hijo_de(tNodo nodoPadre, tNodo nodoHermano);
void eliminar_nodo(tElemento elemento);

void crear_raiz(tArbol arbol, tElemento elemento) {
    if (arbol->raiz == NULL) {
        exit(ARB_OPERACION_INVALIDA);
    }

    arbol->raiz = (tNodo) malloc(sizeof(struct nodo));

    arbol->raiz->elemento = elemento;
    arbol->raiz->padre = NULL;
    crear_lista(&arbol->raiz->hijos);
}

tNodo a_insertar(tArbol arbol, tNodo nodoPadre, tNodo nodoHermano, tElemento elemento) {
    tPosicion posicionNodoHermano = l_primera(nodoPadre->hijos);

    if (nodoHermano != NULL) {    
        posicionNodoHermano = es_hijo_de(nodoPadre, nodoHermano);

        if (posicionNodoHermano != NULL) {
            exit(ARB_POSICION_INVALIDA);
        }
    }

    tNodo nodoNuevo = (tNodo) malloc(sizeof(struct nodo));
    nodoNuevo->padre = nodoPadre;
    nodoNuevo->elemento = elemento;
    crear_lista(&nodoNuevo->hijos);

    l_insertar(nodoPadre->hijos, posicionNodoHermano, nodoNuevo);

    return nodoNuevo;
}

/**
 Destruye el arbol A, elimininando cada uno de sus nodos.
 Los elementos almacenados en el arbol son eliminados mediante la funcion fEliminar parametrizada.
**/
void a_destruir(tArbol * a, void (*fEliminar)(tElemento)) {
    fEliminarUsuario = fEliminar;
    
    tLista hijos = (*a)->raiz->hijos;
    l_destruir(&hijos, &eliminar_nodo);

    fEliminar((*a)->raiz->elemento);
    (*a)->raiz->elemento = NULL;

    free((*a)->raiz);
    (*a)->raiz = NULL;

    free((*a));
    a = NULL;

    fEliminarUsuario = NULL;
}

tNodo a_raiz(tArbol a) {
    return a->raiz;
}

/**
 Obtiene y retorna una lista con los nodos hijos de N en A.
**/
tLista a_hijos(tArbol arbol, tNodo nodo) {
    return nodo->hijos;
}

// Funciones privadas

tPosicion es_hijo_de(tNodo nodoPadre, tNodo nodoHermano) {
    tPosicion cursor = l_primera(nodoPadre->hijos);

    while (cursor != NULL) {
        tNodo hijo = (tNodo) l_recuperar(nodoPadre->hijos, cursor);

        if (hijo == nodoHermano) {
            return cursor;
        }
    }

    return NULL;
}

void eliminar_nodo(tElemento elemento) {
    tNodo nodo_borrar = (tNodo) elemento;

    l_destruir(&nodo_borrar->hijos, &eliminar_nodo);
    fEliminarUsuario(nodo_borrar->elemento);
    nodo_borrar->elemento = NULL;
    nodo_borrar->padre = NULL;

    free(nodo_borrar);
}