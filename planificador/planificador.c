#include <stdlib.h>
#include <math.h>
#include "../colacp/colacp.h"
#include "../constantes.h"
#include "planificador.h"

	//definir los tipos de TClave=int y TValor=TCiudad

	void fEliminar(TEntrada entrada_a_eliminar){
		entrada_a_eliminar->clave=ELE_NULO;
		entrada_a_eliminar->valor=ELE_NULO;
		free(entrada_a_eliminar);
	}

	int calcular_distancia (TCiudad desde, TCiudad hasta){
        return abs(hasta->pos_x - desde->pos_x) + abs(hasta->pos_y - desde->pos_y);
	}

	TColaCP ordear(int(*prioridad)(TEntrada,TEntrada), TCiudad ciudad_actual, TCiudad arr_ciudades[], int cantidad){
		TColaCP cola = crear_cola_cp(*prioridad);
		for (int i; i< cantidad; i++){

			TEntrada entrada_ciudad = (TEntrada) malloc(sizeof(struct entrada));
			entrada_ciudad->clave=calcular_distancia(ciudad_actual, arr_ciudades[i]);
			entrada_ciudad->valor=arr_ciudades[i];
			insertar(cola, entrada_ciudad);
		}
		return cola;
	}

	void mostrar (TColaCP cola){
        for (int i=1; i<=cp_cantidad(cola);i++){
            TEntrada entrada_mayor_prioridad = cp_eliminar(cola);
            printf("%d. %s.\n", i, entrada_mayor_prioridad->valor->nombre);
            fEliminar(entrada_mayor_prioridad);
        }
        cp_destruir(cola,(*fEliminar));
	}

	int prioridad_ascendente(TEntrada ciudad1, TEntrada ciudad2){
		int retorno = 0;
		int distancia1=ciudad1->clave;
		int distancia2=ciudad2->clave;
		if (distancia1<distancia2)
			retorno=1;
		else if (distancia1>distancia2)
			retorno=-1;
		return retorno;
	}

	int prioridad_descendente(TEntrada ciudad1, TEntrada ciudad2){
		int retorno = 0;
		int distancia1=ciudad1->clave;
		int distancia2=ciudad2->clave;
		if (distancia1<distancia2)
			retorno=-1;
		else if (distancia1>distancia2)
			retorno=1;
		return retorno;
	}


	TCiudad * eliminar_elemento_arreglo(TCiudad ciudades[], TCiudad ciudad, int cantidad){
		int pos = 0;
		TCiudad *ciudades_aux = (TCiudad *) malloc(sizeof(TCiudad) * (cantidad - 1));
		for (int i = 0; i < cantidad; i++) {
            if (ciudad != ciudades[i]){
                ciudades_aux[pos] = ciudades[i];
                pos++;
            }
        }
        free(ciudades);
        return ciudades_aux;
	}

	void mostrar_ascendente(TCiudad * arreglo_ciudades, int cantidad){
        printf("Mostrar ascendente \n");
		mostrar (ordenar(*prioridad_ascendente, arreglo_ciudades[0], arreglo_ciudades, cantidad));
	}

    void mostrar_desscendente(TCiudad * arreglo_ciudades, int cantidad){
		printf("Mostrar desscendente \n");
		mostrar (ordenar(*prioridad_descendente, arreglo_ciudades[0], arreglo_ciudades, cantidad));
    }

    void reducir_horas_de_manejo(TCiudad * arreglo_ciudades, int cantidad){
		printf("Reducir horas manejo\n");
		int total_recorrido=0;
		int orden=1;
		int cantidad_ciudades=cantidad;
		TCiudad *copia_arreglo_ciudades = (TCiudad *) malloc(sizeof(TCiudad) * (cantidad_ciudades));
		for (int i=0; i < cantidad_ciudades; i++){
			copia_arreglo_ciudades[i]=arreglo_ciudades[i];
		}

		TCiudad ciudad_actual = copia_arreglo_ciudades[0];

		while (cantidad_ciudades>1){
			TColaCP cola = ordenar(*prioridad_ascendente, ciudad_actual, copia_arreglo_ciudades);
			TEntrada mayor_prioridad = cp_eliminar(cola);
			ciudad_actual=mayor_prioridad->valor;
			printf("%d. %s.\n", orden, ciudad_actual->valor->nombre);
			total_recorrido+= mayor_prioridad->clave;
			copia_arreglo_ciudades=eliminar_elemento_arreglo(copia_arreglo_ciudades, ciudad_actual, cantidad_ciudades);
			cantidad_ciudades = cantidad_ciudades - 1;
			fEliminar(mayor_prioridad);
			cp_destruir(cola,*fEliminar);
			orden++;
		}
		free(copia_arreglo_ciudades);
		printf("Total recorrido: %d.", total_recorrido);
    }





