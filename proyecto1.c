#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//====== REGISTROS ======

typedef struct{
    char nombre[51];
    int minutos;
    int segundos;
    int puntuacion;
    int duracionSegundos; //para avanzar el tiempo en la grilla
    int ultimaVez; //necesaria para saber si ya pasaron 4 horas
}cancion;


typedef struct{
    char empresa[31];
    int segundos; //para avanzar el tiempo en la grilla
    int veces; //el maximo de veces a repetir
    int vecesUsadas; //cuantas veces se ha repetido en la grilla
}publicidad;


typedef struct{
    char nombre[101];
    int minutos;
    int segundos;
    int segmentos; //el maximo de veces a repetir
    int preferencia;
    int duracionSegundos; //para avanzar el tiempo en la grilla
    int veces; //cuantas veces se ha repetido en la grilla
}show;


typedef struct{
    char nombre[101];
    char tipo;
    int horas;
    int minutos;
    int segundos;
}programacion; //registro para imprimir la grilla en los archivos

//====== CONSTANTES ======

#define MAX_CANCIONES 1000
#define MAX_PUBLICIDADES 100
#define MAX_SHOWS 15
#define MAX_GRILLA 2000


//====== ARREGLOS ======

cancion canciones[MAX_CANCIONES];
publicidad publicidades[MAX_PUBLICIDADES];
show shows[MAX_SHOWS];
programacion grilla[MAX_GRILLA];

//====== PROTOTIPADO ======

//leer archivos de entrada:
void cargarCanciones(FILE *archivo, int *totalCanciones);
void cargarPublicidades(FILE *archivo, int *totalPublicidades);
void cargarShows(FILE *archivo, int *totalShows);
void generarGrillaDiaria(char dia, *totalCanciones, *totalPublicidades, *totalShows);

int main(){
    FILE *archivoCanciones = fopen("canciones.in", "r");
    FILE *archivoPublicidades = fopen("publicidad.in", "r");
    FILE *archivoShows = fopen("shows.in", "r");

    if(archivoCanciones == NULL){
        printf("Error: canciones.in no encontrado\n");
    }else if(archivoPublicidades == NULL){
        printf("Error: publicidad.in no encontrado\n");
    }else if(archivoShows == NULL){
        printf("Error: shows.in no encontrado\n");
    }else{
        int totalCanciones = 0, totalPublicidades = 0, totalShows = 0;
        int i;
        //Arreglo para generar los archivos de salida.
        char dias[7][10] = {"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"};
        
        //cargar datos de entrada (archivos de entrada).
        cargarCanciones(archivoCanciones, &totalCanciones);
        cargarPublicidades(archivoPublicidades, &totalPublicidades);
        cargarShows(archivoShows, &totalShows);

        //Utilizaremos un ciclo para generar las grillas diarias.
        for(i=0; i<7; i++){
            generarGrillaDiaria(dias[i], &totalCanciones, &totalPublicidades, &totalShows);
        }
    }

    fclose(archivoCanciones);
    fclose(archivoPublicidades);
    fclose(archivoShows);

    return 0;
}

//====== FUNCIONES ======

