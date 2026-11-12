# Simulador de Misiones Artemis – Inyección Translunar (TLI)

Trabajo Práctico Integrador Final de **Algoritmos y Estructuras de Datos**  
Universidad Tecnológica Nacional – Facultad Regional Resistencia  
Ingeniería en Sistemas de Información

## Video de presentación

https://youtu.be/-_l8v5unZrk

## Integrantes

| Integrante | Comisión | Grupo |
|---|---|---|
| Gonzalez Joaquin | K1.2 | B9 |
| Galarza Lucio | K1.2 | B9 |
| Martinez Franco | K1.2 | B9 |
| Saucedo Ivan | K1.2 | B9 |
| Masat Julian | K1.2 | B9 |

## Descripción del proyecto

El proyecto consiste en un simulador interactivo por consola basado en la **Situación 3: Inyección Translunar (TLI) e inercia en trayectoria libre**, del Trabajo Práctico Integrador Final.

El escenario plantea que la etapa superior ICPS realiza la maniobra de Inyección Translunar y lleva a la nave Orion a una velocidad de 10,9 km/s. Luego, la etapa se desacopla, los motores se apagan y la nave continúa su movimiento bajo la acción gravitatoria considerada en el modelo.

El programa permite consultar y modificar los datos de la misión, analizar las distintas etapas del viaje, resolver los cálculos energéticos, realizar un desafío de precisión y consultar un reporte final.

## Datos iniciales del escenario

| Dato | Valor |
|---|---:|
| Velocidad al terminar la TLI | 10.900 m/s (10,9 km/s) |
| Masa de Orion | 26.000 kg |
| Distancia media Tierra-Luna | 384.000.000 m |
| Altura de la órbita baja | 192.000 m |

El programa permite modificar estos valores dentro de los siguientes rangos:

| Dato | Rango permitido |
|---|---:|
| Masa de Orion | 5.000 a 50.000 kg |
| Velocidad de inyección | 7.000 a 12.000 m/s |
| Altura de órbita baja | 160 a 2.000 km |
| Distancia Tierra-Luna | 356.000 a 407.000 km |

También se pueden restaurar los valores iniciales del enunciado.

## Funcionalidades

Al iniciar, el programa presenta la misión y muestra el menú principal:

```text
1. Ver los datos de la misión
2. Cambiar los datos de la misión
3. Etapa A - Inercia: el viaje con motores apagados
4. Etapa B - Energía cinética inicial
5. Etapa C - Trabajo mínimo hasta el punto de equilibrio
6. Etapa D - Comparación energética: ida y vuelta
7. Desafío: inyección de precisión
8. Ver el reporte de la misión
0. Salir
```

Las opciones principales son:

- **Etapa A – Inercia:** muestra cómo continúa moviéndose Orion después del apagado de los motores. Presenta una bitácora con diez puntos entre la órbita baja y el punto de equilibrio, e informa la variación de la velocidad y de la gravedad neta en el recorrido.
- **Etapa B – Energía cinética:** calcula la energía cinética inicial de la nave a partir de su masa y velocidad.
- **Etapa C – Trabajo mínimo:** calcula la posición del punto de equilibrio gravitatorio, la variación de energía potencial por unidad de masa y el trabajo externo adicional que sería necesario para llegar a ese punto con velocidad final nula. También informa la velocidad mínima de inyección estimada.
- **Etapa D – Comparación energética:** compara la energía por unidad de masa necesaria para alcanzar el punto de equilibrio desde la superficie terrestre y desde la superficie lunar, de acuerdo con el modelo utilizado.
- **Desafío de precisión:** permite probar velocidades de inyección. El usuario dispone de seis intentos y gana si la nave alcanza el punto de equilibrio con un exceso de velocidad de hasta 10 m/s sobre la mínima calculada. Al finalizar, puede elegir si desea cargar esa velocidad en los datos de la misión.
- **Reporte final:** resume los datos utilizados, los resultados físicos, las etapas consultadas y el desempeño en el desafío. El reporte también se muestra al salir del programa.

## Modelo físico y supuestos

El simulador utiliza un modelo simplificado para representar el movimiento de la nave:

