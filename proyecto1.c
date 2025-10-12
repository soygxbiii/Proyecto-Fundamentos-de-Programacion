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
            printf("Creando grilla para %s...\n\n", dias[i]);
            crearGrillaDia(archivoDia, totalCanciones, totalPublicidades, totalShows);
            fclose(archivoDia);


            fprintf(archivoSemanal, "DIA %d\n", i+1);
            char nombreArchivo[30];
            sprintf(nombreArchivo, "grilla_%s.out", dias[i]);
            FILE *archivoLectura = fopen(nombreArchivo, "r"); //vuelve a abrir los archivos de la grilla pero ahora en modo lectura
            
            if(archivoLectura != NULL){
                int c;
                while((c = fgetc(archivoLectura)) != EOF){
                    fputc(c, archivoSemanal);
                }
                fclose(archivoLectura);
            }
            fprintf(archivoSemanal, "\n");
        }
    }

    printf("------------- MENU -------------\n");
    printf("1. Consultar un momento determinado del dia.\n");
    printf("2. Consultar por Dia y hora especifico.\n");
    printf("3. Salir.\n");
    
    fclose(archivoCanciones);
    fclose(archivoPublicidades);
    fclose(archivoShows);
    fclose(archivoSemanal);
    
    printf("Todas las grillas se han creado exitosamente!\n");
    return 0;
}

//--------------FUNCIONES-------------------

