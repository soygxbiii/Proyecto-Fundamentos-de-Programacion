#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//====== REGISTROS ======

typedef struct{
    char nombre[51];
    int minutos;
    int segundos;
    int puntuacion;
    int veces;
    int ultimaVez;
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

//grillas:
void imprimirArchivo(FILE *archivo, int *horas, int *minutos, int *segundos, char tipo, const char *nombreElemento);
int esHorarioEstelar(int hora);
int calcularPopularidad(int aleatorio);
void llevarTiempo(int *horas, int *minutos, int *segundos, int m, int s);
int escogerPublicidad(int totalP);
int escogerCancion(int totalC, int horas, int minutos, int segundos);
void generarGrilla(FILE *archivo, int *totalCanciones, int *totalPublicidades, int *totalShows);
void grillaGeneral(FILE *archivoEntrada, FILE *archivoSalida, int i);

//====== MAIN ======

int main(){

    FILE *archivoCanciones = fopen("canciones.in", "r");
    FILE *archivoPublicidades = fopen("publicidad.in", "r");
    FILE *archivoShows = fopen("shows.in", "r");
    FILE *archivoGeneral = fopen("grillas_semanal.out", "w+");
    FILE *totalGrillas(const char nombreSalida[]);

    srand(time(NULL));

    if(archivoCanciones == NULL){
        printf("Error: canciones.in no encontrado\n");
    }else if(archivoPublicidades == NULL){
        printf("Error: publicidad.in no encontrado\n");
    }else if(archivoShows == NULL){
        printf("Error: shows.in no encontrado\n");
    }else if(archivoGeneral == NULL){
        printf("Error: grillas_semanal.out no encontrado\n");
    }else{ 
        int totalCanciones = 0, totalPublicidades = 0, totalShows = 0;
        int i;

        //cargar datos de entrada (archivos de entrada).
        cargarCanciones(archivoCanciones, &totalCanciones);
        cargarPublicidades(archivoPublicidades, &totalPublicidades);
        cargarShows(archivoShows, &totalShows);

        //arreglo para crear los archivos de salida eficientemente: 
        char diasSemana[7][10] = {"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"};

        //generar las 7 grillas distintas para la semana:
        for(int i = 0; i < 7; i++)
        {
            int totalElementosGrilla = 0;
            FILE* archivoDia = totalGrillas(diasSemana[i]);
            if(archivoDia == NULL){
                printf("Error.\n");
            }else{
                generarGrilla(archivoDia, &totalCanciones, &totalPublicidades, &totalShows);
                grillaGeneral(archivoDia, archivoGeneral, i+1);
                fclose(archivoDia);
            }
        }
    }

    fclose(archivoCanciones);
    fclose(archivoPublicidades);
    fclose(archivoShows);
    fclose(archivoGeneral);

    return 0;
}

//====== FUNCIONES ======

void cargarCanciones(FILE *archivo, int *totalCanciones){
    int i, j, largoCadena=0, minutos, segundos, puntuacion;
    char c;
    int band = 0;
    char cadena[50];
    char auxNom[50];

    fscanf(archivo,"%d\n",totalCanciones);

    if((*totalCanciones > 0) && (*totalCanciones <= 1000))
    {
        for(i=0; i <= *totalCanciones; i++){
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
                        //printf("Error. El nombre es largo, la cancion %d NO se guardara en la grilla.\n", i+1);
                        while((c != '\n') && (c != EOF)){
                            c = fgetc(archivo);                        
                        }
                    }else{
                        ungetc(c, archivo);
                        fscanf(archivo,"%d",&minutos);
                        fscanf(archivo,"%d",&segundos);
                        if((segundos >= 0) && (segundos < 60)){
                            fscanf(archivo,"%d",&puntuacion);
                            if((puntuacion >= 1) && (puntuacion <= 100)){
                                strcpy(canciones[i].nombre, cadena);
                                canciones[i].minutos = minutos;
                                canciones[i].segundos = segundos;
                                canciones[i].puntuacion = puntuacion;
                                //printf("Cancion %d: %s %d %d %d\n",i+1, canciones[i].nombre, canciones[i].minutos, canciones[i].segundos, canciones[i].puntuacion);
                            }else{
                               // printf("Error: puntuacion invalida para la cancion %d, debe estar en el rango (1 - 100)\n", i+1);
                            }

                        }else{
                            printf("Error: segundos invalidos para la cancion %d\n", i+1);
                            while((c != '\n') && (c != EOF)){
                                c = fgetc(archivo);
                            }
                        }
                        c = fgetc(archivo);
                    }
                    band = 1;
                }
            }
            band = 0;
        }
    
        for(i = 0; i <= *totalCanciones; i++){
            largoCadena = strlen(canciones[i].nombre);
            if(largoCadena == 0){
                for(j = i; j <= *totalCanciones; j++){
                    strcpy(auxNom, canciones[j+1].nombre);
                    strcpy(canciones[j+1].nombre, canciones[j].nombre);
                    strcpy(canciones[j].nombre, auxNom);
                    canciones[j].minutos = canciones[j+1].minutos;
                    canciones[j].segundos = canciones[j+1].segundos;
                    canciones[j].puntuacion = canciones[j+1].puntuacion;
                }
                *totalCanciones = *totalCanciones - 1;
            }
        }
    
        //printf("cantidad de canciones: %d\n", *totalCanciones);

    }else{
        printf("Cantidad de canciones no permitidas. Debe estar dentro de un rango de 1 - 1000\n");
    }
    
    printf("\n");
}


