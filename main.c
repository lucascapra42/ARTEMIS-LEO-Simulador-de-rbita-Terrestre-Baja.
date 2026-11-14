#include <stdio.h>
#include "artemis.h"
#include "validaciones.h"

int main(void)
{
    ParametrosOrbita parametros = {0};
    ResultadosOrbita resultados = {0};
    HistorialMision historial = {0};

    int opcion;
    int orbita_calculada = 0;
    int propulsor_calculado = 0;
    double tiempo_mision_s = 0.0;

    inicializarParametrosPredeterminados(&parametros);
    inicializarHistorial(&historial);

    
    /* MENU PRINCIPAL */
    do {
        printf("\n====================================\n");
        printf("          ARTEMIS LEO\n");
        printf("     SIMULADOR ORBITAL ORION\n");
        printf("====================================\n");
        printf("1. Configurar parametros orbitales\n");
        printf("2. Calcular orbita (A, B y C)\n");
        printf("3. Simular propulsor lateral (D)\n");
        printf("4. Mostrar resultados\n");
        printf("5. Consultar historial de mision\n");
        printf("0. Salir\n");
        printf("====================================\n");

        if (!leerEnteroEnRango("Seleccione una opcion: ",
                               0, 5, &opcion)) {
            printf("\nNo se pudo leer la opcion.\n");
            break;
        }

        switch (opcion) {

            
            case 1:
                printf("\nConfiguracion de parametros orbitales.\n");

                if (configurarParametrosOrbita(&parametros)) {
                    orbita_calculada = 0;
                    propulsor_calculado = 0;
                    tiempo_mision_s = 0.0;
                
                    inicializarHistorial(&historial);
                
                    printf("Nueva configuracion aplicada correctamente.\n");
                    printf("Los calculos anteriores fueron invalidados.\n");
                } else {
                    printf("No se pudo modificar la configuracion.\n");
                    printf("Se mantienen los parametros anteriores.\n");
                }
            
                break;

            case 2:
                printf("\nCalculando orbita de Orion...\n");

                if (calcularOrbita(&parametros, &resultados)) {
                    orbita_calculada = 1;
                    registrarEvento(&historial,
                      "Calculo orbital completado",
                       tiempo_mision_s);
                    printf("Calculos orbitales realizados correctamente.\n");
                } else {
                    printf("Error al calcular la orbita.\n");
                }
                break;

            case 3:
                printf("\nActivando propulsor lateral...\n");

                if (calcularPropulsorLateral(&parametros, &resultados)) {
                    propulsor_calculado = 1;
                       tiempo_mision_s += parametros.duracion_pulso;

                        registrarEvento(&historial,
                          "Simulacion del propulsor lateral completada",
                            tiempo_mision_s);
                    printf("Simulacion del propulsor completada.\n");
                } else {
                    printf("Error al calcular el propulsor.\n");
                }
                break;

            case 4:
                if (orbita_calculada && propulsor_calculado) {
                    mostrarResultados(&resultados);
                } else {
                    printf("\nPrimero debe completar los calculos orbitales "
                           "y del propulsor.\n");
                }
                break;

            case 5:
                mostrarHistorial(&historial);
                break;

            case 0:
                printf("\nFinalizando simulador ARTEMIS LEO.\n");
                break;

            default:
                printf("Opcion no valida.\n");
                break;
        }

    } while (opcion != 0);

    return 0;

}

