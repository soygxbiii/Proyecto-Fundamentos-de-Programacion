#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct{
    char nombre[51];
    int minutos;
    int segundos;
    int puntuacion;
}cancion;

typedef struct{
    char empresa[31];
    int segundos;
    int veces;
}publicidad;

typedef struct{
    char nombre[101];
    int minutos;
    int segundos;
    int segmentos;
    int preferencia;
}show;

// cancion canciones[1000];
// publicidad publicidades[50];
// show shows[15];

void cargarCanciones(FILE *archivo);
void cargarPublicidades(FILE *archivo);
void cargarShows(FILE *archivo);
// bool es_horario_estelar(int segundos_totales);

void segundos_a_horas(int segundos_totales, int *horas, int *minutos, int *segundos);

int main(){

    int segundos_totales, horas, minutos, segundos, opcion;
    segundos_totales = 0;

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

        int opcion;
        do{

            printf("\t--------------------------------------------");
            printf("\n\t|            MENÚ DE CONSULTAS:            |\n");
            printf("\t| 1. Consultar momento determinado del dia |\n");
            printf("\t| 2. Salir                                 |\n");
            printf("\t--------------------------------------------\n");

            scanf("\n%d", &opcion);

            if(opcion == 1){
            // consultar_momentoDeterminado(no se como hacerlo jafdjjsksaskd); 
            }
        }while(opcion != 2);
        printf("\n\t\t¡Gracias por usar el programa!\n");
    return 0;


    }

    fclose(archivo_Canciones); 
    fclose(archivo_Publicidad);
    fclose(archivo_Shows);
    return 0;
}

void cargarCanciones(FILE *archivo){
    int totalCanciones;
    int i, j;
    char c;
    bool band = false;
    cancion miCancion;

    printf("\n");
    fscanf(archivo,"%d\n",&totalCanciones);
    printf("Total de canciones: %d\n",totalCanciones);

    for(i = 1; i <= totalCanciones; i++){
        j = 0;
        while(!band){
            c = fgetc(archivo);
            if(j >= 50){
                strcpy(miCancion.nombre, "(Error: debe estar dentro de un rango de 1-50)");
                miCancion.minutos = 0;
                miCancion.segundos = 0;
                miCancion.puntuacion = 0;
                band = true;
                while((c = fgetc(archivo) != '\n') && (c != EOF)){

                }
            }else{
                if(!isdigit(c)){
                    miCancion.nombre[j] = c;
                }else{
                    band = true;
                    miCancion.nombre[j] = '\0';
                    ungetc(c, archivo);
                }
            }
            j++;
        }
            
        fscanf(archivo,"%d",&miCancion.minutos);
        fscanf(archivo,"%d",&miCancion.segundos);
        fscanf(archivo,"%d\n",&miCancion.puntuacion);
        printf("Cancion %d: Nombre: %s  Minutos: %d  Segundos: %d  Puntuacion: %d\n",i, miCancion.nombre, miCancion.minutos, miCancion.segundos, miCancion.puntuacion);
        band = false;
    }

    printf("\n");
    rewind(archivo);
}

void cargarPublicidades(FILE *archivo){
    int i, j;
    char c;
    bool band = false;
    publicidad miPublicidad;

    i = 1;

    printf("\n");
    while(!feof(archivo)){
        j = 0;
        while((!band) && (j <= 31)){
            c = fgetc(archivo);
            if(!isdigit(c)){
                miPublicidad.empresa[j] = c;
                j++;
            }else{
                band = true;
                miPublicidad.empresa[j] = '\0';
                ungetc(c, archivo);
            }
        }

        fscanf(archivo, "%d", &miPublicidad.segundos);
        fscanf(archivo, "%d\n", &miPublicidad.veces);
        printf("Publicidad %d: Empresa: %s  Segundos: %d  Veces: %d\n",i, miPublicidad.empresa, miPublicidad.segundos, miPublicidad.veces);
        band = false;
        i++;

    }

    printf("\n");
    rewind(archivo);
}

void cargarShows(FILE *archivo){
    int totalShows;
    int i, j;
    char c;
    bool band = false;
    show miShow;

    printf("\n");
    fscanf(archivo, "%d\n",&totalShows);

    if((totalShows >= 3) && (totalShows <= 15)){
        printf("Total de Shows: %d\n",totalShows);
        for(i = 1; i <= totalShows; i++){
            j = 0;
            while((!band) && (j <= 101)){
                c = fgetc(archivo);
                if(!isdigit(c)){
                    miShow.nombre[j] = c;
                    j++;
                }else{
                    band = true;
                    miShow.nombre[j] = '\0';
                    ungetc(c, archivo);
                }
            }

            fscanf(archivo, "%d", &miShow.minutos);
            fscanf(archivo, "%d", &miShow.segundos);
            fscanf(archivo, "%d", &miShow.segmentos);
            fscanf(archivo, "%d\n", &miShow.preferencia);
            printf("Show %d: nombre: %s  Minutos: %d  Segundos: %d  Segmentos: %d  Preferencia: %d\n",i, miShow.nombre, miShow.minutos, miShow.segundos, miShow.segmentos, miShow.preferencia);
            band = false;
        }

        printf("\n");
        rewind(archivo);

    }else{
        printf("(Cantidad de shows no permitidos. Debe estar dentro de un rango de 3 - 15)\n");
        printf("\n");

    }

    rewind(archivo);

}

//esto es para imprimir la grilla usando HH MM SS, pero preferiblemente usemos segundos_totales para programar
/*void segundos_a_hora(int segundos_totales, int *horas, int *minutos, int *segundos){
    *horas = segundos_totales / 3600;
    *minutos = (segundos_totales % 3600) / 60;
    *segundos = segundos_totales % 60;
}

bool es_horario_estelar(int segundos_totales){
    return (segundos_totales >= 25200 && segundos_totales <= 32400) || (segundos_totales >= 43200 && segundos_totales <= 50400) || (segundos_totales >= 64800 && segundos_totales <= 68400);
    // 25200 a 32400 es 7:00 AM - 9:00 AM
    // 43200 a 50400 es 12:00 PM - 2:00 PM 
    // 64800 a 68400 es 06:00 PM - 07:00 PM
}*/