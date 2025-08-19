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

void cargarCanciones(FILE *e);

/*cancion canciones[1000];
publicidad publicidades[50];
show shows[15];*/


int main(){

    FILE *archivo_Canciones = fopen("canciones.in", "r");
    FILE *archivo_Publicidad = fopen("publicidad.in", "r");
    FILE *archivo_Shows = fopen("shows.in", "r");

    if(archivo_Canciones == NULL){
        printf("Error:\nEl archivo canciones.in no ha sido encontrado\n");
    }else if(archivo_Publicidad == NULL){
        printf("Error:\nEl archivo publicidad.in no ha sido encontrado\n");
        fclose(archivo_Canciones);
    }else if(archivo_Shows == NULL){
        printf("Error:\nEl archivo shows.in no ha sido encontrado\n");
        fclose(archivo_Canciones);
        fclose(archivo_Publicidad);
    }else{
        cargarCanciones(archivo_Canciones);
        //cargarPublicidad();
        //cargarShow();


    }

    return 0;
}

void cargarCanciones(FILE *e){
    int totalCanciones;
    int i;
    cancion miCancion;

    fscanf(e,"%d",&totalCanciones);
    printf("Total de canciones: %d\n",totalCanciones);

    for(i=1;i<=totalCanciones;i++){
        fscanf(e,"%s",miCancion.nombre);
        fscanf(e,"%d",&miCancion.minutos);
        fscanf(e,"%d",&miCancion.segundos);
        fscanf(e,"%d",&miCancion.puntuacion);
        printf("Cancion %d: Nombre: %s, %d %d %d\n",i,miCancion.nombre,miCancion.minutos,miCancion.segundos,miCancion.puntuacion);
    }

    rewind(e);
}