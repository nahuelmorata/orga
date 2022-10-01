#include <stdlib.h>
#include <stdio.h>
#include "colacp/colacp.h"
#include "constantes.h"

int comparador(TEntrada entrada1, TEntrada entrada2) {
    int clave1 = *((int *) entrada1->clave);
    int clave2 = *((int *) entrada2->clave);
    if (clave1 < clave2) {
        return 1;
    }
    if (clave2 < clave1) {
        return -1;
    }
    return 0;
}

void eliminar_entrada(TEntrada entrada) {
    free(entrada->clave);
    free(entrada->valor);
    free(entrada);
}

TEntrada generar_entrada(int clave, int valor) {
    TEntrada entrada = (TEntrada) malloc(sizeof(struct entrada));
    int *clavePuntero = (int*) malloc(sizeof(int));
    int *claveValor = (int*) malloc(sizeof(int));
    *clavePuntero = clave;
    *claveValor = valor;
    entrada->clave = clavePuntero;
    entrada->valor = claveValor;
    return entrada;
}

void mostrar_nodo(TNodo nodo) {
    printf("Padre: %d\n", *(int*)(nodo->entrada->clave));
    if (nodo->hijo_izquierdo != ELE_NULO) {
        printf("Hijo izquierdo: %d\n", *(int*)(nodo->hijo_izquierdo->entrada->clave));
    }
    if (nodo->hijo_derecho != ELE_NULO) {
        printf("Hijo derecho: %d\n", *(int*)(nodo->hijo_derecho->entrada->clave));
    }

    if (nodo->hijo_izquierdo != ELE_NULO) {
        mostrar_nodo(nodo->hijo_izquierdo);
    }
    if (nodo->hijo_derecho != ELE_NULO) {
        mostrar_nodo(nodo->hijo_derecho);
    }
}

void mostrar_cola(TColaCP cola) {
    mostrar_nodo(cola->raiz);
}

int main() {
    TColaCP cola = crear_cola_cp(comparador);

    cp_insertar(cola, generar_entrada(20, 1));
    cp_insertar(cola, generar_entrada(45, 2));
    cp_insertar(cola, generar_entrada(30, 3));
    cp_insertar(cola, generar_entrada(16, 4));
    cp_insertar(cola, generar_entrada(18, 5));
    cp_insertar(cola, generar_entrada(33, 6));
    cp_insertar(cola, generar_entrada(25, 7));

    mostrar_cola(cola);

    TEntrada entrada = cp_eliminar(cola);
    printf("Entrada eliminada: %i", *((int *)entrada->clave));
    eliminar_entrada(entrada);

    mostrar_cola(cola);

    cp_destruir(cola, eliminar_entrada);
}