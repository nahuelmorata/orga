#include "arbol.h"
#include <stdlib.h>
#include <stdio.h>

void eliminarParaArbol(tElemento elemento);
void mostrar_arbol(tArbol arbol);
void mostrar_hijos(tLista hijos, int profundidad);

int probar_arbol(){

    char *a, *b, *c, *d, *e, *x, *y, *z;

    a = (char *)malloc(sizeof(char));
    b = (char *)malloc(sizeof(char));
    c = (char *)malloc(sizeof(char));
    d = (char *)malloc(sizeof(char));
    e = (char *)malloc(sizeof(char));
    x = (char *)malloc(sizeof(char));
    y = (char *)malloc(sizeof(char));
    z = (char *)malloc(sizeof(char));

    *a = 'a';
    *b = 'b';
    *c = 'c';
    *d = 'd';
    *e = 'e';
    *x = 'x';
    *y = 'y';
    *z = 'z';

    //Inicializa arbol y raiz
    printf("Crea el arbol con raiz 'a':\n");
    tArbol arbol;
    crear_arbol(&arbol);
    crear_raiz(arbol, a);
    tNodo raiz = a_raiz(arbol);
    mostrar_arbol(arbol);

    /**

    Arbol:

                    a
            ________|_______
           |        |       |
           x        b       c
         __|__    __|__
        |     |  |     |
        y     z  d     e

    */

    //Inserta nodos
    printf("\nInserta 'b' y 'c' como hijos de la raiz:\n");
    a_insertar(arbol, raiz, NULL, b);
    a_insertar(arbol, raiz, NULL, c);
    mostrar_arbol(arbol);

    printf("\nInserta 'd' y 'e' como hijos de 'b':\n");
    tNodo nodoB = l_recuperar(a_hijos(arbol, raiz), l_primera(a_hijos(arbol, raiz)));
    a_insertar(arbol, nodoB, NULL, d);
    a_insertar(arbol, nodoB, NULL, e);
    mostrar_arbol(arbol);

    printf("\nInserta 'x' como hermano de 'b':\n");
    a_insertar(arbol, raiz, nodoB, x);
    mostrar_arbol(arbol);

    printf("\nInserta 'z' como hermano de 'b':\n");
    a_insertar(arbol, raiz, nodoB, z);
    mostrar_arbol(arbol);

    tNodo nodoX = l_recuperar(a_hijos(arbol, raiz), l_primera(a_hijos(arbol, raiz)));

    //ERROR
    //printf("CREAR RAIZ EN ARBOL NO VACIO - ARB_OPERACION_INVALIDA 10\n");
    //crear_raiz(arbol, b);

    //ERROR
    //printf("INSERTAR NODO CON HERMANO DE DISTINTO PADRE - ARB_POSICION_INVALIDA 11\n");
    //a_insertar(arbol, nodoX, nodoB, y);

    //ERROR
    //printf("ELIMINAR RAIZ CON MAS DE UN HIJO - ARB_OPERACION_INVALIDA 10\n");
    //a_eliminar(arbol, a_raiz(arbol), &eliminarParaArbol);


    printf("\nInserta 'y' y 'z' como hijos de 'x':\n");
    a_insertar(arbol, nodoX, NULL, y);
    a_insertar(arbol, nodoX, NULL, z);
    mostrar_arbol(arbol);


    //Sub-Arbol
    printf("\nCrea un sub-arbol a partir de 'b':\n");
    tArbol subArbolB;
    a_sub_arbol(arbol, nodoB, &subArbolB);
    mostrar_arbol(subArbolB);
    mostrar_arbol(arbol);

    //Eliminar nodo intermedio
    printf("\nElimina 'x'; 'y' y 'z' pasan a ser hijos de 'a':\n");
    a_eliminar(arbol, nodoX, &eliminarParaArbol);
    mostrar_arbol(arbol);

    //Eliminar nodo hoja, y luego raiz con un solo hijo
    printf("\nElimina 'd' del subarbol y luego elimina la raiz de este:\n");
    a_eliminar(subArbolB, l_recuperar(a_hijos(subArbolB, a_raiz(subArbolB)), l_primera(a_hijos(subArbolB, a_raiz(subArbolB)))), &eliminarParaArbol);
    mostrar_arbol(subArbolB);
    a_eliminar(subArbolB, a_raiz(subArbolB), &eliminarParaArbol);
    mostrar_arbol(subArbolB);


    //Destruir
    printf("\nSe destruye el arbol:\n");
    a_destruir(&arbol, &eliminarParaArbol);
    printf("\nSe destruye el sub-arbol:\n");
    a_destruir(&subArbolB, &eliminarParaArbol);

    return 0;
}

void eliminarParaArbol(tElemento elemento) {
    printf("Elemento eliminado: '%c'\n", *((char *)elemento));
    free(elemento);
}

void mostrar_arbol(tArbol arbol) {
    printf("%c\n", *(char *)arbol->raiz->elemento);

    mostrar_hijos(arbol->raiz->hijos, 1);
}

void mostrar_hijos(tLista hijos, int profundidad) {
    tPosicion cursor = l_primera(hijos);
    tPosicion fin = l_fin(hijos);

    while (cursor != fin) {
        tNodo nodo_hijo = (tNodo) l_recuperar(hijos, cursor);

        for (int i = 0; i < profundidad - 1; i++) {
            printf("│   ");
        }
        printf("└──");

        printf("%c\n", *(char *) nodo_hijo->elemento);

        if (l_longitud(hijos) != 0) {
            mostrar_hijos(nodo_hijo->hijos, profundidad + 1);
        }

        cursor = l_siguiente(hijos, cursor);
    }
}
