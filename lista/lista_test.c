#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

void eliminar(tElemento elemento);
void mostrarLista(tLista lista);

int main() {
    tLista lista;
    crear_lista(&lista);
    printf("longitud %i\n", l_longitud(lista));

    l_insertar(lista, l_primera(lista), "a");
    printf("longitud %i\n", l_longitud(lista));

    l_insertar(lista, l_siguiente(lista, l_primera(lista)), "b");
    printf("longitud %i\n", l_longitud(lista));

    mostrarLista(lista);
    
    l_eliminar(lista, l_primera(lista), eliminar);
    printf("longitud %i\n", l_longitud(lista));
    mostrarLista(lista);

    l_eliminar(lista, l_primera(lista), eliminar);
    printf("longitud %i\n", l_longitud(lista));
    mostrarLista(lista);

    return 0;
}

void eliminar(tElemento elemento) {
    
}

void mostrarLista(tLista lista) {
    tPosicion cursor = l_primera(lista)->siguiente;

    while(cursor != NULL) {
        printf("elemento %s\n", cursor->elemento);

        cursor = cursor->siguiente;
    }
}