/* ============================================================
   artemis.c
   Logica del simulador: calculos fisicos, etapas de la mision,
   desafio y reporte final.

   Modelo fisico (simplificado):
   - La Tierra y la Luna se consideran quietas.
   - La nave se mueve sobre la linea que une sus centros.
   - Solo actuan la gravedad de la Tierra y la de la Luna.
   - No hay rozamiento ni otras fuerzas (motores apagados).
   Los limites de este modelo se muestran al usuario en la
   opcion 9 del menu (funcion mostrarSupuestos).

   UTN FRRe - Algoritmos y Estructuras de Datos - K1.2 - Grupo B9
   ============================================================ */

#include <stdio.h>
#include <math.h>
#include "artemis.h"
#include "validaciones.h"

/* Nombres de las etapas. Es una tabla de textos: 4 filas (una
   por etapa) de hasta 45 caracteres cada una. */
static const char NOMBRES_ETAPAS[CANT_ETAPAS][45] = {
    "Etapa A - Inercia con motores apagados",
    "Etapa B - Energia cinetica inicial",
    "Etapa C - Trabajo minimo al equilibrio",
    "Etapa D - Comparacion ida y vuelta"
};

/* ============================================================
   Funciones internas (solo se usan dentro de este archivo)
   ============================================================ */

/* Calcula con que velocidad pasa la nave por un punto ubicado a
   la distancia d del centro de la Tierra, usando conservacion de
   la energia. Devuelve 1 si la nave llega a ese punto y 0 si no
   llega. La velocidad se deja en la variable apuntada. */
static int velocidadEnDistancia(const Mision *m, double d, double *velocidad)
{
    double r0 = RADIO_TIERRA + m->altura;
    double cineticaInicial;   /* J/kg al terminar la TLI           */
    double subida;            /* J/kg que "cuesta" llegar hasta d  */
    double cineticaEnD;       /* J/kg que le quedan en d           */

    cineticaInicial = 0.5 * m->velocidad * m->velocidad;
    subida = potencialPorKg(d, m->distancia)
           - potencialPorKg(r0, m->distancia);
    cineticaEnD = cineticaInicial - subida;

    if (cineticaEnD < 0) {
        *velocidad = 0.0;
        return 0;
    }
    *velocidad = sqrt(2.0 * cineticaEnD);
    return 1;
}

/* Busca, avanzando de a pasos, hasta que distancia del centro
   de la Tierra llega la nave antes de detenerse. */
static double alcanceMaximo(const Mision *m, double limite)
{
    double d = RADIO_TIERRA + m->altura;
    double paso = (limite - d) / PASOS_BUSQUEDA;
    double v;

    while (d + paso <= limite &&
           velocidadEnDistancia(m, d + paso, &v) == 1) {
        d = d + paso;
    }
    return d;
}

/* ============================================================
   Inicio
   ============================================================ */

void mostrarPresentacion(void)
{
    printf("\n");
    dibujarLinea('=');
    printf("            SIMULADOR DE MISIONES ARTEMIS\n");
    printf("       Escenario 3: Inyeccion Translunar (TLI)\n");
    dibujarLinea('=');
    printf(" UTN - Facultad Regional Resistencia\n");
    printf(" Algoritmos y Estructuras de Datos - Comision K1.2 - Grupo B9\n");
    printf("\n");
    printf(" La nave Orion esta en orbita baja alrededor de la Tierra.\n");
    printf(" La etapa ICPS enciende sus motores (maniobra TLI), lleva la\n");
    printf(" nave a 10.9 km/s y se desacopla. Desde ese momento Orion\n");
    printf(" viaja hacia la Luna con los motores apagados.\n");
    printf("\n");
    printf(" Usted es el director de vuelo. Con este simulador puede:\n");
    printf("   - revisar y cambiar los datos de la mision,\n");
    printf("   - seguir el viaje con motores apagados (ley de inercia),\n");
    printf("   - calcular la energia de la nave,\n");
    printf("   - averiguar si llega al punto de equilibrio Tierra-Luna,\n");
    printf("   - comparar el viaje de ida con el de vuelta,\n");
    printf("   - y probar el desafio de inyeccion de precision.\n");
}

/* Carga en la mision los valores oficiales del enunciado. */
void inicializarMision(Mision *m)
{
    m->masa      = MASA_OFICIAL;
    m->velocidad = VELOCIDAD_OFICIAL;
    m->altura    = ALTURA_OFICIAL;
    m->distancia = DISTANCIA_OFICIAL;
}

/* Deja la sesion "en cero": ninguna etapa vista, sin intentos. */
void inicializarSesion(Sesion *s)
{
    int i;

    for (i = 0; i < CANT_ETAPAS; i++) {
        s->etapaVista[i] = 0;
    }
    s->cantIntentos   = 0;
    s->desafioJugado  = 0;
    s->desafioLogrado = 0;
}

/* ============================================================
   Datos de la mision
   ============================================================ */

