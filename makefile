# Makefile del Simulador de Misiones Artemis (Grupo B9)
#
# Uso:
#   make          compila el programa y genera el ejecutable "artemis"
#   make ejecutar compila (si hace falta) y ejecuta
#   make clean    borra los archivos generados por la compilacion
#
# Importante: las lineas de mandatos empiezan con TABULADOR, no con espacios.

CC = gcc
CFLAGS = -Wall -Wextra -g
OBJS = main.o artemis.o validaciones.o

# Primera regla: es la que se ejecuta al escribir solamente "make".
all: artemis

# El ejecutable se arma uniendo (montando) los tres archivos objeto.
# -lm agrega la biblioteca matematica (se usa sqrt y ceil).
artemis: $(OBJS)
	$(CC) $(CFLAGS) -o artemis $(OBJS) -lm

# Cada .o se compila a partir de su .c. Si cambia un .h del que depende,
# se vuelve a compilar.
main.o: main.c artemis.h validaciones.h
	$(CC) $(CFLAGS) -c main.c

artemis.o: artemis.c artemis.h validaciones.h
	$(CC) $(CFLAGS) -c artemis.c

validaciones.o: validaciones.c validaciones.h
	$(CC) $(CFLAGS) -c validaciones.c

ejecutar: artemis
	./artemis

clean:
	rm -f $(OBJS) artemis artemis.exe
