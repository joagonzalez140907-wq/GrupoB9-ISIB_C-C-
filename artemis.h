
#ifndef ARTEMIS_H
#define ARTEMIS_H


#define G_UNIVERSAL   6.674e-11     
#define MASA_TIERRA   5.972e24      
#define RADIO_TIERRA  6371000.0     
#define MASA_LUNA     7.346e22      
#define RADIO_LUNA    1737400.0     


#define FRACCION_L1   0.849


#define MASA_OFICIAL        26000.0        
#define VELOCIDAD_OFICIAL   10900.0        
#define ALTURA_OFICIAL      192000.0       
#define DISTANCIA_OFICIAL   384000000.0    


#define MASA_MIN          5000.0       
#define MASA_MAX          50000.0      
#define VELOCIDAD_MIN     7000.0       
#define VELOCIDAD_MAX     12000.0      
#define ALTURA_MIN_KM     160.0        
#define ALTURA_MAX_KM     2000.0       
#define DISTANCIA_MIN_KM  356000.0     
#define DISTANCIA_MAX_KM  407000.0     
#define COMPARACION_MAX   20000.0      


#define CANT_ETAPAS       4     
#define PUNTOS_TRAYECTO   10    
#define MAX_INTENTOS      6     
#define PASOS_BUSQUEDA    20000 
#define TOLERANCIA        10.0  
#define MARGEN_DATO       50.0  





typedef struct {
    double masa;        
    double velocidad;   
    double altura;      
    double distancia;   
} Mision;


typedef struct {
    double radioInicial;        

    
    double energiaCinetica;     
    double cineticaPorKg;       

   
    double equilibrioTierra;    
    double equilibrioLuna;      
    double potencialInicial;    
    double potencialEquilibrio; 
    double trabajoGravedad;     
    double trabajoExtra;        
    double velocidadMinima;     
    double velocidadOrbital;    
    double velocidadEscape;     
    double puntoL1;             
    double trabajoDesdeOrbita;  
    int    llega;               
    double velocidadLlegada;    
    double alcance;             


    double trabajoDesdeTierra;  
    double trabajoDesdeLuna;    
    double velocidadDesdeTierra;
    double velocidadDesdeLuna;  
    double relacion;            
} Resultados;


typedef struct {
    double distancia;   
    double gravedad;    
    double velocidad;   
    int    llega;       
} PuntoTrayecto;


typedef struct {
    double velocidad;   
    int    llego;       
    double exceso;      
                           
                           
} Intento;


typedef struct {
    int     etapaVista[CANT_ETAPAS];   
    Intento intentos[MAX_INTENTOS];    
    int     cantIntentos;              
    int     desafioJugado;             
    int     desafioLogrado;            
} Sesion;




void mostrarPresentacion(void);
void inicializarMision(Mision *m);
void inicializarSesion(Sesion *s);


void mostrarDatos(const Mision *m);
void configurarMision(Mision *m);


double energiaCinetica(double masa, double velocidad);
double potencialPorKg(double r, double distancia);
double gravedadNeta(double r, double distancia);
void   puntoEquilibrio(double distancia, double *desdeTierra,
                       double *desdeLuna);
void   calcularResultados(const Mision *m, Resultados *r);
void   calcularTrayecto(const Mision *m, PuntoTrayecto puntos[],
                        int cantidad);


void etapaInercia(const Mision *m, Sesion *s);
void etapaEnergiaCinetica(const Mision *m, Sesion *s);
void etapaTrabajoMinimo(const Mision *m, Sesion *s);
void etapaComparacion(const Mision *m, Sesion *s);


void jugarDesafio(Mision *m, Sesion *s);
void mostrarReporte(const Mision *m, const Sesion *s);
void mostrarSupuestos(const Mision *m);

#endif
