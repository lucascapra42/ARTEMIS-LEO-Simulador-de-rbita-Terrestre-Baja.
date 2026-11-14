#include "validaciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

/* Lectura segura de una linea */

static int leerLinea(const char *mensaje, char *linea,
                     size_t capacidad)
{
    int siguiente;
    size_t longitud;

    if (mensaje != NULL) {
        printf("%s", mensaje);
        fflush(stdout);
    }

    if (fgets(linea, (int)capacidad, stdin) == NULL) {
        return 0;
    }

    longitud = strlen(linea);

    if (longitud > 0 && linea[longitud - 1] == '\n') {
        linea[longitud - 1] = '\0';
    } else {
        siguiente = getchar();

        if (siguiente != '\n' && siguiente != EOF) {
            while (siguiente != '\n' && siguiente != EOF) {
                siguiente = getchar();
            }

            return -1;
        }
    }

    return 1;
}

/* Ignorar espacios al principio */

static char *saltarEspacios(char *texto)
{
    while (isspace((unsigned char)*texto)) {
        texto++;
    }

    return texto;
}

/* Validar numeros enteros */

int leerEnteroEnRango(const char *mensaje,
                      int minimo,
                      int maximo,
                      int *valor)
{
    char linea[256];
    char *inicio;
    char *fin;
    long numero;
    int estado;

    if (valor == NULL || minimo > maximo) {
        return 0;
    }

    while (1) {
        estado = leerLinea(mensaje, linea, sizeof(linea));

        if (estado == 0) {
            return 0;
        }

        if (estado == -1) {
            printf("Entrada demasiado larga.\n");
            continue;
        }

        inicio = saltarEspacios(linea);

        errno = 0;
        numero = strtol(inicio, &fin, 10);

        if (inicio == fin || errno == ERANGE) {
            printf("Error: ingrese un numero entero valido.\n");
            continue;
        }

        fin = saltarEspacios(fin);

        if (*fin != '\0') {
            printf("Error: no ingrese letras ni caracteres adicionales.\n");
            continue;
        }

        if (numero < minimo || numero > maximo) {
            printf("Error: el valor debe estar entre %d y %d.\n",
                   minimo, maximo);
            continue;
        }

        *valor = (int)numero;

        return 1;
    }
}

/* Validar numeros decimales */

int leerDoubleEnRango(const char *mensaje,
                      double minimo,
                      double maximo,
                      double *valor)
{
    char linea[256];
    char *inicio;
    char *fin;
    double numero;
    int estado;

    if (valor == NULL || !isfinite(minimo) ||
        !isfinite(maximo) || minimo > maximo) {
        return 0;
    }

    while (1) {
        estado = leerLinea(mensaje, linea, sizeof(linea));

        if (estado == 0) {
            return 0;
        }

        if (estado == -1) {
            printf("Entrada demasiado larga.\n");
            continue;
        }

        inicio = saltarEspacios(linea);

        errno = 0;
        numero = strtod(inicio, &fin);

        if (inicio == fin || errno == ERANGE ||
            !isfinite(numero)) {
            printf("Error: ingrese un numero decimal valido.\n");
            continue;
        }

        fin = saltarEspacios(fin);

        if (*fin != '\0') {
            printf("Error: no ingrese caracteres adicionales.\n");
            continue;
        }

        if (numero < minimo || numero > maximo) {
            printf("Error: el valor debe estar entre %.3e y %.3e.\n",
                minimo, maximo);
            continue;
        }

        *valor = numero;

        return 1;
    }
}

/* Validar caracteres permitidos */

int leerCaracterPermitido(const char *mensaje,
                          const char *opciones,
                          char *valor)
{
    char linea[256];
    char *inicio;
    char candidato;
    int estado;

    if (opciones == NULL || valor == NULL || opciones[0] == '\0') {
        return 0;
    }

    while (1) {
        estado = leerLinea(mensaje, linea, sizeof(linea));

        if (estado == 0) {
            return 0;
        }

        if (estado == -1) {
            printf("Entrada demasiado larga.\n");
            continue;
        }

        inicio = saltarEspacios(linea);

        if (*inicio == '\0') {
            printf("Error: debe ingresar un caracter.\n");
            continue;
        }

        candidato = (char)toupper((unsigned char)*inicio);
        inicio++;

        inicio = saltarEspacios(inicio);

        if (*inicio != '\0') {
            printf("Error: ingrese solamente un caracter.\n");
            continue;
        }

        const char *opcion;

        for (opcion = opciones; *opcion != '\0'; opcion++) {
            if (candidato == (char)toupper((unsigned char)*opcion)) {
                *valor = candidato;
                return 1;
            }
        }

        printf("Error: opcion no permitida.\n");
    }
}


