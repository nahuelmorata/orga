#ifndef COLA_CP_H
#define COLA_CP_H

typedef void * TClave;
typedef void * TValor;

typedef struct entrada {
	TClave clave;
	TValor valor;
} * TEntrada;

typedef struct nodo {
	TEntrada entrada;
	struct nodo * padre;
	struct nodo * hijo_izquierdo;
	struct nodo * hijo_derecho;
} * TNodo;

typedef struct cola_con_prioridad {
	int cantidad_elementos;
	TNodo raiz;
	int (*comparador)(TEntrada, TEntrada);
} * TColaCP;


/*
    * Crea una cola con prioridad
    * @param f, una funcion de ordenamiento
    * @return una cola con prioridad vacia
*/
TColaCP crear_cola_cp(int (*f)(TEntrada, TEntrada));

/*
    * Inserta una entrada en la cola con prioridad
    * @param cola, una cola con prioridad
    * @param entr entrada a insertar
    * @return verdadero si procede con exito, falso en caso contrario
*/
int cp_insertar(TColaCP cola, TEntrada entr);

/*
    * Elimina la entrada con mayor prioridad
    * @param cola, una cola con prioridad
    * @return  la entrada eliminada, o ELE NULO si la cola es vacia
*/
TEntrada cp_eliminar(TColaCP cola);


/*
    * Retorna la cantidad de entradas
    * @return cantidad de entradas
*/
int cp_cantidad(TColaCP cola);

/*
	* Elimina todas las entradas y libera toda la memoria utilizada por la cola
	* @param cola, una cola con prioridad
    * @param fEliminar, una funcion de  eliminacion
*/
void cp_destruir(TColaCP cola, void (*fEliminar)(TEntrada) );

#endif