void cargarPublicidades(FILE *archivo, int *totalPublicidades){
    int i = 0, j, largoCadena, contPublicidades=0;
    int segundos;
    char c;
    int band = 0;
    char cadena[31];

    while(!feof(archivo) && (i < 100)){
        j = 0;
        while((!band)){
            c = fgetc(archivo);
            if((!isdigit(c)) && (j<=30)){
                cadena[j] = c;
                j++;
            }else{
                band = 1;
                cadena[j] = '\0';
                largoCadena = strlen(cadena);
                if(largoCadena > 30){
                    //printf("Error. El nombre es largo, la publicidad %d NO se guardara en la grilla.\n", i+1);
                    while((c != '\n') && (c != EOF)){
                        c = fgetc(archivo);
                    };
                }else{
                    ungetc(c, archivo);
                    fscanf(archivo,"%d",&segundos);
                    if((segundos >= 0) && (segundos < 60)){
                        strcpy(publicidades[i].empresa, cadena);
                        publicidades[i].segundos = segundos;
                        fscanf(archivo,"%d",&publicidades[i].veces);
                        contPublicidades++;
                        //printf("Publicidad %d: Empresa: %s  Segundos: %d  Veces: %d\n",i+1, publicidades[i].empresa, publicidades[i].segundos, publicidades[i].veces);
                    }else{
                        //printf("Error: segundos invalidos para la publicidad %d\n", i+1);
                        while((c != '\n') && (c != EOF)){
                            c = fgetc(archivo);
                        }
                    }
                }
                c = fgetc(archivo);
            }
        }
        band = 0;
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
        }
    }
    
    *totalPublicidades = contPublicidades;
    //printf("cantidad de publicidades: %d\n", *totalPublicidades);
    //printf("\n");
}


