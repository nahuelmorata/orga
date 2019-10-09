#include "arbol.h"
#include <stdlib.h>

void (*fEliminarUsuario)(tElemento);

// Declaracion de funciones privadas

tPosicion obtenerPosicion(tNodo n);
void eliminar_nodo(tElemento elemento);
tNodo crear_nodo(tNodo nodoPadre, tElemento elemento);
void fEliminarAux(tElemento e);

// -----------------------

/**
Inicializa un arbol vacio.
Una referencia al arbol creado es referenciado en *A.
**/
void crear_arbol(tArbol * a){
    *a = (tArbol) malloc(sizeof(struct arbol));
    if (*a == NULL)
        exit(ARB_ERROR_MEMORIA);

    (*a)->raiz = NULL;
}

/**
Crea la raiz de A.
Si A no es vacio, finaliza indicando ARB_OPERACION_INVALIDA.
**/
void crear_raiz(tArbol arbol, tElemento elemento) {
    if (arbol->raiz != NULL) { //si la raiz no es nula, el arbol no esta vacio
        exit(ARB_OPERACION_INVALIDA);
    }

    arbol->raiz = (tNodo) malloc(sizeof(struct nodo));

    if (arbol->raiz == NULL) {
        exit(ARB_ERROR_MEMORIA);
    }

    arbol->raiz->elemento = elemento;
    arbol->raiz->padre = NULL;
    crear_lista(&(arbol->raiz->hijos));
}

/**
 Inserta y retorna un nuevo nodo en A.
 El nuevo nodo se agrega en A como hijo de NP, hermano izquierdo de NH, y cuyo rotulo es E.
 Si NH es NULL, el nuevo nodo se agrega como ultimo hijo de NP.
 Si NH no corresponde a un nodo hijo de NP, finaliza indicando ARB_POSICION_INVALIDA.
 NP direcciona al nodo padre, mientras NH al nodo hermano derecho del nuevo nodo a insertar.
**/
tNodo a_insertar(tArbol arbol, tNodo nodoPadre, tNodo nodoHermano, tElemento elemento) {
    tNodo nodoNuevo;
    if (nodoHermano != NULL) {
        if (nodoHermano->padre != nodoPadre) {
            exit(ARB_POSICION_INVALIDA);
        }

        nodoNuevo = crear_nodo(nodoPadre, elemento);

        tPosicion posicionHermano = obtenerPosicion(nodoHermano);

        l_insertar(nodoPadre->hijos, posicionHermano, nodoNuevo);
    } else {
        nodoNuevo = crear_nodo(nodoPadre, elemento);

        l_insertar(nodoPadre->hijos, l_fin(nodoPadre->hijos), nodoNuevo);
    }

    return nodoNuevo;
}

/**
 Elimina el nodo N de A.
 El elemento almacenado en el arbol es eliminado mediante la funcion fEliminar parametrizada.
 Si N es la raiz de A, y tiene un solo hijo, este pasa a ser la nueva raiz del arbol.
 Si N es la raiz de A, y a su vez tiene mas de un hijo, finaliza retornando ARB_OPERACION_INVALIDA.
 Si N no es la raiz de A y tiene hijos, estos pasan a ser hijos del padre de N, en el mismo orden y a partir de la posicion que ocupa N en la lista de hijos de su padre.
**/
void a_eliminar(tArbol a, tNodo n, void (*fEliminar)(tElemento)){
    int cantHijos = l_longitud(n->hijos);

    if(n == a->raiz){ //n es raiz

        if(cantHijos == 0){
            a->raiz=NULL;
        }

        else if(cantHijos == 1){
            tNodo nuevaRaiz = l_recuperar(n->hijos, l_primera(n->hijos));
            a->raiz = nuevaRaiz;
            nuevaRaiz->padre = NULL;
        }

        else if(cantHijos > 1){
            exit(ARB_OPERACION_INVALIDA);
        }
    }

    else{ //n no es raiz
        tLista hermanos = n->padre->hijos;
        tPosicion cursorHijo = l_primera(n->hijos);
        tPosicion posN = obtenerPosicion(n);
        tNodo hijo;

        for(int i=0; i<cantHijos; i++){
            hijo = l_recuperar(n->hijos, cursorHijo);
            hijo->padre = n->padre;
            l_insertar(hermanos, posN, hijo);
            posN = l_siguiente(hermanos, posN);
            cursorHijo = l_siguiente(n->hijos, cursorHijo);
        }

        l_eliminar(hermanos, posN, &fEliminarAux);
    }

    fEliminar(n->elemento);
    l_destruir(&(n->hijos), &fEliminarAux);
    n->hijos = NULL;
    n->padre = NULL;
    free(n);
}

/**
 Destruye el arbol A, eliminando cada uno de sus nodos.
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

/**
Recupera y retorna el elemento del nodo N.
*/
tElemento a_recuperar(tArbol a, tNodo n){
    return n->elemento;
}

/**
Recupera y retorna el nodo correspondiente a la raiz de A.
**/
tNodo a_raiz(tArbol a) {
    return a->raiz;
}

/**
 Obtiene y retorna una lista con los nodos hijos de N en A.
**/
tLista a_hijos(tArbol arbol, tNodo nodo) {
    return nodo->hijos;
}

/**
 Inicializa un nuevo arbol en *SA.
 El nuevo arbol en *SA se compone de los nodos del subarbol de A a partir de N.
 El subarbol de A a partir de N debe ser eliminado de A.
**/
void a_sub_arbol(tArbol a, tNodo n, tArbol * sa){
    crear_arbol(sa);
    (*sa)->raiz = n;

    if(n->padre != NULL){
        tLista hermanos = n->padre->hijos;
        tPosicion nPos = obtenerPosicion(n);
        l_eliminar(hermanos, nPos, &fEliminarAux);
        n->padre = NULL;
    }

    else{
        a->raiz = NULL;
    }
}


// Funciones privadas

/**
 Retorna la posicion correspondiente al nodo N relativa a la lista de hijos de su padre
*/
tPosicion obtenerPosicion(tNodo n) {
    tLista hijosPadre = n->padre->hijos;
    tPosicion cursor = l_primera(hijosPadre);
    tPosicion fin = l_fin(hijosPadre);

    while (cursor != fin) {
        tNodo hijo = (tNodo) l_recuperar(hijosPadre, cursor);

        if (hijo == n) {
            return cursor;
        }

        cursor = l_siguiente(hijosPadre, cursor);
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

tNodo crear_nodo(tNodo nodoPadre, tElemento elemento) {

    tNodo nodoNuevo = (tNodo) malloc(sizeof(struct nodo));

    if (nodoNuevo == NULL) {
        exit(ARB_ERROR_MEMORIA);
    }

    nodoNuevo->padre = nodoPadre;
    nodoNuevo->elemento = elemento;
    crear_lista(&(nodoNuevo->hijos));

    return nodoNuevo;
}

void fEliminarAux(tElemento e){

}
