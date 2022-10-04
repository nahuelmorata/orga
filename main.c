#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "planificador/planificador.h"

TCiudad *agrandar_arreglo(TCiudad ciudades[], int cantidad) {
    TCiudad *ciudades_aux = (TCiudad *) malloc(sizeof(TCiudad) * cantidad);
    for (int i = 0; i < cantidad - 1; i++) {
        ciudades_aux[i] = ciudades[i];
    }
    free(ciudades);
    return ciudades_aux;
}

void leer_nombre(TCiudad ciudad, FILE *archivo) {
    char nombre_final[100];
    int caracter_leido = fgetc(archivo);
    unsigned int contador = 0;

    while (caracter_leido != ';' && contador < 100) {
        nombre_final[contador] = (char) caracter_leido;

        contador++;
        caracter_leido = fgetc(archivo);
    }

    ciudad->nombre = (char *) malloc(sizeof(char) * (contador + 1));

    ciudad->nombre[contador] = '\0';
    memcpy(ciudad->nombre, nombre_final, contador);
}


TCiudad* leer_archivo(char *path_archivo, int *cantidad_elementos) {
    TCiudad *ciudades = (TCiudad *) malloc(sizeof(TCiudad) * (*cantidad_elementos));
    FILE *archivo_ciudades = fopen(path_archivo, "r");
    int indice = 0;
    TCiudad ciudad_leida;

    while(!feof(archivo_ciudades)) {
        if (*cantidad_elementos == indice) {
            ciudades = agrandar_arreglo(ciudades, *cantidad_elementos + 1);
            *cantidad_elementos = *cantidad_elementos + 1;
        }
        ciudades[indice] = (TCiudad) malloc(sizeof(struct ciudad));
        ciudad_leida = ciudades[indice];
        if (indice == 0) {
            ciudad_leida->nombre = NULL;
            fscanf(archivo_ciudades, "%f;%f\n", &ciudad_leida->pos_x, &ciudad_leida->pos_y);
        } else {
            leer_nombre(ciudad_leida, archivo_ciudades);
            fscanf(archivo_ciudades, "%f;%f\n", &ciudad_leida->pos_x, &ciudad_leida->pos_y);
        }
        indice++;
        printf("\n");
    }

    fclose(archivo_ciudades);
    return ciudades;
}

void liberar_ciudades(TCiudad *ciudades, int cantidad_elementos) {
    for(int i = 0; i < cantidad_elementos; i++) {
        if (ciudades[i]->nombre != NULL) {
            free(ciudades[i]->nombre);
        }
        free(ciudades[i]);
    }
    free(ciudades);
}



int main(int argc, char* args[]) {
    if (argc < 2) {
        printf("Falta el archivo de viajes. El formato es %s <archivo>", args[0]);
        return 0;
    }

    int cantidad_elementos = 2;
    TCiudad *ciudades = leer_archivo(args[1], &cantidad_elementos);

    for(int i = 0; i < cantidad_elementos; i++) {
        TCiudad ciudad = ciudades[i];
        printf("%s en x = %f, y = %f\n", ciudad->nombre, ciudad->pos_x, ciudad->pos_y);
    }

    int opcion;
    printf("Menu \n 1...Mostrar ascendente \n 2...Mostrar descendente \n 3...Reducir horas de manejo \n 4...Salir \n");
    scanf ("%d",&opcion);

    switch(opcion){
      case 1:{
          mostrar_ascendente (ciudades, cantidad_elementos);
          break;
      }
      case 2:{
          mostrar_descendente (ciudades, cantidad_elementos);
          break;
      }
      case 3:{
          reducir_horas_de_manejo (ciudades, cantidad_elementos);
          break;
      }
      case 4: {
          return 0;
          break;
      }
      default:{
        printf ("La opcion ingresada es incorrecta");
        break;
      }
    }

    liberar_ciudades(ciudades, cantidad_elementos);



    return 0;
}
