//16-10-2025
//Abimar Roche    Gabriela Mora
//32197246        31541893
//Fundamentos de Programacion Sec. 06

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

void cargarCanciones(FILE *archivo, int *totalCanciones, cancion canciones[]);
void cargarPublicidades(FILE *archivo, int *totalPublicidades, publicidad publicidades[]);
void cargarShows(FILE *archivo, int *totalShows, show shows[]);

int esHorarioEstelar(int hora);
int calcularPopularidad(int indice, cancion canciones[]);
int elegirPublicidad(int totalPublicidades, publicidad publicidades[]);
int elegirCancion(int totalCanciones, cancion canciones[], int horas, int minutos, int segundos);

void imprimirEnArchivo(FILE *archivo, int horas, int minutos, int segundos, char tipo, char nombre[]);
void avanzarTiempo(int *horas, int *minutos, int *segundos, int minutos_add, int segundos_add);
void crearGrillaDia(FILE *archivo, int totalCanciones, int totalPublicidades, int totalShows, cancion canciones[], publicidad publicidades[], show shows[]);
FILE* crearArchivoDia(char nombreDia[]);

void momentoDia(FILE *archivo, int opcionHora);
void buscarCancion(FILE *archivo, char nombreCancion[]);

//====== MAIN ======

int main()
{
    FILE *archivoCanciones = fopen("canciones.in", "r");
    FILE *archivoPublicidades = fopen("publicidad.in", "r");
    FILE *archivoShows = fopen("shows.in", "r");
    FILE *archivoSemanal = fopen("grillas_semanal.out", "w");
    
    srand(time(NULL));
    
    if(archivoCanciones == NULL){
        printf("Error: canciones.in no encontrado\n");
    }else if(archivoPublicidades == NULL){
        printf("Error: publicidad.in no encontrado\n");
    }else if(archivoShows == NULL){
        printf("Error: shows.in no encontrado\n");
    }else{

        int opcion, subOpcion, i;
        int totalCanciones = 0, totalPublicidades = 0, totalShows = 0;
        char nombreArchivo[30];
        char dias[7][10] = {"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"};

        cargarCanciones(archivoCanciones, &totalCanciones, canciones);
        cargarPublicidades(archivoPublicidades, &totalPublicidades, publicidades);
        cargarShows(archivoShows, &totalShows, shows);
        
        for(i = 0; i < 7; i++)
        {
            FILE *archivoDia = crearArchivoDia(dias[i]);

            if(archivoDia != NULL)
            {
                crearGrillaDia(archivoDia, totalCanciones, totalPublicidades, totalShows, canciones, publicidades, shows);
                fclose(archivoDia);
    
                fprintf(archivoSemanal, "DIA %d\n", i+1);
                sprintf(nombreArchivo, "grilla_%s.out", dias[i]);
                FILE *archivoLectura = fopen(nombreArchivo, "r"); //vuelve a abrir los archivos de la grilla pero ahora en modo lectura
                
                if(archivoLectura != NULL)
                {
                    int c;
                    while((c = fgetc(archivoLectura)) != EOF)
                    {
                        fputc(c, archivoSemanal);
                    }
                    fclose(archivoLectura);
                }

                fprintf(archivoSemanal, "\n");
            }
        }
    
        printf("\n\t--------------------------------------------");
        printf("\n\t|            MENÚ DE CONSULTAS:            |\n");
        printf("\t| 1. Elegir dia de la semana               |\n");
        printf("\t| 2. Salir                                 |\n");
        printf("\t--------------------------------------------\n");
    
        printf("\nEscoge una opcion: ");
        scanf("\n%d", &opcion);
    
        switch(opcion)
        {
            case 1:
                printf("\t--------------------------------------------");
                printf("\n\t|         ELEGIR DIA DE LA SEMANA:         |\n");
                printf("\t| 1. Lunes                                 |\n");
                printf("\t| 2. Martes                                |\n");
                printf("\t| 3. Miercoles                             |\n");
                printf("\t| 4. Jueves                                |\n");
                printf("\t| 5. Viernes                               |\n");
                printf("\t| 6. Sabado                                |\n");
                printf("\t| 7. Domingo                               |\n");
                printf("\t--------------------------------------------\n");
    
    
                do{
                    printf("\nEscoge una opcion: ");
                    scanf("\n%d", &i);

                    if(!(i >= 1 && i <= 7))
                    {
                        printf("Error: Ingrese un número valido\n");
                    }

                }while(i < 1 || i > 7);
    
                sprintf(nombreArchivo, "grilla_%s.out", dias[i-1]);
                FILE *archivoLectura = fopen(nombreArchivo, "r"); //vuelve a abrir los archivos de la grilla pero ahora en modo lectura
    
                do{
                    printf("\t--------------------------------------------");
                    printf("\n\t|               CONSULTAR:                 |\n");
                    printf("\t| 1. Mostrar por pantalla                  |\n");
                    printf("\t| 2. Momento determinado del dia           |\n");
                    printf("\t| 3. Buscar cancion                        |\n");
                    printf("\t| 4. Salir                                 |\n");
                    printf("\t--------------------------------------------\n");
            
                    printf("\nEscoge una opcion: ");
                    scanf("\n%d", &subOpcion);

                    switch(subOpcion){
                        case 1:
                            rewind(archivoLectura);

                            printf("\nCargando...\n");
                            if(archivoLectura != NULL)
                            {
                                int c;
                                while((c = fgetc(archivoLectura)) != EOF)
                                {
                                    putchar(c);
                                }
                                printf("¡Generada exitosamente la grilla del dia %s!\n\n", dias[i-1]);
                            }
                        break;
        
                        case 2:
                            printf("\nIngrese solo la hora, debe estar entre 00 y 23: ");
                            int opcionHora;
        
                            do
                            {
                                scanf("%d", &opcionHora);
                                if(opcionHora < 0 || opcionHora > 23)
                                {
                                    printf("Error: La hora debe estar entre 00 y 23: ");
                                }
                            }while(opcionHora < 0 || opcionHora > 23);
        
                            printf("\nCargando...\n");
                            printf("--- Programación desde las %02d:00 ---\n", opcionHora);
    
                            if(archivoLectura != NULL)
                            {
                                momentoDia(archivoLectura, opcionHora);
                                printf("\n");
                            }
                        break;
                        
                        case 3:
                            printf("\nIngrese el nombre de la canción a buscar: ");
                            char nombreCancion[100];

                            getchar();
                            fgets(nombreCancion, sizeof(nombreCancion), stdin);
                            nombreCancion[strcspn(nombreCancion, "\n")] = 0;
    
                            printf("Buscando '%s'...\n\n", nombreCancion);
    
                            if(archivoLectura != NULL)
                            {
                                buscarCancion(archivoLectura, nombreCancion);
                                printf("\n");
                            }
                        break;
                        
                        case 4:
                        printf("Saliendo...\n¡Gracias por usar el programa!\n");
                        break;
        
                        default:
                        printf("Opcion no reconocida, vuelva a intentarlo.\n");
                        break;
                    }
                }while(subOpcion != 4);
                break;
    
            case 2:
                printf("Saliendo...\n¡Gracias por usar el programa!\n");
            break;
    
            default:
                printf("Opcion no reconocida, vuelva a intentarlo.\n");
            break;
        }
    }

    fclose(archivoCanciones);
    fclose(archivoPublicidades);
    fclose(archivoShows);
    fclose(archivoSemanal);

    return 0;
}

