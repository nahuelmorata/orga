#include "arbol.h"
#include <stdlib.h>

void (*fEliminarUsuario)(tElemento);

tPosicion es_hijo_de(tNodo nodoHermano);
void eliminar_nodo(tElemento elemento);
tNodo crear_nodo(tNodo nodoPadre, tElemento elemento);

void crear_raiz(tArbol arbol, tElemento elemento) {
    if (arbol->raiz == NULL) {
        exit(ARB_OPERACION_INVALIDA);
    }

    arbol->raiz = (tNodo) malloc(sizeof(struct nodo));

    if (arbol->raiz == NULL) {
        exit(ARB_ERROR_MEMORIA);
    }

    arbol->raiz->elemento = elemento;
    arbol->raiz->padre = NULL;
    crear_lista(&arbol->raiz->hijos);
}

tNodo a_insertar(tArbol arbol, tNodo nodoPadre, tNodo nodoHermano, tElemento elemento) {
    tNodo nodoNuevo;
    if (nodoHermano != NULL) {
        if (nodoHermano->padre != nodoPadre) {
            exit(ARB_POSICION_INVALIDA);
        }

        nodoNuevo = crear_nodo(nodoPadre, elemento);

        tPosicion posicionHermano = es_hijo_de(nodoHermano);

        l_insertar(nodoPadre->hijos, posicionHermano, nodoNuevo);
    } else {
        nodoNuevo = crear_nodo(nodoPadre, elemento);

        l_insertar(nodoPadre->hijos, l_fin(nodoPadre->hijos), nodoNuevo);
    }

    return nodoNuevo;
}

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

tLista a_hijos(tArbol arbol, tNodo nodo) {
    return nodo->hijos;
}

// Funciones privadas

tPosicion es_hijo_de(tNodo nodoHermano) {
    tLista hijosPadre = nodoHermano->padre->hijos;
    tPosicion cursor = l_primera(hijosPadre);

    while (cursor != l_fin(hijosPadre)) {
        tNodo hijo = (tNodo) l_recuperar(hijosPadre, cursor);

        if (hijo == nodoHermano) {
            return cursor;
        }

        cursor = l_siguiente(hijosPadre, cursor);
    }
}

void eliminar_nodo(tElemento elemento) {
    tNodo nodo_borrar = (tNodo) elemento;

    l_destruir(&nodo_borrar->hijos, &eliminar_nodo);
    fEliminarUsuario(nodo_borrar->elemento);
    nodo_borrar->elemento = NULL;
    nodo_borrar->padre = NULL;

    free(nodo_borrar);
}

tNodo crear_nodo(tNodo nodoPadre, tElemento elemento) {
    tNodo nodoNuevo = (tNodo) malloc(sizeof(struct nodo));

    if (nodoNuevo == NULL) {
        exit(ARB_ERROR_MEMORIA);
    }

    nodoNuevo->padre = nodoPadre;
    nodoNuevo->elemento = elemento;
    crear_lista(&nodoNuevo->hijos);

    return nodoNuevo;
}