void cargarCanciones(FILE *archivo, int *totalCanciones){
    int i, j, largoCadena=0, minutos, segundos, puntuacion;
    char c;
    int band = 0;
    char cadena[50];
    char auxNom[50];

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
                                c = fgetc(archivo);
                            }else{
                               // printf("Error: puntuacion invalida para la cancion %d, debe estar en el rango (1 - 100)\n", i+1);
                            }

                        }else{
                            printf("Error: segundos invalidos para la cancion %d\n", i+1);
                            while((c != '\n') && (c != EOF)){
                                c = fgetc(archivo);
                            }
                        }
                    }
                    band = 1;
                }
            }
            band = 0;
        }
    
        for(i = 0; i < *totalCanciones; i++){
            largoCadena = strlen(canciones[i].nombre);
            if(largoCadena == 0){
                for(j = i; j < *totalCanciones; j++){
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

    for(i = 0;i <= contPublicidades; i++){
        largoCadena = strlen(publicidades[i].empresa);
        if(largoCadena == 0){
            for(j = 0; j <= *totalPublicidades; j++){
                strcpy(publicidades[j+1].empresa, publicidades[j].empresa);
                publicidades[j].segundos = publicidades[j+1].segundos;
                publicidades[j].veces = publicidades[j+1].veces;
            }
        }
    }
    
    *totalPublicidades = contPublicidades;

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

    }else{
        printf("(Cantidad de shows no permitidos. Debe estar dentro de un rango de 3 - 15)\n");
        printf("\n");
    }
}


void imprimirEnArchivo(FILE *archivo, int horas, int minutos, int segundos, char tipo, char nombre[]){
    fprintf(archivo, "%02d %02d %02d %c %s\n", horas, minutos, segundos, tipo, nombre);
    printf("%02d %02d %02d %c %s\n", horas, minutos, segundos, tipo, nombre);

}

int esHorarioEstelar(int hora){
    int bandera = 0;
    if((hora >= 7 && hora <= 9) ||  (hora >= 12 && hora <= 14) || (hora >= 18 && hora <= 19)){
        bandera = 1;
    }

    return bandera;
}

int calcularPopularidad(int i){
    int puntuacion = canciones[i].puntuacion;
    int veces;
    
    if(puntuacion >= 50){
        veces = 5;
    }else if((puntuacion >= 20) && (puntuacion < 50)){
        veces = 3;
    }else if((puntuacion >= 10) && (puntuacion < 20)){
        veces = 2;
    }else{
        veces = 1;
    }

    return veces;
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
    int tiempoActual = horas * 3600 + minutos * 60 + segundos;
    int intentos = 0;
    int i, popularidad;
    
    // Primero buscar canciones que no se hayan usado hoy
    for(i = 0; i < totalCanciones; i++){
        if(canciones[i].veces == 0){
            canciones[i].veces++;
            canciones[i].ultimaVez = tiempoActual;
            return i;
        }
    }
    
    // Si todas se han usado, buscar las que pueden repetirse (cada 4 horas)
    while(intentos < totalCanciones){
        i = rand() % totalCanciones;
        popularidad = calcularPopularidad(i);
        
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
    int horas = 0;
    int minutos = 5;
    int segundos = 0;
    int contShows = 0;
    int bandera, i, opcion;
    
    // Reiniciar contadores
    for(i = 0; i < totalCanciones; i++){
        canciones[i].veces = 0;
        canciones[i].ultimaVez = -1;
    }
    for(i = 0; i < totalPublicidades; i++){
        publicidades[i].vecesUsadas = 0;
    }
    for(i = 0; i < totalShows; i++){
        shows[i].veces = 0;
    }
    
    fprintf(archivo, "--------------GRILLA DEL DIA-------------\n\n");
    //fprintf(archivo, "==============\n");
    
    while(horas < 24){        
        bandera = esHorarioEstelar(horas);


        if(bandera == 1){
            opcion = 3;
        }else{
            opcion = (rand() % 2) + 1; //1 o 2
        }
        
        switch(opcion){
            case 1: 
                i = elegirCancion(totalCanciones, horas, minutos, segundos);
                if(i != -1){
                    imprimirEnArchivo(archivo, horas, minutos, segundos, 'C', canciones[i].nombre);
                    avanzarTiempo(&horas, &minutos, &segundos, canciones[i].minutos, canciones[i].segundos);
                }
            break;
            
            case 2: 
                i = elegirPublicidad(totalPublicidades);
                if(i != -1){
                    imprimirEnArchivo(archivo, horas, minutos, segundos, 'P', publicidades[i].empresa);
                    avanzarTiempo(&horas, &minutos, &segundos, 0, publicidades[i].segundos);
                }
            break;
            
            case 3: 
                if(contShows < totalShows){
                    while(shows[contShows].veces < shows[contShows].segmentos){
                        // Mostrar el show
                        imprimirEnArchivo(archivo, horas, minutos, segundos, 'S', shows[contShows].nombre);
                        avanzarTiempo(&horas, &minutos, &segundos, shows[contShows].minutos, shows[contShows].segundos);
                        
                        // Canción después del show
                        i = elegirCancion(totalCanciones, horas, minutos, segundos);
                        if(i != -1){
                            imprimirEnArchivo(archivo, horas, minutos, segundos, 'C', canciones[i].nombre);
                            avanzarTiempo(&horas, &minutos, &segundos, canciones[i].minutos, canciones[i].segundos);
                        }
                        
                        // Publicidad después de la canción
                        i = elegirPublicidad(totalPublicidades);
                        if(i != -1){
                            imprimirEnArchivo(archivo, horas, minutos, segundos, 'P', publicidades[i].empresa);
                            avanzarTiempo(&horas, &minutos, &segundos, 0, publicidades[i].segundos);
                        }

                        shows[contShows].veces = shows[contShows].veces + 1;

                    }                    
                }
                contShows++;
            break;
        }
        
        // Pausa entre elementos (segundo de separacion).
        avanzarTiempo(&horas, &minutos, &segundos, 0, 1);
        
        // Verificar si hemos llegado al final del día
        if(horas >= 24 || (horas == 23 && minutos >= 59)){
            break;
        }
    }
    
    fprintf(archivo, "==============\n");
}

FILE* crearArchivoDia(char nombreDia[]){
    char nombreArchivo[30];
    sprintf(nombreArchivo, "grilla_%s.out", nombreDia);
    return fopen(nombreArchivo, "w");
}