void cargarCanciones(FILE *archivo, int *totalCanciones)
{
    int i, j, largoCadena=0, k=0;
    char c;
    bool band = false;
    char cadena[50];

    fscanf(archivo,"%d\n",totalCanciones);

    if((*totalCanciones > 0) && (*totalCanciones <= 1000))
    {
        for(i=0; i < *totalCanciones; i++){
            j = 0;
            while(!band){
                c = fgetc(archivo);
                if((!isdigit(c)) && j <= 50){
                    cadena[j] = c;
                    j++;
                }else{
                    cadena[j] = '\0';
                    largoCadena = strlen(cadena);
                    if(largoCadena > 50){
                        printf("Error. El nombre es largo, la cancion %d NO se guardara en la grilla.\n", i+1);
                        while((c != '\n') && (c != EOF)){
                            c = fgetc(archivo);                        
                        }
                    }else{
                        ungetc(c, archivo);
                        strcpy(canciones[i].nombre, cadena);
                        fscanf(archivo,"%d",&canciones[i].minutos);
                        fscanf(archivo,"%d",&canciones[i].segundos);
                        fscanf(archivo,"%d",&canciones[i].puntuacion);
                        c = fgetc(archivo);
                        printf("Cancion %d: %s %d %d %d\n",i+1, canciones[i].nombre, canciones[i].minutos, canciones[i].segundos, canciones[i].puntuacion);


                        /*if(canciones[i].segundos < 0 || canciones[i].segundos > 60)
                        {
                            printf("Error: segundos invalidos para la cancion %d\n", i+1);
                            while((c = fgetc(archivo)) != '\n' && c != EOF)
                            {

                            }
                            strcpy(canciones[i].nombre, "");
                        }else{
                            fscanf(archivo,"%d\n",&canciones[i].puntuacion);
                            if(canciones[i].puntuacion < 1 || canciones[i].puntuacion > 100)
                            {
                                printf("Error: puntuacion invalida para la cancion %d\n", i+1);
                                while((c = fgetc(archivo)) != '\n' && c != EOF)
                                {

                                }
                                strcpy(canciones[i].nombre, "");
                            }else{
                                printf("Cancion %d: Nombre: %s  Minutos: %d  Segundos: %d  Puntuacion: %d\n",i+1, canciones[i].nombre, canciones[i].minutos, canciones[i].segundos, canciones[i].puntuacion);
                                canciones[i].duracionSegundos = canciones[i].minutos * 60 + canciones[i].segundos;
                                printf("Duración en segundos: %d\n", canciones[i].duracionSegundos);
                                canciones[i].ultimaVez = -1;
                            }
                            
                        }*/
                    }
                    band = true;
                }
            }
            band = false;
        }
    
        for(i = 0; i < *totalCanciones; i++){
            largoCadena = strlen(canciones[i].nombre);
            if(largoCadena == 0){
                for(j = 0; j < *totalCanciones; j++){
                    strcpy(canciones[j+1].nombre, canciones[j].nombre);
                    canciones[j].minutos = canciones[j+1].minutos;
                    canciones[j].segundos = canciones[j+1].segundos;
                    canciones[j].puntuacion = canciones[j+1].puntuacion;
                }
                strcpy(canciones[j+1].nombre, "");
                k++;
            }
        }
    
        *totalCanciones = *totalCanciones - k;
        printf("cantidad de canciones: %d\n", *totalCanciones);

    }else{
        printf("Cantidad de canciones no permitidas. Debe estar dentro de un rango de 1 - 1000\n");
    }
    
    printf("\n");
}


void cargarPublicidades(FILE *archivo, int *totalPublicidades)
{
    int i = 0, j, k = 0, largoCadena, contPublicidades=0;
    char c;
    bool band = false;
    char cadena[31];

    while(!feof(archivo) && (i < 100)){
        j = 0;
        while((!band)){
            c = fgetc(archivo);
            if((!isdigit(c)) && (j<=30)){
                cadena[j] = c;
                j++;
            }else{
                band = true;
                cadena[j] = '\0';
                largoCadena = strlen(cadena);
                if(largoCadena > 30){
                    printf("Error. El nombre es largo, la publicidad %d NO se guardara en la grilla.\n", i+1);
                    while((c = fgetc(archivo)) != '\n' && c != EOF){

                    };
                }else{

                    strcpy(publicidades[i].empresa, cadena);
                    ungetc(c, archivo);
                    fscanf(archivo, "%d", &publicidades[i].segundos);
                    if(publicidades[i].segundos < 1 || publicidades[i].segundos > 60)
                    {
                        printf("Error: segundos invalidos para la publicidad %d\n", i+1);
                        while((c = fgetc(archivo)) != '\n' && c != EOF)
                        {

                        }
                        strcpy(publicidades[i].empresa, "");
                    }else{
                        fscanf(archivo, "%d\n", &publicidades[i].veces);
                        printf("Publicidad %d: Empresa: %s  Segundos: %d  Veces: %d\n",i+1, publicidades[i].empresa, publicidades[i].segundos, publicidades[i].veces);
                        publicidades[i].vecesUsadas = 0;
                    }
                }
            }
        }
        band = false;
        contPublicidades++;
        i++;
    }

    for(i = 0;i < contPublicidades; i++){
        largoCadena = strlen(publicidades[i].empresa);
        if(largoCadena == 0){
            for(j = 0; j < *totalPublicidades; j++){
                strcpy(publicidades[j+1].empresa, publicidades[j].empresa);
                publicidades[j].segundos = publicidades[j+1].segundos;
                publicidades[j].veces = publicidades[j+1].veces;
            }
            strcpy(publicidades[j+1].empresa, "");
            k++;
        }
    }
    
    *totalPublicidades = contPublicidades - k;
    printf("cantidad de publicidades: %d\n", *totalPublicidades);
    printf("\n");
}