void mostrarDatos(const Mision *m)
{
    mostrarTitulo("DATOS DE LA MISION");
    printf(" Masa de Orion ................ : %10.1f kg\n", m->masa);
    printf(" Velocidad al terminar la TLI . : %10.1f m/s  (%.2f km/s)\n",
           m->velocidad, m->velocidad / 1000.0);
    printf(" Altura de la orbita baja ..... : %10.1f km\n",
           m->altura / 1000.0);
    printf(" Distancia Tierra-Luna ........ : %10.1f km\n",
           m->distancia / 1000.0);
    printf(" Distancia al centro terrestre  : %10.1f km  (radio + altura)\n",
           (RADIO_TIERRA + m->altura) / 1000.0);

    if (m->masa == MASA_OFICIAL && m->velocidad == VELOCIDAD_OFICIAL &&
        m->altura == ALTURA_OFICIAL && m->distancia == DISTANCIA_OFICIAL) {
        printf("\n Estos son los valores oficiales del enunciado.\n");
    } else {
        printf("\n Hay valores modificados respecto del enunciado.\n");
    }
}

/* Submenu para cambiar los datos. Recibe la mision por puntero
   porque tiene que modificar la variable original de main. */
void configurarMision(Mision *m)
{
    int opcion;

    do {
        mostrarTitulo("CAMBIAR LOS DATOS DE LA MISION");
        printf(" 1. Masa de Orion            (actual: %.1f kg)\n", m->masa);
        printf(" 2. Velocidad tras la TLI    (actual: %.1f m/s)\n",
               m->velocidad);
        printf(" 3. Altura de la orbita baja (actual: %.1f km)\n",
               m->altura / 1000.0);
        printf(" 4. Distancia Tierra-Luna    (actual: %.1f km)\n",
               m->distancia / 1000.0);
        printf(" 5. Volver a los valores oficiales del enunciado\n");
        printf(" 0. Volver al menu principal\n\n");

        opcion = leerEntero(" Opcion", 0, 5);

        switch (opcion) {
            case 1:
                m->masa = leerReal(" Nueva masa en kg",
                                   MASA_MIN, MASA_MAX);
                break;
            case 2:
                m->velocidad = leerReal(" Nueva velocidad en m/s",
                                        VELOCIDAD_MIN, VELOCIDAD_MAX);
                break;
            case 3:
                /* Se pide en km y se guarda en metros. */
                m->altura = 1000.0 * leerReal(" Nueva altura en km",
                                              ALTURA_MIN_KM, ALTURA_MAX_KM);
                break;
            case 4:
                m->distancia = 1000.0 *
                    leerReal(" Nueva distancia Tierra-Luna en km",
                             DISTANCIA_MIN_KM, DISTANCIA_MAX_KM);
                break;
            case 5:
                inicializarMision(m);
                printf(" Se cargaron los valores oficiales.\n");
                break;
            default:
                break;
        }

        if (opcion >= 1 && opcion <= 4) {
            printf(" Dato actualizado.\n");
        }
    } while (opcion != 0);
}

/* ============================================================
   Calculos fisicos
   ============================================================ */

/* Energia cinetica: K = 1/2 * m * v^2  (resultado en joules). */
double energiaCinetica(double masa, double velocidad)
{
    return 0.5 * masa * velocidad * velocidad;
}

/* Energia potencial gravitatoria por cada kg de nave, en un punto
   ubicado a r metros del centro de la Tierra. Se suman el aporte
   de la Tierra y el de la Luna:
       U/m = - G*MT/r - G*ML/(D - r)                            */
double potencialPorKg(double r, double distancia)
{
    double tierra = -G_UNIVERSAL * MASA_TIERRA / r;
    double luna   = -G_UNIVERSAL * MASA_LUNA / (distancia - r);

    return tierra + luna;
}

/* Aceleracion neta de la gravedad sobre la nave. Positiva = la
   nave es frenada (gana la Tierra); cero = punto de equilibrio. */
double gravedadNeta(double r, double distancia)
{
    double tierra = G_UNIVERSAL * MASA_TIERRA / (r * r);
    double luna   = G_UNIVERSAL * MASA_LUNA /
                    ((distancia - r) * (distancia - r));

    return tierra - luna;
}

/* Punto donde la atraccion de la Tierra y la de la Luna valen lo
   mismo. Como la funcion tiene que entregar DOS resultados, los
   devuelve a traves de punteros.
       x = D / (1 + raiz(ML / MT))                              */
void puntoEquilibrio(double distancia, double *desdeTierra, double *desdeLuna)
{
    *desdeTierra = distancia / (1.0 + sqrt(MASA_LUNA / MASA_TIERRA));
    *desdeLuna   = distancia - *desdeTierra;
}

/* Calcula todos los resultados de las consignas B, C y D para la
   mision m y los guarda en la estructura apuntada por r. */