- La Tierra y la Luna se consideran inmóviles durante el análisis.
- La nave se desplaza sobre la línea que une los centros de la Tierra y la Luna.
- Con los motores apagados, solo se consideran las fuerzas gravitatorias de la Tierra y de la Luna.
- No se incluyen la influencia del Sol, el rozamiento, la rotación del sistema ni otros efectos externos.
- Los cálculos se realizan con unidades del Sistema Internacional.

### Constantes utilizadas

| Constante | Valor |
|---|---:|
| Constante de gravitación universal, `G` | 6,674 × 10⁻¹¹ N·m²/kg² |
| Masa de la Tierra | 5,972 × 10²⁴ kg |
| Radio de la Tierra | 6.371.000 m |
| Masa de la Luna (dato adoptado) | 7,35 × 10²² kg |
| Radio de la Luna (dato adoptado) | 1.737.000 m |

Los valores de la Tierra corresponden a los datos utilizados por la cátedra. La masa y el radio de la Luna no están especificados en los datos del escenario y se incorporan como valores aproximados adoptados para el modelo.

### Interpretación del punto de equilibrio

En este programa, el punto de equilibrio es la posición sobre la línea Tierra-Luna en la que las magnitudes de las fuerzas gravitatorias ejercidas por ambos cuerpos sobre la nave son iguales. Se obtiene mediante:

\[
x = \frac{D}{1 + \sqrt{M_L/M_T}}
\]

Donde:

- `D` es la distancia entre los centros de la Tierra y la Luna.
- `M_L` es la masa de la Luna.
- `M_T` es la masa de la Tierra.
- `x` es la distancia desde el centro de la Tierra hasta el punto de equilibrio.

Esta posición corresponde al equilibrio de las fuerzas gravitatorias en el modelo estático utilizado. **No debe confundirse con el punto de Lagrange L1**, cuyo cálculo requiere considerar también el movimiento de rotación del sistema.

### Cálculo del trabajo adicional

El programa calcula la energía potencial gravitatoria por unidad de masa considerando la contribución de la Tierra y de la Luna. Para una posición `r` medida desde el centro terrestre:

\[
\frac{U(r)}{m} = -\frac{G M_T}{r} - \frac{G M_L}{D-r}
\]

La energía cinética inicial por unidad de masa es:

\[
\frac{K}{m} = \frac{v^2}{2}
\]

La variación de energía potencial por unidad de masa entre la órbita inicial y el punto de equilibrio se compara con la energía cinética disponible. El trabajo externo adicional mínimo se calcula como:

\[
\frac{W_{ext}}{m} = \max\left(0,\frac{\Delta U}{m}-\frac{K}{m}\right)
\]

La velocidad mínima teórica para alcanzar el punto de equilibrio con velocidad final nula se obtiene a partir de la conservación de la energía:

\[
v_{min} = \sqrt{2\frac{\Delta U}{m}}
\]

El programa también presenta el trabajo necesario si la nave parte de una órbita circular baja, como referencia para analizar la maniobra TLI.

### Alcance y limitaciones

- El modelo es un análisis unidimensional sobre la línea que une los centros de los dos cuerpos; no calcula una trayectoria orbital real.
- El punto de equilibrio utilizado surge de igualar las fuerzas gravitatorias y no representa el punto L1 del problema restringido de tres cuerpos.
- No se calcula el tiempo de viaje entre la Tierra y la Luna.
- La comparación de la etapa D se realiza en términos de energía por unidad de masa. **No equivale a una relación directa entre masas de combustible**, ya que para estimar el combustible real se necesitarían datos de los sistemas de propulsión, como el impulso específico.
- El resultado del trabajo adicional de la etapa C depende de la diferencia entre valores energéticos de magnitud similar. Por eso, su interpretación debe considerar la precisión con la que se expresa la velocidad de inyección.

## Resultados de referencia

Con los valores iniciales del escenario y las constantes adoptadas en el código, se obtienen los siguientes resultados aproximados:

| Consigna | Resultado |
|---|---|
| B | Energía cinética inicial: `1,5445 × 10¹² J` |
| C | Punto de equilibrio: `345.654 km` desde el centro de la Tierra |
| C | Variación de energía potencial: `5,9462 × 10⁷ J/kg` |
| C | Energía cinética inicial por unidad de masa: `5,9405 × 10⁷ J/kg` |
| C | Trabajo adicional mínimo: `5,7023 × 10⁴ J/kg` para 10.900 m/s |
| C | Velocidad mínima de inyección estimada: `10.905,3 m/s` |
| D | Relación de energía por unidad de masa entre ida y vuelta: `23,7` veces |

