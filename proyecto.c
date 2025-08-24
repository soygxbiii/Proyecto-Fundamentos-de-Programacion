#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//====== REGISTROS ======

typedef struct{
    char nombre[51];
    int minutos;
    int segundos;
    int puntuacion;
    int duracion_en_segundos; //para avanzar el tiempo en la grilla
    int ultima_vez; //necesaria para saber si ya pasaron 4 horas
}cancion;


typedef struct{
    char empresa[31];
    int segundos; //para avanzar el tiempo en la grilla
    int veces; //el maximo de veces a repetir
    int veces_usadas; //cuantas veces se ha repetido en la grilla
}publicidad;


typedef struct{
    char nombre[101];
    int minutos;
    int segundos;
    int segmentos; //el maximo de veces a repetir
    int preferencia;
    int duracion_en_segundos; //para avanzar el tiempo en la grilla
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
#define MAX_PUBLICIDADES  100
#define MAX_SHOWS  15


//====== ARREGLOS ======

cancion canciones[MAX_CANCIONES];
publicidad publicidades[MAX_PUBLICIDADES];
show shows[MAX_SHOWS];
programacion grilla[2000];


//====== PROTOTIPADO ======

//leer archivos de entrada:
void cargarCanciones(FILE *archivo, int *totalCanciones);
void cargarPublicidades(FILE *archivo, int *totalPublicidades);
void cargarShows(FILE *archivo, int *totalShows);

//validaciones:
void segundos_a_horas(int segundos_totales, int *horas, int *minutos, int *segundos);
bool es_horario_estelar(int segundos_totales);
bool se_puede_repetir_cancion(int aleatorio, int segundos_totales);

//grillas:
void generar_grilla(int *totalCanciones, int *totalPublicidades, int *totalShows, int *total_elementos_grilla);
void imprimir_en_archivo(const char nombre_salida[], FILE *archivo, int total_elementos_grilla);
void total_grillas(const char dias_semana[], FILE *archivo_Canciones, FILE *archivo_Publicidad, FILE *archivo_Shows);


//====== MAIN ======

int main()
{
    int total_elementos_grilla = 0;
    int segundos_totales = 0;
    int opcion;
    int totalCanciones = 0, totalPublicidades = 0, totalShows = 0;

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
        
        //con esto cada grilla será diferente:
        srand(time(NULL)); 

        //arreglo para crear los archivos de salida eficientemente: 
        char dias_semana[7][10] = {"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"};

        //generar las 7 grillas distintas para la semana:
        for(int i = 0; i < 7; i++) {
            total_grillas(dias_semana[i],archivo_Canciones, archivo_Publicidad, archivo_Shows);
        }

        for(int i = 0; i < totalPublicidades; i++){
            printf("intentos: %s %d\n", publicidades[i].empresa, publicidades[i].veces_usadas);
        }
        
        int opcion;
        do{

            printf("\t--------------------------------------------");
            printf("\n\t|            MENÚ DE CONSULTAS:            |\n");
            printf("\t| 1. Consultar momento determinado del dia |\n");
            printf("\t| 2. Salir                                 |\n");
            printf("\t--------------------------------------------\n");

            printf("\nElige una opcion: ");
            scanf("\n%d", &opcion);

            switch(opcion)
            {
                case 1:
                    printf("Opcion 1.\n");
                    
                break;

                case 2:
                    printf("Saliendo...\n");
                    printf("¡Gracias por usar el programa!\n");
                break;

                default:
                    printf("Opcion no reconocida, vuelve a intentarlo.\n");
                break;
            }
        }while(opcion != 2);
    }
    
    return 0;
}


//====== FUNCIONES ======

