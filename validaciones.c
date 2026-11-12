
   validaciones.c
   Implementacion del ingreso seguro de datos.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validaciones.h"

void leerLinea(char texto[], int tam)
{
    int largo;
    int c;

    /* fgets devuelve NULL si ya no hay nada para leer (por
       ejemplo, si se cierra la entrada). En ese caso terminamos
       el programa de forma ordenada para no quedar en un ciclo
       infinito pidiendo datos. */
    if (fgets(texto, tam, stdin) == NULL) {
        printf("\nSe cerro la entrada de datos. Fin del programa.\n");
        exit(0);
    }

    largo = (int) strlen(texto);

    if (largo > 0 && texto[largo - 1] == '\n') {
        /* fgets guarda tambien el ENTER; lo borramos. */
        texto[largo - 1] = '\0';
    } else {
        /* La linea era mas larga que el arreglo. Leemos y
           descartamos lo que sobro para que no moleste en la
           proxima lectura. */
        do {
            c = getchar();
        } while (c != '\n' && c != EOF);
    }
}


void recortar(char texto[])
{
    int inicio = 0;
    int fin;
    int i;

    /* Buscamos el primer caracter que no sea un espacio. */
    while (texto[inicio] == ' ' || texto[inicio] == '\t') {
        inicio++;
    }

    /* Buscamos el ultimo caracter que no sea un espacio. */
    fin = (int) strlen(texto) - 1;
    while (fin >= inicio &&
           (texto[fin] == ' ' || texto[fin] == '\t' || texto[fin] == '\r')) {
        fin--;
    }

    /* Corremos la parte util al principio del arreglo. */
    for (i = inicio; i <= fin; i++) {
        texto[i - inicio] = texto[i];
    }
    texto[fin - inicio + 1] = '\0';
}


int esEntero(const char texto[])
{
    int i = 0;
    int digitos = 0;

    if (texto[i] == '+' || texto[i] == '-') {
        i++;
    }

    while (texto[i] != '\0') {
        if (texto[i] < '0' || texto[i] > '9') {
            return 0;               /* encontramos algo que no es digito */
        }
        digitos++;
        i++;
    }

    if (digitos >= 1 && digitos <= 9) {
        return 1;
    }
    return 0;
}


int esReal(const char texto[])
{
    int i = 0;
    int digitos = 0;
    int separadores = 0;

    if (texto[i] == '+' || texto[i] == '-') {
        i++;
    }

    while (texto[i] != '\0') {
        if (texto[i] >= '0' && texto[i] <= '9') {
            digitos++;
        } else if (texto[i] == '.' || texto[i] == ',') {
            separadores++;
        } else {
            return 0;               /* letra o simbolo no permitido */
        }
        i++;
    }

    if (digitos >= 1 && digitos <= 15 && separadores <= 1) {
        return 1;
    }
    return 0;
}


int leerEntero(const char mensaje[], int minimo, int maximo)
{
    char linea[TAM_LINEA];
    int valor = 0;
    int valido = 0;

    do {
        printf("%s (%d a %d): ", mensaje, minimo, maximo);
        leerLinea(linea, TAM_LINEA);
        recortar(linea);

        if (linea[0] == '\0') {
            mostrarError("No ingreso ningun dato.");
        } else if (!esEntero(linea)) {
            mostrarError("Debe ingresar un numero entero, sin letras.");
        } else {
            sscanf(linea, "%d", &valor);
            if (valor < minimo || valor > maximo) {
                printf("  [!] Debe estar entre %d y %d.", minimo, maximo);
                printf(" Intente de nuevo.\n");
            } else {
                valido = 1;
            }
        }
    } while (valido == 0);

    return valor;
}


double leerReal(const char mensaje[], double minimo, double maximo)
{
    char linea[TAM_LINEA];
    double valor = 0.0;
    int valido = 0;
    int i;

    do {
        printf("%s (%.0f a %.0f): ", mensaje, minimo, maximo);
        leerLinea(linea, TAM_LINEA);
        recortar(linea);

        if (linea[0] == '\0') {
            mostrarError("No ingreso ningun dato.");
        } else if (!esReal(linea)) {
            mostrarError("Debe ingresar un numero, sin letras.");
        } else {
            /* C usa el punto como separador decimal. Si el usuario
               escribio una coma, la cambiamos por un punto. */
            for (i = 0; linea[i] != '\0'; i++) {
                if (linea[i] == ',') {
                    linea[i] = '.';
                }
            }
            sscanf(linea, "%lf", &valor);

            if (valor < 0 && minimo >= 0) {
                mostrarError("Este dato no puede ser negativo.");
            } else if (valor < minimo || valor > maximo) {
                printf("  [!] Debe estar entre %.1f y %.1f.", minimo, maximo);
                printf(" Intente de nuevo.\n");
            } else {
                valido = 1;
            }
        }
    } while (valido == 0);

    return valor;
}


char leerSiNo(const char mensaje[])
{
    char linea[TAM_LINEA];
    char respuesta = ' ';

    while (respuesta != 's' && respuesta != 'n') {
        printf("%s", mensaje);
        leerLinea(linea, TAM_LINEA);
        recortar(linea);

        /* Solo vale una respuesta de UNA letra: s, S, n o N. */
        if (strlen(linea) != 1) {
            mostrarError("Responda con la letra s (si) o n (no).");
        } else if (linea[0] == 's' || linea[0] == 'S') {
            respuesta = 's';
        } else if (linea[0] == 'n' || linea[0] == 'N') {
            respuesta = 'n';
        } else {
            mostrarError("Responda con la letra s (si) o n (no).");
        }
    }

    return respuesta;
}


void dibujarLinea(char simbolo)
{
    int i;

    for (i = 0; i < ANCHO_PANTALLA; i++) {
        printf("%c", simbolo);
    }
    printf("\n");
}


void mostrarTitulo(const char texto[])
{
    printf("\n");
    dibujarLinea('-');
    printf(" %s\n", texto);
    dibujarLinea('-');
}

void mostrarError(const char mensaje[])
{
    printf("  [!] %s Intente de nuevo.\n", mensaje);
}

}
void pausar(void)
{
    char linea[TAM_LINEA];

    printf("\nPresione ENTER para continuar...");
    leerLinea(linea, TAM_LINEA);
}
