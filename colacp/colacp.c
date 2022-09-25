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