void calcularResultados(const Mision *m, Resultados *r)
{
    double sobrante;   /* J/kg que sobran (+) o faltan (-) */

    r->radioInicial = RADIO_TIERRA + m->altura;

    /* --- Consigna B --- */
    r->energiaCinetica = energiaCinetica(m->masa, m->velocidad);
    r->cineticaPorKg   = r->energiaCinetica / m->masa;

    /* --- Consigna C --- */
    puntoEquilibrio(m->distancia, &r->equilibrioTierra, &r->equilibrioLuna);

    r->potencialInicial = potencialPorKg(r->radioInicial, m->distancia);
    r->potencialEquilibrio = potencialPorKg(r->equilibrioTierra,
                                            m->distancia);
    r->trabajoGravedad = r->potencialEquilibrio - r->potencialInicial;
    r->velocidadMinima = sqrt(2.0 * r->trabajoGravedad);

    r->velocidadOrbital   = sqrt(G_UNIVERSAL * MASA_TIERRA / r->radioInicial);
    r->trabajoDesdeOrbita = r->trabajoGravedad
                          - 0.5 * r->velocidadOrbital * r->velocidadOrbital;

    /* Datos de referencia para interpretar el modelo (opcion 9). */
    r->velocidadEscape = sqrt(2.0 * G_UNIVERSAL * MASA_TIERRA
                              / r->radioInicial);
    r->puntoL1         = FRACCION_L1 * m->distancia;

    sobrante = r->cineticaPorKg - r->trabajoGravedad;

    if (sobrante >= 0) {
        /* La energia cinetica alcanza: no hace falta trabajo extra. */
        r->llega            = 1;
        r->trabajoExtra     = 0.0;
        r->velocidadLlegada = sqrt(2.0 * sobrante);
        r->alcance          = r->equilibrioTierra;
    } else {
        /* No alcanza: lo que falta es el trabajo adicional minimo. */
        r->llega            = 0;
        r->trabajoExtra     = -sobrante;
        r->velocidadLlegada = 0.0;
        r->alcance          = alcanceMaximo(m, r->equilibrioTierra);
    }

    /* --- Consigna D --- */
    r->trabajoDesdeTierra = r->potencialEquilibrio
                          - potencialPorKg(RADIO_TIERRA, m->distancia);
    r->trabajoDesdeLuna   = r->potencialEquilibrio
                          - potencialPorKg(m->distancia - RADIO_LUNA,
                                           m->distancia);
    r->velocidadDesdeTierra = sqrt(2.0 * r->trabajoDesdeTierra);
    r->velocidadDesdeLuna   = sqrt(2.0 * r->trabajoDesdeLuna);
    r->relacion             = r->trabajoDesdeTierra / r->trabajoDesdeLuna;
}

/* Llena el arreglo puntos con la bitacora de vuelo: reparte
   "cantidad" puntos entre la orbita baja y el punto de equilibrio
   y calcula la gravedad y la velocidad en cada uno. */
void calcularTrayecto(const Mision *m, PuntoTrayecto puntos[], int cantidad)
{
    double r0 = RADIO_TIERRA + m->altura;
    double equilibrio;
    double desdeLuna;
    double separacion;
    int i;

    puntoEquilibrio(m->distancia, &equilibrio, &desdeLuna);
    separacion = (equilibrio - r0) / (cantidad - 1);

    for (i = 0; i < cantidad; i++) {
        puntos[i].distancia = r0 + i * separacion;
        puntos[i].gravedad  = gravedadNeta(puntos[i].distancia, m->distancia);
        if (fabs(puntos[i].gravedad) < 0.00005) {
            puntos[i].gravedad = 0.0;   /* evita mostrar -0.0000 */
        }
        puntos[i].llega     = velocidadEnDistancia(m, puntos[i].distancia,
                                                   &puntos[i].velocidad);
    }
}

/* ============================================================
   Etapa A - Primera Ley de Newton (inercia)
   ============================================================ */