//--------------FUNCIONES-------------------

void cargarCanciones(FILE *archivo, int *totalCanciones, cancion canciones[])
{
    int i, j, minutos, segundos, puntuacion;
    int band = 0, largoCadena = 0 ;
    char c;
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
    
    }else{
        printf("Cantidad de canciones no permitidas. Debe estar dentro de un rango de 1 - 1000\n");
    }
    
}


void cargarPublicidades(FILE *archivo, int *totalPublicidades, publicidad publicidades[])
{
    int i = 0, band = 0, contPublicidades = 0;
    int segundos, j, largoCadena;
    char c;
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


void cargarShows(FILE *archivo, int *totalShows, show shows[])
{
    int i, j, largoCadena, minutos, segundos, segmentos, preferencia;
    int band = 0;
    char c;
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
    }
}


int esHorarioEstelar(int hora)
{
    int bandera = 0;

    if((hora >= 7 && hora <= 9) ||  (hora >= 12 && hora <= 14) || (hora >= 18 && hora <= 19))
    {
        bandera = 1;
    }

    return bandera;
}

int calcularPopularidad(int i, cancion canciones[])
{
    int puntuacion = canciones[i].puntuacion;
    int veces;
    
    if(puntuacion >= 50)
    {
        veces = 5;
    }
    
    if((puntuacion >= 20) && (puntuacion < 50))
    {
        veces = 3;
    }
    
    if((puntuacion >= 10) && (puntuacion < 20))
    {
        veces = 2;
    }
    
    if(puntuacion < 10)
    {
        veces = 1;
    }

    return veces;
}

int elegirPublicidad(int totalPublicidades, publicidad publicidades[])
{
    int bandera = -1;

    for(int intentos = 0; intentos < 10; intentos++)
    {
        int i = rand() % totalPublicidades;
        if(publicidades[i].vecesUsadas < publicidades[i].veces)
        {
            publicidades[i].vecesUsadas++;
            bandera = i;
        }
    }
    
    return bandera;
}

int elegirCancion(int totalCanciones, cancion canciones[], int horas, int minutos, int segundos)
{
    int tiempoActual = horas * 3600 + minutos * 60 + segundos;
    int bandera = -1;
    
    // Elegir una canción aleatoria
    int i = rand() % totalCanciones;
    int popularidad = calcularPopularidad(i, canciones);
    
    // Verificar si se puede repetir (usando tu lógica)
    if(canciones[i].ultimaVez == -1 || (tiempoActual - canciones[i].ultimaVez) >= 14400)
    {
        
        // Verificar límite de popularidad
        if(canciones[i].veces < popularidad)
        {
            canciones[i].veces++;
            canciones[i].ultimaVez = tiempoActual;
            bandera = i;
        }
    }
    
    return bandera;
}


