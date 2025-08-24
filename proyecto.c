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
void segundosHoras(int segundosTotales, int *horas, int *minutos, int *segundos);
bool horarioEstelar(int segundosTotales);
bool repetirCancion(int aleatorio, int segundosTotales);

//grillas:
void generarGrilla(int *totalCanciones, int *totalPublicidades, int *totalShows, int *totalElementosGrilla);
void imprimirArchivo(const char nombreSalida[], FILE *archivo, int totalElementosGrilla);
void totalGrillas(const char *nombreDia, int *totalCanciones, int *totalPublicidades, int *totalShows, int *totalElementosGrilla);


//====== MAIN ======

int main()
{
    FILE *archivoCanciones = fopen("canciones.in", "r");
    FILE *archivoPublicidades = fopen("publicidad.in", "r");
    FILE *archivoShows = fopen("shows.in", "r");

    //NULL == 0
    //if(!0) = if(1) -> Por eso entra al if

    if(!archivoCanciones) 
    {
        printf("Error: canciones.in no encontrado\n");
    }

    if(!archivoPublicidades) 
    {
        printf("Error: publicidad.in no encontrado\n");
    }

    if(!archivoShows) 
    {
        printf("Error: shows.in no encontrado\n");
    }

    if(archivoCanciones && archivoPublicidades && archivoShows)
    {
        int opcion;
        int totalCanciones = 0, totalPublicidades = 0, totalShows = 0;
        
        //leer datos:
        cargarCanciones(archivoCanciones, &totalCanciones);
        cargarPublicidades(archivoPublicidades, &totalPublicidades);
        cargarShows(archivoShows, &totalShows);

        //con esto cada grilla será diferente:
        srand(time(NULL)); 

        //arreglo para crear los archivos de salida eficientemente: 
        char diasSemana[7][10] = {"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"};

        //generar las 7 grillas distintas para la semana:
        for(int i = 0; i < 7; i++)
        {
            int totalElementosGrilla = 0;

            //rebobinar los archivos al inicio para leerlos desde el principio:
            rewind(archivoCanciones);
            rewind(archivoPublicidades);
            rewind(archivoShows);

            totalGrillas(diasSemana[i], &totalCanciones, &totalPublicidades, &totalShows, &totalElementosGrilla);
        }

        /*for(int i = 0; i < totalPublicidades; i++){
            printf("intentos: %s %d\n", publicidades[i].empresa, publicidades[i].vecesUsadas);
        }*/
        
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
    }else{
        printf("Saliendo...\n");
    }

    // archivo != NULL -> 1
    //if(1) entonces entra para cerrarlo

    if(archivoCanciones)
    {
        fclose(archivoCanciones);
    }

    if(archivoPublicidades)
    {
        fclose(archivoPublicidades);
    }
    
    if(archivoShows)
    {
        fclose(archivoShows);
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
                    canciones[i].duracionSegundos = canciones[i].minutos * 60 + canciones[i].segundos;
                    canciones[i].ultimaVez = -1;
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
                    publicidades[i].vecesUsadas = 0;
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
                        shows[i].duracionSegundos = shows[i].minutos * 60 + shows[i].segundos;   
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

void segundosHoras(int segundosTotales, int *horas, int *minutos, int *segundos)
{
    *horas = segundosTotales / 3600;
    *minutos = (segundosTotales % 3600) / 60;
    *segundos = segundosTotales % 60;
}


bool horarioEstelar(int segundosTotales)
{
    bool si_es = false;

    if((segundosTotales >= 25200 && segundosTotales <= 32400) || (segundosTotales >= 43200 && segundosTotales <= 50400) || (segundosTotales >= 64800 && segundosTotales <= 68400))
    {
        si_es = true;
    }

    return (si_es);

    // 25200 a 32400 es 7:00 AM - 9:00 AM
    // 43200 a 50400 es 12:00 PM - 2:00 PM 
    // 64800 a 68400 es 06:00 PM - 07:00 PM
}

bool repetirCancion(int aleatorio, int segundosTotales)
{
    int cuatroHoras = 14400;

    //-1 significa que la cancion nunca se ha puesto:
    if(canciones[aleatorio].ultimaVez == -1)
    {
        return true;
    }
    
    //si han pasado 4 horas o mas desde la ultima reproduccion:
    if(segundosTotales - canciones[aleatorio].ultimaVez >= cuatroHoras)
    {
        return true;
    }

    return false;
}

void generarGrilla(int *totalCanciones, int *totalPublicidades, int *totalShows, int *totalElementosGrilla)
{
    int segundosTotales = 300; //inicia a las 00:05:00
    int horas, minutos, segundos;
    bool elementoProgramado;
    int opcion;
    int publicidadesConsecutivas = 0;
    int maxPublicidadesSeguidas = 2; //máximo 2 publicidades seguidas
    
    //reiniciar contadores:
    for(int i = 0; i < *totalPublicidades; i++) {
        publicidades[i].vecesUsadas = 0;
    }
    for(int i = 0; i < *totalShows; i++) {
        shows[i].veces = 0;
    }
    for(int i = 0; i < *totalCanciones; i++) {
        canciones[i].ultimaVez = -1;
    }

    while(segundosTotales < 86399 && *totalElementosGrilla < 2000)
    {
        segundosHoras(segundosTotales, &horas, &minutos, &segundos);
        elementoProgramado = false;
        
        //generar un número aleatorio del 1 al 3 para decidir el tipo de contenido:
        //1: show, 2: canción, 3: publicidad:
        opcion = (rand() % 3) + 1; 
        
        //colocar canciones si excede el maximo de publicidades seguidas:
        if(publicidadesConsecutivas >= maxPublicidadesSeguidas)
        {
            opcion = 2; //entrar a 2: cancion
        }
        
        switch(opcion)
        {
            //shows:
            case 1: 
                if(horarioEstelar(segundosTotales))
                {
                    int intentos = 0;
                    bool showEncontrado = false;
                    
                    //intentos < *totalshows: 
                    //encontrar shows hasta que intentos sea menor que el doble de la cantidad de shows totales (evitar bucles infinitos):
                    while(intentos < *totalShows * 2 && !showEncontrado)
                    {
                        //genera un numero aleatorio del 1 hasta la cantidad total de shows:
                        int aleatorio = rand() % *totalShows;
                        
                        //guarda el show hasta que alcanza la cantidad de veces que tiene que aparecer:
                        if(shows[aleatorio].veces != shows[aleatorio].segmentos)
                        {
                            grilla[*totalElementosGrilla].horas = horas;
                            grilla[*totalElementosGrilla].minutos = minutos;
                            grilla[*totalElementosGrilla].segundos = segundos;
                            grilla[*totalElementosGrilla].tipo = 'S';
                            strcpy(grilla[*totalElementosGrilla].nombre, shows[aleatorio].nombre);
                            
                            shows[aleatorio].veces++; //incrementa la cantidad de veces que tiene que aparecer
                            segundosTotales += shows[aleatorio].duracionSegundos + 1; //avanza el tiempo en la grilla
                            (*totalElementosGrilla)++;
                            elementoProgramado = true; //se usará después
                            showEncontrado = true; //sale del while
                            publicidadesConsecutivas = 0; //para que la grilla quede balanceada, y no repita seguidamente las publicidades
                        }
                        intentos++;
                    }
                }
                break;
            
            //canciones:
            case 2: 
                {
                    int intentos = 0;
                    bool cancionEncontrada = false;
                    
                    //intentos < *totalCanciones * 3:
                    //requiere del triple de intentos para encontrar una cancion que no haya sido repetida en las anteriores 4 horas:
                    while(intentos < *totalCanciones * 3 && !cancionEncontrada)
                    {

                        //genera un numero aleatorio del 1 hasta la cantidad total de canciones:
                        int aleatorio = rand() % *totalCanciones;
                        
                        //funcion que retorna true si la cancion puede ser guardada:
                        if(repetirCancion(aleatorio, segundosTotales))
                        {
                            grilla[*totalElementosGrilla].horas = horas;
                            grilla[*totalElementosGrilla].minutos = minutos;
                            grilla[*totalElementosGrilla].segundos = segundos;
                            grilla[*totalElementosGrilla].tipo = 'C';
                            strcpy(grilla[*totalElementosGrilla].nombre, canciones[aleatorio].nombre);
                            
                            //guarda la ultima vez que fue reproducida la cancion para evitar repetirla en las proximas 4 horas:
                            canciones[aleatorio].ultimaVez = segundosTotales;
                            segundosTotales += canciones[aleatorio].duracionSegundos + 1; //avanza el tiempo en la grilla
                            (*totalElementosGrilla)++;
                            elementoProgramado = true; //se usará después
                            cancionEncontrada = true; //sale del while
                            publicidadesConsecutivas = 0; //para que la grilla quede balanceada, y no repita seguidamente las publicidades
                        }
                        intentos++;
                    }
                }
                break;
            
            //publicidades:
            case 3:
                {
                    int intentos = 0;
                    bool publicidadEncontrada = false;
                    
                    //intentos < *totalPublicidades * 3:
                    //necesita el triple de intentos para encontrar una publicidad disponible, porque es posible que las otras ya hayan llegado al maximo de repeticiones:
                    while(intentos < *totalPublicidades * 3 && !publicidadEncontrada)
                    {
                        //genera un numero aleatorio del 1 hasta la cantidad total de publicidades:
                        int aleatorio = rand() % *totalPublicidades;
                        
                        //guarda en la grilla la publicidad si aun no ha alcanzado su maximo de repeticion:
                        if(publicidades[aleatorio].vecesUsadas < publicidades[aleatorio].veces)
                        {
                            grilla[*totalElementosGrilla].horas = horas;
                            grilla[*totalElementosGrilla].minutos = minutos;
                            grilla[*totalElementosGrilla].segundos = segundos;
                            grilla[*totalElementosGrilla].tipo = 'P';
                            strcpy(grilla[*totalElementosGrilla].nombre, publicidades[aleatorio].empresa);
                            
                            //aumenta la cantidad de veces que ha sido guardada esa publicidad
                            publicidades[aleatorio].vecesUsadas++;
                            segundosTotales += publicidades[aleatorio].segundos + 1; //avanza la grilla
                            (*totalElementosGrilla)++;
                            elementoProgramado = true; //se usará despues
                            publicidadEncontrada = true; //sale del while
                            publicidadesConsecutivas++; //lo que hace es que si llega a 2 publicidades consecutivas, ingresa automaticamente en la opcion 2 para guardar una cancion:
                        }
                        intentos++;
                    }
                }
                break;
        }
        
        //si terminó el switch y no guardó show, cancion o publicidad entonces avanza el tiempo:
        if(!elementoProgramado) {
            segundosTotales += 1;
        }
    }

}

void imprimirArchivo(const char nombreSalida[], FILE *archivo, int totalElementosGrilla)
{
    printf("Guardando grilla en el archivo %s\n", nombreSalida);

    for(int i = 0; i < totalElementosGrilla; i++)
    {
        fprintf(archivo, "%02d %02d %02d %c %s\n", grilla[i].horas, grilla[i].minutos, grilla[i].segundos, grilla[i].tipo, grilla[i].nombre);
    }

    printf("Archivo guardado exitosamente con %d elementos\n\n", totalElementosGrilla);
}

void totalGrillas(const char *nombreDia, int *totalCanciones, int *totalPublicidades, int *totalShows, int *totalElementosGrilla)
{
    
    //crea los archivos para las 7 grillas:
    char nombreSalida[25];
    sprintf(nombreSalida, "grilla_%s.out", nombreDia); //{"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"}
    FILE *archivoSalida = fopen(nombreSalida, "w");
    
    if(!archivoSalida)
    {
        printf("Error al crear %s\n", nombreSalida);
    }else{
        generarGrilla(totalCanciones, totalPublicidades, totalShows, totalElementosGrilla);

        //genera la grilla en el archivo correspondiente:
        imprimirArchivo(nombreSalida, archivoSalida, *totalElementosGrilla);
        fclose(archivoSalida);
    }
    
}