void etapaInercia(const Mision *m, Sesion *s)
{
    PuntoTrayecto puntos[PUNTOS_TRAYECTO];
    int i;
    int ultimo = 0;     /* indice del ultimo punto al que llega */

    calcularTrayecto(m, puntos, PUNTOS_TRAYECTO);

    mostrarTitulo("ETAPA A - INERCIA: EL VIAJE CON LOS MOTORES APAGADOS");
    printf(" Primera Ley de Newton: un cuerpo mantiene su velocidad (en\n");
    printf(" valor y direccion) mientras la fuerza neta sobre el sea\n");
    printf(" cero.\n");
    printf("\n");
    printf(" Al apagarse los motores, Orion no se detiene: sigue\n");
    printf(" avanzando por inercia. Si no actuara ninguna fuerza,\n");
    printf(" mantendria sus %.1f m/s para siempre. Pero la gravedad\n",
           m->velocidad);
    printf(" de la Tierra tira de la nave hacia atras y la va frenando.\n");
    printf("\n");
    printf(" Bitacora de vuelo, desde la orbita baja hasta el punto de\n");
    printf(" equilibrio:\n\n");
    printf("  Punto   Distancia a la    Gravedad neta    Velocidad\n");
    printf("          Tierra (km)       (m/s2)           (m/s)\n");

    for (i = 0; i < PUNTOS_TRAYECTO; i++) {
        printf("  %3d     %10.0f        %8.4f       ",
               i + 1, puntos[i].distancia / 1000.0, puntos[i].gravedad);
        if (puntos[i].llega == 1) {
            printf("%9.1f\n", puntos[i].velocidad);
            ultimo = i;
        } else {
            printf(" no llega\n");
        }
    }

    printf("\n Que muestra la tabla:\n");
    printf(" - La velocidad baja en cada punto: la gravedad terrestre\n");
    printf("   actua como una fuerza de frenado.\n");
    printf(" - La gravedad se debilita con la distancia (depende de\n");
    printf("   1/r2): la nave pierde velocidad cada vez mas despacio.\n");

    if (ultimo >= 2) {
        printf("   En el primer tramo perdio %.1f m/s; en el ultimo tramo\n",
               puntos[0].velocidad - puntos[1].velocidad);
        printf("   que recorre, solo %.1f m/s.\n",
               puntos[ultimo - 1].velocidad - puntos[ultimo].velocidad);
    }

    printf(" - En el punto de equilibrio la gravedad neta vale cero: la\n");
    printf("   Tierra y la Luna tiran con la misma fuerza en sentidos\n");
    printf("   opuestos.\n\n");

    if (puntos[PUNTOS_TRAYECTO - 1].llega == 1) {
        printf(" Con estos datos Orion LLEGA al punto de equilibrio con\n");
        printf(" %.1f m/s. Despues, la Luna la atrae y vuelve a acelerar.\n",
               puntos[PUNTOS_TRAYECTO - 1].velocidad);
    } else {
        printf(" Con estos datos, y en este modelo simplificado, Orion NO\n");
        printf(" llega al punto de equilibrio: se queda sin velocidad\n");
        printf(" antes y vuelve a caer hacia la Tierra. La etapa C\n");
        printf(" calcula cuanta energia le falta y que tan preciso es\n");
        printf(" ese valor.\n");
    }

    s->etapaVista[0] = 1;
}

/* ============================================================
   Etapa B - Energia cinetica inicial
   ============================================================ */

void etapaEnergiaCinetica(const Mision *m, Sesion *s)
{
    Resultados r;
    double otraVelocidad;
    double otraEnergia;

    calcularResultados(m, &r);

    mostrarTitulo("ETAPA B - ENERGIA CINETICA INICIAL");
    printf(" La energia cinetica es la energia que tiene un cuerpo por\n");
    printf(" estar en movimiento.\n\n");
    printf(" Formula:  K = 1/2 * m * v^2\n");
    printf(" Datos:    m = %.1f kg      v = %.1f m/s\n",
           m->masa, m->velocidad);
    printf(" Calculo:  K = 0.5 * %.1f * (%.1f)^2\n", m->masa, m->velocidad);
    printf("\n Resultado: K = %.4e J   (%.2f GJ)\n",
           r.energiaCinetica, r.energiaCinetica / 1.0e9);
    printf(" Por cada kg de nave: K/m = %.4e J/kg\n", r.cineticaPorKg);
    printf("\n (1 GJ = mil millones de joules. La notacion 1.5e+12\n");
    printf("  significa 1.5 multiplicado por 10 elevado a la 12.)\n");

    /* Comparacion opcional: muestra que K crece con el cuadrado
       de la velocidad. */
    while (leerSiNo("\n Desea comparar con otra velocidad? (s/n): ") == 's') {
        otraVelocidad = leerReal(" Velocidad a comparar en m/s",
                                 0.0, COMPARACION_MAX);
        otraEnergia = energiaCinetica(m->masa, otraVelocidad);
        printf(" Con %.1f m/s la energia cinetica seria %.4e J,\n",
               otraVelocidad, otraEnergia);
        printf(" es decir, %.2f veces la de la mision.\n",
               otraEnergia / r.energiaCinetica);
    }

    s->etapaVista[1] = 1;
}

/* ============================================================
   Etapa C - Conservacion de la energia y trabajo minimo
   ============================================================ */

/* Muestra cuanto cambia el trabajo adicional si la velocidad
   cambia en MARGEN_DATO m/s hacia abajo o hacia arriba. Sirve
   para no presentar como exacto un resultado que es la resta de
   dos numeros muy parecidos. */
