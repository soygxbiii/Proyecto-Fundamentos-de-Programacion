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
    int veces;
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

//grillas:
//void totalGrillas(const char nombreSalida[]);
void imprimirArchivo(FILE *archivo, int *horas, int *minutos, int *segundos, char *tipo, const char *nombreElemento);
bool esHorarioEstelar(int hora);
int popularidad(int aleatorio);
void generarGrilla(FILE *archivo, int *totalCanciones, int *totalPublicidades, int *totalShows);
FILE* totalGrillas(const char nombreSalida[]);

//====== MAIN ======

int main(){
    FILE *archivoCanciones = fopen("canciones.in", "r");
    FILE *archivoPublicidades = fopen("publicidad.in", "r");
    FILE *archivoShows = fopen("shows.in", "r");
    srand(time(NULL));

    if(archivoCanciones == NULL){
        printf("Error: canciones.in no encontrado\n");
    }else if(archivoPublicidades == NULL){
        printf("Error: publicidad.in no encontrado\n");
    }else if(archivoShows == NULL){
        printf("Error: shows.in no encontrado\n");
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
                //fprintf(archivoDia, "%d", i);
                fclose(archivoDia);

            }
        }
    }

    fclose(archivoCanciones);
    fclose(archivoPublicidades);
    fclose(archivoShows);

    return 0;
}

//====== FUNCIONES ======

void cargarCanciones(FILE *archivo, int *totalCanciones){
    int i, j, largoCadena=0, minutos, segundos, puntuacion;
    char c;
    bool band = false;
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
                        printf("Error. El nombre es largo, la cancion %d NO se guardara en la grilla.\n", i+1);
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
                                printf("Cancion %d: %s %d %d %d\n",i+1, canciones[i].nombre, canciones[i].minutos, canciones[i].segundos, canciones[i].puntuacion);
                            }else{
                                printf("Error: puntuacion invalida para la cancion %d, debe estar en el rango (1 - 100)\n", i+1);
                            }

                        }else{
                            printf("Error: segundos invalidos para la cancion %d\n", i+1);
                            while((c != '\n') && (c != EOF)){
                                c = fgetc(archivo);
                            }
                        }
                        c = fgetc(archivo);
                    }
                    band = true;
                }
            }
            band = false;
        }
    
        for(i = 0; i <= *totalCanciones; i++){
            largoCadena = strlen(canciones[i].nombre);
            if(largoCadena == 0){
                printf("I: %d\n",i);
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
    
        printf("cantidad de canciones: %d\n", *totalCanciones);

    }else{
        printf("Cantidad de canciones no permitidas. Debe estar dentro de un rango de 1 - 1000\n");
    }
    
    printf("\n");
}


void cargarPublicidades(FILE *archivo, int *totalPublicidades){
    int i = 0, j, largoCadena, contPublicidades=0;
    int segundos;
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
                        printf("Publicidad %d: Empresa: %s  Segundos: %d  Veces: %d\n",i+1, publicidades[i].empresa, publicidades[i].segundos, publicidades[i].veces);
                    }else{
                        printf("Error: segundos invalidos para la publicidad %d\n", i+1);
                        while((c != '\n') && (c != EOF)){
                                c = fgetc(archivo);
                        }
                    }
                }
                c = fgetc(archivo);
            }
        }
        band = false;
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
    printf("cantidad de publicidades: %d\n", *totalPublicidades);
    printf("\n");
}