void cargarCanciones(FILE *archivo, int *totalCanciones)
{
    int i, j, largoCadena, k=0;
    char c;
    bool band = false;
    char cadena[50];

    //printf("\n");
    fscanf(archivo,"%d\n",totalCanciones);
    //printf("Total de canciones: %d\n", *totalCanciones);

    for(i=0; i < *totalCanciones; i++){
        j = 0;
        while(!band){
            c = fgetc(archivo);
            if((!isdigit(c)) && j <= 50){
                cadena[j] = c;
                j++;
            }else{
                band = true;
                largoCadena = strlen(cadena);
                if(largoCadena > 50){
                    //printf("Error. El nombre es largo, debe estar en el rango de (1 - 50)\n");
                    //printf("Por lo tanto, la cancion %d NO se guardara en la grilla.\n", i+1);
                    while((c = fgetc(archivo)) != '\n' && c != EOF){

                    };
                }else{
                    cadena[j] = '\0';
                    strcpy(canciones[i].nombre, cadena);
                    ungetc(c, archivo);
                    fscanf(archivo,"%d",&canciones[i].minutos);
                    fscanf(archivo,"%d",&canciones[i].segundos);
                    fscanf(archivo,"%d\n",&canciones[i].puntuacion);
                    //printf("Cancion %d: Nombre: %s  Minutos: %d  Segundos: %d  Puntuacion: %d\n",i+1, canciones[i].nombre, canciones[i].minutos, canciones[i].segundos, canciones[i].puntuacion);
                    canciones[i].duracion_en_segundos = canciones[i].minutos * 60 + canciones[i].segundos;
                    canciones[i].ultima_vez = -1;
                }
            }
        }
        band = false;
    }

    for(i = 0; i < *totalCanciones; i++){
        largoCadena = strlen(canciones[i].nombre);
        if(largoCadena == 0){
            for(j = i; j < *totalCanciones; j++){
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
    
    //printf("\n");
}


void cargarPublicidades(FILE *archivo, int *totalPublicidades)
{
    int i = 0, j, largoCadena, contPublicidades=0;
    char c;
    bool band = false;
    char cadena[31];

    //printf("\n");

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
                    //printf("Error. El nombre es largo, debe estar en el rango de (1 - 30)\n");
                    //printf("Por lo tanto, la publicidad %d NO se guardara en la grilla.\n", i+1);
                    while((c = fgetc(archivo)) != '\n' && c != EOF){

                    };
                }else{

                    strcpy(publicidades[i].empresa, cadena);
                    ungetc(c, archivo);
                    fscanf(archivo, "%d", &publicidades[i].segundos);
                    fscanf(archivo, "%d\n", &publicidades[i].veces);
                    //printf("Publicidad %d: Empresa: %s  Segundos: %d  Veces: %d\n",i+1, publicidades[i].empresa, publicidades[i].segundos, publicidades[i].veces);
                    publicidades[i].veces_usadas = 0;
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
        }
    }
    
    *totalPublicidades = contPublicidades;
    // printf("cantidad de publicidades: %d\n", *totalPublicidades);
    //printf("\n");
}


void cargarShows(FILE *archivo, int *totalShows)
{
    int i, j, largoCadena, k=0;
    char c;
    bool band = false;
    char cadena[101];

    //printf("\n");
    fscanf(archivo, "%d\n", totalShows);

    if((*totalShows >= 3) && (*totalShows <= 15)){
        //printf("Total de Shows: %d\n",*totalShows);
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
                    //printf("Largo cadena %d, %d\n", i, largoCadena);
                    if(largoCadena > 100){
                        //printf("Error. El nombre es largo, debe estar en el rango de (1 - 100)\n");
                        //printf("Por lo tanto, el show %d NO se guardara en la grilla.\n", i+1);
                        while((c = fgetc(archivo)) != '\n' && c != EOF){

                        };
                    }else{
                        cadena[j] = '\0';
                        strcpy(shows[i].nombre, cadena);
                        fscanf(archivo, "%d", &shows[i].minutos);
                        fscanf(archivo, "%d", &shows[i].segundos);
                        fscanf(archivo, "%d", &shows[i].segmentos);
                        fscanf(archivo, "%d\n", &shows[i].preferencia);
                        //printf("Show %d: nombre: %s  Minutos: %d  Segundos: %d  Segmentos: %d  Preferencia: %d\n",i+1, shows[i].nombre, shows[i].minutos, shows[i].segundos, shows[i].segmentos, shows[i].preferencia);
                        shows[i].duracion_en_segundos = shows[i].minutos * 60 + shows[i].segundos;   
                        shows[i].veces = 0;     
 
                    }
                    
                }
            }
            band = false;
        }
    }else{
        //printf("(Cantidad de shows no permitidos. Debe estar dentro de un rango de 3 - 15)\n");
        //printf("\n");
    }

    for(i = 0; i < *totalShows; i++){
        largoCadena = strlen(shows[i].nombre);
        if(largoCadena ==  0){
            for(j = i; j < *totalShows; j++){
            strcpy(shows[j+1].nombre, shows[j].nombre);
            shows[j].minutos = shows[j+1].minutos;
            shows[j].segundos = shows[j+1].segundos;
            shows[j].segmentos = shows[j+1].segmentos;
            shows[j].preferencia = shows[j+1].preferencia;
            }
        strcpy(shows[j+1].nombre, "");
        k++;
        }
    }

    *totalShows = *totalShows - k;

    //printf("\n");
}

void segundos_a_horas(int segundos_totales, int *horas, int *minutos, int *segundos)
{
    *horas = segundos_totales / 3600;
    *minutos = (segundos_totales % 3600) / 60;
    *segundos = segundos_totales % 60;
}