static void mostrarPrecision(const Mision *m)
{
    double variacion[3] = { -MARGEN_DATO, 0.0, MARGEN_DATO };
    Mision prueba;
    Resultados rp;
    int i;
    int llegan = 0;     /* en cuantos de los tres casos llega */

    printf("\n 5) Precision del resultado\n");
    printf("    Wext/m es la resta de dos numeros muy parecidos, por eso\n");
    printf("    cambia mucho si la velocidad cambia poco. Un dato dado\n");
    printf("    con tres cifras (como 10.9 km/s) puede diferir hasta\n");
    printf("    %.0f m/s del valor real. Probando con esa diferencia:\n\n",
           MARGEN_DATO);
    printf("      Velocidad (m/s)   K/m (J/kg)     Wext/m (J/kg)\n");

    for (i = 0; i < 3; i++) {
        prueba = *m;                    /* copia de la mision */
        prueba.velocidad = m->velocidad + variacion[i];
        calcularResultados(&prueba, &rp);

        printf("      %10.1f        %.4e     ",
               prueba.velocidad, rp.cineticaPorKg);
        if (rp.llega == 1) {
            printf("0 (sobra energia)\n");
            llegan++;
        } else {
            printf("%.4e\n", rp.trabajoExtra);
        }
    }

    if (llegan == 0) {
        printf("\n    En los tres casos falta energia: se puede afirmar\n");
        printf("    que la nave necesita trabajo adicional.\n");
    } else if (llegan == 3) {
        printf("\n    En los tres casos la energia alcanza: se puede\n");
        printf("    afirmar que no hace falta trabajo adicional.\n");
    } else {
        printf("\n    El resultado cambia de signo dentro del margen del\n");
        printf("    dato: con esta precision no se puede asegurar si\n");
        printf("    falta o sobra energia. Lo que si se puede afirmar es\n");
        printf("    que la velocidad de la mision es, en la practica, la\n");
        printf("    minima necesaria para llegar al punto de equilibrio.\n");
    }
}

void etapaTrabajoMinimo(const Mision *m, Sesion *s)
{
    Resultados r;

    calcularResultados(m, &r);

    mostrarTitulo("ETAPA C - TRABAJO MINIMO HASTA EL PUNTO DE EQUILIBRIO");
    printf(" Energia mecanica: Em = K + U. Si solo actua la gravedad, Em\n");
    printf(" se conserva. Si hace falta mas energia, la diferencia es el\n");
    printf(" trabajo externo: Wext = Em final - Em inicial.\n");

    printf("\n 1) Punto de equilibrio gravitatorio Tierra-Luna\n");
    printf("    (donde las dos atracciones valen lo mismo; no es el\n");
    printf("    punto de Lagrange L1, ver opcion 9)\n");
    printf("    x = D / (1 + raiz(ML/MT))\n");
    printf("    Distancia al centro de la Tierra : %10.0f km\n",
           r.equilibrioTierra / 1000.0);
    printf("    Distancia al centro de la Luna   : %10.0f km\n",
           r.equilibrioLuna / 1000.0);

    printf("\n 2) Energia potencial por kg:  U/m = -G*MT/r - G*ML/(D-r)\n");
    printf("    En la orbita baja (r = %.0f km) : %.4e J/kg\n",
           r.radioInicial / 1000.0, r.potencialInicial);
    printf("    En el punto de equilibrio        : %.4e J/kg\n",
           r.potencialEquilibrio);
    printf("    Diferencia (lo que hay que subir): %.4e J/kg\n",
           r.trabajoGravedad);

    printf("\n 3) Energia cinetica por kg que ya tiene la nave\n");
    printf("    K/m = 1/2 * v^2                  : %.4e J/kg\n",
           r.cineticaPorKg);

    printf("\n 4) Trabajo adicional minimo (llegar con velocidad cero)\n");
    printf("    Wext/m = diferencia de U/m - K/m\n");

    if (r.llega == 1) {
        printf("    Wext/m = 0 J/kg\n\n");
        printf(" La energia cinetica de la nave alcanza y sobra: no hace\n");
        printf(" falta trabajo adicional. Orion cruza el punto de\n");
        printf(" equilibrio con %.1f m/s.\n", r.velocidadLlegada);
    } else {
        printf("    Wext/m = %.4e J/kg\n", r.trabajoExtra);
        printf("    Para toda la nave (%.0f kg): Wext = %.4e J\n\n",
               m->masa, r.trabajoExtra * m->masa);
        printf(" A la nave le falta esa energia: sin un empuje extra se\n");
        printf(" detiene a unos %.0f km del centro de la Tierra (le faltan\n",
               r.alcance / 1000.0);
        printf(" %.0f km para el punto de equilibrio) y vuelve a caer.\n",
               (r.equilibrioTierra - r.alcance) / 1000.0);
    }

    mostrarPrecision(m);

    /* Se muestra redondeada hacia arriba para que, si el usuario
       la carga en la mision, la nave efectivamente llegue. */
    printf("\n Velocidad minima de inyeccion: %.1f m/s\n",
           ceil(r.velocidadMinima * 10.0) / 10.0);
    printf(" (la mision tiene %.1f m/s: diferencia de %+.1f m/s, que es\n",
           m->velocidad, m->velocidad - r.velocidadMinima);
    printf("  un %.2f %% de la velocidad minima)\n",
           100.0 * fabs(m->velocidad - r.velocidadMinima)
                 / r.velocidadMinima);

    printf("\n Dato adicional: antes de la TLI, en orbita circular, la\n");
    printf(" nave iba a %.1f m/s. Partiendo de esa orbita, el trabajo\n",
           r.velocidadOrbital);
    printf(" necesario es %.4e J/kg: es el que debe hacer la TLI.\n",
           r.trabajoDesdeOrbita);

    s->etapaVista[2] = 1;
}

