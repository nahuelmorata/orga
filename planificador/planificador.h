typedef struct ciudad {
    char * nombre;
    float pos_x;
    float pos_y;
} * TCiudad;

/**
    * Muestra las ciudades enumeradas en orden ascendente por la distancia a cada una desde la ubicacion inicial
    * @param TCiudad * arreglo_ciudades Un arreglo de punteros a ciudades
    * @param int cantidad Tamaño del arreglo arreglo_ciudades
    */
void mostrar_ascendente(TCiudad * arreglo_ciudades, int cantidad);

/**
    * Muestra las ciudades enumeradas en orden desscendente por la distancia a cada una desde la ubicacion inicial
    * @param TCiudad * arreglo_ciudades Un arreglo de punteros a ciudades
    * @param int cantidad Tamaño del arreglo arreglo_ciudades
    */
void mostrar_descendente(TCiudad * arreglo_ciudades, int cantidad);

/**
    * Muestra el recorrido mas corto para recorrer todas las ciudades desde la ubicacion inicial y la distancia total recorrida
    * @param TCiudad * arreglo_ciudades Un arreglo de punteros a ciudades
    * @param int cantidad Tamaño del arreglo arreglo_ciudades
    */
void reducir_horas_de_manejo(TCiudad * arreglo_ciudades, int cantidad);
