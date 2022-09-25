#include <stdlib.h>
#include "colacp.h"
#include "../constantes.h";

TColaCP crear_cola_cp(int (*f)(TEntrada, TEntrada)) {
    TColaCP cola = (TColaCP) malloc(sizeof(struct cola_con_prioridad));
    cola->raiz = ELE_NULO;
    cola->comparador = f;
    cola->cantidad_elementos = 0;
    return cola;
}
