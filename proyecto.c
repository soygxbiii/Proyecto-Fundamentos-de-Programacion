#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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
    int minutos;
    int segundos;
    int segmentos;
    int preferencia;
} show;

void cargarCanciones(FILE *e);
void cargarPublicidades(FILE *e);
void cargarShows(FILE *e);

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
        cargarPublicidades(archivo_Publicidad);
        cargarShows(archivo_Shows);
    }

    return 0;
}

void cargarCanciones(FILE *e){
    int totalCanciones;
    int i, j;
    char c;
    bool band = false;
    cancion miCancion;

    fscanf(e,"%d\n",&totalCanciones);
    printf("Total de canciones: %d\n",totalCanciones);

    for(i=1;i<=totalCanciones;i++){
        j = 0;
        while(!band){
            c = fgetc(e);
            if(j>=50){
                strcpy(miCancion.nombre, "El nombre es largo, el rango es de 1 - 50.");
                miCancion.minutos = 0;
                miCancion.segundos = 0;
                miCancion.puntuacion = 0;
                band = true;
                while((c = fgetc(e) != '\n') && (c != !feof)){

                }
            }else{
                if(!isdigit(c)){
                    miCancion.nombre[j] = c;
                }else{
                    band = true;
                    miCancion.nombre[j] = '\0';
                    ungetc(c,e);
                }
            }
            j++;
        }
            
        fscanf(e,"%d",&miCancion.minutos);
        fscanf(e,"%d",&miCancion.segundos);
        fscanf(e,"%d\n",&miCancion.puntuacion);
        printf("Cancion %d: nombre: %s, %d %d %d\n",i, miCancion.nombre, miCancion.minutos, miCancion.segundos, miCancion.puntuacion);
        band = false;
    }

    rewind(e);
}

void cargarPublicidades(FILE *e){
    int i, j;
    char c;
    bool band = false;
    publicidad miPublicidad;

    i = 1;

    while(!feof(e)){
        j = 0;
        while((!band) && (j<=31)){
            c = fgetc(e);
            if(!isdigit(c)){
                miPublicidad.empresa[j] = c;
                j++;
            }else{
                band = true;
                miPublicidad.empresa[j] = '\0';
                ungetc(c,e);
            }
        }

        fscanf(e, "%d", &miPublicidad.segundos);
        fscanf(e, "%d\n", &miPublicidad.veces);
        printf("Publicidad %d: empresa: %s, %d %d\n",i, miPublicidad.empresa, miPublicidad.segundos, miPublicidad.veces);
        band = false;
        i++;

    }

    rewind(e);
}

void cargarShows(FILE *e){
    int totalShows;
    int i, j;
    char c;
    bool band = false;
    show miShow;

    fscanf(e, "%d\n",&totalShows);
    printf("Total de Shows: %d\n",totalShows);

    if((totalShows >= 3) && (totalShows <= 15)){
        for(i=1;i<=totalShows;i++){
            j = 0;
            while((!band) && (j<=101)){
                c = fgetc(e);
                if(!isdigit(c)){
                    miShow.nombre[j] = c;
                    j++;
                }else{
                    band = true;
                    miShow.nombre[j] = '\0';
                    ungetc(c,e);
                }
            }

            fscanf(e, "%d", &miShow.minutos);
            fscanf(e, "%d", &miShow.segundos);
            fscanf(e, "%d", &miShow.segmentos);
            fscanf(e, "%d\n", &miShow.preferencia);
            printf("Show %d: nombre: %s, %d %d %d %d\n",i, miShow.nombre, miShow.minutos, miShow.segundos, miShow.segmentos, miShow.preferencia);
            band = false;
        }
    }else{
        printf("Cantidad de shows no permitios. Debe estar entre el rango de 3 - 15.\n");
    }

    rewind(e);

}
