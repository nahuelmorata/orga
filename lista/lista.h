#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#define LST_ELEMENTO_NULO           1
#define LST_NO_EXISTE_SIGUIENTE     2
#define LST_NO_EXISTE_ANTERIOR      3
#define LST_POSICION_INVALIDA       4
#define LST_ERROR_MEMORIA           5

struct celda{
    void * elemento;
    struct celda * siguiente;
};

typedef struct celda * tLista;
typedef struct celda * tPosicion;
typedef void * tElemento;

/**
 Inicializa una lista vacia.
 Una referencia a la lista creada es referenciada en *L.
**/
extern void crear_lista(tLista * l);

/**
 Inserta el elemento E, en la posicion P, en L.
 Con L = A,B,C,D y la posicion P direccionando C, luego:
 L' = A,B,E,C,D
**/
extern void l_insertar(tLista l, tPosicion p, tElemento e);

/**
 Elimina la celda P de L. El elemento almacenado en la posicion P es eliminado mediante la funcion fEliminar parametrizada.
 Si P es fin(L), finaliza indicando LST_POSICION_INVALIDA.
**/
extern void l_eliminar(tLista l, tPosicion p, void (*fEliminar)(tElemento));

/**
 Destruye la lista L, elimininando cada una de sus celdas. Los elementos almacenados en las celdas son eliminados mediante la funcion fEliminar parametrizada.
**/
extern void l_destruir(tLista * l, void (*fEliminar)(tElemento));

 /**
 Recupera y retorna el elemento en la posicion P.
 Si P es fin(L), finaliza indicando LST_POSICION_INVALIDA.
**/
extern tElemento l_recuperar(tLista l, tPosicion p);

/**
 Recupera y retorna la primera posicion de L.
 Si L es vacia, primera(L) = ultima(L) = fin(L).
**/
extern tPosicion l_primera(tLista l);

/**
 Recupera y retorna la posicion siguiente a P en L.
 Si P es fin(L), finaliza indicando LST_NO_EXISTE_SIGUIENTE.
**/
extern tPosicion l_siguiente(tLista l, tPosicion p);

/**
 Recupera y retorna la posicion anterior a P en L.
 Si P es primera(L), finaliza indicando LST_NO_EXISTE_ANTERIOR.
**/
extern tPosicion l_anterior(tLista l, tPosicion p);

 /**
 Recupera y retorna la ultima posicion de L.
 Si L es vacia, primera(L) = ultima(L) = fin(L).
**/
extern tPosicion l_ultima(tLista l);

 /**
 Recupera y retorna la posicion fin de L.
 Si L es vacia, primera(L) = ultima(L) = fin(L).
**/
extern tPosicion l_fin(tLista l);

/**
 Retorna la longitud actual de la lista.
**/
extern int l_longitud(tLista l);

#endif // LISTA_H_INCLUDED
