#include <stdio.h>
#include <string.h>

typedef struct {
    char nombre[51];
    int minutos;
    int segundos;
    int puntuacion;
} cancion;

typedef struct {
    char empresa[31];
    int segundos;
    int veces;
} publicidad;

typedef struct {
    char nombre[101];
    int minutos_segmento;
    int segundos_segmento;
    int segmentos;
    int preferencia;
} show;


cancion canciones[1000];
publicidad publicidades[50];
show shows[15];


void cargar_Cancion(){
}

void cargar_Publicidad(){
}

void cargar_Show(){
}


int main(){

    FILE *archivo_Canciones = fopen("canciones.in", "r");
    FILE *archivo_Publicidad = fopen("publicidad.in", "r");
    FILE *archivo_Shows = fopen("shows.in", "r");

    if(archivo_Canciones == NULL){
        printf("Error:\nEl archivo no ha sido encontrado\n");
        return 1;
    }

    if(archivo_Publicidad == NULL){
        printf("Error:\nEl archivo no ha sido encontrado\n");
        fclose(archivo_Canciones);
        return 1;
    }

    if(archivo_Shows == NULL){
        printf("Error:\nEl archivo no ha sido encontrado\n");
        fclose(archivo_Canciones);
        fclose(archivo_Publicidad);
        return 1;
    }

    cargarCancion();
    cargarPublicidad();
    cargarShow();

    return 0;
}