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
    int ultimaHora; //necesaria para saber si ya pasaron 4 horas
    int ultimoM; // Complementa si ya pasaron las 4 horas
}cancion;

typedef struct{
    char empresa[31];
    int segundos;
    int veces;
    int vecesUsadas;
}publicidad;

typedef struct{
    char nombre[101];
    int minutos;
    int segundos;
    int segmentos;
    int preferencia;
    int veces;
}show;

//====== CONSTANTES ======

#define MAX_CANCIONES 1000
#define MAX_PUBLICIDADES 100
#define MAX_SHOWS 15

//====== ARREGLOS ======

cancion canciones[MAX_CANCIONES];
publicidad publicidades[MAX_PUBLICIDADES];
show shows[MAX_SHOWS];

//====== PROTOTIPADO ======

void cargarCanciones(FILE *archivo, int *totalCanciones);
void cargarPublicidades(FILE *archivo, int *totalPublicidades);
void cargarShows(FILE *archivo, int *totalShows);
void imprimirEnArchivo(FILE *archivo, int horas, int minutos, int segundos, char tipo, char nombre[]);
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
    fclose(archivoSemanal);
    
    printf("Todas las grillas se han creado exitosamente!\n");
    return 0;
}

//====== FUNCIONES ======

void cargarCanciones(FILE *archivo, int *totalCanciones){
    fscanf(archivo, "%d", totalCanciones);
    
    for(int i = 0; i < *totalCanciones; i++){
        char nombre[51];
        int minutos, segundos, puntuacion;
        
        // Leer nombre hasta encontrar un dígito
        int j = 0;
        char c;
        while((c = fgetc(archivo)) != EOF){
            if(isdigit(c)){
                ungetc(c, archivo);
                break;
            }
            if(j < 50 && c != '\n'){
                nombre[j++] = c;
            }
        }
        if(j > 0 && nombre[j-1] == ' '){
            j--;
        }
        nombre[j] = '\0';
        
        fscanf(archivo, "%d %d %d", &minutos, &segundos, &puntuacion);
        
        strcpy(canciones[i].nombre, nombre);
        canciones[i].minutos = minutos;
        canciones[i].segundos = segundos;
        canciones[i].puntuacion = puntuacion;
        canciones[i].veces = 0;
        canciones[i].ultimaVez = -1;
    }
}

void cargarPublicidades(FILE *archivo, int *totalPublicidades){
    *totalPublicidades = 0;
    char linea[100];
    
    while(fgets(linea, sizeof(linea), archivo) != NULL && *totalPublicidades < MAX_PUBLICIDADES){
        char empresa[31];
        int segundos, veces;
        
        if(sscanf(linea, "%30[^0-9] %d %d", empresa, &segundos, &veces) == 3){
            // Quitar espacios al final del nombre
            int len = strlen(empresa);
            while(len > 0 && empresa[len-1] == ' '){
                empresa[len-1] = '\0';
                len--;
            }
            
            strcpy(publicidades[*totalPublicidades].empresa, empresa);
            publicidades[*totalPublicidades].segundos = segundos;
            publicidades[*totalPublicidades].veces = veces;
            publicidades[*totalPublicidades].vecesUsadas = 0;
            (*totalPublicidades)++;
        }
    }
}

void cargarShows(FILE *archivo, int *totalShows){
    fscanf(archivo, "%d", totalShows);
    
    for(int i = 0; i < *totalShows; i++){
        char nombre[101];
        int minutos, segundos, segmentos, preferencia;
        
        // Leer nombre hasta encontrar un dígito
        int j = 0;
        char c;
        while((c = fgetc(archivo)) != EOF){
            if(isdigit(c)){
                ungetc(c, archivo);
                break;
            }
            if(j < 100 && c != '\n'){
                nombre[j++] = c;
            }
        }
        if(j > 0 && nombre[j-1] == ' '){
            j--;
        }
        nombre[j] = '\0';
        
        fscanf(archivo, "%d %d %d %d", &minutos, &segundos, &segmentos, &preferencia);
        
        strcpy(shows[i].nombre, nombre);
        shows[i].minutos = minutos;
        shows[i].segundos = segundos;
        shows[i].segmentos = segmentos;
        shows[i].preferencia = preferencia;
        shows[i].veces = 0;
    }
}