bool es_horario_estelar(int segundos_totales)
{
    bool si_es = false;

    if((segundos_totales >= 25200 && segundos_totales <= 32400) || (segundos_totales >= 43200 && segundos_totales <= 50400) || (segundos_totales >= 64800 && segundos_totales <= 68400))
    {
        si_es = true;
    }

    return (si_es);

    // 25200 a 32400 es 7:00 AM - 9:00 AM
    // 43200 a 50400 es 12:00 PM - 2:00 PM 
    // 64800 a 68400 es 06:00 PM - 07:00 PM
}

bool se_puede_repetir_cancion(int aleatorio, int segundos_totales)
{
    int cuatroHoras = 14400;

    //-1 significa que la cancion nunca se ha puesto:
    if(canciones[aleatorio].ultima_vez == -1)
    {
        return true;
    }
    
    //si han pasado 4 horas o mas desde la ultima reproduccion:
    if(segundos_totales - canciones[aleatorio].ultima_vez >= cuatroHoras)
    {
        return true;
    }

    return false;
}


void generar_grilla(int *totalCanciones, int *totalPublicidades, int *totalShows, int *total_elementos_grilla)
{
    int segundos_totales = 300; //inicia a las 00:05:00
    int horas, minutos, segundos;
    bool elemento_programado;
    int opcion;
    int publicidades_consecutivas = 0;
    int max_publicidades_seguidas = 5; //máximo 5 publicidades seguidas
    
    //reiniciar contadores:
    for(int i = 0; i < *totalPublicidades; i++) {
        publicidades[i].veces_usadas = 0;
    }
    for(int i = 0; i < *totalShows; i++) {
        shows[i].veces = 0;
    }
    for(int i = 0; i < *totalCanciones; i++) {
        canciones[i].ultima_vez = -1;
    }

    while(segundos_totales < 86399 && *total_elementos_grilla < 2000)
    {
        segundos_a_horas(segundos_totales, &horas, &minutos, &segundos);
        elemento_programado = false;
        
        //generar un número aleatorio del 1 al 3 para decidir el tipo de contenido:
        //1: show, 2: canción, 3: publicidad:
        opcion = (rand() % 3) + 1; 
        
        //colocar canciones si excede el maximo de publicidades seguidas:
        if(publicidades_consecutivas >= max_publicidades_seguidas)
        {
            opcion = 2; //entrar a 2: cancion
        }
        
        switch(opcion)
        {
            //shows:
            case 1: 
                if(es_horario_estelar(segundos_totales))
                {
                    int intentos = 0;
                    bool show_encontrado = false;
                    
                    //intentos < *totalshows: 
                    //encontrar shows hasta que intentos sea menor que el doble de la cantidad de shows totales (evitar bucles infinitos):
                    while(intentos < *totalShows * 2 && !show_encontrado)
                    {
                        //genera un numero aleatorio del 1 hasta la cantidad total de shows:
                        int aleatorio = rand() % *totalShows;
                        
                        //guarda el show hasta que alcanza la cantidad de veces que tiene que aparecer:
                        if(shows[aleatorio].veces != shows[aleatorio].segmentos)
                        {
                            grilla[*total_elementos_grilla].horas = horas;
                            grilla[*total_elementos_grilla].minutos = minutos;
                            grilla[*total_elementos_grilla].segundos = segundos;
                            grilla[*total_elementos_grilla].tipo = 'S';
                            strcpy(grilla[*total_elementos_grilla].nombre, shows[aleatorio].nombre);
                            
                            shows[aleatorio].veces++; //incrementa la cantidad de veces que tiene que aparecer
                            segundos_totales += shows[aleatorio].duracion_en_segundos + 1; //avanza el tiempo en la grilla
                            (*total_elementos_grilla)++;
                            elemento_programado = true; //se usará después
                            show_encontrado = true; //sale del while
                            publicidades_consecutivas = 0; //para que la grilla quede balanceada, y no repita seguidamente las publicidades
                        }
                        intentos++;
                    }
                }
                break;
            
            //canciones:
            case 2: 
                {
                    int intentos = 0;
                    bool cancion_encontrada = false;
                    
                    //intentos < *totalCanciones * 3:
                    //requiere del triple de intentos para encontrar una cancion que no haya sido repetida en las anteriores 4 horas:
                    while(intentos < *totalCanciones * 3 && !cancion_encontrada)
                    {

                        //genera un numero aleatorio del 1 hasta la cantidad total de canciones:
                        int aleatorio = rand() % *totalCanciones;
                        
                        //funcion que retorna true si la cancion puede ser guardada:
                        if(se_puede_repetir_cancion(aleatorio, segundos_totales))
                        {
                            grilla[*total_elementos_grilla].horas = horas;
                            grilla[*total_elementos_grilla].minutos = minutos;
                            grilla[*total_elementos_grilla].segundos = segundos;
                            grilla[*total_elementos_grilla].tipo = 'C';
                            strcpy(grilla[*total_elementos_grilla].nombre, canciones[aleatorio].nombre);
                            
                            //guarda la ultima vez que fue reproducida la cancion para evitar repetirla en las proximas 4 horas:
                            canciones[aleatorio].ultima_vez = segundos_totales;
                            segundos_totales += canciones[aleatorio].duracion_en_segundos + 1; //avanza el tiempo en la grilla
                            (*total_elementos_grilla)++;
                            elemento_programado = true; //se usará después
                            cancion_encontrada = true; //sale del while
                            publicidades_consecutivas = 0; //para que la grilla quede balanceada, y no repita seguidamente las publicidades
                        }
                        intentos++;
                    }
                }
                break;
            
            //publicidades:
            case 3:
                {
                    int intentos = 0;
                    bool publicidad_encontrada = false;
                    
                    //intentos < *totalPublicidades * 3:
                    //necesita el triple de intentos para encontrar una publicidad disponible, porque es posible que las otras ya hayan llegado al maximo de repeticiones:
                    while(intentos < *totalPublicidades * 3 && !publicidad_encontrada)
                    {
                        //genera un numero aleatorio del 1 hasta la cantidad total de publicidades:
                        int aleatorio = rand() % *totalPublicidades;
                        
                        //guarda en la grilla la publicidad si aun no ha alcanzado su maximo de repeticion:
                        if(publicidades[aleatorio].veces_usadas < publicidades[aleatorio].veces)
                        {
                            grilla[*total_elementos_grilla].horas = horas;
                            grilla[*total_elementos_grilla].minutos = minutos;
                            grilla[*total_elementos_grilla].segundos = segundos;
                            grilla[*total_elementos_grilla].tipo = 'P';
                            strcpy(grilla[*total_elementos_grilla].nombre, publicidades[aleatorio].empresa);
                            
                            //aumenta la cantidad de veces que ha sido guardada esa publicidad
                            publicidades[aleatorio].veces_usadas++;
                            segundos_totales += publicidades[aleatorio].segundos + 1; //avanza la grilla
                            (*total_elementos_grilla)++;
                            elemento_programado = true; //se usará despues
                            publicidad_encontrada = true; //sale del while
                            publicidades_consecutivas++; //lo que hace es que si llega a 5 publicidades consecutivas, ingresa automaticamente en la opcion 2 para guardar una cancion:
                        }
                        intentos++;
                    }
                }
                break;
        }
        
        //si terminó el switch y no guardó show, cancion o publicidad entonces avanza el tiempo:
        if(!elemento_programado) {
            segundos_totales += 1;
        }
    }

}

