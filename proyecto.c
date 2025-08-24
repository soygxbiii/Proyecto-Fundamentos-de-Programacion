#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct{
    char nombre[51];
    int minutos;
    int segundos;
    int puntuacion;
    int duracion_en_segundos;
    int ultima_vez;
}cancion;


typedef struct{
    char empresa[31];
    int segundos;
    int veces;
    int veces_usadas;
}publicidad;


typedef struct{
    char nombre[101];
    int minutos;
    int segundos;
    int segmentos;
    int preferencia;
    int duracion_en_segundos;
    int usado;
}show;


typedef struct{
    char nombre[101];
    char tipo;
    int horas;
    int minutos;
    int segundos;
}programacion;

#define MAX_CANCIONES 1000
#define MAX_PUBLICIDADES  100
#define MAX_SHOWS  15

cancion canciones[MAX_CANCIONES];
publicidad publicidades[MAX_PUBLICIDADES];
show shows[MAX_SHOWS];
programacion grilla[2000];


void cargarCanciones(FILE *archivo, int *totalCanciones);
void cargarPublicidades(FILE *archivo, int *totalPublicidades);
void cargarShows(FILE *archivo, int *totalShows);
void segundos_a_horas(int segundos_totales, int *horas, int *minutos, int *segundos);
bool es_horario_estelar(int segundos_totales);
bool se_puede_repetir_cancion(int aleatorio, int segundos_totales);
void grilla_del_dia(int *totalCanciones, int *totalPublicidades, int *totalShows, int *total_elementos_grilla);
void grilla_archivo(FILE *archivo, int total_elementos_grilla);