void cargarShows(FILE *archivo, int *totalShows){
    int i, j, largoCadena, minutos, segundos, segmentos, preferencia;
    char c;
    bool band = false;
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
                        printf("Error. El nombre es largo, el show %d NO se guardara en la grilla.\n", i+1);
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
                                printf("Show %d: %s %d %d %d %d\n",i+1, shows[i].nombre, shows[i].minutos, shows[i].segundos, shows[i].segmentos, shows[i].preferencia);
                            }else{
                                printf("Error: preferencia invalida para la cancion %d, debe estar en el rango (1 - 10)\n", i+1);
                            }
                        }else{
                            printf("Error: segundos invalidos para la cancion %d\n", i+1);
                            while((c != '\n') && (c != EOF)){
                                c = fgetc(archivo);
                            }
                        }
                        c = fgetc(archivo);
                    }
                    band = true;
                }
            }
            band = false;
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

        printf("cantidad de shows: %d\n", *totalShows);
        printf("\n");

    }else{
        printf("(Cantidad de shows no permitidos. Debe estar dentro de un rango de 3 - 15)\n");
        printf("\n");
    }
}

void imprimirArchivo(FILE *archivo, int *horas, int *minutos, int *segundos, char *tipo, const char *nombreElemento){
    fprintf(archivo, "%02d ", *horas);
    fprintf(archivo, "%02d ", *minutos);
    fprintf(archivo, "%02d ", *segundos);
    fprintf(archivo, "%c ", *tipo);
    fprintf(archivo, "%s", nombreElemento);
}

bool esHorarioEstelar(int hora){
    bool bandera;

    if((hora >= 7) & (hora <= 9)){
        bandera = true;
    }else if((hora >= 12) && (hora <= 14)){
        bandera = true;
    }else if((hora >= 18) && (hora <= 19)){
        bandera = true;
    }else{
        bandera = false;
    }

    return bandera;
}