void cargarShows(FILE *archivo, int *totalShows)
{
    int i, j, largoCadena, k=0;
    char c;
    bool band = false;
    char cadena[101];

    fscanf(archivo, "%d\n", totalShows);

    if((*totalShows >= 3) && (*totalShows <= 15)){
        for(i = 0; i < *totalShows; i++){
            j = 0;
            while(!band){
                c = fgetc(archivo);
                if(!isdigit(c) && (j<=100)){
                    cadena[j] = c;
                    j++;
                }else{
                    band = true;
                    largoCadena = strlen(cadena);
                    if(largoCadena > 100){
                        printf("Error. El nombre es largo, el show %d NO se guardara en la grilla.\n", i+1);
                        while((c = fgetc(archivo)) != '\n' && c != EOF){

                        };
                    }else{
                        cadena[j] = '\0';
                        strcpy(shows[i].nombre, cadena);
                        ungetc(c, archivo);
                        fscanf(archivo, "%d", &shows[i].minutos);
                        fscanf(archivo, "%d", &shows[i].segundos);
                        if(shows[i].segundos < 0 || shows[i].segundos > 60)
                        {
                            printf("Error: segundos invalidos para el show %d\n", i+1);
                            while((c = fgetc(archivo)) != '\n' && c != EOF)
                            {

                            }
                            strcpy(shows[i].nombre, "");
                        }else{
                            fscanf(archivo, "%d", &shows[i].segmentos);
                            fscanf(archivo, "%d\n", &shows[i].preferencia);
                            if(shows[i].preferencia < 1 || shows[i].preferencia > 60)
                            {
                                printf("Error: preferencia invalida para el show %d\n", i+1);
                                while((c = fgetc(archivo)) != '\n' && c != EOF)
                                {

                                }
                                strcpy(shows[i].nombre, "");
                            }else{
                                printf("Show %d: nombre: %s  Minutos: %d  Segundos: %d  Segmentos: %d  Preferencia: %d\n",i+1, shows[i].nombre, shows[i].minutos, shows[i].segundos, shows[i].segmentos, shows[i].preferencia);
                                shows[i].duracionSegundos = shows[i].minutos * 60 + shows[i].segundos; 
                                printf("Duración en segundos: %d\n", shows[i].duracionSegundos);  
                                shows[i].veces = 0;  
                            }
                        }   
                    }
                }
            }
            band = false;
        }

        int nuevoTotal = 0;
        for(i = 0; i < *totalShows; i++){
        if(strlen(shows[i].nombre) > 0){
            if(i != nuevoTotal){

                strcpy(shows[nuevoTotal].nombre, shows[i].nombre);
                shows[nuevoTotal].minutos = shows[i].minutos;
                shows[nuevoTotal].segundos = shows[i].segundos;
                shows[nuevoTotal].segmentos = shows[i].segmentos;
                shows[nuevoTotal].preferencia = shows[i].preferencia;
                shows[nuevoTotal].duracionSegundos = shows[i].duracionSegundos;
                shows[nuevoTotal].veces = shows[i].veces;
            }
            nuevoTotal ++;

        }
    
    }

        *totalShows = nuevoTotal;
        printf("cantidad de shows: %d\n", *totalShows);
        printf("\n");

    }else{
        printf("(Cantidad de shows no permitidos. Debe estar dentro de un rango de 3 - 15)\n");
        printf("\n");
    }
}

void generarGrillaDiaria(char *dia, *totalCanciones, *totalPublicidades, *totalShows){
    char nombreSalida[20];
}