int main()
{
    int total_elementos_grilla = 0;
    int segundos_totales = 0, horas = 0, minutos = 0, segundos = 0;
    int opcion;
    int totalCanciones = 0, totalPublicidades = 0, totalShows = 0;

    FILE *archivo_Canciones = fopen("canciones.in", "r");
    FILE *archivo_Publicidad = fopen("publicidad.in", "r");
    FILE *archivo_Shows = fopen("shows.in", "r");
    FILE *archivo_Grilla = fopen("grilla_lunes.out", "w");

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
        
        printf("Cargando canciones...");
        cargarCanciones(archivo_Canciones, &totalCanciones);
            
        printf("Cargando publicidades...");
        cargarPublicidades(archivo_Publicidad, &totalPublicidades);
            
        printf("Cargando shows...");
        cargarShows(archivo_Shows, &totalShows);

        grilla_del_dia(&totalCanciones, &totalPublicidades, &totalShows, &total_elementos_grilla);

        if(archivo_Grilla != NULL){
            grilla_archivo(archivo_Grilla, total_elementos_grilla);
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

    fclose(archivo_Canciones); 
    fclose(archivo_Publicidad);
    fclose(archivo_Shows);

    return 0;
}


void cargarCanciones(FILE *archivo, int *totalCanciones)
{
    int i, j, largoCadena, k=0;
    char c;
    bool band = false;
    char cadena[50];

    printf("\n");
    fscanf(archivo,"%d\n",totalCanciones);
    printf("Total de canciones: %d\n", *totalCanciones);

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
                    printf("Error. El nombre es largo, debe estar en el rango de (1 - 50)\n");
                    printf("Por lo tanto, la cancion %d NO se guardara en la grilla.\n", i+1);
                    while((c = fgetc(archivo)) != '\n' && c != EOF){

                    };
                }else{
                    cadena[j] = '\0';
                    strcpy(canciones[i].nombre, cadena);
                    ungetc(c, archivo);
                    fscanf(archivo,"%d",&canciones[i].minutos);
                    fscanf(archivo,"%d",&canciones[i].segundos);
                    fscanf(archivo,"%d\n",&canciones[i].puntuacion);
                    printf("Cancion %d: Nombre: %s  Minutos: %d  Segundos: %d  Puntuacion: %d\n",i+1, canciones[i].nombre, canciones[i].minutos, canciones[i].segundos, canciones[i].puntuacion);
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
    
    printf("\n");
    rewind(archivo);
}


void cargarPublicidades(FILE *archivo, int *totalPublicidades)
{
    int i = 0, j, largoCadena, contPublicidades=0;
    char c;
    bool band = false;
    char cadena[31];

    printf("\n");

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
                    printf("Error. El nombre es largo, debe estar en el rango de (1 - 30)\n");
                    printf("Por lo tanto, la publicidad %d NO se guardara en la grilla.\n", i+1);
                    while((c = fgetc(archivo)) != '\n' && c != EOF){

                    };
                }else{

                    strcpy(publicidades[i].empresa, cadena);
                    ungetc(c, archivo);
                    fscanf(archivo, "%d", &publicidades[i].segundos);
                    fscanf(archivo, "%d\n", &publicidades[i].veces);
                    printf("Publicidad %d: Empresa: %s  Segundos: %d  Veces: %d\n",i+1, publicidades[i].empresa, publicidades[i].segundos, publicidades[i].veces);
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
    printf("\n");
    rewind(archivo);
}


void cargarShows(FILE *archivo, int *totalShows)
{
    int i, j, largoCadena, k=0;
    char c;
    bool band = false;
    char cadena[101];

    printf("\n");
    fscanf(archivo, "%d\n", totalShows);

    if((*totalShows >= 3) && (*totalShows <= 15)){
        printf("Total de Shows: %d\n",*totalShows);
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
                    printf("Largo cadena %d, %d\n", i, largoCadena);
                    if(largoCadena > 100){
                        printf("Error. El nombre es largo, debe estar en el rango de (1 - 100)\n");
                        printf("Por lo tanto, el show %d NO se guardara en la grilla.\n", i+1);
                        while((c = fgetc(archivo)) != '\n' && c != EOF){

                        };
                    }else{
                        cadena[j] = '\0';
                        strcpy(shows[i].nombre, cadena);
                        fscanf(archivo, "%d", &shows[i].minutos);
                        fscanf(archivo, "%d", &shows[i].segundos);
                        fscanf(archivo, "%d", &shows[i].segmentos);
                        fscanf(archivo, "%d\n", &shows[i].preferencia);
                        printf("Show %d: nombre: %s  Minutos: %d  Segundos: %d  Segmentos: %d  Preferencia: %d\n",i+1, shows[i].nombre, shows[i].minutos, shows[i].segundos, shows[i].segmentos, shows[i].preferencia);
                        shows[i].duracion_en_segundos = shows[i].minutos * 60 + shows[i].segundos;   
                        shows[i].usado = 0;     
 
                    }
                    
                }
            }
            band = false;
        }
    }else{
        printf("(Cantidad de shows no permitidos. Debe estar dentro de un rango de 3 - 15)\n");
        printf("\n");
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

    printf("\n");
    rewind(archivo);
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


void grilla_del_dia(int *totalCanciones, int *totalPublicidades, int *totalShows, int *total_elementos_grilla)
{
    int i;
    int segundos_totales = 300; //00:05:00
    int horas, minutos, segundos;
    bool continuar = false;
    *total_elementos_grilla = 0;

    for(i = 0; i < *totalPublicidades; i++)
    {
        publicidades[i].veces_usadas = 0; //reiniciar
    }

    for(i = 0; i < *totalShows; i++)
    {
        shows[i].usado = 0; //reiniciar
    }

    for(i = 0; i < *totalCanciones; i++)
    {
        canciones[i].ultima_vez = -1; //reiniciar
    }

    while(segundos_totales < 86399 && *total_elementos_grilla < 2000)
    {
        segundos_a_horas(segundos_totales, &horas, &minutos, &segundos);
        bool elementoProgramado = false;
        continuar = false;
        
        //Generar shows segun horarios estelares:
        if(es_horario_estelar(segundos_totales) && !continuar)
        {
            i = 0;
            while(i < *totalShows && !elementoProgramado && !continuar)
            {
                //No puede repetirse shows, por eso el if:
                if(shows[i].usado == 0)
                {
                    grilla[*total_elementos_grilla].horas = horas;
                    grilla[*total_elementos_grilla].minutos = minutos;
                    grilla[*total_elementos_grilla].segundos = segundos;
                    grilla[*total_elementos_grilla].tipo = 'S';
                    strcpy(grilla[*total_elementos_grilla].nombre, shows[i].nombre);

                    //Ya fue usado, no puede entrar al if:
                    shows[i].usado = 1;
                    //Seguir la grilla con 1 segundo de separacion:
                    segundos_totales += shows[i].duracion_en_segundos + 1;
                    (*total_elementos_grilla)++;
                    elementoProgramado = true;
                    continuar = true;
                }
                i++;
            }
        }

        //Generar publicidad:
        if(!elementoProgramado && !continuar)
        {   
            i = 0;
            while(i < *totalPublicidades && !elementoProgramado && !continuar)
            {
                //Generar si es menor al maximo de veces que puede repetirse la publicidad:
                if(publicidades[i].veces_usadas < publicidades[i].veces)
                {
                    grilla[*total_elementos_grilla].horas = horas;
                    grilla[*total_elementos_grilla].minutos = minutos;
                    grilla[*total_elementos_grilla].segundos = segundos;
                    grilla[*total_elementos_grilla].tipo = 'P';
                    strcpy(grilla[*total_elementos_grilla].nombre, publicidades[i].empresa);

                    //Incrementa la cantidad de veces que se ha repetido la publicidad:
                    publicidades[i].veces_usadas++;
                    //Seguir la grilla con 1 segundo de separacion
                    segundos_totales += publicidades[i].segundos + 1;
                    (*total_elementos_grilla)++;
                    elementoProgramado = true;
                    continuar = true;
                }
                i++;
            }
        }

        //Generar canciones:
        if(!elementoProgramado && !continuar)
        {
            int intentos = 0;
            int aleatorio;

            //totalCanciones * 2 se refiere a la cantidad maxima de intentos que puede hacer el algoritmo para buscar una cancion que no se haya repetido:
            while(intentos < *totalCanciones * 2 && !elementoProgramado && !continuar)
            {
                //Generar una cancion aleatoria para poner en la grilla:
                aleatorio = rand() % *totalCanciones;
                if(se_puede_repetir_cancion(aleatorio, segundos_totales))
                {
                    grilla[*total_elementos_grilla].horas = horas;
                    grilla[*total_elementos_grilla].minutos = minutos;
                    grilla[*total_elementos_grilla].segundos = segundos;
                    grilla[*total_elementos_grilla].tipo = 'C';
                    strcpy(grilla[*total_elementos_grilla].nombre, canciones[aleatorio].nombre);

                    //Guarda la ultima vez que se genero una cancion para verificar en la funcion (se_puede_repetir_cancion) si ya han pasado 4 horas):
                    canciones[aleatorio].ultima_vez = segundos_totales;
                    //Continua la grilla con 1 segundo de separacion:
                    segundos_totales += canciones[aleatorio].duracion_en_segundos + 1;
                    (*total_elementos_grilla)++;
                    elementoProgramado = true;
                    continuar = true;
                }
                intentos++;
            }
        }
        
        //Si no ha podido entrar a ninguno if, sigue la grilla con 1 segundo de separacion:
        if(!elementoProgramado){
            segundos_totales += 1;
        }

    }

    printf("Se ha generado la grilla del día con %d elementos\n", *total_elementos_grilla);
}

void grilla_archivo(FILE *archivo, int total_elementos_grilla)
{
    int i;

    printf("Guardando grilla en el archivo\n");

    for(i = 0; i < total_elementos_grilla; i++)
    {
        fprintf(archivo, "%02d %02d %02d %c %s\n", grilla[i].horas, grilla[i].minutos, grilla[i].segundos, grilla[i].tipo, grilla[i].nombre);
    }

    fclose(archivo);
    printf("Archivo guardado exitosamente con %d elementos\n\n", total_elementos_grilla);
}