/* ============================================================
   main.c
   Punto de entrada del programa. Contiene solamente la funcion
   main, el menu principal y la inicializacion de los datos.

   Simulador de Misiones Artemis
   Escenario 3: Inyeccion Translunar (TLI) e inercia en
   trayectoria libre.

   UTN FRRe - Algoritmos y Estructuras de Datos - K1.2 - Grupo B9
   Integrantes: Gonzalez Joaquin, Galarza Lucio, Martinez Franco,
                Saucedo Ivan, Masat Julian.
   ============================================================ */

#include <stdio.h>
#include "artemis.h"
#include "validaciones.h"


void mostrarMenu(void)
{
    printf("\n");
    dibujarLinea('=');
    printf(" MENU PRINCIPAL - CONTROL DE MISION\n");
    dibujarLinea('=');
    printf(" 1. Ver los datos de la mision\n");
    printf(" 2. Cambiar los datos de la mision\n");
    printf(" 3. Etapa A - Inercia: el viaje con motores apagados\n");
    printf(" 4. Etapa B - Energia cinetica inicial\n");
    printf(" 5. Etapa C - Trabajo minimo hasta el punto de equilibrio\n");
    printf(" 6. Etapa D - Comparacion energetica: ida y vuelta\n");
    printf(" 7. Desafio: inyeccion de precision\n");
    printf(" 8. Ver el reporte de la mision\n");
    printf(" 9. Supuestos y limites del modelo\n");
    printf(" 0. Salir\n\n");
}

int main(void)
{
    Mision mision;      
    Sesion sesion;      
    int opcion;
    int salir = 0;      

    
    inicializarMision(&mision);
    inicializarSesion(&sesion);

    mostrarPresentacion();
    pausar();

    
    while (salir == 0) {
        mostrarMenu();
        opcion = leerEntero(" Elija una opcion", 0, 9);

        switch (opcion) {
            case 1:
                mostrarDatos(&mision);
                break;
            case 2:
                configurarMision(&mision);
                break;
            case 3:
                etapaInercia(&mision, &sesion);
                break;
            case 4:
                etapaEnergiaCinetica(&mision, &sesion);
                break;
            case 5:
                etapaTrabajoMinimo(&mision, &sesion);
                break;
            case 6:
                etapaComparacion(&mision, &sesion);
                break;
            case 7:
                jugarDesafio(&mision, &sesion);
                break;
            case 8:
                mostrarReporte(&mision, &sesion);
                break;
            case 9:
                mostrarSupuestos(&mision);
                break;
            case 0:
                if (leerSiNo(" Seguro que desea salir? (s/n): ") == 's') {
                    salir = 1;
                }
                break;
            default:
               
                break;
        }

        
        if (salir == 0 && opcion != 0 && opcion != 2) {
            pausar();
        }
    }

    
    mostrarReporte(&mision, &sesion);
    printf("\n Fin de la simulacion. Gracias por usar el simulador.\n\n");

    return 0;
}
