#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../colacp/colacp.h"
#include "../constantes.h"
#include "planificador.h"

/**
 * Devuelve clon de ciudades en profundidad
 * @param ciudades Ciudades a clonar
 * @param cantidad Cantidad de ciudades
 * @return Clon de ciudades
 */
TCiudad *clonar_ciudades(TCiudad *ciudades, int cantidad);

/**
* Elimina y libera la memoria de una entrada
* @param entrada_a_eliminar La entrada eliminada
*/
void fEliminar(TEntrada entrada_a_eliminar) {
    free(entrada_a_eliminar->clave);
    free(entrada_a_eliminar);
}

/**
* Calcula la distancia entre dos ciudades
* @param TCiudad desde Un puntero a una ciudad de partida.
* @param TCiudad hasta Un puntero a una ciudad de llegada.
* @return La distancia entre  dos ciudades
*/
float calcular_distancia (TCiudad desde, TCiudad hasta) {
    return abs(hasta->pos_x - desde->pos_x) + abs(hasta->pos_y - desde->pos_y);
}

/**
* Determina la entrada con mayor prioridad ascendente
* @param TEntrada cuidad1 Un puntero a una entrada
* @param TEntrada cuidad2 Un puntero a una entrada
* @return 0 si las ciudades tiene igal prioridad, 1 si la ciudad1 tiene mayor prioridad que la ciudad2 y -1 en caso contrario
*/
int prioridad_ascendente(TEntrada ciudad1, TEntrada ciudad2) {
    int retorno = 0;
    float distancia1 = *((float *) ciudad1->clave);
    float distancia2 = *((float *) ciudad2->clave);
    if (distancia1 < distancia2)
        retorno = 1;
    else if (distancia1 > distancia2)
        retorno = -1;
    return retorno;
}

/**
* Determina la entrada con mayor prioridad descendente
* @param TEntrada cuidad1 Un puntero a una entrada
* @param TEntrada cuidad2 Un puntero a una entrada
* @return 0 si las ciudades tiene igal prioridad, 1 si la ciudad2 tiene mayor prioridad que la ciudad1 y -1 en caso contrario
*/
int prioridad_descendente(TEntrada ciudad1, TEntrada ciudad2) {
    int retorno = 0;
    float distancia1 = *((float *) ciudad1->clave);
    float distancia2 = *((float *) ciudad2->clave);
    if (distancia1 < distancia2)
        retorno = -1;
    else if (distancia1 > distancia2)
        retorno = 1;
    return retorno;
}

/**
* Elimina un elemento de un arreglo
* @param TEntrada cuidad1es[] Un arreglo de ciudades
* @param TCiudad ciudad Una ciudad a eliminar
* @param int cantidad El tama�o del arreglo
* @return El arreglo sin la ciudad eliminada
*/
TCiudad * eliminar_elemento_arreglo(TCiudad ciudades[], TCiudad ciudad, int cantidad) {
    int pos = 0;
    TCiudad * ciudades_aux = (TCiudad *) malloc(sizeof(TCiudad) * ( cantidad - 1));
    for (int i = 0; i < cantidad; i++) {
        if (ciudad->nombre != ciudades[i]->nombre) {
            ciudades_aux[pos] = ciudades[i];
            pos++;
        }
    }
    return ciudades_aux;
}

/**
* Ordena las ciudades recibidas por una funcion_prioridad dada
* @param int funcion_prioridad Funcion de funcion_prioridad
* @param TCiudad ciudad_actual Un puntero a la ciudad desde la cual se ordena
* @param TCiudad arr_ciudades[] Un arreglo de punteros a ciudades
* @param int cantidad Tamaño del arreglo arr_ciudades
* @return El arreglo sin la ciudad eliminada
*/
TColaCP ordenar(int (*funcion_prioridad)(TEntrada, TEntrada), TCiudad ciudad_actual, TCiudad arr_ciudades[], int cantidad) {
    TColaCP cola = crear_cola_cp(funcion_prioridad);
		for (int i=1; i < cantidad; i++){
            TEntrada entrada_ciudad = (TEntrada) malloc(sizeof(struct entrada));
            float *clavePuntero = (float*) malloc(sizeof(float));

            *clavePuntero = calcular_distancia(ciudad_actual, arr_ciudades[i]);

            entrada_ciudad->clave = clavePuntero;
            entrada_ciudad->valor = arr_ciudades[i];

			cp_insertar(cola, entrada_ciudad);
		}
    return cola;
}

/**
* Muestra por pantalla las ciudades ordenadas por una prioridad dada
* @param TColaCP cola Una cola con prioridad de punteros a ciudades
*/
void mostrar(TColaCP cola) {
    int i=0;
    while (cp_cantidad(cola)>0){
        TEntrada entrada_mayor_prioridad = cp_eliminar(cola);
        TCiudad ciudad_mayor_prioridad = (TCiudad) entrada_mayor_prioridad->valor;
        printf("%d. %s.\n", ++i, ciudad_mayor_prioridad->nombre);
        fEliminar(entrada_mayor_prioridad);
    }
    cp_destruir(cola,fEliminar);
}

void mostrar_ascendente(TCiudad * arreglo_ciudades, int cantidad) {
    printf("Mostrar ascendente \n");
    TColaCP cola_ordenada = ordenar(prioridad_ascendente, arreglo_ciudades[0], arreglo_ciudades, cantidad);
    mostrar (cola_ordenada);
}

void mostrar_descendente(TCiudad * arreglo_ciudades, int cantidad){
    printf("Mostrar descendente \n");
    TColaCP cola_ordenada = ordenar(prioridad_descendente, arreglo_ciudades[0], arreglo_ciudades, cantidad);
    mostrar (cola_ordenada);
}

void reducir_horas_de_manejo(TCiudad * arreglo_ciudades, int cantidad){
    printf("Reducir horas manejo\n");
    float total_recorrido = 0;
    int orden = 1;
    int cantidad_ciudades = cantidad;

    TCiudad *copia_arreglo_ciudades = clonar_ciudades(arreglo_ciudades, cantidad_ciudades);
    TCiudad ciudad_actual = copia_arreglo_ciudades[0];

    while (cantidad_ciudades > 1) {
        TColaCP cola = ordenar(prioridad_ascendente, ciudad_actual, copia_arreglo_ciudades, cantidad_ciudades);
        TEntrada mayor_prioridad = cp_eliminar(cola);
        cp_destruir(cola, fEliminar);

        ciudad_actual = (TCiudad) mayor_prioridad->valor;
        TCiudad * aux_copia_clon = eliminar_elemento_arreglo(copia_arreglo_ciudades, ciudad_actual, cantidad_ciudades);
        cantidad_ciudades = cantidad_ciudades - 1;

        printf("%d. %s.\n", orden, ciudad_actual->nombre);

        total_recorrido = total_recorrido + *((float*) mayor_prioridad->clave);

        fEliminar(mayor_prioridad);

        free(copia_arreglo_ciudades);
        copia_arreglo_ciudades = aux_copia_clon;

        orden++;
    }
    free(copia_arreglo_ciudades);
    printf("Total recorrido: %f.", total_recorrido);
}

TCiudad *clonar_ciudades(TCiudad *ciudades, int cantidad) {
    TCiudad *clon = (TCiudad *) malloc(sizeof(TCiudad) * cantidad);

    for (int i = 0; i < cantidad; i++) {
        clon[i] = ciudades[i];
    }

    return clon;
}