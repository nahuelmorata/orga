#include <stdlib.h>
#include "colacp.h"
#include "../constantes.h";

TColaCP crear_cola_cp(int (*f)(TEntrada, TEntrada)) {
    funcion_prioridad = f;
    TColaCP cola = (TColaCP) malloc(sizeof(struct cola_con_prioridad));
    cola->raiz = ELE_NULO;
    cola->cantidad_elementos = 0;
    return cola;
}

void cp_destruir(TColaCP cola, void (*fEliminar)(TEntrada)){
    //mientras el arbol no esta vacio, llamo a fEliminar
    while (cp_cantidad(cola) > 0){
        cp-eliminar(cola);
    }
    //borrar la memoria asociada  
    
    
}