void imprimirEnArchivo(FILE *archivo, int horas, int minutos, int segundos, char tipo, char nombre[]){
    fprintf(archivo, "%02d %02d %02d %c %s\n", horas, minutos, segundos, tipo, nombre);
    printf("%02d %02d %02d %c %s\n", horas, minutos, segundos, tipo, nombre);

}

int esHorarioEstelar(int hora){
    if((hora >= 7 && hora <= 9) || 
       (hora >= 12 && hora <= 14) || 
       (hora >= 18 && hora <= 19)){
        return 1;
    }
    return 0;
}

int calcularPopularidad(int indice){
    int puntuacion = canciones[indice].puntuacion;
    
    if(puntuacion >= 80) return 5;
    else if(puntuacion >= 60) return 4;
    else if(puntuacion >= 40) return 3;
    else if(puntuacion >= 20) return 2;
    else return 1;
}

void avanzarTiempo(int *horas, int *minutos, int *segundos, int minutos_add, int segundos_add){
    *segundos += segundos_add;
    *minutos += minutos_add;
    
    if(*segundos >= 60){
        *minutos += *segundos / 60;
        *segundos = *segundos % 60;
    }
    
    if(*minutos >= 60){
        *horas += *minutos / 60;
        *minutos = *minutos % 60;
    }
    
    if(*horas >= 24){
        *horas = 23;
        *minutos = 59;
        *segundos = 59;
    }
}

int elegirPublicidad(int totalPublicidades){
    if(totalPublicidades == 0) return -1;
    
    for(int intentos = 0; intentos < 10; intentos++){
        int i = rand() % totalPublicidades;
        if(publicidades[i].vecesUsadas < publicidades[i].veces){
            publicidades[i].vecesUsadas++;
            return i;
        }
    }
    
    return rand() % totalPublicidades;
}

int escogerCancion(int totalC, int horas, int minutos, int segundos){
    int bandera = 0, esDisponible;
    int i, popularidad, resultado = 0;
    int segundosTotales = horas * 3600 + minutos * 60 + segundos;
    int cuatroHoras = 14400;

    while(!bandera){
        i = (rand() % totalC);
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
    
    // Si no hay canciones disponibles, devolver una aleatoria
    return rand() % totalCanciones;
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
    for(int i = 0; i < totalPublicidades; i++){
        publicidades[i].vecesUsadas = 0;
    }
    for(int i = 0; i < totalShows; i++){
        shows[i].veces = 0;
    }
    
    fprintf(archivo, "GRILLA DEL DIA\n");
    fprintf(archivo, "==============\n");
    
    while(horas < 24){
        int opcion;
        
        bandera = esHorarioEstelar(horas);

        if(bandera){
            opcion = 3;
        }else if((horas >= 6) && (horas < 21 )){
            opcion = (rand() % 3) + 1; //1, 2 o 3
        }else{
            opcion = (rand() % 2) + 1; //1 o 2
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
                break;
            }
        }
        
        // Pequeña pausa entre elementos
        avanzarTiempo(&horas, &minutos, &segundos, 0, 1);
        
        // Verificar si hemos llegado al final del día
        if(horas >= 24 || (horas == 23 && minutos >= 59)){
            break;
        }
    }
    
    fprintf(archivo, "==============\n");
    fprintf(archivo, "FIN DE GRILLA\n");
}

void crearArchivoSemanal(FILE *archivoEntrada, FILE *archivoSalida, int dia){
    char linea[200];
    
    fseek(archivoEntrada, 0, SEEK_SET);
    
    fprintf(archivoSalida, "=== DIA %d ===\n", dia);
    while(fgets(linea, sizeof(linea), archivoEntrada) != NULL){
        fprintf(archivoSalida, "%s", linea);
    }
    fprintf(archivoSalida, "\n");
}

FILE * totalGrillas(const char *nombreDia){
    //crea los archivos para las 7 grillas:
    char nombreSalida[25];
    sprintf(nombreSalida, "grilla_%s.out", nombreDia); //{"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"}
    FILE *archivoSalida = fopen(nombreSalida, "w");
    
    return archivoSalida;
}