void cargarShows(FILE *archivo, int *totalShows){
    int i, j, largoCadena, minutos, segundos, segmentos, preferencia;
    char c;
    int band = 0;
    char cadena[101];
    char auxNom[101];

    fscanf(archivo, "%d\n", totalShows);

    if((*totalShows >= 3) && (*totalShows <= 15)){
        for(i = 0; i <= *totalShows; i++){
            j = 0;
            while(!band){
                c = fgetc(archivo);
                if(!isdigit(c) && (j<=100)){
                    cadena[j] = c;
                    j++;
                }else{
                    cadena[j] = '\0';
                    largoCadena = strlen(cadena);
                    if(largoCadena > 100){
                        //printf("Error. El nombre es largo, el show %d NO se guardara en la grilla.\n", i+1);
                        while((c = fgetc(archivo)) != '\n' && c != EOF){

                        }
                    }else{
                        ungetc(c, archivo);
                        fscanf(archivo,"%d",&minutos);
                        fscanf(archivo,"%d",&segundos);
                        if((segundos >= 0) && (segundos < 60)){
                            fscanf(archivo,"%d",&segmentos);
                            fscanf(archivo,"%d",&preferencia);
                            if((preferencia >= 1) && (preferencia <= 10)){
                                strcpy(shows[i].nombre, cadena);
                                shows[i].minutos = minutos;
                                shows[i].segundos = segundos;
                                shows[i].segmentos = segmentos;
                                shows[i].preferencia = preferencia;
                                shows[i].veces = 0;
                                //printf("Show %d: %s %d %d %d %d\n",i+1, shows[i].nombre, shows[i].minutos, shows[i].segundos, shows[i].segmentos, shows[i].preferencia);
                            }else{
                                //printf("Error: preferencia invalida para la cancion %d, debe estar en el rango (1 - 10)\n", i+1);
                            }
                        }else{
                            //printf("Error: segundos invalidos para la cancion %d\n", i+1);
                            while((c != '\n') && (c != EOF)){
                                c = fgetc(archivo);
                            }
                        }
                        c = fgetc(archivo);
                    }
                    band = 1;
                }
            }
            band = 0;
        }

        for(i = 0; i <= *totalShows; i++){
            largoCadena = strlen(shows[i].nombre);
            if(largoCadena == 0){
                for(j = i; j <= *totalShows; j++){
                    strcpy(auxNom, shows[j+1].nombre);
                    strcpy(shows[j+1].nombre, shows[j].nombre);
                    strcpy(shows[j].nombre, auxNom);
                    shows[j].minutos = shows[j+1].minutos;
                    shows[j].segundos = shows[j+1].segundos;
                    shows[j].segmentos = shows[j+1].segmentos;
                    shows[j].preferencia = shows[j+1].preferencia;
                    shows[j].veces = shows[j+1].veces;
                }
                *totalShows = *totalShows -1;
            }
        }

        //printf("cantidad de shows: %d\n", *totalShows);
        //printf("\n");

    }else{
        printf("(Cantidad de shows no permitidos. Debe estar dentro de un rango de 3 - 15)\n");
        printf("\n");
    }
}

void imprimirArchivo(FILE *archivo, int *horas, int *minutos, int *segundos, char tipo, const char *nombreElemento){

    fprintf(archivo, "%02d %02d %02d %c %s\n", *horas, *minutos, *segundos, tipo, nombreElemento);
    /*fprintf(archivo, "%02d ", *horas);
    fprintf(archivo, "%02d ", *minutos);
    fprintf(archivo, "%02d ", *segundos);
    fprintf(archivo, "%c ", tipo);
    fprintf(archivo, "%s\n", nombreElemento);*/
    printf("%02d %02d %02d %c %s\n", *horas, *minutos, *segundos, tipo, nombreElemento);

}

int esHorarioEstelar(int hora){
    int bandera;

    if((hora >= 7) & (hora <= 9)){
        bandera = 1;
    }else if((hora >= 12) && (hora <= 14)){
        bandera = 1;
    }else if((hora >= 18) && (hora <= 19)){
        bandera = 1;
    }else{
        bandera = 0;
    }

    return bandera;
}

int calcularPopularidad(int aleatorio)
{
    int veces;
    int puntuacion = canciones[aleatorio].puntuacion;
    
    if(puntuacion >= 50){
        veces = 4;
    }else if((puntuacion >= 20) && (puntuacion < 50)){
        veces = 2;
    }else{
        veces = 1;
    }

    return veces;
}

void llevarTiempo(int *horas, int *minutos, int *segundos, int m, int s){
    *minutos = *minutos + m;
    *segundos = *segundos + s + 1;

    if(*segundos >= 60){
        *minutos = *minutos + 1;
        *segundos = *segundos - 60;
    }
        
    if(*minutos >= 60){
        *horas = *horas + 1;
        *minutos = *minutos - 60;
    }

}

int escogerPublicidad(int totalP){
    int i, resultado = 0;
    int bandera = 0;
    while(!bandera){
        i = (rand() % totalP) + 1;
        if(publicidades[i].vecesUsadas <= publicidades[i].veces){
            resultado = i;
            publicidades[i].vecesUsadas++;
            bandera = 1;
        }
    }
    return resultado;
}