/* ============================================================
   Etapa D - Comparacion Tierra-Luna / Luna-Tierra
   ============================================================ */

void etapaComparacion(const Mision *m, Sesion *s)
{
    Resultados r;

    calcularResultados(m, &r);

    mostrarTitulo("ETAPA D - COMPARACION ENERGETICA: IDA Y VUELTA");
    printf(" En los dos viajes hay que \"subir\" hasta el punto de\n");
    printf(" equilibrio. Despues, la gravedad del otro astro hace el\n");
    printf(" resto sin gastar combustible.\n\n");
    printf(" Trabajo minimo por kg, saliendo desde la superficie:\n\n");
    printf("  Viaje           Trabajo (J/kg)  Vel. minima   Para la nave\n");
    printf("  Tierra a Luna   %.4e   %8.1f m/s   %.3e J\n",
           r.trabajoDesdeTierra, r.velocidadDesdeTierra,
           r.trabajoDesdeTierra * m->masa);
    printf("  Luna a Tierra   %.4e   %8.1f m/s   %.3e J\n",
           r.trabajoDesdeLuna, r.velocidadDesdeLuna,
           r.trabajoDesdeLuna * m->masa);
    printf("\n El viaje de ida necesita %.1f veces mas energia por kg.\n",
           r.relacion);

    printf("\n Por que pasa esto:\n");
    printf(" - La Tierra tiene %.0f veces la masa de la Luna: su atraccion\n",
           MASA_TIERRA / MASA_LUNA);
    printf("   es mucho mas fuerte y cuesta mas alejarse de ella.\n");
    printf(" - El punto de equilibrio esta a %.0f km de la Tierra y solo\n",
           r.equilibrioTierra / 1000.0);
    printf("   a %.0f km de la Luna. A la ida se sube casi todo el camino\n",
           r.equilibrioLuna / 1000.0);
    printf("   contra la gravedad terrestre; a la vuelta, un tramo corto\n");
    printf("   contra la gravedad lunar, que ademas es debil.\n");
    printf("\n Energia no es lo mismo que combustible:\n");
    printf(" - El numero anterior compara ENERGIAS por kg, no masas de\n");
    printf("   combustible. Para saber cuanto combustible hace falta hay\n");
    printf("   que conocer los motores (con que velocidad expulsan los\n");
    printf("   gases), y el enunciado no da ese dato. Por eso el\n");
    printf("   simulador no calcula masas de combustible.\n");
    printf(" - Lo que si se sabe: mas energia por kg exige mas velocidad\n");
    printf("   de salida (%.1f contra %.1f km/s). Para dar mas velocidad\n",
           r.velocidadDesdeTierra / 1000.0, r.velocidadDesdeLuna / 1000.0);
    printf("   hace falta mas combustible, y ese combustible tambien hay\n");
    printf("   que acelerarlo. Por eso el combustible necesario crece\n");
    printf("   mas rapido que la velocidad que se quiere alcanzar.\n");

    s->etapaVista[3] = 1;
}

/* ============================================================
   Desafio: inyeccion de precision
   ============================================================ */

