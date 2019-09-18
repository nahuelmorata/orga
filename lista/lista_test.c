#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

void eliminar(tElemento elemento);
void mostrarLista(tLista lista);

int main() {
    char *a, *b, *c, *d, *e;

    a = (char *)malloc(sizeof(char));
    b = (char *)malloc(sizeof(char));
    c = (char *)malloc(sizeof(char));
    d = (char *)malloc(sizeof(char));
    e = (char *)malloc(sizeof(char));

    *a = 'a';
    *b = 'b';
    *c = 'c';
    *d = 'd';
    *e = 'e';

    //INICIALIZA
    tLista lista;
    crear_lista(&lista);
    printf("Se crea una lista nueva. Longitud: %i\n", l_longitud(lista));

    // Casos que rompe el programa
    //l_eliminar(lista, l_fin(lista), &eliminar);
    //l_recuperar(lista, l_fin(lista));
    //l_siguiente(lista, l_fin(lista));
    //l_anterior(lista, l_primera(lista));

    //INSERTA
    l_insertar(lista, l_primera(lista), a);
    printf("Se inserta la 'a'. Longitud: %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    l_insertar(lista, l_siguiente(lista, l_primera(lista)), d);
    printf("Se inserta la 'd' en la posicion siguiente a la primera. Longitud: %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    l_insertar(lista,  l_ultima(lista), c);
    printf("Se inserta la 'c' en la anteultima posicion. Longitud: %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    l_insertar(lista, l_anterior(lista, l_ultima(lista)), b);
    printf("Se inserta la 'b' en la posicion anterior a la anteultima. Longitud: %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    l_insertar(lista, l_fin(lista), e);
    printf("Se inserta la 'e' al final de la lista. Longitud: %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    //ELIMINA
    printf("Se elimina el primer elemento de la lista.\n");
    l_eliminar(lista, l_primera(lista), &eliminar);
    printf("Longitud: %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    printf("Se elimina el anteultimo elemento de la lista.\n");
    l_eliminar(lista, l_anterior(lista, l_ultima(lista)), &eliminar);
    printf("longitud %i\n", l_longitud(lista));
    mostrarLista(lista);
    printf("\n");

    printf("Se destruye la lista.\n");
    l_destruir(&lista, eliminar);

    return 0;
}

void eliminar(tElemento elemento) {
    printf("Elemento eliminado %c\n", *((char *)elemento));
    free(elemento);
}

void mostrarLista(tLista lista) {
    tPosicion cursor = l_primera(lista);
    tPosicion finLista = l_fin(lista);
    char* valor;

    while(cursor != finLista) {
        valor = (char *) l_recuperar(lista, cursor);
        printf("[%c] ", *valor);
        cursor = l_siguiente(lista, cursor);
    }

    printf("\n");
}