int popularidad(int aleatorio)
{
    int veces;
    int puntuacion = canciones[aleatorio].puntuacion;
    
    if(puntuacion >= 90){
        veces = 7;
    }else{
        if(puntuacion >= 80){
        veces = 6;
        }else{
            if(puntuacion >= 70){
                veces = 5;
                }else{
                    if(puntuacion >= 60){
                        veces = 4;
                    }else{
                        if (puntuacion >= 50){
                            veces = 3;
                        }else{
                            if (puntuacion >= 40){
                            veces = 2;
                        }else{
                            veces = 1;
                        }
                    }
                }
            }
        }
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

void generarGrilla(FILE *archivo, int *totalCanciones, int *totalPublicidades, int *totalShows){
    int horas = 0;
    int minutos = 5;
    int segundos = 0;
    int opcion, i, popularidad;
    bool bandera;
    bool desabilitarOpcion1 = false;
    //bool desabilitarOpcion2 = false;
    bool desabilitarOpcion3 = false;
    char tipo = '';
    fprintf(archivo, "%02d", minutos);

    while((horas != 23) || (minutos != 59) || (segundos != 59)){
        bandera = esHorarioEstelar(horas);
        
        if(bandera){
            opcion = 3;
        }else{
            if(desabilitarOpcion3){
                opcion = (rand() % 2) + 1; //1 o 2
            }else{
                opcion = (rand() % 3) + 1; //1, 2 o 3
            }
            /*if(desabilitarOpcion1 && desabilitarOpcion2){
                opcion = 3; //publicidad
            }else{
                if(desabilitarOpcion1 && desabilitarOpcion3){
                    opcion = 2; //canciones
                }else{
                    if(desabilitarOpcion2 && desabilitarOpcion3){
                        opcion = 1; //shows
                    }else{
                        if(desabilitarOpcion1){
                            opcion = (rand() % 2) + 2; //2 o 3
                            //rand() % 2 genera: 0 o 1, entonces... rand() % 2) + 2 genera: 0 + 1: 2 y 1 + 2: 3
                        }else{
                            if(desabilitarOpcion2){
                                opcion = (rand() % 2) * 2 + 1; //1 o 3
                                //rand() % 2 genera 0 o 1, entonces... (rand() % 2) * 2 genera: 0 * 2: 0 y 1 * 2: 2
                                //entonces...  ((rand() % 2) * 1) + 2 genera: 0 + 1: 1 y 2 + 1: 3
                            }else{
                                if(desabilitarOpcion3){
                                    opcion = (rand() % 2) + 1; //1 o 2
                                    //rand() % 2 genera 0 o 1, entonces... (rand() % 2) + 1 genera: 0 + 1: 1 y 1+1: 1
                                }else{
                                    opcion = (rand() % 3) + 1; //1, 2 o 3
                                }
                            }
                        }
                    }
                }
            }*/
        }

        switch(opcion){
            case 1:
                // Case Cancion
                tipo = 'C';
                i = (rand() % *totalCanciones) + 1;
                popularidad = popularidad(i);
                if(canciones[i].veces <= popularidad){
                    imprimirArchivo(archivo, &horas, &minutos, &segundos, &tipo, canciones[i].nombre);
                    canciones[i].veces = canciones[i].veces + 1;
                }else{
                    i = 0;
                    while((i <= *totalCanciones) && (canciones[i].veces >= popularidad)){
                        i++;
                    }

                    if(i <= *totalCanciones){
                        imprimirArchivo(archivo, &horas, &minutos, &segundos, &tipo, canciones[i].nombre);
                    }else{
                        desabilitarOpcion2 = true;
                    }
                }
                minutos = minutos + canciones[i].minutos;
                segundos = segundos + canciones[i].segundos + 1;
            break;

            case 2:
                // Case Publicidad
                tipo = 'C';
                i = (rand() % *totalPublicidades) + 1;
                if(publicidades[i].vecesUsadas <= publicidades[i].veces){
                    imprimirArchivo(archivo, &horas, &minutos, &segundos, &tipo, publicidades[i].nombre);
                    publicidades[i].vecesUsadas++;
                }else{
                    i = 0;
                    while((i <= *totalPublicidades) && (publicidades[i].vecesUsadas >= publicidades[i].veces)){
                        i++;
                    }

                    if(i <= *totalPublicidades){
                        imprimirArchivo(archivo, &horas, &minutos, &segundos, &tipo, publicidades[i].nombre);
                    }else{
                        desabilitarOpcion3 = true;
                    }
                    
                }
                segundos = segundos + publicidades[i].segundos + 1;
            break;

            case 3:
                // Case Show
                tipo = 'S';
                i = (rand() % *totalShows) + 1;
                if(shows[i].veces <= shows[i].segmentos){
                    imprimirArchivo(archivo, &horas, &minutos, &segundos, &tipo, shows[i].nombre);
                    llevarTiempo(&horas, &minutos, &segundos, shows[i].minutos, shows[i].segundos);
                    tipo = 'C';
                    imprimirArchivo(archivo, &horas, &minutos, &segundos, &tipo, canciones[i].nombre);
                    llevarTiempo(&horas, &minutos, &segundos, canciones[i].minutos, canciones[i].segundos);
                    shows[i].veces = shows[i].veces + 1;
                }else{
                    i = 0;
                    while((i <= *totalShows) && (shows[i].veces >= shows[i].segmentos)){
                        i++;
                    }
                    if(i <= *totalShows){
                        imprimirArchivo(archivo, &horas, &minutos, &segundos, &tipo, shows[i].nombre);
                        llevarTiempo(&horas, &minutos, &segundos, shows[i].minutos, shows[i].segundos);
                        tipo = 'C';
                        imprimirArchivo(archivo, &horas, &minutos, &segundos, &tipo, canciones[i].nombre);
                        llevarTiempo(&horas, &minutos, &segundos, canciones[i].minutos, canciones[i].segundos);
                    }else{
                        desabilitarOpcion1 = true;
                    } 
                }
            break;
        }  
    }



}

FILE* totalGrillas(const char *nombreDia){
    //crea los archivos para las 7 grillas:
    char nombreSalida[25];
    sprintf(nombreSalida, "grilla_%s.out", nombreDia); //{"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"}
    FILE *archivoSalida = fopen(nombreSalida, "w");
    
    return archivoSalida;
}
