#include "arbol.h"
#include <stdlib.h>
#include <stdio.h>


void eliminarParaArbol(tElemento elemento);
void mostrarPreorden(tArbol arbol, tNodo n);
void mostrarPreordenRec(tArbol arbol, tNodo n, int cont);

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
    mostrarPreorden(arbol, raiz);

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
    mostrarPreorden(arbol, raiz);

    printf("\nInserta 'd' y 'e' como hijos de 'b':\n");
    tNodo nodoB = l_recuperar(a_hijos(arbol, raiz), l_primera(a_hijos(arbol, raiz)));
    a_insertar(arbol, nodoB, NULL, d);
    a_insertar(arbol, nodoB, NULL, e);
    mostrarPreorden(arbol, raiz);

    printf("\nInserta 'x' como hermano de 'b':\n");
    a_insertar(arbol, raiz, nodoB, x);
    mostrarPreorden(arbol, raiz);


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
    mostrarPreorden(arbol, raiz);


    //Sub-Arbol
    printf("\nCrea un sub-arbol a partir de 'b':\n");
    tArbol subArbolB;
    a_sub_arbol(arbol, nodoB, &subArbolB);
    mostrarPreorden(subArbolB, a_raiz(subArbolB));
    mostrarPreorden(arbol, raiz);

    //Eliminar nodo intermedio
    printf("\nElimina 'x'; 'y' y 'z' pasan a ser hijos de 'a':\n");
    a_eliminar(arbol, nodoX, &eliminarParaArbol);
    mostrarPreorden(arbol, raiz);

    //Eliminar nodo hoja, y luego raiz con un solo hijo
    printf("\nElimina 'd' del subarbol y luego elimina la raiz de este:\n");
    a_eliminar(subArbolB, l_recuperar(a_hijos(subArbolB, a_raiz(subArbolB)), l_primera(a_hijos(subArbolB, a_raiz(subArbolB)))), &eliminarParaArbol);
    mostrarPreorden(subArbolB, a_raiz(subArbolB));
    a_eliminar(subArbolB, a_raiz(subArbolB), &eliminarParaArbol);
    mostrarPreorden(subArbolB, a_raiz(subArbolB));


    //Destruir
    printf("\nSe destruye el arbol:\n");
    a_destruir(&arbol, &eliminarParaArbol);
    printf("\nSe destruye el sub-arbol:\n");
    a_destruir(&subArbolB, &eliminarParaArbol);

    return 0;
}

void mostrarPreorden(tArbol arbol, tNodo n){
    mostrarPreordenRec(arbol, n, 0);
    printf("\n");
}

void mostrarPreordenRec(tArbol arbol, tNodo n, int cont){
    char* valor = (char *) a_recuperar(arbol, n);
    printf("[%i%c] ", cont++, *valor);

    tLista hijos = a_hijos(arbol, n);
    int cantHijos = l_longitud(hijos);

    tPosicion hijoPos = l_primera(hijos);
    tNodo hijo;
    for(int i=0; i<cantHijos; i++){
        hijo = l_recuperar(hijos, hijoPos);
        mostrarPreordenRec(arbol, hijo, cont);
        hijoPos = l_siguiente(hijos, hijoPos);
    }
}

void eliminarParaArbol(tElemento elemento) {
    printf("Elemento eliminado: '%c'\n", *((char *)elemento));
    free(elemento);
}