int escogerCancion(int totalC, int horas, int minutos, int segundos){
    int bandera = 0, esDisponible;
    int i, popularidad, resultado = 0;
    int segundosTotales = horas * 3600 + minutos * 60 + segundos;
    int cuatroHoras = 14400;

    while(!bandera){
        i = (rand() % totalC) + 1;
        popularidad = calcularPopularidad(i);

        if(canciones[i].ultimaVez == -1){
           
            esDisponible = 1;
        } else {
        
            if(segundosTotales - canciones[i].ultimaVez >= cuatroHoras){
                esDisponible = 1;
            } else {
                esDisponible = 0;
            }
        }

        if((canciones[i].veces <= popularidad) && (esDisponible == 1)){
            resultado = i;
            canciones[i].veces++;
            canciones[i].ultimaVez = segundosTotales;
            bandera = 1;
        }
    }

    return resultado;
}

void generarGrilla(FILE *archivo, int *totalCanciones, int *totalPublicidades, int *totalShows){
    int horas = 0;
    int minutos = 5;
    int segundos = 0;
    int opcion, i, contShows = 0;
    int bandera, desabilitarOpcion3 = 0;
    int totalP = *totalPublicidades, totalC = *totalCanciones;
    char tipo = ' ';
    //fprintf(archivo, "%02d", minutos);

    for(i = 0; i <= *totalCanciones; i++){
        canciones[i].veces = 1;
        canciones[i].ultimaVez = -1;
    }

    for(i = 0; i <= *totalShows; i++){
        shows[i].veces = 1;
    }

    for(i = 0; i <= *totalPublicidades; i++){
        publicidades[i].vecesUsadas = 1;
    }

    while(horas <= 23 || (horas == 23 && minutos <= 59) || (horas == 23 && minutos == 59 && segundos <= 59)){
        bandera = esHorarioEstelar(horas);
        
        if(bandera){
            opcion = 3;
        }else{
            if(desabilitarOpcion3){
                opcion = (rand() % 2) + 1; //1 o 2
            }else{
                opcion = (rand() % 3) + 1; //1, 2 o 3
            }
        }

        switch(opcion){
            case 1:
                // Case Cancion
                tipo = 'C';
                i = escogerCancion(totalC, horas, minutos, segundos);
                imprimirArchivo(archivo, &horas, &minutos, &segundos, tipo, canciones[i].nombre);
                llevarTiempo(&horas, &minutos, &segundos, canciones[i].minutos, canciones[i].segundos);                  
            break;

            case 2:
                // Case Publicidad
                tipo = 'P';
                i = escogerPublicidad(totalP);
                imprimirArchivo(archivo, &horas, &minutos, &segundos, tipo, publicidades[i].empresa);
                llevarTiempo(&horas, &minutos, &segundos, 0, publicidades[i].segundos);
            break;

            case 3:
            // Case Show
            if(contShows <= *totalShows){ // ContShow: LLeva el control de los shows reproducidos, de manera que no se repita el mismo show en el dia.
                while(shows[contShows].veces <= shows[contShows].segmentos){
                    i = escogerCancion(totalC, horas, minutos, segundos);
                    imprimirArchivo(archivo, &horas, &minutos, &segundos, tipo = 'S', shows[contShows].nombre);
                    llevarTiempo(&horas, &minutos, &segundos, shows[contShows].minutos, shows[contShows].segundos);
                    imprimirArchivo(archivo, &horas, &minutos, &segundos, tipo = 'C', canciones[i].nombre);
                    llevarTiempo(&horas, &minutos, &segundos, canciones[i].minutos, canciones[i].segundos);
                    i = escogerPublicidad(totalP);
                    imprimirArchivo(archivo, &horas, &minutos, &segundos, tipo = 'P', publicidades[i].empresa);
                    llevarTiempo(&horas, &minutos, &segundos, 0, publicidades[i].segundos);
                    shows[contShows].veces = shows[contShows].veces + 1;
                }
                contShows++;
            }else{
                desabilitarOpcion3 = 1; // Si no hay mas shows por reproducir, el programa solo reproducira canciones y publicidades.
            }
        break;
    }  
}

}

void grillaGeneral(FILE *archivoEntrada, FILE *archivoSalida, int i){
    char linea[1000];

    rewind(archivoEntrada);
    while(fgets(linea, sizeof(linea),archivoEntrada) != NULL){
        fprintf(archivoSalida, "%d | %s", i, linea);
    }
}

FILE * totalGrillas(const char *nombreDia){
    //crea los archivos para las 7 grillas:
    char nombreSalida[25];
    sprintf(nombreSalida, "grilla_%s.out", nombreDia); //{"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"}
    FILE *archivoSalida = fopen(nombreSalida, "w");
    
    return archivoSalida;
}