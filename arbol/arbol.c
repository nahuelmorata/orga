#include "arbol.h"


/**
Crea la raiz de A.
Si A no es vacio, finaliza indicando ARB_OPERACION_INVALIDA.
**/
extern void crear_raiz(tArbol a, tElemento e);

/**
 Inserta y retorna un nuevo nodo en A.
 El nuevo nodo se agrega en A como hijo de NP, hermano izquierdo de NH, y cuyo rotulo es E.
 Si NH es NULL, el nuevo nodo se agrega como primer hijo de NP.
 Si NH no corresponde a un nodo hijo de NP, finaliza indicando ARB_POSICION_INVALIDA.
 NP direcciona al nodo padre, mientras NH al nodo hermano derecho del nuevo nodo a insertar.
**/
extern tNodo a_insertar(tArbol a, tNodo np, tNodo nh, tElemento e);

/**
 Destruye el arbol A, elimininando cada uno de sus nodos.
 Los elementos almacenados en el arbol son eliminados mediante la funcion fEliminar parametrizada.
**/
extern void a_destruir(tArbol * a, void (*fEliminar)(tElemento));

/**
Recupera y retorna el nodo correspondiente a la raiz de A.
**/
extern tNodo a_raiz(tArbol a);

/**
 Obtiene y retorna una lista con los nodos hijos de N en A.
**/
extern tLista a_hijos(tArbol a, tNodo n);