void total_grillas(const char *nombre_dia, FILE *archivo_Canciones, FILE *archivo_Publicidad, FILE *archivo_Shows)
{
    //rebobinar los archivos al inicio para leerlos desde el principio:
    rewind(archivo_Canciones);
    rewind(archivo_Publicidad);
    rewind(archivo_Shows);
    
    //reiniciar para la grilla de cada dia:
    int totalCanciones = 0, totalPublicidades = 0, totalShows = 0, total_elementos_grilla = 0;
    
    //leer datos:
    cargarCanciones(archivo_Canciones, &totalCanciones);
    cargarPublicidades(archivo_Publicidad, &totalPublicidades);
    cargarShows(archivo_Shows, &totalShows);
    
    //crea los archivos para las 7 grillas:
    char nombre_salida[25];
    sprintf(nombre_salida, "grilla_%s.out", nombre_dia); //{"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"}
    FILE *archivo_salida = fopen(nombre_salida, "w");
    
    /*if(archivo_salida == NULL) {
        printf("Error al crear %s\n", nombre_salida);
        return;
    }*/
    
    generar_grilla(&totalCanciones, &totalPublicidades, &totalShows, &total_elementos_grilla);

    //genera la grilla en el archivo correspondiente:
    imprimir_en_archivo(nombre_dia, archivo_salida, total_elementos_grilla);
    fclose(archivo_salida);
}


void imprimir_en_archivo(const char nombre_salida[], FILE *archivo, int total_elementos_grilla)
{
    printf("Guardando grilla en el archivo %s\n", nombre_salida);

    for(int i = 0; i < total_elementos_grilla; i++)
    {
        fprintf(archivo, "%02d %02d %02d %c %s\n", grilla[i].horas, grilla[i].minutos, grilla[i].segundos, grilla[i].tipo, grilla[i].nombre);
    }

    printf("Archivo guardado exitosamente con %d elementos\n\n", total_elementos_grilla);
}