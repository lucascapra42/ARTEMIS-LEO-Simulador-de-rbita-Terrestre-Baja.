#ifndef VALIDACIONES_H
#define VALIDACIONES_H

/* Lectura segura de enteros */
int leerEnteroEnRango(const char *mensaje,
                      int minimo,
                      int maximo,
                      int *valor);

/* Lectura segura de numeros decimales */
int leerDoubleEnRango(const char *mensaje,
                      double minimo,
                      double maximo,
                      double *valor);

/* Lectura segura de caracteres permitidos */
int leerCaracterPermitido(const char *mensaje,
                          const char *opciones,
                          char *valor);

#endif