Con la velocidad inicial de 10.900 m/s, el modelo determina que Orion no alcanza el punto de equilibrio. La velocidad mínima estimada es aproximadamente 10.905,3 m/s. Los resultados pueden variar cuando el usuario modifica los datos de la misión.

## Estructura del proyecto

```text
artemis-tli-grupo-b9/
├── main.c
├── artemis.c
├── artemis.h
├── validaciones.c
├── validaciones.h
├── Makefile
├── .gitignore
└── README.md
```

| Archivo | Responsabilidad |
|---|---|
| `main.c` | Contiene la función `main`, el menú principal y la inicialización de la misión y de la sesión. |
| `artemis.h` | Define constantes, estructuras y prototipos relacionados con la simulación. |
| `artemis.c` | Implementa los cálculos físicos, las etapas, el desafío y el reporte final. |
| `validaciones.h` | Declara las funciones de lectura, validación e interfaz. |
| `validaciones.c` | Implementa la lectura y validación de datos ingresados por el usuario y funciones auxiliares de interfaz. |
| `Makefile` | Define las reglas de compilación y limpieza de archivos generados. |
| `.gitignore` | Excluye del control de versiones los archivos generados durante la compilación. |

### Estructuras principales

Las estructuras definidas en `artemis.h` organizan los datos utilizados durante la ejecución:

- `Mision`: contiene los datos de entrada de la misión, como masa, velocidad, altura y distancia Tierra-Luna.
- `Resultados`: almacena los resultados obtenidos a partir de los datos de la misión.
- `PuntoTrayecto`: representa un punto de la bitácora de vuelo.
- `Intento`: registra los datos de cada intento realizado en el desafío.
- `Sesion`: conserva el registro de las etapas consultadas y de los intentos realizados para construir el reporte final.

## Requisitos

Para compilar el proyecto se necesita:

- Un compilador de C compatible con GCC.
- La biblioteca matemática estándar, enlazada mediante `-lm`.
- `make` para utilizar directamente el archivo `Makefile` (opcional si se compila con el comando alternativo).

En Windows se puede utilizar MinGW-w64, por ejemplo mediante MSYS2. En Linux, GCC y Make pueden instalarse mediante el gestor de paquetes de la distribución.

## Compilación

Desde una terminal, ubicarse en la carpeta del proyecto.

Con `make`:

```bash
make
```

En Windows con MinGW, el comando puede ser:

```bash
mingw32-make
```

Si no se dispone de `make`, se puede compilar directamente con GCC:

```bash
gcc -Wall -Wextra main.c artemis.c validaciones.c -o artemis -lm
```

Para eliminar los archivos generados durante la compilación:

```bash
make clean
```

## Ejecución

En Linux, macOS o Git Bash:

```bash
./artemis
```

En Windows, desde CMD o PowerShell:

```powershell
artemis.exe
```

## Validaciones de entrada

El programa valida los datos ingresados antes de utilizarlos. Entre otros casos, controla que:

- no se ingresen campos vacíos;
- se ingresen números cuando el dato requerido es numérico;
- no se utilicen valores decimales en opciones que requieren números enteros;
- los valores respeten los rangos establecidos;
- las respuestas a preguntas de tipo sí/no sean válidas.

La lectura numérica admite coma o punto como separador decimal y contempla entradas demasiado largas o el cierre de la entrada estándar para evitar que el programa continúe con datos inválidos.

## Uso de herramientas de inteligencia artificial

Durante el desarrollo se utilizó **Claude (Anthropic)** como herramienta de apoyo para consultas técnicas y tareas relacionadas con el código y la documentación. La herramienta no reemplaza la responsabilidad del equipo sobre la revisión, comprensión y validación del proyecto presentado.

## Fuentes

- Enunciado del Trabajo Práctico Integrador Final de Algoritmos y Estructuras de Datos, UTN – Facultad Regional Resistencia.
- Material de estudio y datos de referencia proporcionados por la cátedra para Física I y Algoritmos y Estructuras de Datos.
