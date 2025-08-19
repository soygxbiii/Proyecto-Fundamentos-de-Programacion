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

Cancion canciones[1000];
Publicidad publicidades[50];
Show shows[15];

void cargar_canciones(){
}

void cargar_publicidad(){
}

int main(){
    cargar_canciones();
    cargar_publicidad();
    
    return 0;
}