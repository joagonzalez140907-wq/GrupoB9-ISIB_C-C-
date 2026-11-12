/* ============================================================
   validaciones.h
   Modulo de validaciones e interfaz.
   Aca solo se DECLARAN las funciones (prototipos). El codigo de
   cada una esta en validaciones.c.

   UTN FRRe - Algoritmos y Estructuras de Datos - K1.2 - Grupo B9
   ============================================================ */

#ifndef VALIDACIONES_H
#define VALIDACIONES_H

/* Cantidad maxima de caracteres que guardamos de cada linea
   que escribe el usuario (incluye el caracter de fin '\0'). */
#define TAM_LINEA 100

/* Cantidad de caracteres de las lineas separadoras de pantalla. */
#define ANCHO_PANTALLA 62

/* Lee una linea completa del teclado y la guarda en texto. */
void leerLinea(char texto[], int tam);

/* Saca los espacios del principio y del final de un texto. */
void recortar(char texto[]);

/* Devuelven 1 si el texto tiene forma de numero y 0 si no. */
int esEntero(const char texto[]);
int esReal(const char texto[]);

/* Piden un dato y repiten el pedido hasta que sea valido.
   leerEntero y leerReal agregan al mensaje el rango permitido,
   por ejemplo: "Nueva masa en kg (5000 a 50000): ". */
int    leerEntero(const char mensaje[], int minimo, int maximo);
double leerReal(const char mensaje[], double minimo, double maximo);
char   leerSiNo(const char mensaje[]);

/* Interfaz: lineas, titulos, mensajes de error y pausa. */
void dibujarLinea(char simbolo);
void mostrarTitulo(const char texto[]);
void mostrarError(const char mensaje[]);
void pausar(void);

#endif