void jugarDesafio(Mision *m, Sesion *s)
{
    Mision prueba;      /* copia de la mision para probar velocidades */
    Resultados r;
    double v;
    int n;
    char respuesta;

    mostrarTitulo("DESAFIO: INYECCION DE PRECISION");
    printf(" Objetivo: elegir la velocidad de inyeccion para que Orion\n");
    printf(" llegue al punto de equilibrio gastando lo menos posible.\n\n");
    printf(" Reglas:\n");
    printf(" - Tiene %d intentos.\n", MAX_INTENTOS);
    printf(" - Si la velocidad es baja, la nave no llega y vuelve a caer.\n");
    printf(" - Si es muy alta, llega pero desperdicia combustible.\n");
    printf(" - Gana si llega con un exceso de hasta %.0f m/s sobre la\n",
           TOLERANCIA);
    printf("   velocidad minima necesaria.\n");
    printf(" - Se usan la altura (%.1f km) y la distancia Tierra-Luna\n",
           m->altura / 1000.0);
    printf("   (%.1f km) cargadas en la mision.\n", m->distancia / 1000.0);

    /* Cada partida empieza de cero. */
    s->cantIntentos   = 0;
    s->desafioJugado  = 1;
    s->desafioLogrado = 0;

    prueba = *m;        /* copia todos los campos de la mision */

    while (s->cantIntentos < MAX_INTENTOS && s->desafioLogrado == 0) {
        n = s->cantIntentos;

        printf("\n Intento %d de %d\n", n + 1, MAX_INTENTOS);
        v = leerReal(" Velocidad de inyeccion en m/s",
                     VELOCIDAD_MIN, VELOCIDAD_MAX);

        prueba.velocidad = v;
        calcularResultados(&prueba, &r);

        /* Guardamos el intento en el arreglo de la sesion. */
        s->intentos[n].velocidad = v;
        s->intentos[n].llego     = r.llega;
        s->intentos[n].exceso    = v - r.velocidadMinima;
        s->cantIntentos++;

        if (r.llega == 0) {
            printf(" -> No llega. Orion se detuvo a %.0f km de la Tierra,\n",
                   r.alcance / 1000.0);
            printf("    a %.0f km del punto de equilibrio. Pruebe con mas\n",
                   (r.equilibrioTierra - r.alcance) / 1000.0);
            printf("    velocidad.\n");
        } else if (s->intentos[n].exceso > TOLERANCIA) {
            printf(" -> Llega, pero cruza el punto de equilibrio a\n");
            printf("    %.1f m/s: gasto de mas. Pruebe con menos.\n",
                   r.velocidadLlegada);
        } else {
            printf(" -> Inyeccion precisa. Orion llega al punto de\n");
            printf("    equilibrio con apenas %.1f m/s de exceso.\n",
                   s->intentos[n].exceso);
            s->desafioLogrado = 1;
        }
    }

    if (s->desafioLogrado == 1) {
        printf("\n DESAFIO LOGRADO en %d intento(s).\n", s->cantIntentos);
        respuesta = leerSiNo(" Cargar esa velocidad en la mision? (s/n): ");
        if (respuesta == 's') {
            m->velocidad = s->intentos[s->cantIntentos - 1].velocidad;
            printf(" Velocidad de la mision actualizada a %.1f m/s.\n",
                   m->velocidad);
        }
    } else {
        printf("\n Se terminaron los intentos. Puede revisar la etapa C y\n");
        printf(" volver a jugar desde el menu.\n");
    }
}

/* ============================================================
   Reporte final
   ============================================================ */

void mostrarReporte(const Mision *m, const Sesion *s)
{
    Resultados r;
    int i;
    int vistas = 0;

    calcularResultados(m, &r);

    printf("\n");
    dibujarLinea('=');
    printf(" REPORTE FINAL DE LA MISION\n");
    dibujarLinea('=');

    printf(" Datos usados\n");
    printf("   Masa: %.1f kg   Velocidad TLI: %.1f m/s\n",
           m->masa, m->velocidad);
    printf("   Altura: %.1f km   Distancia Tierra-Luna: %.1f km\n",
           m->altura / 1000.0, m->distancia / 1000.0);

    printf("\n Resultados fisicos\n");
    printf("   B) Energia cinetica inicial ........ : %.4e J\n",
           r.energiaCinetica);
    printf("   C) Punto de equilibrio ............. : %.0f km de la Tierra\n",
           r.equilibrioTierra / 1000.0);
    printf("      Trabajo contra la gravedad ...... : %.4e J/kg\n",
           r.trabajoGravedad);
    printf("      Trabajo adicional minimo (Wext/m) : %.4e J/kg\n",
           r.trabajoExtra);
    printf("      Velocidad minima de inyeccion ... : %.1f m/s\n",
           ceil(r.velocidadMinima * 10.0) / 10.0);
    printf("   D) Energia ida / energia vuelta .... : %.1f veces\n",
           r.relacion);

    if (r.llega == 1) {
        printf("\n   Estado: Orion LLEGA al punto de equilibrio\n");
        printf("   (lo cruza a %.1f m/s).\n", r.velocidadLlegada);
    } else {
        printf("\n   Estado: en el modelo, Orion NO llega al punto de\n");
        printf("   equilibrio. Se detiene a %.0f km del centro terrestre.\n",
               r.alcance / 1000.0);
    }
    if (fabs(m->velocidad - r.velocidadMinima) <= MARGEN_DATO) {
        printf("   Atencion: la velocidad esta a menos de %.0f m/s de la\n",
               MARGEN_DATO);
        printf("   minima. Esa diferencia es menor que la precision del\n");
        printf("   dato, asi que el estado anterior no es concluyente.\n");
    }

    /* Recorremos el arreglo de etapas para contar las vistas. */
    printf("\n Rendimiento del director de vuelo\n");
    for (i = 0; i < CANT_ETAPAS; i++) {
        if (s->etapaVista[i] == 1) {
            printf("   [x] %s\n", NOMBRES_ETAPAS[i]);
            vistas++;
        } else {
            printf("   [ ] %s\n", NOMBRES_ETAPAS[i]);
        }
    }
    printf("   Etapas revisadas: %d de %d\n", vistas, CANT_ETAPAS);

    if (s->desafioJugado == 0) {
        printf("\n   Desafio: no se jugo.\n");
    } else {
        printf("\n   Desafio - intentos realizados:\n");
        printf("     Nro   Velocidad (m/s)   Resultado\n");
        for (i = 0; i < s->cantIntentos; i++) {
            printf("     %2d    %10.1f        ",
                   i + 1, s->intentos[i].velocidad);
            if (s->intentos[i].llego == 0) {
                printf("no llego (faltaron %.1f m/s)\n",
                       -s->intentos[i].exceso);
            } else if (s->intentos[i].exceso > TOLERANCIA) {
                printf("llego con %.1f m/s de mas\n", s->intentos[i].exceso);
            } else {
                printf("inyeccion precisa\n");
            }
        }

        if (s->desafioLogrado == 0) {
            printf("   Calificacion: desafio no logrado.\n");
        } else if (s->cantIntentos <= 2) {
            printf("   Calificacion: EXCELENTE (logrado en %d intento(s)).\n",
                   s->cantIntentos);
        } else if (s->cantIntentos <= 4) {
            printf("   Calificacion: MUY BUENO (logrado en %d intentos).\n",
                   s->cantIntentos);
        } else {
            printf("   Calificacion: BUENO (logrado en %d intentos).\n",
                   s->cantIntentos);
        }
    }

    dibujarLinea('=');
}