void imprimirEnArchivo(FILE *archivo, int horas, int minutos, int segundos, char tipo, char nombre[])
{
    fprintf(archivo, "%02d %02d %02d %c %s\n", horas, minutos, segundos, tipo, nombre);
}

void avanzarTiempo(int *horas, int *minutos, int *segundos, int minutos_add, int segundos_add)
{
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

void crearGrillaDia(FILE *archivo, int totalCanciones, int totalPublicidades, int totalShows, cancion canciones[], publicidad publicidades[], show shows[])
{
    int horas = 0;
    int minutos = 5;
    int segundos = 0;
    int contShows = 0;
    int bandera, i, opcion;
    
    // Reiniciar contadores
    for(i = 0; i < totalCanciones; i++)
    {
        canciones[i].veces = 0;
        canciones[i].ultimaVez = -1;
    }

    for(i = 0; i < totalPublicidades; i++)
    {
        publicidades[i].vecesUsadas = 0;
    }

    for(i = 0; i < totalShows; i++)
    {
        shows[i].veces = 0;
    }
    
    fprintf(archivo, "=============== GRILLA DEL DIA ===============\n");
    
    while(horas < 24)
    {        
        bandera = esHorarioEstelar(horas);


        if(bandera == 1){
            opcion = 3;
        }else{
            opcion = (rand() % 2) + 1; //1 o 2
        }
        
        switch(opcion)
        {
            case 1: 
                i = elegirCancion(totalCanciones, canciones, horas, minutos, segundos);
                if(i != -1){
                    imprimirEnArchivo(archivo, horas, minutos, segundos, 'C', canciones[i].nombre);
                    avanzarTiempo(&horas, &minutos, &segundos, canciones[i].minutos, canciones[i].segundos);
                }
            break;
            
            case 2: 
                i = elegirPublicidad(totalPublicidades, publicidades);
                if(i != -1){
                    imprimirEnArchivo(archivo, horas, minutos, segundos, 'P', publicidades[i].empresa);
                    avanzarTiempo(&horas, &minutos, &segundos, 0, publicidades[i].segundos);
                }
            break;
            
            case 3: 
                if(contShows < totalShows)
                {
                    while(shows[contShows].veces < shows[contShows].segmentos){
                        // Mostrar el show
                        imprimirEnArchivo(archivo, horas, minutos, segundos, 'S', shows[contShows].nombre);
                        avanzarTiempo(&horas, &minutos, &segundos, shows[contShows].minutos, shows[contShows].segundos);
                        
                        // Canción después del show
                        i = elegirCancion(totalCanciones, canciones, horas, minutos, segundos);
                        if(i != -1){
                            imprimirEnArchivo(archivo, horas, minutos, segundos, 'C', canciones[i].nombre);
                            avanzarTiempo(&horas, &minutos, &segundos, canciones[i].minutos, canciones[i].segundos);
                        }
                        
                        // Publicidad después de la canción
                        i = elegirPublicidad(totalPublicidades, publicidades);
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
    
    fprintf(archivo, "==============================================\n");
}

FILE* crearArchivoDia(char nombreDia[])
{
    char nombreArchivo[30];
    sprintf(nombreArchivo, "grilla_%s.out", nombreDia);
    return fopen(nombreArchivo, "w");
}

void momentoDia(FILE *archivo, int opcionHora)
{
    int c;
    int imprimir = 0;

    rewind(archivo);

    while((c = fgetc(archivo)) != EOF)
    {
        if(!imprimir)
        {
            int d1 = c;
            int d2 = fgetc(archivo);
            int hora = (d1 - '0') * 10 + (d2 - '0');
            
            if(hora == opcionHora)
            {
                imprimir = 1;
                putchar(d1);
                putchar(d2);
            }else{
                while(fgetc(archivo) != '\n' && c != EOF)
                {

                }
            }
        }else{
            putchar(c);
            if(c == '\n')
            {
                int p1 = fgetc(archivo);
                int p2 = fgetc(archivo);
                int proxHora = (p1 - '0') * 10 + (p2 - '0');
                
                // Si la hora cambia, desactivar impresión
                if(proxHora != opcionHora)
                {
                    imprimir = 0;
                }else{
                    putchar(p1);
                    putchar(p2);
                }
            }
        }
    }
}

void buscarCancion(FILE *archivo, char nombreCancion[])
{
    char linea[200];
    int encontrada = 0;

    rewind(archivo);

    while(fgets(linea, sizeof(linea), archivo) != NULL)
    {
        if(strstr(linea, nombreCancion) != NULL)
        {
            encontrada = 1;
            printf("%s", linea);
        }
    }
    
    if(!encontrada)
    {
        printf("No se encontró la canción '%s'\n", nombreCancion);
    }
}