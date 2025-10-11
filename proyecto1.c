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
int calcularPopularidad(int indice);
void avanzarTiempo(int *horas, int *minutos, int *segundos, int minutos_add, int segundos_add);
int elegirPublicidad(int totalPublicidades);
int elegirCancion(int totalCanciones, int horas, int minutos, int segundos);
void crearGrillaDia(FILE *archivo, int totalCanciones, int totalPublicidades, int totalShows);
void crearArchivoSemanal(FILE *archivoEntrada, FILE *archivoSalida, int dia);
FILE* crearArchivoDia(char nombreDia[]);

//====== MAIN ======

int main(){
    FILE *archivoCanciones = fopen("canciones.in", "r");
    FILE *archivoPublicidades = fopen("publicidad.in", "r");
    FILE *archivoShows = fopen("shows.in", "r");
    FILE *archivoSemanal = fopen("grillas_semanal.out", "w");
    
    srand(time(NULL));
    
    if(archivoCanciones == NULL){
        printf("Error: No se pudo abrir canciones.in\n");
        return 1;
    }
    if(archivoPublicidades == NULL){
        printf("Error: No se pudo abrir publicidad.in\n");
        fclose(archivoCanciones);
        return 1;
    }
    if(archivoShows == NULL){
        printf("Error: No se pudo abrir shows.in\n");
        fclose(archivoCanciones);
        fclose(archivoPublicidades);
        return 1;
    }
    if(archivoSemanal == NULL){
        printf("Error: No se pudo crear grillas_semanal.out\n");
        fclose(archivoCanciones);
        fclose(archivoPublicidades);
        fclose(archivoShows);
        return 1;
    }
    
    int totalCanciones = 0, totalPublicidades = 0, totalShows = 0;
    
    cargarCanciones(archivoCanciones, &totalCanciones);
    cargarPublicidades(archivoPublicidades, &totalPublicidades);
    cargarShows(archivoShows, &totalShows);
    
    printf("Datos cargados:\n");
    printf("- Canciones: %d\n", totalCanciones);
    printf("- Publicidades: %d\n", totalPublicidades);
    printf("- Shows: %d\n", totalShows);
    
    char dias[7][10] = {"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"};
    
    for(int i = 0; i < 7; i++){
        FILE *archivoDia = crearArchivoDia(dias[i]);
        if(archivoDia != NULL){
            printf("Creando grilla para %s...\n", dias[i]);
            crearGrillaDia(archivoDia, totalCanciones, totalPublicidades, totalShows);
            crearArchivoSemanal(archivoDia, archivoSemanal, i+1);
            fclose(archivoDia);
            printf("\n");
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

int elegirCancion(int totalCanciones, int horas, int minutos, int segundos){
    if(totalCanciones == 0) return -1;
    
    int tiempoActual = horas * 3600 + minutos * 60 + segundos;
    int intentos = 0;
    
    // Primero buscar canciones que no se hayan usado hoy
    for(int i = 0; i < totalCanciones; i++){
        if(canciones[i].veces == 0){
            canciones[i].veces++;
            canciones[i].ultimaVez = tiempoActual;
            return i;
        }
    }
    
    // Si todas se han usado, buscar las que pueden repetirse (cada 4 horas)
    while(intentos < 100){
        int i = rand() % totalCanciones;
        int popularidad = calcularPopularidad(i);
        
        // Verificar si la canción puede repetirse (cada 4 horas)
        int puedeRepetirse = 1;
        if(canciones[i].ultimaVez != -1){
            int tiempoDesdeUltima = tiempoActual - canciones[i].ultimaVez;
            if(tiempoDesdeUltima < 14400){ // 4 horas en segundos (4 * 60 * 60 = 14400)
                puedeRepetirse = 0;
            }
        }
        
        // Verificar límites de repetición según popularidad
        if(puedeRepetirse && canciones[i].veces < popularidad){
            canciones[i].veces++;
            canciones[i].ultimaVez = tiempoActual;
            return i;
        }
        
        intentos++;
    }
    
    // Si después de muchos intentos no encuentra canción, buscar cualquier canción que pueda repetirse
    for(int i = 0; i < totalCanciones; i++){
        int puedeRepetirse = 1;
        if(canciones[i].ultimaVez != -1){
            int tiempoDesdeUltima = tiempoActual - canciones[i].ultimaVez;
            if(tiempoDesdeUltima < 14400){
                puedeRepetirse = 0;
            }
        }
        
        if(puedeRepetirse){
            canciones[i].veces++;
            canciones[i].ultimaVez = tiempoActual;
            return i;
        }
    }
    
    // Si no hay canciones disponibles, devolver una aleatoria
    return rand() % totalCanciones;
}

void crearGrillaDia(FILE *archivo, int totalCanciones, int totalPublicidades, int totalShows){
    int horas = 5;
    int minutos = 0;
    int segundos = 0;
    int showsUsados = 0;
    int bandera;
    
    // Reiniciar contadores
    for(int i = 0; i < totalCanciones; i++){
        canciones[i].veces = 0;
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
            case 1: {
                int cancionIdx = elegirCancion(totalCanciones, horas, minutos, segundos);
                if(cancionIdx != -1){
                    imprimirEnArchivo(archivo, horas, minutos, segundos, 'C', canciones[cancionIdx].nombre);
                    avanzarTiempo(&horas, &minutos, &segundos, canciones[cancionIdx].minutos, canciones[cancionIdx].segundos);
                }
                break;
            }
            
            case 2: {
                int publicidadIdx = elegirPublicidad(totalPublicidades);
                if(publicidadIdx != -1){
                    imprimirEnArchivo(archivo, horas, minutos, segundos, 'P', publicidades[publicidadIdx].empresa);
                    avanzarTiempo(&horas, &minutos, &segundos, 0, publicidades[publicidadIdx].segundos);
                }
                break;
            }
            
            case 3: {
                if(showsUsados < totalShows){
                    // Mostrar el show
                    imprimirEnArchivo(archivo, horas, minutos, segundos, 'S', shows[showsUsados].nombre);
                    avanzarTiempo(&horas, &minutos, &segundos, shows[showsUsados].minutos, shows[showsUsados].segundos);
                    
                    // Canción después del show
                    int cancionIdx = elegirCancion(totalCanciones, horas, minutos, segundos);
                    if(cancionIdx != -1){
                        imprimirEnArchivo(archivo, horas, minutos, segundos, 'C', canciones[cancionIdx].nombre);
                        avanzarTiempo(&horas, &minutos, &segundos, canciones[cancionIdx].minutos, canciones[cancionIdx].segundos);
                    }
                    
                    // Publicidad después de la canción
                    int publicidadIdx = elegirPublicidad(totalPublicidades);
                    if(publicidadIdx != -1){
                        imprimirEnArchivo(archivo, horas, minutos, segundos, 'P', publicidades[publicidadIdx].empresa);
                        avanzarTiempo(&horas, &minutos, &segundos, 0, publicidades[publicidadIdx].segundos);
                    }
                    
                    showsUsados++;
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

FILE* crearArchivoDia(char nombreDia[]){
    char nombreArchivo[30];
    sprintf(nombreArchivo, "grilla_%s.out", nombreDia);
    return fopen(nombreArchivo, "w");
}