/* ============================================================
   Supuestos y limites del modelo
   ============================================================ */

void mostrarSupuestos(const Mision *m)
{
    Resultados r;
    double diferenciaU;   /* J/kg entre el punto usado y L1 */

    calcularResultados(m, &r);
    diferenciaU = r.potencialEquilibrio
                - potencialPorKg(r.puntoL1, m->distancia);

    mostrarTitulo("SUPUESTOS Y LIMITES DEL MODELO");
    printf(" Que supone el simulador:\n");
    printf(" 1. La Tierra y la Luna estan quietas.\n");
    printf(" 2. La nave se mueve en linea recta entre los dos centros.\n");
    printf(" 3. Solo actuan la gravedad de la Tierra y la de la Luna.\n");
    printf(" 4. La velocidad de inyeccion es la que tiene la nave a la\n");
    printf("    altura de la orbita baja, medida desde el centro de la\n");
    printf("    Tierra.\n");
    printf(" 5. La masa y el radio de la Luna no estan en el enunciado:\n");
    printf("    se tomaron de la ficha de datos de la NASA.\n");

    printf("\n Como interpretar la velocidad de inyeccion:\n");
    printf("    Velocidad de la orbita circular : %8.1f m/s\n",
           r.velocidadOrbital);
    printf("    Velocidad de la mision          : %8.1f m/s\n",
           m->velocidad);
    printf("    Velocidad de escape de la Tierra: %8.1f m/s\n",
           r.velocidadEscape);
    if (m->velocidad < r.velocidadOrbital) {
        printf("    La mision va mas lento que la orbita circular: con\n");
        printf("    esa velocidad la nave no se aleja de la Tierra.\n");
    } else if (m->velocidad < r.velocidadEscape) {
        printf("    La mision va mas rapido que la orbita circular pero\n");
        printf("    mas lento que la velocidad de escape: Orion NO escapa\n");
        printf("    de la Tierra. Queda en una orbita muy alargada, y por\n");
        printf("    eso puede volver sin motores (retorno libre).\n");
    } else {
        printf("    La mision supera la velocidad de escape: sin la Luna,\n");
        printf("    la nave se alejaria de la Tierra y no volveria.\n");
    }

    printf("\n Punto de equilibrio usado y punto de Lagrange L1:\n");
    printf("    Igualdad de fuerzas (el que usa el programa): %7.0f km\n",
           r.equilibrioTierra / 1000.0);
    printf("    Punto L1 (valor de referencia, aproximado)  : %7.0f km\n",
           r.puntoL1 / 1000.0);
    printf("    L1 tiene en cuenta ademas que la Luna gira alrededor de\n");
    printf("    la Tierra; por eso queda %.0f km mas cerca de la Tierra.\n",
           (r.equilibrioTierra - r.puntoL1) / 1000.0);
    printf("    En energia potencial la diferencia entre los dos puntos\n");
    printf("    es de %.1e J/kg, un %.2f %% del trabajo contra la\n",
           diferenciaU, 100.0 * diferenciaU / r.trabajoGravedad);
    printf("    gravedad. No cambia las conclusiones generales. Pero\n");
    printf("    cuando el trabajo adicional de la etapa C es chico, su\n");
    printf("    valor depende de cual de los dos puntos se elija.\n");

    printf("\n Que NO calcula el simulador:\n");
    printf(" - El tiempo de viaje ni la trayectoria real, que es curva\n");
    printf("   (la velocidad real de la TLI es tangente a la orbita).\n");
    printf(" - El efecto del movimiento de la Luna y de la gravedad del\n");
    printf("   Sol sobre la nave.\n");
    printf(" - La masa de combustible: depende de los motores, y el\n");
    printf("   enunciado no da esos datos.\n");
}
