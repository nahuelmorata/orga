#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

void eliminar(tElemento elemento);
void mostrarLista(tLista lista);

int main() {
    //INICIALIZA
    tLista lista;
    crear_lista(&lista);
    printf("Se crea una lista nueva. Longitud: %i\n", l_longitud(lista));

    //INSERTA
    l_insertar(lista, l_primera(lista), "a");
    printf("Se inserta la 'a'. Longitud: %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    l_insertar(lista, l_siguiente(lista, l_primera(lista)), "d");
    printf("Se inserta la 'd' en la posicion siguiente a la primera. Longitud: %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    l_insertar(lista,  l_ultima(lista), "c");
    printf("Se inserta la 'c' en la ultima posicion. Longitud: %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    l_insertar(lista, l_anterior(lista, l_ultima(lista)), "b");
    printf("Se inserta la 'b' en la posicion anterior a la ultima. Longitud: %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    l_insertar(lista, l_fin(lista), "e");
    printf("Se inserta la 'e' al final de la lista. Longitud: %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    //ELIMINA
    printf("Se elimina el primer elemento de la lista.\n");
    l_eliminar(lista, l_primera(lista), eliminar);
    printf("Longitud: %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    printf("Se elimina el anteultimo elemento de la lista.\n");
    l_eliminar(lista, l_anterior(lista, l_ultima(lista)), eliminar);
    printf("longitud %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    printf("Se destruye la lista.\n");
    l_destruir(&lista, eliminar);

    return 0;
}

void eliminar(tElemento elemento) {
    printf("Elemento eliminado\n");
}

void mostrarLista(tLista lista) {
    tPosicion cursor = l_primera(lista);
    tPosicion finLista = l_fin(lista);
    char* valor;

    while(cursor != finLista) {
        valor = (char*) l_recuperar(lista, cursor);
        printf("[%s] ", valor);
        cursor = l_siguiente(lista, cursor);
    }

    printf("\n");
}
